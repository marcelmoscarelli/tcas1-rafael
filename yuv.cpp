#include <iostream>
#include <fstream>
#include "yuv.h"

yuv::yuv()
{}

yuv::~yuv()
{
    if (video_file.is_open()) {
        video_file.close();
    }
}

void yuv::open_file(const char * file_path, int n_frames, int width, int height)
{
    this->n_frames = n_frames;
    this->width = width;
    this->height = height;

    if (video_file.is_open()) {
        video_file.close();
    }
    video_file.open(file_path, std::ios::binary);
    if (!video_file.is_open()) {
        std::cerr << "File could not open. Verify file path and try again." << std::endl;
        exit(-1);
    }

    std::cout << "File \"" << file_path << "\" opened." << std::endl;
    std::cout << "Number of frames: " << n_frames << std::endl;
    std::cout << "Resolution: " << width << "x" << height << std::endl << std::endl;

    ref_frame = new unsigned char*[height];
    cur_frame = new unsigned char*[height];
    //for (int j = 0; j < height; ++j) {
    //    orig_frame[j] = new unsigned char[width];
    //    pred_frame[j] = new unsigned char[width];
    //    rec_samples[j] = new bool[width];
    //}
    //block_mode = new int*[height / 4];
    //for (int j = 0; j < height / 4; ++j) {
    //    block_mode[j] = new int[width / 4];
    //}
}

void yuv::load_nth_frame(int n)
{
    if (n < 1 || n >= n_frames) {
        std::cerr << "Invalid frame index (" << n << " of " << n_frames - 1 << ")." << std::endl;
        exit(-1);
    }

    video_file.seekg(static_cast<int>((n - 1) * width * height * 1.5));
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            ref_frame[j][i] = static_cast<unsigned char>(video_file.get());
        }
    }

    video_file.seekg(static_cast<int>(n * width * height * 1.5));
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cur_frame[j][i] = static_cast<unsigned char>(video_file.get());
        }
    }

    std::cout << "Frame " << n << "/" << n_frames - 1 << " loaded." << std::endl << std::endl;
}

void yuv::get_block(int pos_y, int pos_x, int ref_or_cur)
{
    if (pos_x >= width / block_size || pos_y >= height / block_size) {
        std::cerr << "Invalid original block position (" << pos_x << "," << pos_y << ")." << std::endl;
        exit(-1);
    }

    for (int y = 0; y < block_size; ++y) {
        for (int x = 0; x < block_size; ++x) {
            if (ref_or_cur == 0) {
                ref_block[y][x] = ref_frame[pos_y * block_size + y][pos_x * block_size + x];
            }
            else {
                cur_block[y][x] = cur_frame[pos_y * block_size + y][pos_x * block_size + x];
            }
        }
    }
}