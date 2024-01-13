
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

template <typename T>
struct AdamBlock {
	int level = 0;
	int rows = 0;
	int cols = 0;

	std::vector<T> data_;

};

template <typename T>
struct Mat {
	int n_rows_ = 0;
	int n_cols_ = 0;



	void resize(int n, int c)
	{
		data_.resize(n * c);
	}

public:

	std::vector<T> data_;

	Mat(int rows, int cols) : n_rows_(rows), n_cols_(cols)
	{
		resize(rows, cols);
	}


	T& operator()(int row, int col)
	{
		return data_[row * n_cols_ + col];
	}

	const T& operator() (int row, int col) const
	{
		return data_[row * n_cols_ + col];
	}
};




template <typename T>
class MatExtended {

	int n_rows_ = 0;
	int n_cols_ = 0;
	int maxval_ = 0;

	std::vector<T> data_;


	//when I fill this object, I put all in order and i eventually fill with 0 the missing spots

	/*      1    6    4    6    2    6    4    6
			7    7    7    7    7    7    7    7
			5    6    5    6    5    6    5    6
			7    7    7    7    7    7    7    7
			3    6    4    6    3    6    4    6
			7    7    7    7    7    7    7    7
			5    6    5    6    5    6    5    6
			7    7    7    7    7    7    7    7
	*/

public:

	std::vector<T> block1;
	std::vector<T> block2;
	std::vector<T> block3;
	std::vector<T> block4;
	std::vector<T> block5;
	std::vector<T> block6;
	std::vector<T> block7;

	int load1 = 0;
	int load2 = 0;

	int load3 = 0;
	int load4 = 0;
	int load5 = 0;
	int load6 = 0;
	int load7 = 0;

	MatExtended(int rows, int cols, std::vector<T>& data, int mode, int maxval = 0) : n_rows_(rows), n_cols_(cols), maxval_(maxval), data_(data)
	{
		if (mode == 0)
		{
			int size = n_rows_ * n_cols_;

			load1 = size / 64;
			load2 = size / 64;

			load3 = (size / 64) * 2;
			load4 = (size / 64) * 4;
			load5 = (size / 64) * 8;
			load6 = (size / 64) * 16;
			load7 = (size / 64) * 32;

			fillAdamFromMLT(data);

		}

		if (mode == 1)
		{
			//fillAdam();
		}

	}

	void fillAdam()
	{

		//1
		for (int i = 0; i < n_rows_; i = i + 8)
		{
			for (int j = 0; j < n_cols_; j = j + 8)
			{
				block1.push_back(operator()(i, j));
			}
		}


		//2
		for (int i = 0; i < n_rows_; i = i + 8)
		{
			for (int j = 4; j < n_cols_; j = j + 8)
			{
				block2.push_back(operator()(i, j));
			}
		}

		//3
		for (int i = 4; i < n_rows_; i = i + 8)
		{
			for (int j = 4; j < n_cols_; j = j + 4)
			{
				block3.push_back(operator()(i, j));
			}
		}


		//4
		for (int i = 0; i < n_rows_; i = i + 4)
		{
			for (int j = 2; j < n_cols_; j = j + 4)
			{
				block4.push_back(operator()(i, j));
			}
		}

		//5
		for (int i = 2; i < n_rows_; i = i + 4)
		{
			for (int j = 0; j < n_cols_; j = j + 2)
			{
				block5.push_back(operator()(i, j));
			}
		}

		//6
		for (int i = 0; i < n_rows_; i = i + 2)
		{
			for (int j = 1; j < n_cols_; j = j + 2)
			{
				block6.push_back(operator()(i, j));
			}
		}

		//7
		for (int i = 1; i < n_rows_; i = i + 2)
		{
			for (int j = 0; j < n_cols_; j = j + 1)
			{
				block7.push_back(operator()(i, j));
			}
		}

	}

	void fillAdamFromMLT(std::vector<T>& rawdata)
	{

		data_ = rawdata;

		std::cout << "debug";
	}

