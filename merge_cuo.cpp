#include "cuo.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <ctime>
#include <unistd.h>
#include "common.h"
#include <fstream>
using namespace std;
string filesize = "large/";
/// config var
namespace cuo {
int lines = 100000000; // for example, 100 * 100 * 100, every stock has 100 * 10 * 10 lines
int chunk_lines = 1000000;
int end_id = 100;
string prev_file = "/data/team-10/" + filesize + "test2/prev_price";
string hook_file = "/data/team-10/" + filesize + "test2/hook";
int ptr[10] = {0};
int finish_read[10] = {0};
int *hook_read = new int [10 * 100 * 4];
order* data[10] = {nullptr};
std::ifstream infile[10];
map<int, special_info> special_hook[10];

void process_hook(int * hook) {
    int fd = -1;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            fd ++;
            auto order_id = *(hook + fd);
            fd ++;
            auto target_stk = *(hook + fd);
            target_stk --;
            fd ++;
            auto target_trade = *(hook + fd);
            fd ++;
            auto arg = *(hook + fd);
            special_info tmp = {target_stk, target_trade, arg};
            special_hook[i][order_id] = tmp;
            //if(i == 0)cout << "save hook: " << i << " " << order_id << " " << target_stk << " " << target_trade << " " << arg << endl;
        }
    }
    cout << "finish process hook into special map" << endl;
}

void init_config() {

    /// clear map
    for (int i = 0; i < 10; ++i) {
        special_hook[i].clear();
        matcher::bids[i].clear();
        matcher::asks[i].clear();
        matcher::ans[i].reserve(10000);
        finish_read[i] = 0;
        data[i] = new struct order[chunk_lines];
        string filename = "/data/team-10/" + filesize + "order_merge/stock" + to_string(i + 1);
        infile[i].open(filename, std::ios::in | std::ios::binary);
    }

    /// get hook data
    std::ifstream hookinfile(hook_file, std::ios::in | std::ios::binary);
    while(!hookinfile.good()) {
        cout << " cannot find " << hook_file << " , sleep 2s " << endl;
        sleep(2);
    }
    cout << " file is good: " << hook_file << endl;
    hookinfile.read((char *)hook_read, sizeof(int) * 10 * 100 * 4);
    hookinfile.close();
    process_hook(hook_read);

    /// get prev close
    std::ifstream infile(prev_file, std::ios::in | std::ios::binary);
    while(!infile.good()) {
        cout << " cannot find " << prev_file << " , sleep 2s " << endl;
        sleep(2);
    }
    cout << " file is good: " << prev_file << endl;
    int* t = new int[10];
    int tmp = 0;
    infile.read((char *)t, sizeof(tmp) * 10);
    infile.close();
    cout << "start to save prev_high prev_low " << endl;

    for (int i = 0; i < 10; ++i) {
        int tmp = *(t + i); //99998
        double high = (double) tmp * 1.1;
        double low = (double) tmp * 0.9;
        int high_int = round(high);
        int low_int = round(low);
        if (high_int - tmp < 1) high_int = tmp + 1;
        if (tmp - low_int < 1) low_int = tmp - 1;
        matcher::prev_high[i] = high_int;
        matcher::prev_low[i] = low_int;
    }
    cout << "finish save prev price" << endl;
}

void save_order_from_file(const string& filename, struct order* t, int stk) {

    while(!infile[stk].good()) {
        cout << " cannot find " << filename << " , sleep 2s " << endl;
        sleep(2);
    }
    infile[stk].read((char *)t, sizeof(order) * chunk_lines);
    finish_read[stk] ++;
}

inline int get_cvl_from_trade(int stk, int id) {
    auto& the_trade = matcher::ans[stk][id - 1];
    return the_trade.volume;
}

int use_hook(order& my_order) {
    unsigned char ch = my_order.combined;
    int stk_code = (ch >> 4) & 15; //stk_code --;
    if (special_hook[stk_code].count(my_order.order_id) <= 0) {
        cout << "error special hook: " << stk_code << " " << my_order.order_id << endl;
        exit(0);
    }
    auto each = special_hook[stk_code][my_order.order_id];
    int stk = each.target_stk;
    int id = each.target_trade;
    int arg = each.arg;
    if (id > matcher::ans[stk].size()) {
        return 1;
    } else {
        int cvl = get_cvl_from_trade(stk, id);
        if (cvl > arg) {
            // cout << "DEBUG: cancell order" << stk_code << " "
            // << stk << " " << id << " " << arg << " " << cvl << endl;
            return 2;
        } else {
            return 3;
        }
    }
}

void read_block(int stk) {
    string filename = "/data/team-10/" + filesize + "order_merge/stock" + to_string(stk + 1);
    save_order_from_file(filename, data[stk], stk);
    ptr[stk] = 0;
}


void start_cuo() {
    cout << "LOG: start cuo" << endl;
    for (int i = 0; i < 10; ++i) {
        read_block(i);
    }
    cout << "LOG: finish reading stock -- first chunk" << endl;

    while (1) {
        int processed = 0;
        for (int i = 0; i < 10; ++i) {
            if (ptr[i] >= chunk_lines) {
                if (finish_read[i] >= end_id ) {
                    processed ++;
                    continue;
                } else {
                    read_block(i);
                }
            }
            bool need_process = false;
            order each = *(data[i] + ptr[i]);
            unsigned char ch = each.combined;
            int stk_code = (ch >> 4) & 15; //stk_code --;
            assert(i == stk_code);
            if (special_hook[i].count(each.order_id)) {
                //cout << " use hook: " << i << " " << stk_code << " " << each.order_id << endl;
                int ret = use_hook(each);
                if (ret == 1) {
                    // need wait
                    continue;
                } else if (ret == 2) {
                    // cancell order
                    ptr[i] ++;
                    continue;
                } else {
                    need_process = true;
                }
            } else {
                need_process = true;
            }
            if (need_process) {
                matcher::process_order(data[i] + ptr[i]);
                //cout << " ptr[i]: " << i << " " << ptr[i] << endl;
                ptr[i] ++;
            }
        }
        if (processed == 10) break;
    }
    cout << "LOG: finish cuo! " << endl;
    matcher::write_trade();
    return;
}


} // namespace cuo

int main() {
    cuo::init_config();
    cuo::start_cuo();
    return 0;
}
