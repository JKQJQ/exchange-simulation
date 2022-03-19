#pragma once

#include <cstring>
#include <cassert>
//#include "H5Cpp.h"
const int RANK_OUT = 3;
using namespace std;
//using namespace H5;

struct OutputTrade {
    int stk_code;
    int bid_id;
    int ask_id;
    double price;
    int volume;
    OutputTrade(int a, int b, int c, double d, int e) :
    stk_code(a), bid_id(b), ask_id(c), price(d), volume(e) {}
}__attribute__((packed));

struct trade {
    unsigned char stk_code;
    int bid_id;
    int ask_id;
    int price;
    int volume;
    trade(unsigned char a, int b, int c, int d, int e) :
    stk_code(a), bid_id(b), ask_id(c), price(d), volume(e) {}
}__attribute__((packed));

struct order {
    int order_id;
    int price;
    int volume;
    unsigned char combined;
}__attribute__((packed));

struct special_info {
    int target_stk;
    int target_trade;
    int arg;
    //special_info(int a, int b, int c):
    //target_stk(a), target_trade(b), arg(c) {}
};

// void save_hook_data(int *data_read, const string& hook_file) {
//     int NX = 10;
//     int NY = 100;
//     int NZ = 4;
//     H5std_string FILE_NAME(hook_file);
//     H5std_string DATASET_NAME("hook");
//     int size = NX * NY * NZ;
//     memset(data_read, 0, sizeof(int) * size);

//     H5File file(FILE_NAME, H5F_ACC_RDONLY);
//     DataSet dataset = file.openDataSet(DATASET_NAME);
//     DataSpace dataspace = dataset.getSpace();
//     int rank = dataspace.getSimpleExtentNdims();
//     hsize_t dims_out[3] = {1, 1, 1};
//     dataspace.getSimpleExtentDims(dims_out, NULL);

//     printf("rank %d, shape (%llu, %llu, %llu)\n", rank, dims_out[0], dims_out[1], dims_out[2]);

//     hsize_t offset[3];
//     hsize_t count[3];
//     offset[0] = 0;
//     offset[1] = 0;
//     offset[2] = 0;
//     count[0] = NX;
//     count[1] = NY;
//     count[2] = NZ;
//     dataspace.selectHyperslab(H5S_SELECT_SET, count, offset); //select in file, this api can set api

//     hsize_t dimsm[3];
//     dimsm[0] = NX;
//     dimsm[1] = NY;
//     dimsm[2] = NZ;

//     for (int i = 0; i < 3; ++i) assert(dimsm[i] == dims_out[i]);
//     DataSpace memspace(RANK_OUT, dimsm);

//     hsize_t offset_out[3];
//     hsize_t count_out[3];
//     offset_out[0] = 0;
//     offset_out[1] = 0;
//     offset_out[2] = 0;
//     count_out[0] = NX;
//     count_out[1] = NY;
//     count_out[2] = NZ;
//     memspace.selectHyperslab(H5S_SELECT_SET, count_out, offset_out); // select in memory

//     dataset.read(data_read, PredType::NATIVE_INT, memspace, dataspace);
// }
