#include <iostream>
#include <string>
#include "yuv.h"

using namespace std;

int main(int argc, char ** argv)
{
    if (argc != 5) {
        cerr << "Invalid number of arguments." << endl;
        exit(-1);
    }

    yuv * video = new yuv();
    video->open_file(argv[1], stoi(argv[2], nullptr, 10), stoi(argv[3], nullptr, 10), stoi(argv[4], nullptr, 10));
}