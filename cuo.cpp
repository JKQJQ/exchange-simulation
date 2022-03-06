#include "cuo.h"

#include <cstdio>
#include <iostream>

#include "common.h"
using namespace std;
/// config var
int finish_id = 0; // finish trade1 small file id
int doing_id = 1; // doing trade 2 small file (halfly)
int end_id = 100; // 100 * 10 * 10, 100 files in total
int lines = 100; // for example, 100 * 10 * 10, every file has 10 * 10 lines 
string recover_file = "/data/team-10/recover.csv";

int should_do_id = 1;

// < <price, ask/bid_id>, rest_volumn>
map < pair<int, int>, int > bids[11], asks[11]; 

void init_config() {
    for (int i = 1; i <= 10; ++i) {
        bids[i].clear();
        asks[i].clear();
    }
    
}
void add_row_to_map(vector<std::string>& row) {
    auto side = stoi(row[0]); // 1: bid(buy); 2: ask(sell);
    auto stk_code = stoi(row[1]);
    auto price = stoi(row[2]);
    auto appl_id = stoi(row[3]);
    auto vol = stoi(row[4]);
    auto p = make_pair(price, appl_id);
    if (side == 1) {
        p.first *= -1; // bid price越高越优秀，所以从大到小排
        bids[stk_code][p] = volume;
    } else {
        asks[stk_code][p] = volume;
    }
}

void recover_from_before() {
    cout << " === start revocer from id: " << finish_id << endl;
    auto& filename = recover_file;
    std::fstream file (filename, std::ios::in);
    std::string line, word;
    std::vector<std::string> row;
    if(file.is_open()){
        if (getline(file, line)){
            row.clear();
            std::stringstream str(line);
            while (getline(str, word, ','))
                row.push_back(word);
            add_row_to_map(row);
        } else {
            return -1;
        }
    }else{
        std::cout<<"Could not open the file\n";
        return;
    }
    cout << " === finish recovering === " << endl;
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

void start_cuo() {

    // 一切进度以trade1_id的小文件为基准
    struct order* t1 = new struct order[lines];
    struct order* t2 = new struct order[lines];

    int j = 0; // trade2 小矩阵的中间进度
    matcher obj(bids, asks);
    while (finish_id < end_id) {
        string filename = "/data/team-10/trade1_" + to_string(finish_id + 1);
        save_order_from_file(filename, t1);
        filename = "/data/team-10/trade2_" + to_string(doing_id);
        save_order_from_file(filename, t2);
        for (int i = 0; i < lines; ++i) {
            auto order_id1 = (t1+i)->order_id;
            while (order_id > should_do_id) {
                while() {
                    if (j >= lines) {
                        j = 0;
                        doing_id ++;
                    }
                    order_id2 = (t2 + j) ->order_id;
                    if (order_id2 == should_do_id) {
                        obj->process_order(t2 + j);
                    }
                }
            }
        }
        finish_id ++;
    }

    /// rest trade2
    while (doing_id < end_id || j < lines) {

    }
}

int main() {

    init_config();
    recover_from_before();
    start_cuo();

    return 0;
}