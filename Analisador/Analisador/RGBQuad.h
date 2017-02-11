#include <iostream>
using namespace std;

class RGBQuad {
private:
	char rgbBlue;
	char rgbGreen;
	char rgbRed;
	char rgbReserved;
public:
	RGBQuad();
	char getRgbBlue();
	void setRgbBlue(char rgbBlue);
	char getRgbGreen();
	void setRgbGreen(char rgbGreen);
	char getRgbRed();
	void setRgbRed(char rgbRed);
	char getRgbReserved();
	void setRgbReserved(char rgbReserved);
};

RGBQuad::RGBQuad() {}

char RGBQuad::getRgbBlue() {
	return rgbBlue;
}

void RGBQuad::setRgbBlue(char rgbBlue) {
	this->rgbBlue = rgbBlue;
}

char RGBQuad::getRgbGreen() {
	return rgbGreen;
}

void RGBQuad::setRgbGreen(char rgbGreen) {
	this->rgbGreen = rgbGreen;
}

char RGBQuad::getRgbRed() {
	return rgbRed;
}

void RGBQuad::setRgbRed(char rgbRed) {
	this->rgbRed = rgbRed;
}

char RGBQuad::getRgbReserved() {
	return rgbReserved;
}

void RGBQuad::setRgbReserved(char rgbReserved) {
	this->rgbReserved = rgbReserved;
}
