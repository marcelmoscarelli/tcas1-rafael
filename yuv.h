#pragma once
#include <fstream>

class yuv
{
public:
    const int block_size = 8;

    std::ifstream video_file;
    int n_frames;
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
    void get_block(int pos_y, int pos_x, int ref_or_cur);
};

