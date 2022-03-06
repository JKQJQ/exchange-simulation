#pragma once

#include "common.h"

#include <map>

class matcher {
public:
    matcher(map < pair<int, int>, int > bids, 
            map < pair<int, int>, int > asks) {
        bids = _bids;
        asks = _asks;
    }

public:
void process_order(struct order* t) {
    order_id = (t)->order_id;
    price = (t)->price;
    vol = (t)->volume;
    char ch = (t)->combined;
    dir = ((combined >> 3) & 1) ? 1 : 2;
    type = (combined & 7);
    auto order_price = get_a_price(price, dir);
    int quota = 1;
    if (type == 1) {
        start_matching(order_price, volume, 1, dir);
    } if (type == 2) {

    }
}
private:
    int get_a_price(int price) {
        if (type == 0) return price;
        if (type == 1) {
            // 对手方最优
            return (dir == 1)? get_best_ask(): get_best_bid(); 
        }
    }
    
    void start_matching(int order_price, bool isLeave) {
        if (dir == 1) start_buying (order_price);
                else  start_selling(order_prive);
        if (isLeave) {
            auto p = make_pair(order_price, order_id);
            if (dir == 1) {
                // is buy
                bids.
            } else {
                
            }
        }
    }

private:
    int order_id, price, vol;
    int dir, type;
    // < <price, ask/bid_id>, rest_volumn>
    map < pair<int, int>, int > bids[11], asks[11]; 
};