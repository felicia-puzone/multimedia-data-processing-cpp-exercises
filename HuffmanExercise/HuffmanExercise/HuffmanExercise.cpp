
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

class HuffmanNode {
public:
	std::map<char, int> submap;
	HuffmanNode* bit1;
	HuffmanNode* bit0;
	int len = 0;

};

template <typename T>
std::ostream& raw_write(std::ostream& os, const T& value, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<const char*>(&value), size);
}

template <typename T>
std::istream& raw_read(std::istream& is, T& value, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>( & value), size);
}

class Bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit)
	{
		if (n_ == 8)
		{
			raw_write(os_, buffer_);
			buffer_ = 0;
			n_ = 0;
		}

		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;

		return os_;
	}

public:

	Bitwriter(std::ostream& os) : os_(os) {}

	std::ostream& write(uint32_t value, uint8_t n_bit)
	{
		while (n_bit-- > 0)
		{
			write_bit(value >> n_bit);
		}

		return os_;
	}

	std::ostream& flush(uint32_t value = 0)
	{
		while (n_ < 8)
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



int main(int argc, char** argv)
{
	std::cout << "Hello Huffman!\n";

	std::ifstream is(argv[2], std::ios::binary);
	if (!is)
	{
		std::cout << "error input.\n";
		return 1;
	}

	std::ofstream os(argv[3], std::ios::binary);
	if (!is)
	{
		std::cout << "error output.\n";
		return 1;
	}

	if (*argv[1] == 'c')
	{
		std::cout << "compressing...\n";

		std::map<char, int> list_sym;

		for (char symbol; is.read(&symbol, 1); )
		{
			if (list_sym.find(symbol) != list_sym.end())
			{
				list_sym[symbol]++;
			}
			else
				list_sym.insert(std::pair<char, int>(symbol, 1));
		}

		uint8_t size = list_sym.size() > 0 ? list_sym.size() : 0;

		//Creating symbols

		auto temp_map = list_sym;

		HuffmanNode* start = new HuffmanNode();

		start->submap = list_sym;

		bool loop = true;

		HuffmanNode* previous = start;

		while (loop)
		{

			HuffmanNode* next1 = new HuffmanNode();
			HuffmanNode* next2 = new HuffmanNode();

			auto it = std::max_element(temp_map.begin(), temp_map.end());

			//element with greater probability (1)
			next1->submap.insert(std::pair<char, int>(it->first, it->second));
			next1->len = previous->len;
			(next1->len)++;

			//max element key
			//it->first;

			//elements with lower probability grouped togheter

			temp_map.erase(it->first);

			next2->submap = temp_map;
			next2->len = previous->len;
			(next2->len)++;

			previous->bit1 = next1;
			previous->bit0 = next2;

			//Checking if the loop is over
			if (temp_map.size() == 1) loop = false;

			previous = next2;

		}

		os << "HUFFMAN1" << size;

		//Computing table entries

		//String for debug
		std::string code;

		typedef struct {
			char symbol;
			int len;
			std::string code;
		} element;

		std::vector<element> table;

		HuffmanNode* temp = start;
		while (temp->bit0 != NULL)
		{
			element elem;

			elem.symbol = temp->bit1->submap.begin()->first;
			elem.len = temp->bit1->len;
			elem.code.append(code);
			elem.code.append("1");
			table.push_back(elem);

			code.append("0");


			if (temp->bit0->bit0 == NULL)
			{
				//Last element

				element elem;

				elem.symbol = temp->bit0->submap.begin()->first;
				elem.len = temp->bit0->len;
				elem.code.append(code);
				table.push_back(elem);
			}

			temp = temp->bit0;

		}


		// writing into the stream

		Bitwriter bitwriter(os);

		is.clear();
		is.seekg(0, std::ios::beg);

		std::vector<char> input;

		for (char elem; is.read(&elem, 1);)
		{
			input.push_back(elem);
		}


		for (auto& elem : table)
		{
			bitwriter.write(elem.symbol, 8);
			bitwriter.write(elem.len, 5);
			for (int i = 0; i < elem.code.length(); i++)
			{
				if (elem.code[i] == '0') bitwriter.write(0, 1);
				if (elem.code[i] == '1') bitwriter.write(1, 1);
			}
		}

		//Numsymbols

		uint32_t num_syn = input.size();

		bitwriter.write(num_syn >> 24, 8);
		bitwriter.write(num_syn >> 16, 8);
		bitwriter.write(num_syn >> 8, 8);
		bitwriter.write(num_syn, 8);


		for (auto& elem : input)
		{
			for (auto it = table.begin(); it != table.end(); ++it)
			{
				if ((*it).symbol == elem)
				{
					for (int i = 0; i < (*it).code.length(); i++)
					{
						if ((*it).code[i] == '0') bitwriter.write(0, 1);
						if ((*it).code[i] == '1') bitwriter.write(1, 1);
					}
				}
			}
		}

		


		std::cout << "debug";


	}


	is.close();
	os.close();

	return 0;
}
