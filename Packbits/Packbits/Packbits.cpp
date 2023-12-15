#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(int argv, char **argc)
{
    std::cout << "Hello Packbits!\n";
    if (argv != 4)
    {
        printf("wrong number of parameters: <c|d> <input filename> <output filename>");
        exit(1);
    }

    std::string filename_in = argc[2];
    std::string filename_out = argc[3];

    std::ifstream is(filename_in, std::ios::binary);

    if (is.fail())
    {
        printf("could not open file. Exiting...\n");
        exit(1);
    }

    std::ofstream os(filename_out);
   
    if (*argc[1] == 'c') 
    {
        printf("Compressing...\n");

   
        char peeked;
        int c_counter = 0;
        bool loop = true;


        while (loop) 
        {
            peeked = is.peek();
            
            if (peeked != EOF)
            {
                is.get();
                c_counter++;

                while (is.peek() == peeked)
                {
                    c_counter++;
                    is.ignore();
                }

                // RUN
                if (c_counter != 1)
                {
                    int command = 257 - c_counter;
                    os << command;
                    os << peeked;

                    c_counter = 0;
                }

                //COPY
                if (c_counter == 1)
                {
                    int c_counter_copy = 0;
                    std::stringstream ss;
                    ss << peeked;

                    while (is.peek() != peeked)
                    {

                        c_counter_copy++;
                        char c = is.get();
                        
                            

                        peeked = c;

                        if (c != is.peek())
                        {
                            ss << c;
                        }

                        else
                        {
                            peeked = c;
                        }

                    }


                    os << c_counter_copy;
                    os << ss.str();

                }
            }

            else loop = false;
        }

        os << 128;

    }

    if (*argc[1] == 'd')
    {
        printf("Decompressing...\n");

        int command = 0;
        char code;
        bool loop = true;
        
        while (loop)
        {
            is >> command;
            if (command == 128)
            {
                loop = false;
                break;
            }

            if (129 <= command && command <= 255)
            {
                is >> code;
                for (int i = 0; i < (257 - command); i++)
                {
                
                    os << code;
                }
                    
            }

            if (0 <= command && command <= 127)
            {
                for (int i = 0; i < command; i++)
                {
                    is >> code;
                    os << code;
                }
            }

        }


    }

    is.close();
    os.close();

}
