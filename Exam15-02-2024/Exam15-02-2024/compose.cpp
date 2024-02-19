#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// VERSIONE CHE <<potrebbe>> FUNZIONARE SOLO CON DUE IMMAGINI. SOTTO LASCIO COMMENTATA LA PARTE CHE STAVO RISCRIVENDO IN MODO PIù CARINO MA NON 
//HO AVUTO IL TEMPO PER FINIRE. SORRY SPERO VADA BENE ALMENO UN Po'

template <typename T>
class vec4ch {
public:
    T R;
    T G;
    T B;
    T A;
};

template <typename T>
class Image {
    int rows_ = 0;
    int cols_ = 0;
    std::vector<vec4ch<T>> data_;

public:
    Image() {}
    Image(int rows, int cols, std::vector<vec4ch<T>> data) : data_(data), rows_(rows), cols_(cols) {}


    vec4ch<T>& operator()(int row, int col)
    {
        return data_[cols_ * row + col];
    }

    int get_rows()
    {
        return rows_;
    }
    int get_cols()
    {
        return cols_;
    }

    int get_size()
    {
        return rows_ * cols_;
    }

    std::vector<vec4ch<T>>& get_raw_data()
    {
        return data_;
    }

};

Image<uint8_t> pam_decoder(std::istream& is)
{

    std::string magic;
    int width = 0;
    int height = 0;
    int channels = 0;
    int maxval = 0;


    is >> magic;

    if (magic != "P7")
    {
        std::cout << "Error format file\n";
    }
    std::string temp;

    is >> temp;

    if (temp == "WIDTH") is >> width;

    is >> temp;

    if (temp == "HEIGHT") is >> height;

    is >> temp;

    if (temp == "DEPTH") is >> channels;

    is >> temp;

    if (temp == "MAXVAL") is >> maxval;

    is >> temp;

    if (temp == "TUPLTYPE") { is >> temp; }

    is >> temp;
    if (temp == "ENDHDR") {}

    is.ignore();

    std::vector<vec4ch<uint8_t>> data;

    if (channels == 4)
    {
        int i = 4;
        vec4ch<uint8_t> elem;
        for (uint8_t byte; is.read(reinterpret_cast<char*> (&byte), 1);)
        {
            switch (i % 4)
            {
            case 0:
                elem.R = byte;
                i--;
                break;

            case 3:
                elem.G = byte;
                i--;
                break;

            case 2:
                elem.B = byte;
                i--;
                break;
            case 1:
                elem.A = byte;
                data.push_back(elem);
                i = 4;
                break;
            }
        }

    }

    if (channels == 3)
    {
        //RGB
        int i = 3;
        vec4ch<uint8_t> elem;
        for (uint8_t byte; is.read(reinterpret_cast<char*> (&byte), 1);)
        {
            switch (i % 3)
            {
            case 0:
                elem.R = byte;
                i--;
                break;

            case 2:
                elem.G = byte;
                i--;
                break;

            case 1:
                elem.B = byte;
                elem.A = 255;
                data.push_back(elem);
                i = 3;
                break;
            }
        }
    }

    Image<uint8_t> img(height, width, data);

    return img;
}

std::ostream& pam_encoder(std::ostream& os, Image<uint8_t>& img)
{
    os << "P7";
    os << "\n";
    os << "WIDTH";
    os << " ";
    os << img.get_cols();
    os << "\n";
    os << "HEIGHT";
    os << " ";
    os << img.get_rows();
    os << "\n";
    os << "DEPTH";
    os << " ";
    os << 4;
    os << "\n";
    os << "MAXVAL";
    os << " ";
    os << 255;
    os << "\n";
    os << "TUPLTYPE";
    os << " ";
    os << "RGB_ALPHA";
    os << "\n";
    os << "ENDHDR";
    os << "\n";

    for (auto elem : img.get_raw_data())
    {
        os.write(reinterpret_cast<char*>(&elem.R), 1);
        os.write(reinterpret_cast<char*>(&elem.G), 1);
        os.write(reinterpret_cast<char*>(&elem.B), 1);
        os.write(reinterpret_cast<char*>(&elem.A), 1);
    }

    return os;

}

vec4ch<uint8_t> composed_pixel(vec4ch<uint8_t>& p1, vec4ch<uint8_t>& p2)
{
    vec4ch<uint8_t> new_pixel;

    uint8_t new_alpha_o = p1.A + p2.A * (1 - p1.A);



    new_pixel.R = ((((p1.R * p1.A) + (p2.R * p2.A) * (1 - p1.A))) / new_alpha_o) > 255 ? 255 : ((((p1.R * p1.A) + (p2.R * p2.A) * (1 - p1.A))) / new_alpha_o);
    new_pixel.G = ((((p1.G * p1.A) + (p2.G * p2.A) * (1 - p1.A))) / new_alpha_o) > 255 ? 255 : ((((p1.G * p1.A) + (p2.G * p2.A) * (1 - p1.A))) / new_alpha_o);
    new_pixel.B = ((((p1.B * p1.A) + (p2.B * p2.A) * (1 - p1.A))) / new_alpha_o) > 255 ? 255 : ((((p1.B * p1.A) + (p2.B * p2.A) * (1 - p1.A))) / new_alpha_o);
    new_pixel.A = new_alpha_o;

    return new_pixel;
}


