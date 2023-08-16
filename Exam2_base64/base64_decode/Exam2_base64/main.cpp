#include <iostream>
#include "base64.h"
#include <fstream>

int main(int argv, char** argc)
{
	if (argv != 2)
	{
		std::cout << "Wrong parameters. Input takes one text base64 encoded file.";
		return 1;
	}

	std::string filename = argc[1];

	//opening the input stream

	std::ifstream is(filename, std::ios::binary);

	if (is.fail())
	{
		std::cout << "Failed to open input file. Exiting...";
		return 1;
	}


	//Reading all the lines

	std::string encoded;
	std::string temp;

	while (is.peek() != EOF) {
		std::cout << is.peek() << std::endl;
		is >> encoded;
	}

	is.close();

	//decoding routine

	std::string decoded;

	std::string test("UHJldHR5IGxvbmcgdGV4dCB3aGljaCByZXF1aXJlcyBtb3JlIHRoYW4gNzYgY2hhcmFjdGVycyB0byBlbmNvZGUgaXQgY29tcGxldGVseS4 = \r\n");

	decoded = base64_decode(test);

	//opening output stream

	std::ofstream os("decoded.txt");

	os << decoded;

	os.close();

	printf("Finished");

}

