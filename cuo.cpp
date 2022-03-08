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
/// config var
namespace cuo {
int lines = 10000; // for example, 1000 * 10 * 10, every stock has 100 * 10 * 10 lines
string prev_file = "/data/team-10/trade_merge/prev_price";
string hook_file = "/data/team-10/hook.h5";
int ptr[10] = {0};
int *hook_read = new int [10 * 100 * 4];
map<int, special_info> special_hook[10];

void process_hook(int * hook) {
    int fd = -1;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            fd ++;
            auto order_id = *(hook + fd);
            fd ++;
            auto target_stk = *(hook + fd);
            fd ++;
            auto target_trade = *(hook + fd);
            fd ++;
            auto arg = *(hook + fd);
            special_info tmp = {target_stk, target_trade, arg};
            //tmp.target_stk = target_stk;
            special_hook[i][order_id] = tmp;
            //cout << "save hook: " << i << " " << order_id << " " << target_stk << " " << target_trade << " " << arg << endl;
        }
    }
    cout << "finish process hook into special map" << endl;
}

void init_config() {

    /// get hook data
    save_hook_data(hook_read, hook_file);
    process_hook(hook_read);

    /// get prev close
    auto filename = prev_file;
    std::ifstream infile(prev_file, std::ios::in | std::ios::binary);
    while(!infile.good()) {
        cout << " cannot find " << filename << " , sleep 2s " << endl;
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

    /// clear map
    for (int i = 0; i < 10; ++i) {
        special_hook[i].clear();
        matcher::bids[i].clear();
        matcher::asks[i].clear();
    }
}

void save_order_from_file(const string& filename, struct order* t) {
    std::ifstream infile(filename, std::ios::in | std::ios::binary);
    while(!infile.good()) {
        cout << " cannot find " << filename << " , sleep 2s " << endl;
        sleep(2);
    }
    infile.read((char *)t, sizeof(order) * lines);
    infile.close();
}

int get_cvl_from_trade(int stk, int id) {
    auto& the_trade = matcher::ans[stk][id - 1];
    return the_trade.volume;
}

bool use_hook(order& my_order) {
    unsigned char ch = my_order.combined;
    int stk_code = (ch >> 4) & 15; stk_code --;
    auto each = special_hook[stk_code][my_order.order_id];
    int stk = each.target_stk;
    int id = each.target_trade;
    int arg = each.arg;
    cout << "special order " << stk_code << " " << my_order.order_id << " " << stk<< " " << id;
    if (id >= matcher::ans[stk].size()) {
        cout << "DEBUG: order need wait " << stk_code << " "
        << stk << " " << id << endl;
        return 1;
    } else {
        int cvl = get_cvl_from_trade(stk, id);
        if (cvl > arg) {
            cout << "DEBUG: cancell order" << stk_code << " "
            << stk << " " << id << " " << arg << " " << cvl << endl;
            return 2;
        } else {
            return 3;
        }
    }
}

void start_cuo() {
    cout << "LOG: start cuo" << endl;
    order* t[10] = {nullptr};
    for (int i = 0; i < 10; ++i) {
        t[i] = new struct order[lines];
        string filename = "/data/team-10/trade_merge/stock" + to_string(i);
        save_order_from_file(filename, t[i]);
        ptr[i] = 0;
    }
    cout << "LOG: finish reading stock" << endl;
    while (1) {
        int processed = 0;
        for (int i = 0; i < 10; ++i) {
            if (ptr[i] >= lines) {
                continue;
            }
            bool need_process = false;
            order each = *(t[i] + ptr[i]);
            if (special_hook[i].count(each.order_id)) {
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
                matcher::process_order(t[i] + ptr[i]);
                //cout << " ptr[i]: " << i << " " << ptr[i] << endl;
                ptr[i] ++;
                processed ++;
            }
        }
        if (processed == 0) break;
    }
    cout << "LOG: finish cuo! " << endl;
    return;
}


} // namespace cuo

int main() {
    cuo::init_config();
    cuo::start_cuo();
    return 0;
}
