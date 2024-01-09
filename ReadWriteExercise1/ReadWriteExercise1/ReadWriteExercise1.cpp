
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>


std::string byte_to_hex(const unsigned char &byte)
{
    std::string hex;
    //fino a 9 dec da 10 a 15 A-F
    
    //MS 4-bits
    char first = 0;

    if ((byte >> 4) >= 0 && (byte >> 4) < 10) first = 48 + (byte >> 4);

    else if ((byte >> 4) >= 10 && (byte >> 4) < 16) first = 65 + ( (byte >> 4)  - 10);

    //LS 4-bits

    char second = 0;

    if ((byte & 0x0F) >= 0 && (byte & 0x0F) < 10) second = 48 + (byte & 0x0F);

    else if ((byte & 0x0F) >= 10 && (byte & 0x0F) < 16) second = 65 + ((byte & 0x0F) - 10);


    hex.insert(0, 1, first);
    hex.insert(1, 1, second);

    return hex;
}

int frequencies(std::istream& is, std::ostream& os)
{

    std::map<unsigned char, int> mymap;
    std::string hex;

    for (char byte = 0; is.read(&byte, 1);)
    {
        
        //byte_to_hex(byte, hex);

        if (mymap.count((unsigned char)byte) == 0)
        {
            mymap.insert(std::pair<unsigned char, int>((unsigned char)byte, 1));
        }

        else
        {
            if (mymap.find((unsigned char)byte) != mymap.end()) {
                mymap[(unsigned char)byte]++;
            }
        }

    }

    for (const auto& n : mymap)
       os << byte_to_hex(n.first) << '\t' << n.second << '\n';

    return 0;
}

template <typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T))
{
    return os.write(reinterpret_cast<const char*>(&val), size);
}

int write_32(std::istream& is, std::ostream& os)
{

    std::vector<int16_t> v{
        std::istream_iterator<int16_t>(is),
            std::istream_iterator<int16_t>()
    };

    for (const auto& valuez : v)
    {
        raw_write(os, valuez);
    }

    return 0;
}

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) 
{
    return is.read(reinterpret_cast<char*> (&val), size);
}

int read_32(std::istream& is, std::ostream& os)
{
    std::vector<int16_t> v;

    

    for (int16_t valu = 0;  raw_read(is, valu);)
    {
        v.push_back(valu);
    }

    for (const auto& valu : v) {
        os << valu << '\n';
    }

    return 0;
}

class bitwriter
{
    std::ostream& os_;
    int n_ = 0;
    uint8_t buffer_;

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
    bitwriter(std::ostream& os) : os_(os) {}

    std::ostream& write(uint32_t value, uint8_t n_bits)
    {
        while (n_bits --> 0)
        {
            write_bit(value >> n_bits);
        }
        return os_;
    }

    std::ostream& flush(uint32_t value = 0)
    {
        while (n_ > 0)
        {
            write_bit(value);
        }
        return os_;
    }

    std::ostream& operator()(uint32_t value, uint8_t n_bits)
    {
        return write(value, n_bits);
    }

    ~bitwriter() 
    {
        flush();
    }
};

class Bitreader
{
    uint8_t buffer_ = 0;
    uint8_t n_ = 0;
    std::istream& is_;

public:
    Bitreader(std::istream& is) : is_(is) {}

    uint32_t read_bit()
    {
        if (n_ == 0)
        {
            raw_read(is_, buffer_);
            n_ = 8;
        }

        --n_;

        return (buffer_ >> n_) & 1;
    }

public:
    uint32_t read(uint32_t n_bit)
    {
        uint32_t value = 0;

        while (n_bit-- > 0)
        {
            value = (value << 1) | read_bit();
        }
        return value;
    }
};

int write_int11(std::istream& is, std::ostream& os)
{

    bitwriter bw(os);

    std::vector<int16_t> v{
        std::istream_iterator<int16_t>(is),
            std::istream_iterator<int16_t>()
    };

    for (const auto& valu : v)
    {
        bw(valu, 11);
    }

    return 0;
}

int read_int11(std::istream& is, std::ostream& os) 
{
    int32_t number = 0;
    Bitreader br(is);

    while (is)
    {
        number = br.read(11);

        os << number << '\n';
    }

    return 0;
}



//PARAM: FILE_IN FILE_OUT
int main(int argc, char** argv)
{
    std::cout << ":P \n";

    std::ifstream is(argv[1], std::ios::binary);

    if (!is)
    {
        std::cout << "Error opening input file\n";
        return 1;
    }

    std::ofstream os(argv[2], std::ios::binary);

    if (!os)
    {
        std::cout << "Error opening output file\n";
        return 1;
    }

    //Exercise 1
    //frequencies(is, os);

    //Exercise 2
    //write_32(is, os);

    //Exercise 3
    //read_32(is, os);

    //Exercise 4
    //dubbio: il bitwriter stampa in 11 bit dei numeri che originariamente sono in 16 bit prendendo un valore del vettore alla volta. i bit che scarta sono tutti trascurabili
    //nella codifica? (sì dovrebbero essere 0)
    //write_int11(is, os);

    //Exercise 5

    read_int11(is, os);

    is.close();
    os.close();


    return 0;
}