int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    //Retrieving offsets
    //compute output dimension

    //compositing

    if (argc < 3)
    {
        std::cout << "at least 3 parameters!\n";
        return 1;
    }


    std::string offsetX1_s;
    std::string offsetY1_s;

    int offsetX1 = 0;
    int offsetY1 = 0;

    std::string offsetX2_s;
    std::string offsetY2_s;

    int offsetX2 = 0;
    int offsetY2 = 0;


    std::string outputFilename = argv[1];

    std::string input1Filename;
    std::string input2Filename;


    std::string argv2 = argv[2];

    //come al solito qui doveva esserci una struttura ricorsiva perchè le immagini possono essere
    //anche più di due. Se ho tempo risolvo

    if (argv2 == "-p")
    {
        offsetX1_s = argv[3];
        offsetY1_s = argv[4];


        offsetX1 = std::stoi(offsetX1_s);
        offsetY1 = std::stoi(offsetY1_s);

        input1Filename = argv[5];

        std::string argv6 = argv[6];

        if (argv6 == "-p")
        {
            offsetX2_s = argv[7];
            offsetY2_s = argv[8];

            offsetX2 = std::stoi(offsetX2_s);
            offsetY2 = std::stoi(offsetY2_s);

            input2Filename = argv[9];
        }

    }
    else
    {
        input1Filename = argv2;

        std::string argv3 = argv[3];

        if (argv3 == "-p")
        {
            offsetX2_s = argv[4];
            offsetY2_s = argv[5];

            offsetX2 = std::stoi(offsetX2_s);
            offsetY2 = std::stoi(offsetY2_s);

            input2Filename = argv[6];
        }
        else
        {
            input2Filename = argv[3];
        }

    }

    input1Filename.append(".pam");
    input2Filename.append(".pam");

    //I decode the two images in pam format

    std::ifstream is1(input1Filename, std::ios::binary);
    std::ifstream is2(input2Filename, std::ios::binary);

    //per ora faccio il caso a 8bpp nel caso ho tempo lo estendo
    Image<uint8_t> input_image1;
    Image<uint8_t> input_image2;

    input_image1 = pam_decoder(is1);
    input_image2 = pam_decoder(is2);

    // computing output dimension

    int out_rows = 0;
    int out_cols = 0;

    //input_image1.get_rows() >= input_image2.get_rows() ? out_rows : input_image1.get_rows();
    //input_image2.get_cols() >= input_image2.get_cols() ? out_cols : input_image2.get_cols();

    if (input_image1.get_rows() >= input_image2.get_rows()) out_rows = input_image1.get_rows();
    else out_rows = input_image2.get_rows();

    if (input_image1.get_cols() >= input_image2.get_cols()) out_cols = input_image1.get_cols();
    else out_cols = input_image2.get_cols();

    //Computing the overlapping


    //(0,0,0,1) NERO

    std::vector<vec4ch<uint8_t>> black_image(out_rows * out_cols);
    for (auto& elem : black_image)
    {
        elem.R = 0;
        elem.G = 0;
        elem.B = 0;
        elem.A = 255;
    }

    Image<uint8_t> output(out_rows, out_cols, black_image);

    //overlapping
    /*
    for (int i = offsetX1; i < input_image1.get_rows(); ++i)
    {
        for (int j = offsetY1; j < input_image1.get_cols(); ++j)
        {
            output(i, j) = composed_pixel(output(i, j), input_image1(i, j));
        }
    }
    */
    for (int i = offsetX2; i < input_image2.get_rows(); ++i)
    {
        for (int j = offsetY2; j < input_image2.get_cols(); ++j)
        {
            output(i, j) = composed_pixel(input_image2(i, j), output(i, j));
        }
    }
    

    std::ofstream os(outputFilename.append(".pam"), std::ios::binary);

    pam_encoder(os, output);


    is1.close();
    is2.close();
    os.close();


}


//////////////////////////// MODO CORRETTO SECONDO LA TRACCIA ALPHA VERSION 0.1

/*

int main(int argc, char** argv)
{
    std::cout << "Hello Alpha!\n";

    if (argc < 3)
    {
        std::cout << "at least 3 parameters!\n";
        return 1;
    }

    std::string outputFilename = argv[1];

    std::map<std::string, std::pair<int,int>> list_input_filename_offsets;

    std::string input1Filename;
    std::string input2Filename;

    std::string peek = argv[2];

    int index = 2;
    while (peek == "-p")
    {
        int offsetX;
        int offsetY;

        std::string offsetX_s;
        std::string offsetY_s;


        offsetX_s = argv[index+1];
        offsetY_s = argv[index+2];


        offsetX = std::stoi(offsetX_s);
        offsetY = std::stoi(offsetY_s);

        std::string inputFilename = argv[index+3];

        list_input_filename_offsets.insert(std::pair<std::string, std::pair<int, int>>(inputFilename, std::pair<int, int>(offsetX, offsetY)));

        if (index + 4 < argc)
        {
            peek = argv[index + 4];
            index = index + 4;
        }
        else peek = "finish";

    }


    // QUI LA PARTE DI DECODIFICA E TUTTO

        std::vector<Image<uint8_t>> list_images;

    for (auto input_image : list_input_filename_offsets)
    {
        std::string input_name = input_image.first;
        input_name.append(".pam");
        std::ifstream is(input_name, std::ios::binary);

        Image<uint8_t> input_image;
        input_image = pam_decoder(is);

        //....

        // l'idea era inserire gli offset direttamente nelle immagini o qualcosa del genere per usarli poi alla fine nell'output

    }

*/