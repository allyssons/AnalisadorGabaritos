﻿#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <dirent.h>

#include "BMP.h"

using namespace std;

class Operacoes {
private:
	vector<string> caminhosArquivos; // Lista com caminho dos arquivos a serem analisados
	string caminhoPasta;
	BMP *bmp;
	FILE *file;
	unsigned char** matriz;
	int** matrizRetangulo;
	int matrizBolas[50][2];
public:
	void obtemArquivos(string caminhoPasta); // Obtém dos arquivos no diretório especificado
	void abreArquivo(string caminhoArquivo); // Abre um arquivo
	void obtemBitmapBitsSemPaleta(); // Obtém BitmapBits caso não haja paleta
	void obtemBitmapBitsComPaleta(); // Obtém BitmapBits caso haja paleta
	void armazenaImagemProcessada(string contadorImagem, bool floydSteinberg); // Armazena imagem
	char verificaPosicaoCorNaPaleta(char red, char green, char blue); // Verifica qual a posi��o da cor na paleta de cores
	void convertePara256Cores(); // Converte BMP 24 Bits para 256 tons de cinza
	void transformaEmMatriz(); // Transforma o vetor em matriz, alterando a ordem dos valores
	void converteFloydSteinberg(); // Converte BitmapBits utilizando algoritmo Floyd Steinberg
	int calculaValorErro(int tamanho); // Calcula o valor do erro
	BMP& getBmp();
	void setBmp(BMP& bmp);
	string& getCaminhoPasta();
	void setCaminhoPasta(string& caminhoPasta);
	vector<string>& getCaminhosArquivos();
	void setCaminhosArquivos(vector<string>& caminhosArquivos);
	FILE*& getFile();
	void setFile(FILE*& file);
	unsigned char** getMatriz();
	void setMatriz(unsigned char** matriz);
	string getCaminho(int position);
	void converteBinaria();
	void encontraRetangulo();
	int checkMascara(int a, int b);
	void ordenaRetangulo();
	void achaBola();
	int checaBola(int a, int b);
	void vetorResposta();
	void ordenaBolas();
	void corrigeGabarito();
};

// Obtém arquivos do diretório especificado, retorna lista com o caminho dos arquivos
void Operacoes::obtemArquivos(string caminhoPasta) {
	const char * caminhoArray = caminhoPasta.c_str();
	ifstream in;

	DIR *diretorio;
	struct dirent *arquivo;
	string caminhoArquivo;
	int tamanho;

	if ((diretorio = opendir(caminhoArray)) != NULL) { // Diretório existe

													   // Percorre todos os arquivos do diretório
		while ((arquivo = readdir(diretorio)) != NULL) {
			// Verifica se o arquivo é uma imagem .bmp
			tamanho = strlen(arquivo->d_name);

			caminhoArquivo = caminhoPasta + "\\" + arquivo->d_name;

			// Verifica se arquivo  é um .bmp
			if (arquivo->d_name[tamanho - 1] == 'p' &&
				arquivo->d_name[tamanho - 2] == 'm' &&
				arquivo->d_name[tamanho - 3] == 'b' &&
				arquivo->d_name[tamanho - 4] == '.') {

				caminhosArquivos.push_back(caminhoArquivo);
			}
		}
		closedir(diretorio);
	}
	else { //Erro ao abrir o diretório
		cout << "\n\nErro ao abrir o diretorio\n";
	}
}

