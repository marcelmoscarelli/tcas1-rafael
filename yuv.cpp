#include <iostream>
#include <fstream>
#include <cmath>
#include <bitset>
#include "yuv.h"
#include "results.h"

yuv::yuv()
{}

yuv::~yuv()
{
    if (video_file.is_open()) {
        video_file.close();
    }
    for (int j = 0; j < height; ++j) {
        delete[] ref_frame[j];
        delete[] cur_frame[j];
    }
    delete[] ref_frame;
    delete[] cur_frame;
}

void yuv::open_file(const char * file_path, int n_frames, int width, int height)
{
    this->num_frames = n_frames;
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
    for (int j = 0; j < height; ++j) {
        ref_frame[j] = new unsigned char[width];
        cur_frame[j] = new unsigned char[width];
    }
}

void yuv::load_nth_frame(int n)
{
    if (n < 1 || n >= num_frames) {
        std::cerr << "Invalid frame index (" << n << " of " << num_frames - 1 << ")." << std::endl;
        exit(-1);
    }

    cur_frame_n = n;

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
}

void yuv::get_block(int pos_y, int pos_x)
{
    if (pos_x >= width / 8 || pos_y >= height / 8) {
        std::cerr << "Invalid original block position (" << pos_x << "," << pos_y << ")." << std::endl;
        exit(-1);
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            ref_block[y][x] = ref_frame[pos_y * 8 + y][pos_x * 8 + x];
            cur_block[y][x] = cur_frame[pos_y * 8 + y][pos_x * 8 + x];
        }
    }
}

void yuv::pred_cur_frame(results * report)
{
    if (height % 8 != 0 || width % 8 != 0) {
        std::cerr << "Resolution must be a multiple of 8." << std::endl;
        exit(-1);
    }

    int height_in_8s = height / 8;
    int width_in_8s = width / 8;

    std::cout << "Processing frame " << cur_frame_n << " of " << num_frames - 1 << "." << std::endl << std::endl;

    int sub_sad[8][8];
    int sub_sad_apps1[8][8];
    int sub_sad_apps2[8][8];
    int sub_sad_apps3[8][8];
    int sub_sad_apps4[8][8];
    int sub_sse[8][8];
    int sub_sse_apps1[8][8];
    int sub_sse_apps2[8][8];
    int sub_sse_apps3[8][8];
    int sub_sse_apps4[8][8];
    for (int j = 0; j < height_in_8s; ++j) {
        for (int i = 0; i < width_in_8s; ++i) {
            int sad = 0, sad_apps1 = 0, sad_apps2 = 0, sad_apps3 = 0, sad_apps4 = 0, sse = 0, sse_apps1 = 0, sse_apps2 = 0, sse_apps3 = 0, sse_apps4 = 0;
            std::cout << "Processing block #" << (j * width_in_8s + i) << " of " << (height_in_8s * width_in_8s) << std::endl;
            get_block(j, i);
            for (int j2 = 0; j2 < 8; ++j2) {
                for (int i2 = 0; i2 < 8; ++i2) {
                    // Precise.
                    int sub = cur_block[j2][i2] - ref_block[j2][i2];
                    sub_sad[j2][i2] = abs(sub);
                    sub_sse[j2][i2] = sub * sub;
                    // Apps1
                    sub = subtratorImpreciso(cur_block[j2][i2], ref_block[j2][i2], 1);
                    sub_sad_apps1[j2][i2] = abs(sub);
                    sub_sse_apps1[j2][i2] = sub * sub;
                    // Apps2
                    sub = subtratorImpreciso(cur_block[j2][i2], ref_block[j2][i2], 2);
                    sub_sad_apps2[j2][i2] = abs(sub);
                    sub_sse_apps2[j2][i2] = sub * sub;
                    // Apps3
                    sub = subtratorImpreciso(cur_block[j2][i2], ref_block[j2][i2], 3);
                    sub_sad_apps3[j2][i2] = abs(sub);
                    sub_sse_apps3[j2][i2] = sub * sub;
                    // Apps4
                    sub = subtratorImpreciso(cur_block[j2][i2], ref_block[j2][i2], 4);
                    sub_sad_apps4[j2][i2] = abs(sub);
                    sub_sse_apps4[j2][i2] = sub * sub;

                    // Adds
                    sad += sub_sad[j2][i2];
                    sad_apps1 += sub_sad_apps1[j2][i2];
                    sad_apps2 += sub_sad_apps2[j2][i2];
                    sad_apps3 += sub_sad_apps3[j2][i2];
                    sad_apps4 += sub_sad_apps4[j2][i2];
                    sse += sub_sse[j2][i2];
                    sse_apps1 += sub_sse_apps1[j2][i2];
                    sse_apps2 += sub_sse_apps2[j2][i2];
                    sse_apps3 += sub_sse_apps3[j2][i2];
                    sse_apps4 += sub_sse_apps4[j2][i2];

                    // Report
                    report->acc_sad[j2][i2] += static_cast<unsigned long int>(sub_sad[j2][i2]);
                    report->acc_sse[j2][i2] += static_cast<unsigned long int>(sub_sse[j2][i2]);
                    report->acc_sad_apps1[j2][i2] += static_cast<unsigned long int>(sub_sad_apps1[j2][i2]);
                    report->acc_sse_apps1[j2][i2] += static_cast<unsigned long int>(sub_sse_apps1[j2][i2]);
                    report->acc_sad_apps2[j2][i2] += static_cast<unsigned long int>(sub_sad_apps2[j2][i2]);
                    report->acc_sse_apps2[j2][i2] += static_cast<unsigned long int>(sub_sse_apps2[j2][i2]);
                    report->acc_sad_apps3[j2][i2] += static_cast<unsigned long int>(sub_sad_apps3[j2][i2]);
                    report->acc_sse_apps3[j2][i2] += static_cast<unsigned long int>(sub_sse_apps3[j2][i2]);
                    report->acc_sad_apps4[j2][i2] += static_cast<unsigned long int>(sub_sad_apps4[j2][i2]);
                    report->acc_sse_apps4[j2][i2] += static_cast<unsigned long int>(sub_sse_apps4[j2][i2]);
                }
            }
            // Report
            report->block_sad[j * width_in_8s + i] = sad;
            report->block_sad_apps1[j * width_in_8s + i] = sad_apps1;
            report->block_sad_apps2[j * width_in_8s + i] = sad_apps2;
            report->block_sad_apps3[j * width_in_8s + i] = sad_apps3;
            report->block_sad_apps4[j * width_in_8s + i] = sad_apps4;
            report->block_sse[j * width_in_8s + i] = sse;
            report->block_sse_apps1[j * width_in_8s + i] = sse_apps1;
            report->block_sse_apps2[j * width_in_8s + i] = sse_apps2;
            report->block_sse_apps3[j * width_in_8s + i] = sse_apps3;
            report->block_sse_apps4[j * width_in_8s + i] = sse_apps4;
            //report->acc_sad += sad;
            //report->acc_sad_apps1 += static_cast<unsigned long int>(sad_apps1);
            //report->acc_sad_apps2 += static_cast<unsigned long int>(sad_apps2);
            //report->acc_sad_apps3 += static_cast<unsigned long int>(sad_apps3);
            //report->acc_sad_apps4 += static_cast<unsigned long int>(sad_apps4);
            //report->acc_sse += sse;
            //report->acc_sse_apps1 += static_cast<unsigned long int>(sse_apps1);
            //report->acc_sse_apps2 += static_cast<unsigned long int>(sse_apps2);
            //report->acc_sse_apps3 += static_cast<unsigned long int>(sse_apps3);
            //report->acc_sse_apps4 += static_cast<unsigned long int>(sse_apps4);
        }
    }
}

