#include <iostream>
#include <fstream>
using namespace std;
struct trade {
    int stk_code;
    int bid_id;
    int ask_id;
    double price;
    int volume;
    //trade(int a, int b, int c, double d, int e) :
    //stk_code(a), bid_id(b), ask_id(c), price(d), volume(e) {}
}__attribute__((packed));

struct order {
    int order_id;
    int price;
    int volume;
    unsigned char combined;
}__attribute__((packed));

int main() {
    int lines = 10000;
    struct order* t = new struct order[lines];
    std::ifstream infile("/data/team-10/medium/trade_merge/stock0",
    std::ios::in | std::ios::binary);

    infile.read((char *)t, sizeof(order) * lines);
    infile.close();
    int L = 9310;
    for(int i = L; i < L + 20; ++i){
        unsigned char ch = (t + i) ->combined;
        int stk_code = (ch >> 4) & 15;
        int type = ch & 7;
        int dir = ((ch >> 3) & 1) ? 1 : 2;
        if (i % 1 == 0) printf("%d %d %d %d %d %d\n",
        stk_code, dir , 
        (t+i)->order_id, (t+i)->price, (t+i)->volume,
        type);
    }
    return 0;
}