// Abre arquivos da pasta especificada
void Operacoes::abreArquivo(string caminhoArquivo) {
	const char *caminhoArray = caminhoArquivo.c_str(); // Converte para array de char

	file = fopen(caminhoArray, "r+b"); // Abre o arquivo
	bmp = new BMP();

	if (file) { // Arquivo está aberto
				// Auxiliares para leitura das informações
		short auxShort;
		int auxInt;

		// Seta o cabeçalho do arquivo
		fread(&auxShort, 1, 2, file);
		bmp->getBifh()->setTipo(auxShort);

		fread(&auxInt, 1, 4, file);
		bmp->getBifh()->setTamanho(auxInt);

		fread(&auxShort, 1, 2, file);
		bmp->getBifh()->setReservado1(auxShort);

		fread(&auxShort, 1, 2, file);
		bmp->getBifh()->setReservado2(auxShort);

		fread(&auxInt, 1, 4, file);
		bmp->getBifh()->setOffsetBits(auxInt);

		// Seta o BitmapInfoHeader
		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiSize(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiWidth(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiHeight(auxInt);

		fread(&auxShort, 1, 2, file);
		bmp->getBiih()->setBiPlanes(auxShort);

		fread(&auxShort, 1, 2, file);
		bmp->getBiih()->setBiBitCount(auxShort);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiCompression(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiSizeImage(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiXPelsPerMeter(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiYPelsPerMeter(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiClrUsed(auxInt);

		fread(&auxInt, 1, 4, file);
		bmp->getBiih()->setBiClrImportant(auxInt);

		// Verifica se existe paleta de cores
		if (bmp->getBifh()->getOffsetBits() == 54) { // Não possui paleta
			obtemBitmapBitsSemPaleta();
		}
		else { // Possui paleta
			obtemBitmapBitsComPaleta();
		}
	}
	else {
		cout << "Não foi possível abrir o arquivo";
	}
}

// Obtém BitmapBits caso não haja paleta
void Operacoes::obtemBitmapBitsSemPaleta() {
	int tamanho;
	int quantidadePercorrida = 0;
	int invalido;

	// Seta o tamanho (quantidade de pixels)
	tamanho = bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight();

	//Seta o tamanho de cada um dos vetores de cor
	bmp->setTamanho(tamanho);

	// Identifica o lixo existente na linha
	invalido = calculaValorErro(3);

	// Percorre cada pixel
	for (int i = 0; i < bmp->getBiih()->getBiSizeImage() / 3; i++) {

		if (quantidadePercorrida == bmp->getBiih()->getBiWidth()) { // Efetuou leitura de todos os bits válidos da linha
			quantidadePercorrida = 0;
			fseek(file, invalido, SEEK_CUR);
		}

		// Auxiliar para leitura
		unsigned char auxChar;

		fread(&auxChar, 1, 1, file);
		bmp->setBitmapBitsB(auxChar, i);
		fread(&auxChar, 1, 1, file);
		bmp->setBitmapBitsG(auxChar, i);

		fread(&auxChar, 1, 1, file);
		bmp->setBitmapBitsR(auxChar, i);

		quantidadePercorrida++;
	}
}

// Obtém BitmapBits caso possuir paleta de cores
void Operacoes::obtemBitmapBitsComPaleta() {
	// Obtém o tamanho da paleta
	int tamanhoPaleta = bmp->getBifh()->getOffsetBits() - 54;

	int quantidadePercorrida = 0;
	int largura = bmp->getBiih()->getBiWidth();
	int invalido = calculaValorErro(1); // Identifica o lixo existente na linha
	unsigned char indice;

	// Aloca paleta e vetores para armazenar cor
	bmp->alocarPaleta(tamanhoPaleta);
	bmp->setTamanho(bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight());

	char auxChar; // Auxiliar para leitura

				  // Lê e armazena o conteúdo da paleta
	for (int i = 0; i < tamanhoPaleta / 4; i++) {

		RGBQuad *rgbQuad = new RGBQuad();

		fread(&auxChar, 1, 1, file);
		rgbQuad->setRgbBlue(auxChar);

		fread(&auxChar, 1, 1, file);
		rgbQuad->setRgbGreen(auxChar);

		fread(&auxChar, 1, 1, file);
		rgbQuad->setRgbRed(auxChar);

		fread(&auxChar, 1, 1, file);
		rgbQuad->setRgbReserved(auxChar);

		bmp->setPaletaCores(i, rgbQuad);
	}

	// Lê e armazena o bitmap bits
	for (int i = 0; i < bmp->getBiih()->getBiSizeImage(); i++) {

		if (quantidadePercorrida == bmp->getBiih()->getBiWidth()) { // Efetuou leitura de todos os bits válidos da linha
			quantidadePercorrida = 0;
			fseek(file, invalido, SEEK_CUR);
		}

		// Obtém o índice da cor da paleta
		fread(&indice, 1, 1, file);

		// Seta as cores
		bmp->setBitmapBitsB(bmp->getPaletaCores(indice).getRgbBlue(), i);
		bmp->setBitmapBitsG(bmp->getPaletaCores(indice).getRgbGreen(), i);
		bmp->setBitmapBitsR(bmp->getPaletaCores(indice).getRgbRed(), i);

		quantidadePercorrida++;
	}
}

// Armazena uma imagem no diretório atual com o nome passado no parâmetro
void Operacoes::armazenaImagemProcessada(string contadorImagem, bool floydSteinberg) {
	string caminho = "NovaImagem" + contadorImagem + ".bmp";

	//cout << "Caminho: " << caminho;
	const char *caminhoArray = caminho.c_str(); // Converte para array de char

	FILE *file = fopen(caminhoArray, "w+b");

	// Escreve FileHeader
	short auxShort = bmp->getBifh()->getTipo();

	int auxInt;

	fwrite(&auxShort, sizeof(short), 1, file);

	auxInt = bmp->getBifh()->getTamanho();
	fwrite(&auxInt, sizeof(int), 1, file);

	auxShort = bmp->getBifh()->getReservado1();
	fwrite(&auxShort, sizeof(short), 1, file);

	auxShort = bmp->getBifh()->getReservado2();
	fwrite(&auxShort, sizeof(short), 1, file);

	auxInt = bmp->getBifh()->getOffsetBits();
	fwrite(&auxInt, sizeof(int), 1, file);

	// InfoHeader
	auxInt = bmp->getBiih()->getBiSize();
	fwrite(&auxInt, 4, 1, file);

	auxInt = bmp->getBiih()->getBiWidth();
	fwrite(&auxInt, 4, 1, file);

	auxInt = bmp->getBiih()->getBiHeight();
	fwrite(&auxInt, 4, 1, file);

	auxShort = bmp->getBiih()->getBiPlanes();
	fwrite(&auxShort, 2, 1, file);

	auxShort = bmp->getBiih()->getBiBitCount();
	fwrite(&auxShort, 2, 1, file);

	auxInt = bmp->getBiih()->getBiCompression();
	fwrite(&auxInt, 4, 1, file);

	auxInt = bmp->getBiih()->getBiSizeImage();
	fwrite(&auxInt, 4, 1, file);
	
	auxInt = bmp->getBiih()->getBiXPelsPerMeter();
	fwrite(&auxInt, 4, 1, file);

	auxInt = bmp->getBiih()->getBiYPelsPerMeter();
	fwrite(&auxInt, 4, 1, file);
	
	auxInt = bmp->getBiih()->getBiClrUsed();
	fwrite(&auxInt, 4, 1, file);
	
	auxInt = bmp->getBiih()->getBiClrImportant();
	fwrite(&auxInt, 4, 1, file);

	int tamanhoPaleta = bmp->getBifh()->getOffsetBits() - 54;

	int quantidadePercorrida = 0;
	unsigned char indiceCor;
	unsigned char zero = 255;
	// Paleta de cores
	if (bmp->getBifh()->getOffsetBits() != 54) { // Possui paleta de cores
		int invalido = calculaValorErro(1); // Identifica o lixo existente na linha

		RGBQuad rgbQuad;

		// Armazena paleta
		for (int i = 0; i < tamanhoPaleta / 4; i++) {
			rgbQuad = bmp->getPaletaCores(i);
			fwrite(&rgbQuad, 4, 1, file);
		}

		// Verifica qual a posição da cor na paleta de cores
		for (int i = 0; i <  bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight(); i++) {
			// Obtem o índice da cor
			indiceCor = verificaPosicaoCorNaPaleta(bmp->getBitmapBitsR(i), bmp->getBitmapBitsG(i), bmp->getBitmapBitsB(i));
			fwrite(&indiceCor, 1, 1, file);
			quantidadePercorrida++;

			if (quantidadePercorrida == bmp->getBiih()->getBiWidth()) { // Efetuou leitura de todos os bits válidos da linha
				quantidadePercorrida = 0;

				// Completa linha com caracteres inválidos
				for (int j = 0; j < invalido; j++) {
					fwrite(&zero, 1, 1, file);
				}
			}
		}
	}
	else { // Não possui paleta
		int invalido = calculaValorErro(3); // Identifica o lixo existente na linha

		char auxChar;

		// Armazena Bitmap Bits
		for (int i = 0; i < bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight(); i++) {

			auxChar = bmp->getBitmapBitsB(i);
			fwrite(&auxChar, sizeof(char), 1, file);

			auxChar = bmp->getBitmapBitsG(i);
			fwrite(&auxChar, sizeof(char), 1, file);

			auxChar = bmp->getBitmapBitsR(i);
			fwrite(&auxChar, sizeof(char), 1, file);

			quantidadePercorrida++;

			if (quantidadePercorrida == bmp->getBiih()->getBiWidth()) { // Efetuou leitura de todos os bits v�lidos da linha
				quantidadePercorrida = 0;

				// Completa linha com caracteres inv�lidos
				for (int j = 0; j < invalido; j++) {
					fwrite(&zero, 1, 1, file);
				}
			}
		}
	}

	fclose(file);
}

// Verifica qual a posição da cor na paleta de cores
char Operacoes::verificaPosicaoCorNaPaleta(char red, char green, char blue) {

	int tamanhoPaleta = bmp->getBifh()->getOffsetBits() - 54;
	int a;
	for (unsigned char i = 0; i < tamanhoPaleta / 4; i++) {
		// TODO	
		int a;

		if (red == bmp->getPaletaCores(i).getRgbRed() &&
			green == bmp->getPaletaCores(i).getRgbGreen() &&
			blue == bmp->getPaletaCores(i).getRgbBlue()) {
			return i;
		}
	}

	printf("Não achou");
	return 0;
}

// Converte BMP 24 para 256 tons de cinza
void Operacoes::convertePara256Cores() {
	// Aloca paleta
	bmp->alocarPaleta(1024);
	unsigned char indicePaleta = 0;

	// Preenche paleta

	while (indicePaleta <= 255) {

		RGBQuad *rgbQuad = new RGBQuad();
		rgbQuad->setRgbBlue(indicePaleta);
		rgbQuad->setRgbGreen(indicePaleta);
		rgbQuad->setRgbRed(indicePaleta);
		rgbQuad->setRgbReserved(0);

		bmp->setPaletaCores(indicePaleta, rgbQuad);

		if (indicePaleta == 255)
			break;

		indicePaleta++;
	}

	// Preenche BitMapBits com a nova cor
	for (int i = 0; i < bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight(); i++) {

		bmp->setBitmapBitsB(bmp->getBitmapBitsG(i), i);
		bmp->setBitmapBitsR(bmp->getBitmapBitsG(i), i);
	}

	// Altera informações no FileHeader
	bmp->getBifh()->setTamanho(54 + 1024 + bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + calculaValorErro(1) *  bmp->getBiih()->getBiHeight()); // Tamanho = Cabe�alhos + Paleta de cores + Tamanho sem lixo largura x altura) + lixo
	bmp->getBifh()->setOffsetBits(54 + 1024); // Deslocamento = cabeçalhos + paleta de cores

											  // Altera informações do InfoHeader
	bmp->getBiih()->setBiBitCount(8); // 8 bits
	bmp->getBiih()->setBiSizeImage(bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + calculaValorErro(1) * bmp->getBiih()->getBiHeight()); // Tamanho = Tamanho sem lixo (largura x altura) + lixo
}

string& Operacoes::getCaminhoPasta() {
	return caminhoPasta;
}

void Operacoes::setCaminhoPasta(string& caminhoPasta) {
	this->caminhoPasta = caminhoPasta;
}

vector<string>& Operacoes::getCaminhosArquivos() {
	return caminhosArquivos;
}

void Operacoes::setCaminhosArquivos(
	vector<string>& caminhosArquivos) {
	this->caminhosArquivos = caminhosArquivos;
}

FILE*& Operacoes::getFile() {
	return file;
}

void Operacoes::setFile(FILE*& file) {
	this->file = file;
}

unsigned char** Operacoes::getMatriz() {
	return matriz;
}

void Operacoes::setMatriz(unsigned char** matriz) {
	this->matriz = matriz;
}

// Calcula o valor do erro
int Operacoes::calculaValorErro(int tamanho) {
	if ((tamanho * bmp->getBiih()->getBiWidth() % 4) != 0) {
		return (4 - (tamanho * bmp->getBiih()->getBiWidth() % 4));
	}
	else {
		return 0;
	}

}

// Transforma o vetor em matriz, alterando a ordem dos valores
void Operacoes::transformaEmMatriz() {
	// Aloca matriz
	matriz = new unsigned char*[bmp->getBiih()->getBiHeight()];
	for (int i = 0; i < bmp->getBiih()->getBiHeight(); i++) {
		matriz[i] = new unsigned char[bmp->getBiih()->getBiWidth()];
	}

	int linha = bmp->getBiih()->getBiHeight() - 1;
	int coluna = 0;

	// Preenche a matriz com os valores do Bitmap Bits
	for (int i = 0; i < bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth(); i++) {
		if (coluna == bmp->getBiih()->getBiWidth()) {
			linha--;
			coluna = 0;
		}
		matriz[linha][coluna] = bmp->getBitmapBitsG(i);

		coluna++;
	}
}

// Converte BitmapBits utilizando algoritmo Floyd Steinberg
void Operacoes::converteFloydSteinberg() {
	unsigned char pixelAntigo;
	unsigned char erro;
	unsigned char limiar = 127;
	
	
	// Transforma Bitmap bits em matriz
	transformaEmMatriz();

	// Converte os pixels
	for (int i = 0; i < bmp->getBiih()->getBiHeight(); i++) {
		for (int j = 0; j < bmp->getBiih()->getBiWidth(); j++) {
			pixelAntigo = matriz[i][j];
			
			if (matriz[i][j] <= limiar) {
				matriz[i][j] = 255;
			}
			else {
				matriz[i][j] = 0;
			}

			erro = pixelAntigo - matriz[i][j];

			if (j + 1 != bmp->getBiih()->getBiWidth()) {
				matriz[i][j + 1] += 7.0 / 16.0 * erro;

				if (matriz[i][j + 1] > 255) {
					matriz[i][j + 1] = 255;
				}
				if (matriz[i][j + 1] < 0) {
					matriz[i][j + 1] = 0;
				}
			}

			if (j + 1 != bmp->getBiih()->getBiWidth() && i + 1 != bmp->getBiih()->getBiHeight()) {
				matriz[i + 1][j + 1] += 1.0 / 16.0 * erro;

				if (matriz[i + 1][j + 1] > 255) {
					matriz[i + 1][j + 1] = 255;
				}
				if (matriz[i + 1][j + 1] < 0) {
					matriz[i + 1][j + 1] = 0;
				}
			}

			if (i + 1 != bmp->getBiih()->getBiHeight()) {
				matriz[i + 1][j] += 5.0 / 16.0 * erro;

				if (matriz[i + 1][j] > 255) {
					matriz[i + 1][j] = 255;
				}
				if (matriz[i + 1][j] < 0) {
					matriz[i + 1][j] = 0;
				}
			}

			if (j - 1 >= 0 && i + 1 != bmp->getBiih()->getBiHeight()) {
				matriz[i + 1][j - 1] += 3.0 / 16.0 * erro;

				if (matriz[i + 1][j - 1] > 255) {
					matriz[i + 1][j - 1] = 255;
				}
				if (matriz[i + 1][j - 1] < 0) {
					matriz[i + 1][j - 1] = 0;
				}
			}

		}
	}

	bmp->setTamanho((bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight()));

	// Altera o BitmapBits
	int posicaoBitmap = 0;

	for (int i = bmp->getBiih()->getBiHeight() - 1; i >= 0; i--) {
		for (int j = 0; j < bmp->getBiih()->getBiWidth(); j++) {
			bmp->setBitmapBitsB(matriz[i][j], posicaoBitmap);
			bmp->setBitmapBitsG(matriz[i][j], posicaoBitmap);
			bmp->setBitmapBitsR(matriz[i][j], posicaoBitmap);
			posicaoBitmap++;
		}
	}

	// Aloca paleta
	int tamanhoPaleta = 1024;
	bmp->alocarPaleta(tamanhoPaleta);

	// Preenche paleta
	RGBQuad *cor1 = new RGBQuad();
	cor1->setRgbBlue(0);
	cor1->setRgbGreen(0);
	cor1->setRgbRed(0);
	cor1->setRgbReserved(0);

	

	bmp->setPaletaCores(0, cor1);

	RGBQuad *cor2 = new RGBQuad();
	cor2->setRgbBlue(255);
	cor2->setRgbGreen(255);
	cor2->setRgbRed(255);
	cor2->setRgbReserved(0);

	bmp->setPaletaCores(1, cor2);

	// Define informações a serem inseridas como lixo
	RGBQuad *rgbQuad = new RGBQuad();
	rgbQuad->setRgbBlue(180);
	rgbQuad->setRgbGreen(180);
	rgbQuad->setRgbRed(180);
	rgbQuad->setRgbReserved(0);

	// preenche a paleta com 'lixo'
	for (int i = 2; i < tamanhoPaleta / 4; i++) {
		bmp->setPaletaCores(i, rgbQuad);
	}

	// Altera cabeçalho
	// Altera informações no FileHeader
	bmp->getBifh()->setTamanho(54 + tamanhoPaleta + bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + (calculaValorErro(1) *  bmp->getBiih()->getBiHeight())); // Tamanho = Cabe�alhos + Paleta de cores + Tamanho sem lixo largura x altura) + lixo
	bmp->getBifh()->setOffsetBits(54 + tamanhoPaleta); // Deslocamento = cabeçalhos + paleta de cores

													   // Altera informa��es do InfoHeader
	bmp->getBiih()->setBiBitCount(8); // 8 bits
	bmp->getBiih()->setBiSizeImage(bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + (calculaValorErro(1) *  bmp->getBiih()->getBiHeight())); // Tamanho = Tamanho sem lixo (largura x altura) + lixo
}

void Operacoes::encontraRetangulo() {
	// Transforma Bitmap bits em matriz
	transformaEmMatriz();

	//Armazenará o valor de cada retangulo
	matrizRetangulo = new int*[3];
	for (int i = 0; i < 3; i++) {
		matrizRetangulo[i] = new int[2];
	}
	int w = 0;
	int z = 0;
	int ans;

	for (int i = 0; i < bmp->getBiih()->getBiHeight(); i++) {
		for (int j = 0; j < bmp->getBiih()->getBiWidth(); j++) {

			if (matriz[i][j] == 0) {
				ans = checkMascara(i, j);
				if (ans == 0) {
					matrizRetangulo[w][z] = i;
					z++;
					matrizRetangulo[w][z] = j;
					z= 0;
					w++;
					for (int a = i; a < (40+i); a++) {
						for (int b = j; b < (90+j); b++) {
							matriz[a][b] = 255;
							if (b == bmp->getBiih()->getBiWidth()) break;
						}
						if (a == bmp->getBiih()->getBiHeight()) break;
					}
				}
			}
		}
	}
}

void Operacoes::achaBola() {
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 2; j++) {
			matrizBolas[i][j] = 0;
		}
	}
	encontraRetangulo();
	ordenaRetangulo();
	double dist1 = sqrt(pow(abs(matrizRetangulo[0][0] - matrizRetangulo[1][0]), 2) + pow(abs(matrizRetangulo[0][1] - matrizRetangulo[1][1]), 2));
	double dist2 = sqrt(pow(abs(matrizRetangulo[0][0] - matrizRetangulo[2][0]), 2) + pow(abs(matrizRetangulo[0][1] - matrizRetangulo[2][1]), 2));
	int w = 0;
	int z = 0;
	double k = 0;
	int flag = -1;
	int fDesloc = dist2 * 0.2173; //Proporcao do ponto do retangulo 0 até o primeiro segmento
	int oDesloc = fDesloc + (dist2 * 0.1739); //Proponcao entre cada segmento
	double aux0 = matrizRetangulo[0][0] + fDesloc;
	double aux1 = matrizRetangulo[0][1] + 20;
	k = (double)(matrizRetangulo[1][0] - matrizRetangulo[0][0]) / (matrizRetangulo[1][1] - matrizRetangulo[0][1]);
	for (int j = 0; j < dist1; j++) {
		if (matriz[(int)aux0][(int)aux1] == 0) {
			flag = checaBola(aux0, aux1);

			if (flag == 0) {
				for (int b = aux1; b < (45 + aux1); b++) {
					matriz[(int)aux0][b] = 255;
				}
				matrizBolas[w][z] = 1; //Corresponde a letra "a"
				z++;
				matrizBolas[w][z] = aux1;
				z = 0;
				w++;
				aux0 += k * 50;
				aux1 += 50;
			}

		}
		aux0 += k;
		aux1++;
	}

	for(int i = 0; i < 4; i++){
		aux0 = matrizRetangulo[0][0] + oDesloc;
		aux1 = matrizRetangulo[0][1] + 20;
		oDesloc += (dist2 * 0.1739);
		k = (double) (matrizRetangulo[1][0] - matrizRetangulo[0][0]) / (matrizRetangulo[1][1] - matrizRetangulo[0][1]);
		for (int j = 0; j < dist1; j++) {
			if (matriz[(int)aux0][(int)aux1] == 0) {
				flag = checaBola(aux0, aux1);
				if (flag == 0) {

					for (int b = aux1; b < (45 + aux1); b++) {
						matriz[(int)aux0][b] = 255;
					}
					matrizBolas[w][z] = i+2; //Corresponde a cada alternativa
					z++;
					matrizBolas[w][z] = aux1;
					z = 0;
					w++;
					aux0 += k * 50;
					aux1 += 50;
					j += 50;
				}
			}
			aux0+=k;
			aux1++;
		}
	}
}

void Operacoes::vetorResposta() {
	achaBola(); //Encontra as respostas e seus respectivos Ys

	double dist2 = sqrt(pow(abs(matrizRetangulo[0][0] - matrizRetangulo[1][0]), 2) + pow(abs(matrizRetangulo[0][1] - matrizRetangulo[1][1]), 2));
	double distTQ = dist2 - dist2*(0.0652173); //Distancia de todas as questoes
	double deslocKQ = distTQ / 10;//Distancia entre cada questao
	double rangeInicial =0;
	double rangeFinal = 0;

	for (int i = 0; i < 50; i++) {
		if (matrizBolas[i][1] == 0) {
			break;
		}
		rangeInicial = matrizRetangulo[0][1] + dist2*(0.0652173);
		rangeFinal = rangeInicial + deslocKQ;
		for (int j = 0; j < 10; j++) {
			if (matrizBolas[i][1] < rangeFinal && matrizBolas[i][1] > rangeInicial) {
				matrizBolas[i][1] = j + 1;
				break;
			}
			rangeInicial = rangeFinal;
			rangeFinal += deslocKQ;
		}

	}

}

void Operacoes::corrigeGabarito() {
	char respostas[10];
	for (int i = 0; i < 10; i++) {
		respostas[i] = 'Z';
	}
	//Procura respostas multiplas
	for (int i = 0; i < 50; i++) {
		if (matrizBolas[i][1] == 0) break;
		for (int j = i; j < 50; j++) {
			if (matrizBolas[j][1] == 0) break;
			if (matrizBolas[i][1] == matrizBolas[j][1]) {
				respostas[i] = 'X';
				break;
			}
		}
	}
	int j;
	int i;
	for (j = 0; j < 10; j++) {
		for (i = 0; i < 50; i++) {
			if (matrizBolas[i][1] == 0) break;
			if (matrizBolas[i][1] == j+1) {
				break;
			}
		}
		if (matrizBolas[i][1] == 0) {
			respostas[j] = 'O';
		}
	}
	int k = 0;
	int aux;
	for (int w = 0; w < 10; w++) {
		if (respostas[w] == 'Z') continue;
		while (w != matrizBolas[k][1]) {
			k++;
		}
		aux = matrizBolas[k][0];
		switch (aux)
		{
		default:
			break;
		}
	}

}
int Operacoes::checaBola(int a, int b) {

	for (int i = b; i < (30 + b); i++) {
		if (matriz[a][i] != 0) {
			return 1;
		}
		if (i == bmp->getBiih()->getBiWidth()) return 1;
	}
	return 0;
}

void Operacoes::ordenaBolas() {
	int maiorY = -1;
	int valMaiorY = -1;
	for (int i = 0; i < 50; i++) {
		if (valMaiorY < matrizBolas[i][1]) {
			maiorY = i;
			valMaiorY = matrizBolas[i][1];
		}
	}
}

void Operacoes::ordenaRetangulo(){
	int matrizAux[3][2];
	//Copia a matriz
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			matrizAux[i][j] = matrizRetangulo[i][j];
		}
	}

	//Encontra retangulo de maior Y
	int maiorY = -1;
	int valMaiorY = -1;
	for(int i = 0; i < 3; i++){
		if (valMaiorY < matrizAux[i][1]) {
			maiorY = i;
			valMaiorY = matrizAux[i][1];
		}
	}
	matrizRetangulo[1][0] = matrizAux[maiorY][0];
	matrizRetangulo[1][1] = matrizAux[maiorY][1];

	//Encontra retangulo de maior X
	int maiorX = -1;
	int valMaiorX = -1;
	for(int i = 0; i < 3; i++){
		if (maiorX < matrizAux[i][0]) {
			maiorX = i;
			valMaiorX = matrizAux[i][0];
		}
	}
	matrizRetangulo[2][0] = matrizAux[maiorX][0];
	matrizRetangulo[2][1] = matrizAux[maiorX][1];


	//Encontra o outro
	matrizRetangulo[0][0] = matrizAux[3 - (maiorX + maiorY)][0];
	matrizRetangulo[0][1] = matrizAux[3 - (maiorX + maiorY)][1];

}

int Operacoes::checkMascara(int a, int b) {
	for (int i = a; i < (40+a); i++) {
		for (int j = b; j < (90 + b); j++) {
			if (matriz[i][j] != 0) {
				return 1;
				if (j == bmp->getBiih()->getBiWidth()) break;
			}
			if (i == bmp->getBiih()->getBiHeight()) break;
		}
	}
	return 0;

}

//Converte uma imagem para uma imagem binária
void Operacoes::converteBinaria() {
	unsigned char limiar = 98;

	// Transforma Bitmap bits em matriz
	transformaEmMatriz();

	// Converte os pixels
	for (int i = 0; i < bmp->getBiih()->getBiHeight(); i++) {
		for (int j = 0; j < bmp->getBiih()->getBiWidth(); j++) {
			if (matriz[i][j] >= limiar) {
				matriz[i][j] = 255;
			}
			else {
				matriz[i][j] = 0;
			}
		}
	}

	bmp->setTamanho((bmp->getBiih()->getBiWidth() * bmp->getBiih()->getBiHeight()));

	// Altera o BitmapBits
	int posicaoBitmap = 0;

	for (int i = bmp->getBiih()->getBiHeight() - 1; i >= 0; i--) {
		for (int j = 0; j < bmp->getBiih()->getBiWidth(); j++) {
			bmp->setBitmapBitsB(matriz[i][j], posicaoBitmap);
			bmp->setBitmapBitsG(matriz[i][j], posicaoBitmap);
			bmp->setBitmapBitsR(matriz[i][j], posicaoBitmap);
			posicaoBitmap++;
		}
	}

	// Aloca paleta
	int tamanhoPaleta = 1024;
	bmp->alocarPaleta(tamanhoPaleta);

	// Preenche paleta
	RGBQuad *cor1 = new RGBQuad();
	cor1->setRgbBlue(0);
	cor1->setRgbGreen(0);
	cor1->setRgbRed(0);
	cor1->setRgbReserved(0);

	bmp->setPaletaCores(0, cor1);

	RGBQuad *cor2 = new RGBQuad();
	cor2->setRgbBlue(255);
	cor2->setRgbGreen(255);
	cor2->setRgbRed(255);
	cor2->setRgbReserved(0);

	bmp->setPaletaCores(1, cor2);

	// Define informações a serem inseridas como lixo
	RGBQuad *rgbQuad = new RGBQuad();
	rgbQuad->setRgbBlue(180);
	rgbQuad->setRgbGreen(180);
	rgbQuad->setRgbRed(180);
	rgbQuad->setRgbReserved(0);

	// preenche a paleta com 'lixo'
	for (int i = 2; i < tamanhoPaleta / 4; i++) {
		bmp->setPaletaCores(i, rgbQuad);
	}

	// Altera cabeçalho
	// Altera informações no FileHeader
	bmp->getBifh()->setTamanho(54 + tamanhoPaleta + bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + (calculaValorErro(1) *  bmp->getBiih()->getBiHeight())); // Tamanho = Cabe�alhos + Paleta de cores + Tamanho sem lixo largura x altura) + lixo
	bmp->getBifh()->setOffsetBits(54 + tamanhoPaleta); // Deslocamento = cabeçalhos + paleta de cores

	// Altera informa��es do InfoHeader
	bmp->getBiih()->setBiBitCount(8); // 8 bits
	bmp->getBiih()->setBiSizeImage(bmp->getBiih()->getBiHeight() * bmp->getBiih()->getBiWidth() + (calculaValorErro(1) *  bmp->getBiih()->getBiHeight())); // Tamanho = Tamanho sem lixo (largura x altura) + lixo
}

string Operacoes::getCaminho(int position) {
	return caminhosArquivos[position];
}


