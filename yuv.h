#pragma once
#include <fstream>
#include "results.h"

class yuv
{
public:
    std::ifstream video_file;
    int num_frames;
    int cur_frame_n;
    int width;
    int height;
    unsigned char ** ref_frame;
    unsigned char ** cur_frame;
    unsigned char ref_block[8][8];
    unsigned char cur_block[8][8];

    yuv();
    ~yuv();
    void open_file(const char * file_path, int n_frames, int width, int height);
    void load_nth_frame(int n);
    void get_block(int pos_y, int pos_x);
    void pred_cur_frame(results * report);

    int subtratorImpreciso(int op1, int op2, int BitsIMP);
};

