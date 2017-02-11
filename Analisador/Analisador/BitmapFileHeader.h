#include <iostream>
using namespace std;

// Cabeçalho do arquivo bitmap
class BitmapFileHeader {

private:
	short tipo;
	int tamanho;
	short reservado1;
	short reservado2;
	int offsetBits;

public:
	BitmapFileHeader(short tipo_, int tamanho, short reservado1, short reservado2, int offsetBits);
	BitmapFileHeader();

	int getOffsetBits();
	void setOffsetBits(int offsetBits);
	short getTipo();
	void setTipo(short tipo);
	int getTamanho();
	void setTamanho(int tamanho);
	short getReservado1();
	void setReservado1(short reservado1);
	short getReservado2();
	void setReservado2(short reservado2);
};

BitmapFileHeader::BitmapFileHeader() {
}

BitmapFileHeader::BitmapFileHeader(short tipo_, int tamanho_, short reservado1_,
	short reservado2_, int offsetBits_) {
	tipo = tipo_;
	tamanho = tamanho_;
	reservado1 = reservado1_;
	reservado2 = reservado2_;
	offsetBits = offsetBits_;
}


short BitmapFileHeader::getTipo() {
	return tipo;
}

void BitmapFileHeader::setTipo(short tipo) {
	this->tipo = tipo;
}

int BitmapFileHeader::getTamanho() {
	return tamanho;
}

void BitmapFileHeader::setTamanho(int tamanho) {
	this->tamanho = tamanho;
}


short BitmapFileHeader::getReservado1() {
	return reservado1;
}

void BitmapFileHeader::setReservado1(short reservado1) {
	this->reservado1 = reservado1;
}

short BitmapFileHeader::getReservado2() {
	return reservado2;
}

void BitmapFileHeader::setReservado2(short reservado2) {
	this->reservado2 = reservado2;
}


int BitmapFileHeader::getOffsetBits() {
	return offsetBits;
}

void BitmapFileHeader::setOffsetBits(int offsetBits) {
	this->offsetBits = offsetBits;
}

