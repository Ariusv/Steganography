#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include "RGB.h"
#include "Header.h"
class BMP
{
private:

	char* Massage;
	unsigned int size_Massage;
	unsigned int Max_size_Massage;
	BMPINFO File_INFO;
	RGB **pixels;

	int Tcode(int byte);

public:
	BMP(char* _way_);
	~BMP();
	void get_info() const;
	void PrintMassage() const;
	void DeCoder();
	void Coder(char* _Massage_, char* way);


};