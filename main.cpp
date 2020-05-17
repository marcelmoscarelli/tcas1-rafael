#include <iostream>
#include <string>
#include "yuv.h"
#include "results.h"

using namespace std;

int main(int argc, char ** argv)
{
    // How to use it:
    // tcas1-rafael <path to yuv file> <number of frames> <width> <height>
    // tcas1-rafael C:\my-yuv-folder\Johnny_1280x720_60.yuv 600 1280 720
    if (argc != 5) {
        cerr << "Invalid number of arguments." << endl;
        exit(-1);
    }

    yuv * video = new yuv();
    video->open_file(argv[1], stoi(argv[2], nullptr, 10), stoi(argv[3], nullptr, 10), stoi(argv[4], nullptr, 10));

    results * report = new results(video->width, video->height, video->num_frames);

    for (int frame = 1; frame < video->num_frames; ++frame) {
        video->load_nth_frame(frame);
        video->pred_cur_frame(report);
        //break;
    }

    report->write_csv(argv[1]);

    delete report;
    delete video;
}