#include "readBMP.h"

pronet::readBMP::readBMP(const char* _imgname)
	: imgname_(_imgname)
{
	if (!_imgname)
		throw std::runtime_error("BitMapFileName is null. You must specified file name!! : readBMP.readBMP(const char*)");
	std::ifstream ifs(_imgname, std::ios::binary);
	if (!ifs.is_open()) {
		throw std::runtime_error(_imgname + std::string(" is not found. You cannot miss fileName!! : readBMP.readBMP(const char*)"));
	}
	
	ifs.read(reinterpret_cast<char*>(&fileh_), sizeof(BITMAPFILEHEADER));
	ifs.read(reinterpret_cast<char*>(&infoh_), sizeof(BITMAPINFOHEADER));
	if (ifs.fail()) {
		ifs.close();
		throw std::runtime_error(_imgname + std::string(" can't read bmpHeader : readBMP.readBMP(const char*)"));
	}

	std::cout << "bitmapinfomation   file : " << _imgname << ", height : " << infoh_.biHeight << ", width : " << infoh_.biWidth << ", format : " << infoh_.biBitCount / BIT_SIZE_ONE_BYTE << std::endl;
	size_t bufSize(infoh_.biWidth * infoh_.biHeight * infoh_.biBitCount / BIT_SIZE_ONE_BYTE);
	img_.resize(bufSize);
	ifs.read(reinterpret_cast<char*>(img_.data()), bufSize);
	if (ifs.fail()) {
		ifs.close();
		throw std::runtime_error(_imgname + std::string(" can't read : readBMP.readBMP(const char*)"));
	}
	ifs.close();
	if (ifs.fail())
		throw std::runtime_error(_imgname + std::string(" can't close : readBMP.readBMP(const char*)"));
}

pronet::readBMP::~readBMP()
{
}
