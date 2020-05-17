#include <fstream>
#include <iostream>
#include <string>
#include "results.h"

results::results(int width, int height, int num_frames)
{
    num_blocks = width * height * (num_frames - 1) / 64;
    block_sad = new int[num_blocks];
    block_sad_apps1 = new int[num_blocks];
    block_sad_apps2 = new int[num_blocks];
    block_sad_apps3 = new int[num_blocks];
    block_sad_apps4 = new int[num_blocks];
    block_sse = new int[num_blocks];
    block_sse_apps1 = new int[num_blocks];
    block_sse_apps2 = new int[num_blocks];
    block_sse_apps3 = new int[num_blocks];
    block_sse_apps4 = new int[num_blocks];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            acc_sad[i][j] = 0;
            acc_sad_apps1[i][j] = 0;
            acc_sad_apps2[i][j] = 0;
            acc_sad_apps3[i][j] = 0;
            acc_sad_apps4[i][j] = 0;
            acc_sse[i][j] = 0;
            acc_sse_apps1[i][j] = 0;
            acc_sse_apps2[i][j] = 0;
            acc_sse_apps3[i][j] = 0;
            acc_sse_apps4[i][j] = 0;
        }
    }
}

results::~results()
{
    delete[] block_sad;
    delete[] block_sad_apps1;
    delete[] block_sad_apps2;
    delete[] block_sad_apps3;
    delete[] block_sad_apps4;
    delete[] block_sse;
    delete[] block_sse_apps1;
    delete[] block_sse_apps2;
    delete[] block_sse_apps3;
    delete[] block_sse_apps4;
}

void results::write_csv(const char * video_file)
{
    std::ofstream csv_file;
    std::string s(video_file);
    std::size_t f = s.rfind("\\");
    std::string s2;
    if (f != std::string::npos) {
        s2 = s.substr(f + 1);
    }
    else {
        f = s.rfind("/");
        if (f != std::string::npos) {
            s2 = s.substr(f + 1);
        }
    }
    std::cout << "Writing report 1 of 2" << std::endl;
    csv_file.open("[" + s2 + "]_distorcao_por_bloco.csv", std::ios::out);
    csv_file << "Block ID;SAD;SAD Apps1;SAD Apps2;SAD Apps3;SAD Apps4;SSE;SSE Apps1;SSE Apps2;SSE Apps3;SSE Apps4" << std::endl;
    for (int i = 0; i < num_blocks; ++i) {
        csv_file << i << ";";
        csv_file << block_sad[i] << ";";
        csv_file << block_sad_apps1[i] << ";";
        csv_file << block_sad_apps2[i] << ";";
        csv_file << block_sad_apps3[i] << ";";
        csv_file << block_sad_apps4[i] << ";";
        csv_file << block_sse[i] << ";";
        csv_file << block_sse_apps1[i] << ";";
        csv_file << block_sse_apps2[i] << ";";
        csv_file << block_sse_apps3[i] << ";";
        csv_file << block_sse_apps4[i] << std::endl;
    }
    csv_file.close();

    std::cout << "Writing report 2 of 2" << std::endl;
    csv_file.open("[" + s2 + "]_distorcao_por_amostra.csv", std::ios::out);
    csv_file << "MIN SAD;MAX SAD;MIN SSE;MAX SSE;Blocks" << std::endl;
    csv_file << "0;" << (255 * num_blocks) << ";0;" << (65025 * num_blocks) << ";" << num_blocks << std::endl << std::endl;

    csv_file << "SAD;;;;;;;;;SAD Apps1;;;;;;;;;SAD Apps2;;;;;;;;;SAD Apps3;;;;;;;;;SAD Apps4" << std::endl;
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sad[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sad_apps1[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sad_apps2[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sad_apps3[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sad_apps4[j][i] << ";";
        }
        csv_file << std::endl;
    }
    csv_file << std::endl << "SSE;;;;;;;;;SSE Apps1;;;;;;;;;SSE Apps2;;;;;;;;;SSE Apps3;;;;;;;;;SSE Apps4" << std::endl;
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sse[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sse_apps1[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sse_apps2[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sse_apps3[j][i] << ";";
        }
        csv_file << ";";
        for (int i = 0; i < 8; ++i) {
            csv_file << acc_sse_apps4[j][i] << ";";
        }
        csv_file << std::endl;
    }
    csv_file.close();
}
