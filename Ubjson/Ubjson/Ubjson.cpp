#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>


using namespace std;

#include "ppm.h"

template <typename T>
std::istream& raw_read(std::istream& is, T& value, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&value), size);
}



int convert(const string& sInput, const string& sOutput) {

	//opening the streams
	std::ifstream is(sInput);
	if (!is)
	{
		std::cout << "error input stream.\n";
		return 1;
	}

	std::ofstream os(sOutput);
	if (!os)
	{
		std::cout << "error output stream.\n";
		return 1;
	}


	unsigned w = 100;
	unsigned h = 100;

	uint8_t x_image = 0;
	uint8_t y_image = 0;
	uint8_t w_image = 10;
	uint8_t h_image = 10;

	image<vec3b> img_overlay(w_image, h_image);


	// Dal file UBJ devo estrarre le informazioni e creare il canvas

	vec3b background_color;

	image<vec3b> img(w, h);


	//Format to read: i <n_uints> uints : i <n_uints> (basic)
	//


	//START OF FILE
	char starter;

	is.read(&starter, 1);
	if (starter != '{')
	{
		std::cout << "incorrect format.\n";
	}

	//CANVAS 

	//
	char readed;
	uint8_t next_size = 0;

	is.read(&readed, 1);
	if (readed == 'i')
	{
		//uint_t size case
		raw_read(is, next_size);

	}

	std::vector<uint8_t> next_string;
	uint8_t next_int = 0;

	char c;

	for (int i = 0; i < next_size; i++)
	{
		is.read(&c, 1);
		next_string.push_back(c);
	}
	std::string str(next_string.begin(), next_string.end());

	if (str != "canvas")
	{
		std::cout << "therese no canvas\n";
	}

	//reading {
	is.read(&c, 1);


	//HERE STARTS THE CANVAS CONTENT
	if (c != '{') { std::cout << "error boh"; return 1; }

	//Elements of the canvas: width height background[list]
	//if the elements are not these ones we should ignore them

	for (int i = 0; i < 3; i++)
	{

		//Sx tag
		next_string.clear();
		is.read(&readed, 1);
		if (readed == 'i')
		{
			//uint_t size case
			raw_read(is, next_size);


			for (int i = 0; i < next_size; i++)
			{
				is.read(&c, 1);
				next_string.push_back(c);
			}
			std::string str(next_string.begin(), next_string.end());

			//case width
			if (str == "width")
			{
				//read the type tag
				char type;
				is.read(&type, 1);

				if (type == 'i') { /*ok*/ }

				uint8_t w_short = 0;

				raw_read(is, w_short);
				w = w_short;
			}

			//case height
			if (str == "height")

			{
				//read the type tag
				char type;
				is.read(&type, 1);

				uint8_t h_short = 0;

				raw_read(is, h_short);
				h = h_short;
			}

			//case backgrounfìd
			if (str == "background")
			{
				//we should manage a json list
				//Dato che non lo trovo scritto da nessuna parte deduco che la lista sia definita da [$U# seguita da tipo e numero di elementi

				next_string.clear();
				for (int i = 0; i < 4; i++)
				{
					char c;
					is.read(&c, 1);
					next_string.push_back(c);
				}

				std::string str(next_string.begin(), next_string.end());


				if (str == "[$U#")
				{
					// JSON LIST
					//read: type, size

					char type;
					is.read(&type, 1);

					uint8_t num_elem_array = 0;

					raw_read(is, num_elem_array);

					//Background color

					char R;
					is.read(&R, 1);
					char G;
					is.read(&G, 1);
					char B;
					is.read(&B, 1);


					vec3b background_RGB((unsigned char)R, (unsigned char)G, (unsigned char)B);
					background_color = background_RGB;




				}


			}
		}
	}


	is.read(&c, 1);
	//HERE ENDS THE CANVAS CONTENT
	if (c != '}') { std::cout << "error ending canvas.\n"; return 1; }



	//SAVING THE CANVAS IMAGE 
	img.resize(w, h);

	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			img(i, j) = background_color;
		}
	}

	//Printing PPM

	writeP6("canvas.ppm", img);


	//PHASE 2: Accessing Elements and finding the image field

	is.read(&c, 1);
	if (c == 'i')
	{
		//uint_t size case
		raw_read(is, next_size);

		next_string.clear();

		for (int i = 0; i < next_size; i++)
		{
			is.read(&c, 1);
			next_string.push_back(c);
		}
		std::string strr(next_string.begin(), next_string.end());

		if (strr != "elements")
		{
			std::cout << "therese no elements!\n";
			return 1;
		}

		char c;
		//reading {
		is.read(&c, 1);

		//Now i need to find only my field of interest: image
		//so i should be 5 in the first place


		//Now i iterate to find the image field

		for (char c; is.read(&c, 1);)
		{
			if (c == 'i')
			{
				raw_read(is, next_size);

				if (next_size == 5)
				{
					next_string.clear();

					for (int i = 0; i < next_size; i++)
					{
						is.read(&c, 1);
						next_string.push_back(c);
					}
					std::string strimg(next_string.begin(), next_string.end());

					if (strimg == "image")
					{
						printf("image : ");
						is.read(&readed, 1);

						//Extracting the image informations

						for (int i = 0; i < 5; i++)
						{

							//Sx tag
							next_string.clear();
							is.read(&readed, 1);
							if (readed == 'i')
							{
								//uint_t size case
								raw_read(is, next_size);


								for (int i = 0; i < next_size; i++)
								{
									is.read(&c, 1);
									next_string.push_back(c);
								}
								std::string str(next_string.begin(), next_string.end());

								//case width
								if (str == "x")
								{
									printf("x,");
									//read the type tag
									char type;
									is.read(&type, 1);

									if (type == 'i') { /*ok*/ }

									uint8_t x = 0;

									raw_read(is, x);
									x_image = x;
								}

								//case height
								if (str == "y")

								{
									printf("y,");
									//read the type tag
									char type;
									is.read(&type, 1);

									uint8_t y = 0;

									raw_read(is, y);
									y_image = y;
								}

								if (str == "width")

								{
									printf("width,");
									//read the type tag
									char type;
									is.read(&type, 1);

									uint8_t width = 0;

									raw_read(is, width);
									w_image = width;

									img_overlay.resize(w_image, h_image);

								}

								if (str == "height")

								{
									printf("height,");
									//read the type tag
									char type;
									is.read(&type, 1);

									uint8_t height = 0;

									raw_read(is, height);
									h_image = height;

									img_overlay.resize(w_image, h_image);
								}

								//case data
								if (str == "data")
								{
									//we should manage a json list
									//Dato che non lo trovo scritto da nessuna parte deduco che la lista sia definita da [$U# seguita da tipo e numero di elementi

									next_string.clear();
									for (int i = 0; i < 4; i++)
									{
										char c;
										is.read(&c, 1);
										next_string.push_back(c);
									}

									std::string str(next_string.begin(), next_string.end());


									if (str == "[$U#")
									{
										// JSON LIST
										//read: type, size

										char type;
										is.read(&type, 1);

										if (type != 'I') { "something wrong but idc\n"; }

										uint16_t num_elem_array = 0;

										char msb = is.get();
										char lsb = is.get();

										num_elem_array = ((msb << 8) | lsb);


										//Reading the image

										std::vector<vec3b> data_vec;

										for (int i = 0; i < num_elem_array; i = i++)
										{
											char R;
											char G;
											char B;
											for (int i = 0; i < 3; i++)
											{
												if (i % 3 == 0)
													is.read(&R, 1);

												if (i % 3 == 1)
													is.read(&G, 1);
												if (i % 3 == 2)
													is.read(&B, 1);

											}

											vec3b pixel((unsigned char)R, (unsigned char)G, (unsigned char)B);

											data_vec.push_back(pixel);


										}
										int count = 0;
										for (auto it = img_overlay.begin();
											it != img_overlay.end(); ++it)
										{
											*it = data_vec[count];
											count++;

										}

									}

								}


							}
						}

						//saving image

						//Viene salvata solo una immagine non c'è il supporto per più roba sorry
						writeP6("image1.ppm", img_overlay);

					}


				}

			}
		}


	}

	//PERFORMING THE OVERLAY

	int y_image_i = (int)y_image;
	int x_image_i = (int)x_image;
	int h_image_i = (int)h_image;
	int w_image_i = (int)w_image;

	for (int i = x_image_i; i < w_image_i; i++)
	{
		for (int j = y_image_i; j < h_image_i; j++)
		{
			img(i, j) = img_overlay(i - y_image_i, j - x_image_i);
		}
	}


	std::cout << "debug";

	// Per accedere ai pixel di img posso usare img(x,y) oppure img.begin() e img.end()

	// Dal file UBJ devo estrarre le informazioni sulle immagini da incollare su img 

	// Output in formato PPM
	if (!writeP6(sOutput, img))
		return EXIT_FAILURE;


	is.close();
	os.close();

	return EXIT_SUCCESS;
}






int main(int argc, char* argv[]) {

	std::cout << "heloo\n";


	string sInput = argv[1];
	string sOutput = argv[2];

	return convert(sInput, sOutput);
}