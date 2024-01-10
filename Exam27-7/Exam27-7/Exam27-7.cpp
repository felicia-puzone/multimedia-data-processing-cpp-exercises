// Exam27-7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>


bool lzvn_decode(std::string inputfile, std::string outputfile)
{

    std::ifstream is(inputfile, std::ios::binary);
    std::ofstream os(outputfile, std::ios::binary);

    if (!is)
    {
        std::cout << "failed opening input file\n";
        return false;
    }

    if (!os)
    {
        std::cout << "failed opening output file\n";
        return false;
    }

    std::string magic;
    is.read(&magic[0], 4);

    uint32_t output_len;
    is.read(reinterpret_cast<char*>(&output_len), 4);

    uint32_t payload_len;
    is.read(reinterpret_cast<char*>(&payload_len), 4);


    std::stringstream output;

    while (payload_len)
    {

        uint8_t byte = 0;

        is.read(reinterpret_cast<char*>(&byte), 1);

        uint16_t distance = 0;

        std::cout << "bog";


        //sml_l

        if ((byte & 0xE0) == 0xE0)
        {

            std::cout << "sml_l";

            //how many literals?

            uint8_t ll = (byte & 0x0F);

            payload_len = payload_len - ll - 1;

            char lit;
            while (ll)
            {
                is.read(&lit, 1);
                output << lit;
                --ll;
            }


            std::cout << "ok sml_l";

        } 
        

        //lrg_l

        //lrg_m

        //sml_m

        //pre_d

        //lrg_d

        //med_d



        else if (byte == 0x0E || byte == 0x16 || byte == 0x06 || byte == 0x1E || byte == 0x26 || byte == 0x2E || byte == 0x36 ||
            byte == 0x3E || (byte & 0xF0) == 0xF0 || (byte & 0xD0) == 0xD0)
        {
            std::cout << "strange byte detected.\n";
            --payload_len;
        }

        //sml_d
        else
        {
            //retrieve literals
            //retrieve match
            //retrieve distance

            std::cout << "sml_d";

            //how many literals?

            uint8_t ll = (byte & 0xC0) >> 6;
            uint8_t mm = ((byte & 0x38) >> 3) + 3;

            uint16_t dd = (byte & 0x07) << 8;

            payload_len = payload_len - ll - 2;

            is.read(reinterpret_cast<char*>(&byte), 1);

            dd = (dd | byte);
            distance = dd;

            
            //Add literal
            char lit;
            while (ll)
            {
                is.read(&lit, 1);
                output << lit;
                --ll;
 
            }

            //add cycled bytes

            std::string tmp = output.str().substr(output.str().length() - dd, output.str().length());

            while (mm)
            {
                output << (tmp[mm % 4]);
                --mm;
            }

            std::cout << "ok sml_d";

        }


    }


}

int main(int argc, char** argv)
{
    std::cout << "Hello LZVN!\n";

    lzvn_decode(argv[1], argv[2]);

}

