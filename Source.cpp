#include <iostream>
#include <fstream>
#include <iostream>
#include "BMP.h"

using namespace std;



int main()
{
	BMP image("tiger1.bmp");
	image.Coder("I lv~", "tiger1.bmp");
	image.DeCoder();
	system("pause");
	return 0;
}
