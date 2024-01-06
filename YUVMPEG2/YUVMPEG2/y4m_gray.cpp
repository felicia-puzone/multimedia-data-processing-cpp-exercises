
#include <iostream>
#include <vector>
#include "mat.h"
#include <fstream>
#include "ppm.h"
#include "pgm.h"
#include <sstream>


bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames)
{
    std::ifstream is(filename, std::ios::binary);

    if (!is)
    {
        std::cout << "error opening input file. Exiting...\n";
        return false;
    }

    std::string magic;
    is >> magic;

    if (magic != "YUV4MPEG2")
    {
        std::cout << "Not a yuv4mpeg file. Be careful.\n";
        return false;
    }

    //HEADER
    is.ignore();

    int height = 0;
    int width = 0;
    std::string chroma;

    for (char extracted; extracted = is.get();)
    {

        if (extracted == '\n') break;

        if (extracted == 'H')
        {
            is >> height;
        }

        if (extracted == 'W')
        {
            is >> width;
        }

        if (extracted == 'C')
        {
            is >> chroma;
        }

        if (extracted == 'I') { std::string x; is >> x; }
        if (extracted == 'F') { std::string f; is >> f; }
        if (extracted == 'A') { std::string a; is >> a; }
        if (extracted == 'X') { std::string x; is >> x; }

        if (extracted == ' ') {}

        if(extracted != 'H' && extracted != 'W' && extracted != 'C' && extracted != 'I' && extracted != 'F' && extracted != 'A' && extracted != 'X' && extracted != ' ')
        {
            std::cout << "An error occurred in the header.\n";
            return false;
        }
    }


    //Image loop

    // Y full size, Cr Cb 1/4 of the size
    int frame_size = height * width + 2 * ((height * width) / 4);

    for (std::string frame_tag; is >> frame_tag;)
    {
        if (frame_tag == "FRAME")
        {   
            is.ignore();
            if (is.peek() == 'I') {}
            if (is.peek() == 'X') {}

            mat<uint8_t> frame(height, width);
            mat<uint8_t> frameCb(height/2, width/2);
            mat<uint8_t> frameCr(height/2, width/2);

            for (auto it = frame.begin();
                it != frame.end(); ++it)
                *it = is.get();

            for (auto it = frameCb.begin();
                it != frameCb.end(); ++it)
                *it = is.get();

            for (auto it = frameCr.begin();
                it != frameCr.end(); ++it)
                *it = is.get();

            frames.push_back(frame);

            std::cout << "debug";

        }
    }


    return true;
}