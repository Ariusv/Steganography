#include "BMP.h"
#include "RGB.h"
#include <iostream>
#include <fstream>
#include "Header.h"


using namespace std;


int BMP::Tcode(int byte)
{

	int copy_mask = File_INFO.S_Mask;
	int	mask_shift = 0;

	while (!(copy_mask & 1)) {
		copy_mask >>= 1;
		mask_shift++;
	}

	return (byte & File_INFO.S_Mask) >> mask_shift;
}



BMP::BMP(char* way)
{
	ifstream file_image(way, ios_base::binary);

	file_image.seekg(14);
	file_image.read((char*)&File_INFO.Size, sizeof(File_INFO.Size));
	file_image.read((char*)&File_INFO.Width, sizeof(File_INFO.Width));
	file_image.read((char*)&File_INFO.Height, sizeof(File_INFO.Height));

	File_INFO.Count_of_Canals = 3;
	File_INFO.Bits_on_Color = 24;
	File_INFO.S_Mask = 255;

	file_image.seekg(54);

	Max_size_Massage = (File_INFO.Width*File_INFO.Height) / 8;
	size_Massage = Max_size_Massage;

	Massage = new char[size_Massage];

	pixels = new RGB *[File_INFO.Height];
	for (int i = 0; i < File_INFO.Height; i++)
		pixels[i] = new RGB[File_INFO.Width];


	File_INFO.Line_Padding = ((File_INFO.Width * (File_INFO.Bits_on_Color / 8)) % 4) & 3;

	int code;


	for (int i = 0; i < File_INFO.Height; i++)
	{
		for (int j = 0; j < File_INFO.Width; j++)
		{
			file_image.read((char*)&code, 1);
			pixels[i][j].c_B = Tcode(code);
			file_image.read((char*)&code, 1);
			pixels[i][j].c_G = Tcode(code);
			file_image.read((char*)&code, 1);
			pixels[i][j].c_R = Tcode(code);
			pixels[i][j].bit_R = (pixels[i][j].c_R % 2 == 0 ? 0 : 1);

		}
		file_image.seekg(File_INFO.Line_Padding, ios::cur);
	}



	file_image.close();


}

void BMP::get_info() const
{
	cout << "______________________________________________________________________________" << endl;
	cout << "                               |    INFO   |                                  " << endl;
	cout << "_______________________________|___________|__________________________________" << endl;

	cout << "Size: " << File_INFO.Size << endl;
	cout << "Height: " << File_INFO.Height << endl;
	cout << "Width: " << File_INFO.Width << endl;
	cout << "Canals on pixel: " << File_INFO.Count_of_Canals << endl;
	cout << "Bits on color: " << File_INFO.Bits_on_Color << endl;
	cout << "Max size of text message: " << Max_size_Massage << endl;
	cout << "Count of pixels: " << File_INFO.Height*File_INFO.Width << endl;

	cout << "_______________________________|___________|__________________________________" << endl;

	cout << "PIXELS: \n\n";
	for (int i = 0; i < File_INFO.Height; i++)
	{
		for (int j = 0; j < File_INFO.Width; j++)
		{
			cout << pixels[i][j].c_R << "  " << pixels[i][j].c_G << "  " << pixels[i][j].c_B << "  "<<pixels[i][j].bit_R<< endl;

		}
		cout << "___________________" << endl;
	}
}

void BMP::DeCoder()
{

	bool *Code_bits = new bool[(File_INFO.Size*8)];

	unsigned int i, j, count = 1;
	int number = 0;
	unsigned int ind = 0, jnd = 0;

	for (i = 0; i < File_INFO.Height; i++)
	{
		for (j = 0; j < File_INFO.Width; j++, count++)

		{

			number += pixels[i][j].bit_R * pow(2, 7 - ind++);
			if (count==8)
			{
				Massage[jnd] = char(number);				

				if (Massage[jnd] == '~')
				{
					size_Massage = jnd;
					delete[] Code_bits;
					return;
				}
				number = 0;
				ind = 0;
				jnd++;
				count = 0;

			}

			
		}

	}

	size_Massage = jnd;
	delete[] Code_bits;

}

void BMP::PrintMassage() const
{
	for (int i = 0; i < size_Massage; i++)
		cout << Massage[i];
}

void BMP::Coder(char* _Massage_, char* way)
{
	strcpy(Massage, _Massage_);
	size_Massage = strlen(Massage);
	unsigned int ind = 0, i, j;

	bool *code_bits = new bool[size_Massage * 8];

	for (i = 0; i < size_Massage; i++)
	{
		int buff = int(Massage[i]);
		for (int k = 7; k >= 0; k--, ind++)
		{
			
			code_bits[ind] = (buff >> k) & 1;
		}
	}


	ind = 0;

	for (i = 0; i < (File_INFO.Height) && (ind<size_Massage*8); i++)
	{
		for (j = 0; (j < File_INFO.Width) && (ind<size_Massage*8) ; j++)
		{
			if (pixels[i][j].bit_R != code_bits[ind])
			{
				pixels[i][j].c_R++;
				pixels[i][j].bit_R = code_bits[ind];
			}
			ind++;
		}
	}

	delete[] code_bits;

	fstream file_image(way, ios_base::binary | ios::out | ios::in);

	file_image.seekp(54);


	for (i = 0; i < (File_INFO.Height) && (ind<size_Massage*8); i++)
	{
		for (j = 0; j < (j < File_INFO.Width) && (ind<size_Massage*8); j++)
		{
			file_image.write((char*)&pixels[i][j].c_B, 1);
			file_image.write((char*)&pixels[i][j].c_G, 1);
			file_image.write((char*)&pixels[i][j].c_R, 1);

		}
		file_image.seekp(File_INFO.Line_Padding, ios::cur);
	}

	file_image.close();


}


BMP::~BMP()
{
	for (int i = 0; i<File_INFO.Height; i++)
		delete[] pixels[i];
	delete[] pixels, Massage;

}

