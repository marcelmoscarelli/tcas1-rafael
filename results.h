#pragma once

class results
{
public:
    int num_blocks;
    unsigned long int acc_sad[8][8];
    unsigned long int acc_sad_apps1[8][8];
    unsigned long int acc_sad_apps2[8][8];
    unsigned long int acc_sad_apps3[8][8];
    unsigned long int acc_sad_apps4[8][8];
    unsigned long int acc_sse[8][8];
    unsigned long int acc_sse_apps1[8][8];
    unsigned long int acc_sse_apps2[8][8];
    unsigned long int acc_sse_apps3[8][8];
    unsigned long int acc_sse_apps4[8][8];
    int * block_sad;
    int * block_sad_apps1;
    int * block_sad_apps2;
    int * block_sad_apps3;
    int * block_sad_apps4;
    int * block_sse;
    int * block_sse_apps1;
    int * block_sse_apps2;
    int * block_sse_apps3;
    int * block_sse_apps4;

    results(int width, int height, int num_frames);
    ~results();

    void write_csv(const char * video_file);
};