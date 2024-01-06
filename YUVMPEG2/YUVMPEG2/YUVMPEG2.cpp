
#include <iostream>
#include <vector>
#include "mat.h"
#include <fstream>
#include "ppm.h"
#include "pgm.h"
#include <sstream>
#include "types.h"



uint8_t thresholdY(uint8_t value)
{
    if (value < 16) return 16;
    if (value > 235) return 235;

    return value;
}

uint8_t thresholdC(uint8_t value)
{
    if (value < 16) return 16;
    if (value > 240) return 240;

    return value;
}

uint8_t thresholdRGB(int value)
{
    if (value < 0) return 0;
    if (value > 255) return 255;

    return (uint8_t)value;
}

vec3b RGB_conversion(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int R = 1.164 * (thresholdY(Y) - 16) + (1.596 * (thresholdC(Cr) - 128) );
    int G = 1.164 * (thresholdY(Y) - 16) - (0.392 * (thresholdC(Cb) - 128)) - (0.813 * (thresholdC(Cr) - 128));
    int B = 1.164 * (thresholdY(Y) - 16) + (2.017 * (thresholdC(Cb) - 128));

    vec3b rgb;
    rgb[0] = thresholdRGB(R);
    rgb[1] = thresholdRGB(G);
    rgb[2] = thresholdRGB(B);

    return rgb;
}


bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames)
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

        if (extracted != 'H' && extracted != 'W' && extracted != 'C' && extracted != 'I' && extracted != 'F' && extracted != 'A' && extracted != 'X' && extracted != ' ')
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

            mat<uint8_t> frameY(height, width);
            mat<uint8_t> frameCb(height / 2, width / 2);
            mat<uint8_t> frameCr(height / 2, width / 2);

            for (auto it = frameY.begin();
                it != frameY.end(); ++it)
                *it = is.get();

            for (auto it = frameCb.begin();
                it != frameCb.end(); ++it)
                *it = is.get();

            for (auto it = frameCr.begin();
                it != frameCr.end(); ++it)
                *it = is.get();

            
            mat<vec3b> frame_rgb(height, width);

            
            for (int i = 0; i < frame_rgb.rows(); i++)
            {
                for (int j = 0; j < frame_rgb.cols(); j++)
                {
                    uint8_t y = frameY(i, j);
                    uint8_t cb = frameCb(i / 2, j / 2);
                    uint8_t cr = frameCr(i / 2, j / 2);

                    frame_rgb(i, j) = RGB_conversion(y, cb, cr);
                }
            }

            frames.push_back(frame_rgb);
            
            std::cout << "helo";

        }
    }


    return true;
}


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


int main(int argc, char** argv)
{
    std::cout << "Hello YUVMPEG2!\n";

    std::vector<mat<uint8_t>> gray_frames;

    std::vector<mat<vec3b>> rgb_framez;

    y4m_extract_color(argv[1], rgb_framez);

    for (int i = 0; i < rgb_framez.size(); i++)
    {
        std::stringstream ss;
        ss << "output_ppm" << i << ".ppm";
        save_ppm(ss.str(), rgb_framez[i]);
    }

    /*y4m_extract_gray(argv[1], gray_frames);

    for (int i = 0; i < gray_frames.size(); i++)
    {
        std::stringstream ss;
        ss << "output_pgm" << i << ".pgm";
        save_pgm(ss.str(), gray_frames[i]);
    } */

    return 0;

}

