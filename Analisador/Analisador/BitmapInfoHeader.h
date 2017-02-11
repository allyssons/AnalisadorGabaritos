#include <iostream>
using namespace std;

class BitmapInfoHeader {
private:
	int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
public:
	BitmapInfoHeader();

	unsigned short getBiBitCount();
	void setBiBitCount(unsigned short biBitCount);
	int getBiClrImportant();
	void setBiClrImportant(int biClrImportant);
	int getBiClrUsed();
	void setBiClrUsed(int biClrUsed);
	int getBiCompression();
	void setBiCompression(int biCompression);
	int getBiHeight();
	void setBiHeight(int biHeight);
	unsigned short getBiPlanes();
	void setBiPlanes(unsigned short biPlanes);
	int getBiSize();
	void setBiSize(int biSize);
	int getBiSizeImage();
	void setBiSizeImage(int biSizeImage);
	int getBiWidth();
	void setBiWidth(int biWidth);
	int getBiXPelsPerMeter();
	void setBiXPelsPerMeter(int biXPelsPerMeter);
	int getBiYPelsPerMeter();
	void setBiYPelsPerMeter(int biYPelsPerMeter);
};

BitmapInfoHeader::BitmapInfoHeader() {

}

unsigned short BitmapInfoHeader::getBiBitCount() {
	return biBitCount;
}

void BitmapInfoHeader::setBiBitCount(unsigned short biBitCount) {
	this->biBitCount = biBitCount;
}

int BitmapInfoHeader::getBiClrImportant() {
	return biClrImportant;
}

void BitmapInfoHeader::setBiClrImportant(int biClrImportant) {
	this->biClrImportant = biClrImportant;
}

int BitmapInfoHeader::getBiClrUsed() {
	return biClrUsed;
}

void BitmapInfoHeader::setBiClrUsed(int biClrUsed) {
	this->biClrUsed = biClrUsed;
}

int BitmapInfoHeader::getBiCompression() {
	return biCompression;
}

void BitmapInfoHeader::setBiCompression(int biCompression) {
	this->biCompression = biCompression;
}

int BitmapInfoHeader::getBiHeight() {
	return biHeight;
}

void BitmapInfoHeader::setBiHeight(int biHeight) {
	this->biHeight = biHeight;
}

unsigned short BitmapInfoHeader::getBiPlanes() {
	return biPlanes;
}

void BitmapInfoHeader::setBiPlanes(unsigned short biPlanes) {
	this->biPlanes = biPlanes;
}

int BitmapInfoHeader::getBiSize() {
	return biSize;
}

void BitmapInfoHeader::setBiSize(int biSize) {
	this->biSize = biSize;
}

int BitmapInfoHeader::getBiSizeImage() {
	return biSizeImage;
}

void BitmapInfoHeader::setBiSizeImage(int biSizeImage) {
	this->biSizeImage = biSizeImage;
}

int BitmapInfoHeader::getBiWidth() {
	return biWidth;
}

void BitmapInfoHeader::setBiWidth(int biWidth) {
	this->biWidth = biWidth;
}

int BitmapInfoHeader::getBiXPelsPerMeter() {
	return biXPelsPerMeter;
}

void BitmapInfoHeader::setBiXPelsPerMeter(int biXPelsPerMeter) {
	this->biXPelsPerMeter = biXPelsPerMeter;
}

int BitmapInfoHeader::getBiYPelsPerMeter() {
	return biYPelsPerMeter;
}

void BitmapInfoHeader::setBiYPelsPerMeter(int biYPelsPerMeter) {
	this->biYPelsPerMeter = biYPelsPerMeter;
}
