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

    uint16_t distance = 0;

    while (payload_len)
    {

        uint8_t byte = 0;

        is.read(reinterpret_cast<char*>(&byte), 1);


        //sml_l

        if (byte >> 4 == 0x0E && (byte != 0xE0))
        {

            std::cout << "sml_l_";

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


            std::cout << "ok sml_l\n";

        } 
        

        //lrg_l

        else if (byte == 0xE0)
        {
            std::cout << "lrg_l_";

            uint8_t ll = 0;
            is.read(reinterpret_cast<char*>(&ll), 1);

            payload_len = payload_len - ll - 2;

            ll = ll + 16;
            char lit;
            while (ll)
            {
                is.read(&lit, 1);
                output << lit;
                --ll;
            }

            std::cout << output.str();
            std::cout << "ok lrg_l\n";

        }

        //lrg_m

        else if (byte == 0xF0)
        {
            std::cout << "lrg_m_";
            payload_len = payload_len - 2;

            uint8_t mm = 0;

            is.read(reinterpret_cast<char*>(&mm), 1);

            mm = mm + 16;

            std::string tmp = output.str().substr(output.str().length() - distance, output.str().length());

            int i = 0;
            while (i != mm)
            {
                output << (tmp[i % distance]);
                ++i;
            }

            std::cout << "ok lrg_m\n";
        }

        //sml_m

        else if ((byte >> 4) == 0x0F)
        {

            std::cout << "sml_m_";

            payload_len = payload_len - 1;

            uint8_t mm = (byte & 0x0F);

            //it uses last distance set: distance

                        //add cycled bytes

            std::string tmp = output.str().substr(output.str().length() - distance, output.str().length());

            int i = 0;
            while (i != mm)
            {
                output << (tmp[i % distance]);
                ++i;
            }

            std::cout << "ok sml_m\n";

        }

        //pre_d

        else if ((byte & 0x07) == 0x06 && byte != 0x06)
        {
            std::cout << "pre_d_";

            //how many literals?

            uint8_t ll = (byte & 0xC0) >> 6;
            uint8_t mm = ((byte >> 3) & 0x07) + 3;

            payload_len = payload_len - ll - 1;


            //Add literal
            char lit;
            while (ll)
            {
                is.read(&lit, 1);
                output << lit;
                --ll;

            }

            //add cycled bytes

            std::string tmp = output.str().substr(output.str().length() - distance, output.str().length());

            int i = 0;
            while (i != mm)
            {
                output << (tmp[i % distance]);
                ++i;
            }

            std::cout << "ok pre_d\n";
        }

        //lrg_d

        else if ((byte & 0x07) == 0x07)
        {
            std::cout << "lrg_d_";

            //how many literals?

            uint8_t ll = (byte & 0xC0) >> 6;
            uint8_t mm = ((byte & 0x38) >> 3) + 3;
            uint16_t dd_ms = 0;
            uint16_t dd_ls = 0;
            uint16_t dd = 0;

            is.read(reinterpret_cast<char*>(&dd_ls), 1);
            is.read(reinterpret_cast<char*>(&dd_ms), 1);

            dd = (dd_ms << 8) | dd_ls;
            distance = dd;

            payload_len = payload_len - ll - 3;


            //Add literal
            char lit;
            while (ll)
            {
                is.read(&lit, 1);
                output << lit;
                --ll;

            }

            //add cycled bytes

            std::string tmp = output.str().substr(output.str().length() - distance, output.str().length());

            int i = 0;
            while (i != mm)
            {
                output << (tmp[i % distance]);
                ++i;
            }

            std::cout << "ok lrg_d\n";

        }

        //med_d

        else if ((byte >> 5) == 0x05)
        {
          std::cout << "med_d_";

          std::cout << "\n" << output.str();
          //how many literals?

          uint8_t ll = (byte & 0x18) >> 3;
          uint8_t mm_msb = (byte & 0x07);

          is.read(reinterpret_cast<char*>(&byte), 1);
          uint8_t mm_lsb = (byte & 0x03);
          uint16_t dd_ls = (byte >> 2);

          uint16_t dd_ms = 0;
          is.read(reinterpret_cast<char*>(&dd_ms), 1);
     
          uint16_t dd = 0;

          dd = dd_ls | (dd_ms << 6);

          distance = dd;

          uint8_t mm = 0;
          mm = mm_lsb | (mm_msb << 2);
          mm = mm + 3;

          payload_len = payload_len - ll - 3;


          //Add literal
          char lit;
          while (ll)
          {
              is.read(&lit, 1);
              output << lit;
              --ll;

          }

          //add cycled bytes
          std::cout << "\n" << output.str();
          std::string tmp = output.str().substr(output.str().length() - distance, output.str().length());

          int i = 0;
          while (i != mm)
          {
              output << (tmp[i % distance]);
              ++i;
          }

          std::cout << "ok med_d\n";
        }

        else if (byte == 0x0E || byte == 0x16 || byte == 0x1E || byte == 0x26 || byte == 0x2E || byte == 0x36 ||
            byte == 0x3E || (byte & 0xF0) == 0xF0 || (byte & 0xD0) == 0xD0)
        {
            std::cout << "strange byte detected.\n";
            --payload_len;
        }

        //eos

        else if (byte == 0x06)
        {
            std::cout << '\n' << output.str() << '\n';
            --payload_len;

            for (int i = 0; i < 7; ++i)
            {
                is.read(reinterpret_cast<char*>(&byte), 1);
                if (byte == 0) --payload_len;
            }
            //Ending

                std::string end_magic;
                is >> end_magic;
                if (end_magic == "bvx$")
                {
                    std::cout << "Tutt appost fratm gg.\n";
                    payload_len = 0;
                }
        }

        //sml_d
        
        else
        {
            //retrieve literals
            //retrieve match
            //retrieve distance

            std::cout << "sml_d_";

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

            int i = 0;
            while (i != mm)
            {
                output << (tmp[i % dd]);
                ++i;
            }

            std::cout << "ok sml_d\n";

        }

    }

    os << output.str();
    os.close();
    is.close();

    return true;

}

int main(int argc, char** argv)
{
    std::cout << "Hello LZVN!\n";

    lzvn_decode(argv[1], argv[2]);

}