int yuv::subtratorImpreciso(int op1, int op2, int BitsIMP) // RAFAEL
{
    std::bitset<32> a(op1);
    std::bitset<32> b(op2);
    std::bitset<32> t(0);
    std::bitset<33> bou(0);
    int result = 0;

    for (int i = 0; i < 32; i++) {
        if (i < BitsIMP) {
            if ((a[i] == 0 && b[i] == 0 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 0;
            }
            if ((a[i] == 0 && b[i] == 0 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 0;
            }
            if ((a[i] == 0 && b[i] == 1 && bou[i] == 0)) {
                bou[i + 1] = 1; t[i] = 1;
            }
            if ((a[i] == 0 && b[i] == 1 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 1;
            }
            if ((a[i] == 1 && b[i] == 0 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 1;
            }
            if ((a[i] == 1 && b[i] == 0 && bou[i] == 1)) {
                bou[i + 1] = 0; t[i] = 1;
            }
            if ((a[i] == 1 && b[i] == 1 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 0;
            }
            if ((a[i] == 1 && b[i] == 1 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 0;
            }
        }
        else {
            if ((a[i] == 0 && b[i] == 0 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 0;
            }
            if ((a[i] == 0 && b[i] == 0 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 1;
            }
            if ((a[i] == 0 && b[i] == 1 && bou[i] == 0)) {
                bou[i + 1] = 1; t[i] = 1;
            }
            if ((a[i] == 0 && b[i] == 1 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 0;
            }
            if ((a[i] == 1 && b[i] == 0 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 1;
            }
            if ((a[i] == 1 && b[i] == 0 && bou[i] == 1)) {
                bou[i + 1] = 0; t[i] = 0;
            }
            if ((a[i] == 1 && b[i] == 1 && bou[i] == 0)) {
                bou[i + 1] = 0; t[i] = 0;
            }
            if ((a[i] == 1 && b[i] == 1 && bou[i] == 1)) {
                bou[i + 1] = 1; t[i] = 1;
            }

        }

        a[i] = 0;
        b[i] = 0;
    }
    result = (int)(t.to_ulong());

    return (result);
}