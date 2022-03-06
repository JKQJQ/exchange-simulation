struct trade {
    int stk_code;
    int bid_id;
    int ask_id;
    double price;
    int volume;
}__attribute__((packed));

struct order {
    int order_id;
    int price;
    int volume;
    char combined;
}__attribute__((packed));