#include <iostream>
#include <cstdlib>
#include "BitmapFileHeader.h"
#include "BitmapInfoHeader.h"
#include "RGBQuad.h"

using namespace std;

class BMP {
private:
	BitmapFileHeader *bifh;
	BitmapInfoHeader *biih;
	RGBQuad *paletaCores;
	unsigned char* bitmapBitsR;
	unsigned char* bitmapBitsG;
	unsigned char* bitmapBitsB;

public:
	BMP();
	BMP(int tamanhoPaleta, int tamanhoBitmap);
	void setTamanho(int tamanho);
	void alocarPaleta(int tamanho);
	BitmapFileHeader*& getBifh();
	void setBifh(BitmapFileHeader*& bifh);
	BitmapInfoHeader*& getBiih();
	void setBiih(BitmapInfoHeader*& biih);
	unsigned char getBitmapBitsB(int position);
	unsigned char* getBitmapBitsB();
	void setBitmapBitsB(unsigned char* bitmapBitsB);
	void setBitmapBitsB(unsigned char bitmapBitsB, int position);
	unsigned char getBitmapBitsG(int position);
	unsigned char* getBitmapBitsG();
	void setBitmapBitsG(unsigned char* bitmapBitsG);
	void setBitmapBitsG(unsigned char bitmapBitsG, int position);
	unsigned char getBitmapBitsR(int position);
	unsigned char* getBitmapBitsR();
	void setBitmapBitsR(unsigned char* bitmapBitsR);
	void setBitmapBitsR(unsigned char bitmapBitsR, int position);
	RGBQuad* getPaletaCores();
	void setPaletaCores(RGBQuad* paletaCores);
	void setPaletaCores(int position, RGBQuad *cor);
	RGBQuad getPaletaCores(int position);
};

BMP::BMP() {
	bifh = new BitmapFileHeader();
	biih = new BitmapInfoHeader();
}

BMP::BMP(int tamanhoPaleta, int tamanhoBitmap) {
	bifh = new BitmapFileHeader();
	biih = new BitmapInfoHeader();
	//paletaCores = new RGBQuad();
	paletaCores = (RGBQuad*)malloc(tamanhoPaleta);
	bitmapBitsR = (unsigned char*)malloc(tamanhoBitmap);
	bitmapBitsG = (unsigned char*)malloc(tamanhoBitmap);
	bitmapBitsB = (unsigned char*)malloc(tamanhoBitmap);
}

// Aloca vetores de armazenamento da cor
void BMP::setTamanho(int tamanho) {
	bitmapBitsR = (unsigned char*)malloc(tamanho);
	bitmapBitsG = (unsigned char*)malloc(tamanho);
	bitmapBitsB = (unsigned char*)malloc(tamanho);
}

// Aloca paleta de cores
void BMP::alocarPaleta(int tamanho) {
	//paletaCores = new RGBQuad();
	paletaCores = (RGBQuad*)malloc(tamanho);
}

BitmapFileHeader*& BMP::getBifh() {
	return bifh;
}

void BMP::setBifh(BitmapFileHeader*& bifh) {
	this->bifh = bifh;
}

BitmapInfoHeader*& BMP::getBiih() {
	return biih;
}

void BMP::setBiih(BitmapInfoHeader*& biih) {
	this->biih = biih;
}

unsigned char* BMP::getBitmapBitsB() {
	return bitmapBitsB;
}

void BMP::setBitmapBitsB(unsigned char* bitmapBitsB) {
	this->bitmapBitsB = bitmapBitsB;
}

unsigned char* BMP::getBitmapBitsG() {
	return bitmapBitsG;
}

void BMP::setBitmapBitsG(unsigned char* bitmapBitsG) {
	this->bitmapBitsG = bitmapBitsG;
}

unsigned char* BMP::getBitmapBitsR() {
	return bitmapBitsR;
}

void BMP::setBitmapBitsR(unsigned char* bitmapBitsR) {
	this->bitmapBitsR = bitmapBitsR;
}

RGBQuad* BMP::getPaletaCores() {
	return paletaCores;
}

void BMP::setPaletaCores(RGBQuad* paletaCores) {
	this->paletaCores = paletaCores;
}

RGBQuad BMP::getPaletaCores(int position) {
	return paletaCores[position];
}

void BMP::setPaletaCores(int position, RGBQuad *cor) {
	this->paletaCores[position] = *cor;
}

unsigned char BMP::getBitmapBitsR(int position) {
	return bitmapBitsR[position];
}

void BMP::setBitmapBitsR(unsigned char bitmapBitsR, int position) {
	this->bitmapBitsR[position] = bitmapBitsR;
}

unsigned char BMP::getBitmapBitsG(int position) {
	return bitmapBitsG[position];
}

void BMP::setBitmapBitsG(unsigned char bitmapBitsG, int position) {
	this->bitmapBitsG[position] = bitmapBitsG;
}

unsigned char BMP::getBitmapBitsB(int position) {
	return bitmapBitsB[position];
}

void BMP::setBitmapBitsB(unsigned char bitmapBitsB, int position) {
	this->bitmapBitsB[position] = bitmapBitsB;
}


