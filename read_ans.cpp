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

int main() {
    int lines = 100;
    struct trade* t = new struct trade[lines];
    std::ifstream infile("/data/100x10x10/trade1",std::ios::in | std::ios::binary);
    //std::ifstream infile("/data/team-10/my_ans/trade1",std::ios::in | std::ios::binary);
    infile.read((char *)t, sizeof(trade) * lines);
    infile.close();
    int L = 0;
    for(int i = L; i < L + 40; ++i){
        printf("%d %d %d %lf %d\n",
        (t+i)->stk_code, (t+i)->bid_id, (t+i)->ask_id,
        (t+i)->price, (t+i)->volume);
    }
    return 0;
}
