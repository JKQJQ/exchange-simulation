#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "common.h"
#define medium large
#define inf 2000000000
using namespace std;
namespace matcher {
int order_id, price, volume;
unsigned int stk_code;
int dir, type, quota, old_price;
bool isLeave;
map < pair<int, int>, int > bids[11], asks[11];
unordered_map<int, int> levels;
int prev_high[11], prev_low[11];
vector<trade> ans[10];

inline long long get_cvl(map < pair<int, int>, int >& bid) {
    long long ret = 0;
    for (auto& each: bid) {
        ret += each.second;
    }
    return ret;
}


inline int get_best_ask() {
    auto& ask = asks[stk_code];
    for (auto it = ask.begin(); it != ask.end(); ) {
        while (it != ask.end() && (*it).second <= 0) {
            it  = ask.erase(it);
        }
        if (it != ask.end()) return (*it).first.first;
        else return 0;
    }
    return 0;
}

inline int get_best_bid() {
    auto& bid = bids[stk_code];
    // return abs(price)
    for (auto it = bid.begin(); it != bid.end(); ) {
        while (it != bid.end() && (*it).second <= 0) {
            it  = bid.erase(it);
        }
        if (it != bid.end()) return -(*it).first.first;
        else return 0;
    }
    return 0;
}

inline int get_a_price(int price) {
    /*
      return a price >= 0, -1 means price invalid
    */
    isLeave = true;
    if (type == 0) {
        // limit order
        // is invalid
        if (price > prev_high[stk_code]) return -1;
        if (price < prev_low[stk_code]) return -1;
        return price;
    }
    if (type == 1) {
        // 对手方最优
        return (dir == 1)? get_best_ask(): get_best_bid();
    }
    if (type == 2) {
        // 本方最优
        return (dir == 1)? get_best_bid(): get_best_ask();
    }
    if (type == 3) {
        // 最优五档, 对手方, TODO
        quota = inf;
        isLeave = false;
        return (dir == 1)? get_best_ask(): get_best_bid();
    }
    if (type == 4) {
        // 吃光对手方所有价格
        quota = inf;
        isLeave = false;
        return (dir == 1)? get_best_ask(): get_best_bid();
    }
    if (type == 5) {
        // 全额成交, 以对手方为成交价
        quota = inf;
        isLeave = false;
        long long cvl = (dir == 1)? get_cvl(asks[stk_code]): get_cvl(bids[stk_code]);
        if (cvl >= volume) {
            return (dir == 1)? get_best_ask(): get_best_bid();
        } else {
            //cout << "LOG: can not eat all " << stk_code << " " << order_id << " " << endl;
            return -1;
        }
    }
    return 0;
}

inline void make_trade(int asn1, int asn2, int price, int& trade_qty) {
    // 写二进制文件
    // std::ifstream infile(prev_file, std::ios::out | std::ios::binary);
    if (type == 3) {
        levels[price] ++;
        if (levels.size() >= 6) {
            quota = 0;
            trade_qty = 0;
            return;
        }
    }
    string filename = "/data/team-10/my_ans_large/trade" + to_string(stk_code + 1);
    std::ofstream outfile;
    outfile.open(filename, ios::out | ios::binary| ios::app);
    if (!outfile.good()) {
        cout << " error: outfile not good" << endl;
        trade_qty = 0;
        return;
    }
    if (trade_qty <= 0) return;
    auto tmp = trade(
            stk_code + 1, asn1, asn2, (double)price/100, trade_qty);
    ans[stk_code].push_back(tmp);
    outfile.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
    outfile.close();
}

inline void start_trading(int order_price,
                   map < pair<int, int>, int >& ask) {
    for (auto it = ask.begin(); it != ask.end(); ) {
        while (it != ask.end() && (*it).second <= 0) {
            it  = ask.erase(it);
        }
        if (it == ask.end()) return;
        auto duifang_price = (*it).first.first;
        auto cvl = min(volume, (*it).second);
        auto asn1 = order_id;
        auto asn2 = (*it).first.second;
        bool nolimit = (type == 3 || type == 4 || type == 5);
        if (order_price >= duifang_price || nolimit) {
            if (dir == 2) swap(asn1, asn2);
            make_trade(asn1, asn2, abs(duifang_price), cvl);
            volume -= cvl;
            (*it).second -= cvl;
        }
        it ++;
        if (volume == 0) return;
    }
}

void start_matching(int order_price, bool isLeave) {
    if (dir == 1) start_trading (order_price, asks[stk_code]);
    else  start_trading(-order_price, bids[stk_code]);
    if (isLeave && volume > 0) {
        auto p = make_pair(order_price, order_id);
        if (dir == 1) {
            // is buy
            p.first *= -1;
            bids[stk_code][p] += volume;
        } else {
            // is sell
            asks[stk_code][p] += volume;
        }
    }
}


void process_order(struct order* t) {
    order_id = (t)->order_id;
    price = ((t)->price);
    volume = (t)->volume;
    unsigned char ch = (t)->combined;
    dir = ((ch >> 3) & 1) ? 1 : 2;
    type = (ch & 7);
    stk_code = (ch >> 4) & 15;
    //stk_code --;
    if (order_id % 1000000 == 0) cout << "debug order: " << stk_code << " " << order_id << " "
         << price << " " << volume << " " << type << " " << dir << endl;
    quota = 1;
    auto order_price = get_a_price(price);
    if (order_price == 0) {
        order_price = price;
        return;
    }
    if (order_price == -1) {
        return;
    }

    auto old_volume = -1;
    levels.clear();
    while (quota > 0 && volume != old_volume) {
        old_volume = volume;
        start_matching(order_price, isLeave);
        quota --;
    }
}



}
