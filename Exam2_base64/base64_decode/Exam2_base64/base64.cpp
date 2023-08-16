#include <iostream>
#include "base64.h"
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

std::map<char, uint8_t> base64_dict{
	{ 'A', 0 },
	{ 'B', 1 },
	{ 'C', 2 },
	{ 'D', 3 },
	{ 'E', 4 },
	{ 'F', 5 },
	{ 'G', 6 },
	{ 'H', 7 },
	{ 'I', 8 },
	{ 'J', 9 },
	{ 'K', 10 },
	{ 'L', 11 },
	{ 'M', 12 },
	{ 'N', 13 },
	{ 'O', 14 },
	{ 'P', 15 },
	{ 'Q', 16 },
	{ 'R', 17 },
	{ 'S', 18 },
	{ 'T', 19 },
	{ 'U', 20 },
	{ 'V', 21 },
	{ 'W', 22 },
	{ 'X', 23 },
	{ 'Y', 24 },
	{ 'Z', 25 },
	{ 'a', 26 },
	{ 'b', 27 },
	{ 'c', 28 },
	{ 'd', 29 },
	{ 'e', 30 },
	{ 'f', 31 },
	{ 'g', 32 },
	{ 'h', 33 },
	{ 'i', 34 },
	{ 'j', 35 },
	{ 'k', 36 },
	{ 'l', 37 },
	{ 'm', 38 },
	{ 'n', 39 },
	{ 'o', 40 },
	{ 'p', 41 },
	{ 'q', 42 },
	{ 'r', 43 },
	{ 's', 44 },
	{ 't', 45 },
	{ 'u', 46 },
	{ 'v', 47 },
	{ 'w', 48 },
	{ 'x', 49 },
	{ 'y', 50 },
	{ 'z', 51 },
	{ '0', 52 },
	{ '1', 53 },
	{ '2', 54 },
	{ '3', 55 },
	{ '4', 56 },
	{ '5', 57 },
	{ '6', 58 },
	{ '7', 59 },
	{ '8', 60 },
	{ '9', 61 },
	{ '+', 62 },
	{ '/', 63 },
	{ '=', 64 }
};


std::string base64_decode(const std::string& input)
{
	std::string outstring;
	std::stringstream ssout(outstring);

	std::string input1(input);

	input1.erase(remove(input1.begin(), input1.end(), ' '), input1.end());
	input1.erase(remove(input1.begin(), input1.end(), '\r'), input1.end());
	input1.erase(remove(input1.begin(), input1.end(), '\n'), input1.end());

	std::stringstream ss(input1);

	while (ss.peek() != EOF)
	{
			//read 4 characters in 6-bit format to form a 24-bit sequence. From that extract 3 bytes.

			uint32_t bit_seq = 0;

			char buffer[4];

			int i = 0;

			while (i < 4 && !ss.eof())
			{
				char temp = ss.get();
				//std::cout << temp << std::endl;

				if (base64_dict.find(temp) != base64_dict.end())
				{
					buffer[i] = base64_dict[temp];
					i++;
				}
			}


			if (buffer[2] == 64 && buffer[3] == 64)
			{
				bit_seq = (buffer[0] << 18) + (buffer[1] << 12);
				char char1 = bit_seq >> 16;

				outstring.append(sizeof(char), char1);
			}

			else if (buffer[2] != 64 && buffer[3] == 64)
			{
				bit_seq = (buffer[0] << 18) + (buffer[1] << 12) + (buffer[2] << 6);
				char char1 = bit_seq >> 16;
				char char2 = bit_seq >> 8;

				outstring.append(sizeof(char), char1);
				outstring.append(sizeof(char), char2);
			}


			//else => no '=' character detected

			else if (buffer[2] != 64 && buffer[3] != 64)
			{
				bit_seq = (buffer[0] << 18) + (buffer[1] << 12) + (buffer[2] << 6) + buffer[3];

				//now i need to write the 3 bytes 

				char char1 = bit_seq >> 16;
				char char2 = bit_seq >> 8;
				char char3 = bit_seq;

				outstring.append(sizeof(char), char1);
				outstring.append(sizeof(char), char2);
				outstring.append(sizeof(char), char3);
			}
	}

	return outstring;
}