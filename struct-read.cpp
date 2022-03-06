#include <iostream>
#include <fstream>
#include "common.h"
using namespace std;

int main() {
    struct trade* t = new struct trade[100];
    std::ifstream infile("/data/100x1000x1000/trade1", std::ios::in | std::ios::binary);
    infile.read((char *)t, sizeof(trade) * 100);
    infile.close();
    for(int i = 0; i < 100; ++i){
        printf("%d %d %d %lf %d\n", (t+i)->stk_code, (t+i)->bid_id, (t+i)->ask_id, (t+i)->price, (t+i)->volume);
    }
    return 0;
}
