
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <cmath>

template <typename T>
std::ostream& raw_write(std::ostream& os, const T& value, size_t size = sizeof(T))
{
    return os.write(reinterpret_cast<const char*>(&value), size);
}

class Bitwriter {
    std::ostream& os_;
    uint8_t buffer_ = 0;
    int n_ = 0;

    std::ostream& write_bit(uint32_t bit)
    {
        
        buffer_ = (buffer_ << 1) | (bit & 1);
        ++n_;
        
        if (n_ == 8)
        {
            raw_write(os_, buffer_);
            n_ = 0;
        }
        return os_;
    }

public:
    
    Bitwriter(std::ostream& os) : os_(os) {}

    std::ostream& write(uint32_t value, uint8_t n_bits)
    {
        while (n_bits-- > 0)
        {
            write_bit(value >> n_bits);
        }
        return os_;
    }

    std::ostream& operator()(uint32_t value, uint8_t n_bits)
    {
        return write(value, n_bits);
    }

    std::ostream& flush(uint32_t value = 0)
    {
        while (n_ > 0)
        {
            write_bit(value);
        }
        return os_;
    }

    ~Bitwriter()
    {
        flush();
    }
};


template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
    return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
    uint8_t buffer_ = 0;
    uint8_t n_ = 0;
    std::istream& is_;

public:
    bitreader(std::istream& is) : is_(is) {}

    uint32_t read_bit() {
        if (n_ == 0) {
            raw_read(is_, buffer_);
            n_ = 8;
        }
        --n_;
        return (buffer_ >> n_) & 1;
    }

    uint32_t read(uint8_t n) {
        uint32_t u = 0;
        while (n-- > 0) {
            u = (u << 1) | read_bit();
        }
        return u;
    }

    uint32_t operator()(uint8_t n) {
        return read(n);
    }

};




int main(int argc, char** argv)
{
    std::cout << "Hello Elias!\n";


    if (argc != 4)
    {
        std::cout << "Wrong number of parameters. [c|d] input.txt output.txt\n";
        return 1;
    }

    std::ifstream is(argv[2], std::ios::binary);
    std::ofstream os(argv[3], std::ios::binary);

    if (!is)
    {
        std::cout << "error opening input file.\n";
        return 1;
    }

    if (!os)
    {
        std::cout << "error opening output file.\n";
        return 1;
    }

    if (*argv[1] == 'c')
    {
        std::cout << "Compressing...\n";

        std::vector<int> v{
            std::istream_iterator<int>(is),
                std::istream_iterator<int>()
        };
        std::cout << "dsdjhd";

        Bitwriter bitwriter(os);


        for (auto& elem : v)
        {
            if (elem >= 0)
            {
                bitwriter(0, log2(elem) + 1);
                bitwriter(elem, log2(elem) + 1);
                bitwriter(1, 1);
            }

            if (elem < 0)
            {
                bitwriter(0, log2(abs(elem)) + 1);
                bitwriter(abs(elem), log2(abs(elem)) + 1);
                bitwriter(0, 1);
            } 
        }
    }

    if (*argv[1] == 'd')
    {
        std::cout << "Decompressing...\n";

        std::vector<int> v;

        bitreader bitreader(is);
        
        int zero_count = 0;
        uint32_t bit = 0;

            while (is)
            {
                bit = bitreader.read(1);
                if (bit == 0) zero_count++;

                if (bit != 0)
                {
                    if (zero_count == 0)
                    {
                        //Zero case
                        os << "0\n";
                    }

                    if (zero_count != 0)
                    {
                        // Not zero

                        //read only if bit is not already the full number
                        if ((zero_count - 1) > 0)
                        {
                            bit = (bit << (zero_count - 1)) | bitreader.read(zero_count - 1);

                        }

                        //otherwise read only the sign
                        int sign = bitreader.read(1);


                        //positive
                        if (sign == 1)
                        {
                            os << bit << '\n';
                        }

                        //negative
                        if (sign == 0)
                        {
                            os << '-' << bit << '\n';
                        }

                        zero_count = 0;
                        std::cout << "dsjfhdsjk";
                    }

                }
            }

        }

    is.close();
    os.close();

    return 0;

}

