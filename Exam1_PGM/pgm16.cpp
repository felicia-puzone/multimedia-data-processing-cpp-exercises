
#include <iostream>
#include "mat.h"
#include <fstream>
#include "pgm16.h"


bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue) {
    
    std::ifstream is;

    is.open(filename, std::ios::binary);

    if (is.fail())
    {
        std::cout << "File not opened succesfully. Exiting..." << std::endl;
        return false;
    }

    std::cout << "File opened succesfully." << std::endl;

    std::string magic;

    std::string comment;

    int width;
    int height;

    std::string next;

    std::string other;

    is >> magic;

    is >> next;

    if (next == "#")
    {
        std::getline(is, comment);
        is >> width;
        is >> height;
        is >> maxvalue;

        ///////////////////////
        // Creating th matrix

        img.resize(height, width);

        is.ignore();

        uint16_t first_byte;
        uint16_t second_byte;


        for (auto slot = img.begin();
            slot != img.end(); ++slot)
        {
            if (maxvalue < 256) {
                *slot = is.get();
           }

            else 
            {
                first_byte = is.get();
                second_byte = is.get();

                *slot = (first_byte << 8) + second_byte;
            }

        }
    }

    else {

        is >> width;
        is >> height;
        is >> maxvalue;

        //same as above ...

    }
    
    return true;

}


int main(int argc, char** argv)
{

    mat<uint16_t> img;
    uint16_t maxvalue;

    std::string filename = argv[1];

    bool ok = load(filename, img, maxvalue);

}