#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <cassert>

typedef uint8_t GRAYSCALE_8;
typedef uint8_t GRAYSCALE_16;
typedef std::array<uint8_t, 3> RGB_8;
typedef std::array<uint16_t, 3> RGB_16;

template <typename T>
class PAM_IMG 
{
    int max_val_ = 0;
    int n_row_ = 0;
    int n_col_ = 0;
    int depth_ = 0;
    std::string tupletype_;
    std::vector<T> data_;

public:
    PAM_IMG(int max_val, int n_row, int n_col, int depth, std::string tupletype) : max_val_(max_val), n_row_(n_row), n_col_(n_col), depth_(depth), tupletype_(tupletype), data_(n_row * n_col) {};

    int rows() { return n_row_; }
    int cols() { return n_col_; }
    int maxval() { return max_val_; }
    int depth() { return depth_; }
    std::string tupletype() { return tupletype_; }
    std::vector<T> data() { return data_; }
    

    T& const operator()(int nrow, int ncol) const
    {
        assert(nrow >= 0 && nrow < n_row_ && ncol >= 0 && ncol < n_col_);
        return data_[nrow * n_col_ + ncol];
    }

    T& operator()(int nrow, int ncol)
    {
        assert(nrow >= 0 && nrow < n_row_ && ncol >= 0 && ncol < n_col_);
        return data_[nrow * n_col_ + ncol];
    }

    T& insert_at(int pos, const T val)
    {
        data_[pos] = val;
        return data_[pos];
    }
};

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& value, size_t size = sizeof(T))
{
    return os.write(reinterpret_cast<const char*>(&value), size);
}

template<typename T>
std::ostream& write_pam(std::ostream& os, PAM_IMG<T>& img)
{
    //HEADER 
    os << "P7\n" << "WIDTH" << " " << img.cols() << "\n" << "HEIGHT" << " " << img.rows() << "\n" << "DEPTH" << " " << img.depth() << "\n"
        << "MAXVAL" << " " << img.maxval() << "\n" << "TUPLTYPE" << " " << img.tupletype() << "\n" << "ENDHDR" << "\n";

    //DATA

    for (auto& elem : img.data())
    {
        raw_write(os, elem);
    }

    return os;
}

template<typename T>
PAM_IMG<T>& color_gradient(PAM_IMG<T>& pam_image)
{
    for (int i = 0; i < pam_image.rows(); i++)
    {
        for (int j = 0; j < pam_image.cols(); j++)
            pam_image(i, j) = i;
    }

    return pam_image;
}

template<typename T>
std::istream& raw_read(std::istream& is, T& value, size_t size = sizeof(T))
{
    return is.read(reinterpret_cast<char*> (&value), size);
}

template<typename T>
PAM_IMG<T> load_pam(std::istream& is)
{
    std::string magic;
    int rows;
    int cols;
    int depth;
    int maxval;
    std::string tupletype;
    std::string ignored;
    
    is >> magic;
    if (magic != "P7")
    {
        std::cout << "NOT a PAM file please be careful\n";
        exit(1);
    }

    is >> ignored;
    is >> cols;
    is >> ignored;
    is >> rows;
    is >> ignored;
    is >> depth;
    is >> ignored;
    is >> maxval;
    is >> ignored;
    is >> tupletype;
    is >> ignored;
    is.get();

    //if (depth == 1 && maxval <= 255) PAM_IMG<GRAYSCALE_8> image(maxval, rows, cols, depth, tupletype);
    //if (depth == 3 && maxval <= 255) PAM_IMG <RGB_8> image(maxval, rows, cols, depth, tupletype);
    //if (depth == 1 && maxval <= 65536) PAM_IMG<GRAYSCALE_16> image(maxval, rows, cols, depth, tupletype);
    //if (depth == 3 && maxval <= 65536) PAM_IMG <RGB_16> image(maxval, rows, cols, depth, tupletype);
    //else
    //{
    //    std::cout << "Format not supported\n";
    //    exit(1);
    //}
    PAM_IMG<T> image(maxval, rows, cols, depth, tupletype);

    int i = 0;
    for (T elem; raw_read(is, elem);)
    {
        image.insert_at(i, elem);
        ++i;
    }

    return image;

}

template <typename T>
PAM_IMG<T>& flip_vertically(PAM_IMG<T>& image)
{
    for (int i = 0; i < (image.rows() / 2); i++)
    {
        for (int j = 0; j < image.cols(); j++)
        {   
            T temp = image(i, j);
            image(i, j) = image(image.rows() - 1 - i, j);
            image(image.rows() - 1 - i, j) = temp;
        }
    }

    return image;
}

template <typename T>
PAM_IMG<T>& flip_horizontally(PAM_IMG<T>& image)
{
    for (int i = 0; i < image.rows(); i++)
    {
        for (int j = 0; j < ( image.cols() / 2 ); j++)
        {
            T temp = image(i, j);
            image(i, j) = image(i, image.cols() - 1 - j);
            image(i, image.cols() - 1 - j) = temp;
        }
    }

    return image;
}



//outputname inputname

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    if (argc < 2)
    {
        std::cout << "Insert at least one parameter.\n";
        return 1;
    }

    std::ofstream os(argv[1], std::ios::binary);
    std::ifstream is(argv[2], std::ios::binary);

    if (!os)
    {
        std::cout << "Error opening output file";
        return 1;
    }

    if (!is)
    {
        std::cout << "Error opening input file";
        return 1;
    }

    //EXERCISE 1 GRAYSCALE IMAGE WRITING

    //PAM_IMG<GRAYSCALE> image(255, 256, 256, 1, "GRAYSCALE");

    //color_gradient(image);

    //write_pam(os, image);

    //EXERCISE 2 FLIPPING THE FROG VERTICALLY

    //auto image = load_pam<GRAYSCALE_8>(is);

    //flip_vertically(image);

    //write_pam(os, image);

    //Exercise 3 FLIPPING THE LAPTOP HORIZONTALLY

    auto image = load_pam<RGB_8>(is);

    flip_horizontally(image);

    write_pam(os, image);

    os.close();
    is.close();

    return 0;

}

