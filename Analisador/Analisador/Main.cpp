#include <iostream>
#include "Operacoes.h"
#include <string>
#include <string.h>

using namespace std;

int main() {

	int i = 0;
	int opcao = 0;
	string idArquivo = "";

	Operacoes *operacoes = new Operacoes();
	string caminhoPasta;

	cout << "Pasta: ";
	//cin >> caminhoPasta;
	caminhoPasta = "C:\\Imagens";

	operacoes->setCaminhoPasta(caminhoPasta);

	cout << "\n-----------------------------" << endl;
	cout << "Pasta: " + caminhoPasta << endl;
	cout << "-----------------------------" << endl;

	// Obtém arquivos do diretório
	operacoes->obtemArquivos(caminhoPasta);

	if (operacoes->getCaminhosArquivos().size() != 0) {
		cout << "Arquivos:" << endl;

		// Abre todos os arquivos
		while (i < operacoes->getCaminhosArquivos().size()) {
			// Abre arquivo
			operacoes->abreArquivo(operacoes->getCaminho(i));
			cout << "\t" << operacoes->getCaminho(i) << endl;
			i++;
		}

		cout << endl;

		while (opcao != 5) {
			cout << "\nESCOLHA UMA OPERACAO" << endl;
			cout << "\t[1] Converter para escala de cinza e salvar" << endl;
			cout << "\t[2] Converter para preto e branco e salvar" << endl;
			cout << "\t[3] Salvar imagem" << endl;
			cout << "\t[4] Teste" << endl;
			cout << "\t[5] Sair" << endl;
			cout << "Opcao escolhida: ";
			cin >> opcao;

			i = 0;

			while (i < operacoes->getCaminhosArquivos().size()) {
				cout <<  i << endl;
				// Abre arquivo
				operacoes->abreArquivo(operacoes->getCaminho(i));

				if (opcao == 1) {
					operacoes->convertePara256Cores();
					operacoes->armazenaImagemProcessada(idArquivo + "EscalaDeCinza", false);
				}
				else if (opcao == 2) {
					operacoes->converteFloydSteinberg();
					operacoes->armazenaImagemProcessada(idArquivo + "P&B", true);
				}
				else if (opcao == 3) {
					operacoes->armazenaImagemProcessada(idArquivo, false);
				}
				else if (opcao == 4) {
					operacoes->converteBinaria();
					operacoes->vetorResposta();
					operacoes->armazenaImagemProcessada(idArquivo+ "Ret", false);
				}
				idArquivo += "1";
				i++;
			}
		}
	}
	else {
		cout << "Não há arquivos na pasta informada";
	}

	return 0;
}

