#include <iostream>
#include <cstdlib>

using namespace std;

class Correcao {
private:
	char* gabarito;
	char* respostas;
	int acertos;
	int erros;
	int multiplasAlternativas;
	int semResposta;

public:
	Correcao(char* gabarito);
	char* getGabarito();
	void setGabarito(char* gabarito);
	char* getRespostas();
	void setRespostas(char* respostas);
	int getAcertos();
	void setAcertos(int acertos);
	int getErros();
	void setErros(int erros);
	int getMultiplasAlternativas();
	void setMultiplasAlternativas(int multiplasAlternativas);
	int getSemResposta();
	void setSemResposta(int semResposta);
};

Correcao::Correcao(char* gabarito) {
	this->gabarito = (char*)malloc(10);
	this->gabarito = gabarito;
	this->respostas = (char*)malloc(10);
	this->acertos = 0;
	this->erros = 0;
	this->multiplasAlternativas = 0;
	this->semResposta = 0;
}

char* Correcao::getGabarito() {
	return gabarito;
}

void Correcao::setGabarito(char* gabarito) {
	this->gabarito = gabarito;
}

char* Correcao::getRespostas() {
	return respostas;
}

void Correcao::setRespostas(char* respostas) {
	this->respostas = respostas;
}

int Correcao::getAcertos() {
	return acertos;
}

void Correcao::setAcertos(int acertos) {
	this->acertos = acertos;
}

int Correcao::getErros() {
	return erros;
}

void Correcao::setErros(int erros) {
	this->erros = erros;
}

int Correcao::getMultiplasAlternativas() {
	return multiplasAlternativas;
}

void Correcao::setMultiplasAlternativas(int multiplasAlternativas) {
	this->multiplasAlternativas = multiplasAlternativas;
}

int Correcao::getSemResposta() {
	return semResposta;
}

void Correcao::setSemResposta(int semResposta) {
	this->semResposta = semResposta;
}