	void decodeAdam(Mat<T>& rawimage, int reslevel = 7)
	{
		if (reslevel == 7)
		{
			int k = 0;

			//1

			for (int i = 0; i < n_rows_; i = i + 8)
			{
				for (int j = 0; j < n_cols_; j = j + 8)
				{

					rawimage(i, j) = data_[k];
					++k;

				}
			}

			//2
			for (int i = 0; i < n_rows_; i = i + 8)
			{
				for (int j = 4; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;

				}
			}

			//3
			for (int i = 8; i < n_rows_; i = i + 8)
			{

				for (int j = 0; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;
				}

			}


			for (int i = 8; i < n_rows_; i = i + 8)
			{
				for (int j = 4; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;
				}
			}

			/*
			std::cout << "debug";

			//4
			for (int i = 0; i < n_rows_; i = i + 8)
			{
				for (int j = 2; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;

				}
			}
			for (int i = 0; i < n_rows_; i = i + 8)
			{
				for (int j = 6; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;

				}
			}
			for (int i = 4; i < n_rows_; i = i + 8)
			{
				for (int j = 2; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;

				}
			}
			for (int i = 4; i < n_rows_; i = i + 8)
			{
				for (int j = 6; j < n_cols_; j = j + 8)
				{
					rawimage(i, j) = data_[k];
					++k;
				}
			}

			

			std::cout << "debug";

			//5

			for (int n = 0; n < 8; n = n + 4)
			{
				for (int m = 0; m < 8; m = m + 2)
				{
					for (int i = 2 + n; i < n_rows_; i = i + 8)
					{
						for (int j = m; j < n_cols_; j = j + 8)
						{
							rawimage(i, j) = data_[k];
							++k;

						}
					}

				}
			}


	
			//6

			for (int n = 0; n < 8; n = n + 2)
			{
				for (int m = 0; m < 8; m = m + 2)
				{

					for (int i = n; i < n_rows_; i = i + 8)
					{
						for (int j = 1 + m; j < n_cols_; j = j + 8)
						{
							rawimage(i, j) = data_[k];
							++k;
						}
					}

				}
			}



			//7
			
			for (int m = 0; m < 8; m = m + 2)
			{
				for (int n = 0; n < 8; n = n + 1)
				{
					for (int i = 1 + m; i < n_rows_; i = i + 8)
					{
						for (int j = n; j < n_cols_; j = j + 8)
						{

							rawimage(i, j) = data_[k];
							++k;

						}
					}

				}
			}*/

			std::cout << "debug";

		}


	}

	T& operator()(int row, int col)
	{
		return data_[row * n_cols_ + col];
	}

	const T& operator() (int row, int col) const
	{
		return data_[row * n_cols_ + col];
	}

};


int main(int argc, char** argv)
{
	std::cout << "Hello Adam7!\n";

	std::ifstream is(argv[2], std::ios::binary);
	std::ofstream os(argv[3], std::ios::binary);

	if (!is)
	{
		std::cout << "Error opening input stream.\n";
		return 1;
	}

	if (!os)
	{
		std::cout << "Error opening output stream.\n";
		return 1;
	}

	if (*argv[1] == 'c')
	{
		std::cout << "Compressing...\n";


		//DECODING PGM into MatExtended Class


		std::string magic;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t maxval = 0;

		is >> magic;

		if (magic != "P5")
		{
			std::cout << "Wrong fileformat. Use a proper PGM file.\n";
			return 1;
		}

		is.ignore();
		if (is.peek() != '#')
		{
			is >> width;
			is >> height;
			is >> maxval;
		}
		else
		{
			is.ignore();
			std::string comment;

			while (is.peek() != '\n') is >> comment;

			is >> width;
			is >> height;
			is >> maxval;

		}

		is.ignore();

		if (maxval < 256)
		{
			std::vector<uint8_t> bytes((std::istream_iterator<uint8_t>(is >> std::noskipws)),
				std::istream_iterator<uint8_t>());

			MatExtended<uint8_t> img(height, width, bytes, 1, maxval);

			//Encoding and WRITING MLT

			os << "MULTIRES";
			os.write(reinterpret_cast<char*>(&width), sizeof(uint32_t));
			os.write(reinterpret_cast<char*>(&height), sizeof(uint32_t));

			for (auto& elem : img.block1)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block2)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block3)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block4)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block5)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block6)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			for (auto& elem : img.block7)
			{
				os.write(reinterpret_cast<char*>(&elem), sizeof(uint8_t));
			}

			std::cout << "debug short 1";

		}


	}

	if (*argv[1] == 'd')
	{
		std::cout << "Decompressing...\n";

		char magic[9];;

		is.read(&magic[0], 8);
		magic[8] = '\0';

		std::string magicS(magic);

		if (magicS != "MULTIRES")
		{
			std::cout << "Wrong format exiting...\n";
			return 1;
		}

		uint32_t height = 0;
		uint32_t width = 0;

		is.read(reinterpret_cast<char*>(&width), sizeof(uint32_t));
		is.read(reinterpret_cast<char*>(&height), sizeof(uint32_t));

		std::vector<uint8_t> data(std::istream_iterator<uint8_t>(is >> std::noskipws),
			std::istream_iterator<uint8_t>());


		MatExtended<uint8_t> img((int)height, (int)width, data, 0);


		Mat<uint8_t> rawmatrix((int)height, (int)width);

		img.decodeAdam(rawmatrix, 7);



		//Encoding PGM

		os << "P5" << '\n' << (int)width << " " << (int)height << '\n' << "255" << '\n';

		for (auto& elem : rawmatrix.data_)
		{
			os.write(reinterpret_cast<char*>(&elem), 1);
		}


		std::cout << "debug";

	}

	is.close();
	os.close();


	return 0;
}
