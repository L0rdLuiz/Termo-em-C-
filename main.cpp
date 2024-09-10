#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <unordered_set>  // cores

using namespace std;

enum cor { preto, amarelo, verde };

struct Nos {
    int id;
    char info;
    cor condicao = preto;
    Nos *prox, *ant;
};

struct No {
    int id;
    string info;
    No *prox, *ant;
};

struct LDE {
    No* comeco;
    No* fim;
};
struct LDEs {
    Nos* comeco;
    Nos* fim;
};

void limpaLDEs(LDEs &lGabarito) {
    lGabarito.comeco = NULL;
    lGabarito.fim = NULL;
}

void limpaLDE(LDE &lista) {
    lista.comeco = NULL;
    lista.fim = NULL;
}

bool inserirFinalLDE(LDEs &lGabarito, char textoGab, int i) {  // lista Gabarito
    Nos* novo = new Nos();
    if (novo == NULL) return false;
    novo->id = i;
    novo->info = textoGab;
    novo->condicao = preto;
    novo->ant = NULL;
    novo->prox = NULL;

    if (lGabarito.comeco == NULL) {   // lista vazia
        lGabarito.comeco = novo;
        lGabarito.fim = novo;
    } else {
        lGabarito.fim->prox = novo;
        novo->ant = lGabarito.fim;
        lGabarito.fim = novo;
    }
    return true;
}

bool inserirFinalLDE(LDE &lista, string textoLista, int i) { // Lista txt
    No* novo = new No();
    if (novo == NULL) return false;
    novo->id = i;
    novo->info = textoLista;
    novo->ant = NULL;
    novo->prox = NULL;

    if (lista.comeco == NULL) {   // lista vazia
        lista.comeco = novo;
        lista.fim = novo;
    } else {
        lista.fim->prox = novo;
        novo->ant = lista.fim;
        lista.fim = novo;
    }
    return true;
}

void mostrarListaLDE(LDE &lista) {    // mostra a lista pro usuario
    No *aux = lista.comeco;
    while (aux != NULL) {
        cout << aux->info << " ";
        cout << aux->id << " ";
        aux = aux->prox;
        cout << endl;
    }
}

int contaListaLDE(LDE lista) {
    int cont = 0;
    No *aux = lista.comeco;
    while (aux != NULL) {
        cont++;
        aux = aux->prox;
    }
    return cont;
}

bool carregaPalavrasTXTnoLDE(LDE &lista, string &listaDePalavras) {
    ifstream listaPalavras("palavras.txt");
    if (!listaPalavras.is_open()) {
        cout << "Erro ao abrir o arquivo para leitura!" << endl;
        return false;
    }
    string palavra;
    int i = 1;
    while (getline(listaPalavras, palavra)) {
        inserirFinalLDE(lista, palavra, i);
        i++;
    }
    listaPalavras.close();
    return true;
}

void passaListaGabaritoLDE(LDE &lista, string &listaDePalavras) {
    ofstream listaPalavras("palavras.txt", ios::trunc);
    if (!listaPalavras.is_open()) {
        cout << "Erro ao abrir o arquivo para escrita!" << endl;
        return;
    }
    No *aux = lista.comeco;
    while (aux != NULL) {
        listaPalavras << aux->info << endl;
        aux = aux->prox;
    }
    listaPalavras.close();
    cout << "Dados escritos com sucesso" << endl;
}

No *pesquisarLDE(LDE lista, int linha) {
    No *aux = lista.comeco;
    while (aux != NULL) {
        if (linha == aux->id) return aux;
        aux = aux->prox;
    }
    return NULL;
}

void pesquisarGabarito(LDEs &lGabarito, char letra, int letraNum, bool &acertou) {
    Nos *aux = lGabarito.comeco;
    int win = 0;
    while (aux != NULL) {
        if (letra == aux->info) {
            if (aux->id == letraNum) {    // mesma casa da letra
                aux->condicao = verde;
            } else {
                aux->condicao = amarelo;     // casa diferente
            }
        }
        if (aux->condicao == verde) {
            win++;
            if (win == 5) acertou = true;
        }
        aux = aux->prox;
    }
}

void criaListaGab(LDEs &lGabarito, No *temp) {
    string palavra = temp->info;
    for (int i = 0; i < 5; i++) {   // Separa a palavra letra por letra e depois insere ela no arquivo
        char letra = palavra[i];
        inserirFinalLDE(lGabarito, letra, i);
    }
}

bool removerPalavraLDE(LDE &lista, int linha) {
    No *aux = pesquisarLDE(lista, linha);
    if (aux == NULL) return false;

    if (aux == lista.comeco) {
        if (aux == lista.fim) {
            lista.comeco = NULL;
            lista.fim = NULL;
        } else {
            lista.comeco = aux->prox;
            lista.comeco->ant = NULL;
        }
    } else if (aux == lista.fim) {
        lista.fim = aux->ant;
        lista.fim->prox = NULL;
    } else {
        No *ant = aux->ant;
        No *prox = aux->prox;
        ant->prox = prox;
        prox->ant = ant;
    }
    delete aux;
    return true;
}

// Função para construir a visualização da palavra com as letras acertadas
void construirVisualizacao(LDEs &lGabarito, bool &acertou) {
    Nos *aux = lGabarito.comeco;
    while (aux != NULL) {
        if (aux->condicao == verde) {
            cout << "\033[1;32m" << aux->info << "\033[0m";  // Verde
        } else if (aux->condicao == amarelo) {
            cout << "voce acertou a amarela letra, mas no lugar errado" << endl;
            cout << "\033[1;33m" << aux->info << "\033[0m" << endl; // Amarelo para letras erradas
        } else {
            cout << '_';
        }
        aux = aux->prox;
    }
    cout << endl;
}

void verificaJogo(LDEs lGabarito, string tentativaDePalavra, bool &acertou) {
    for (int i = 0; i < 5; i++) {
        char letra = tentativaDePalavra[i];
        pesquisarGabarito(lGabarito, letra, i, acertou);
    }
}

int contarLetrasLDE(LDE lista) {
    int nLinhas = 0;
    No *aux = lista.comeco;
    while (aux != NULL) {
        nLinhas++;
        aux = aux->prox;
    }
    return nLinhas;
}

void jogar(LDE lista, LDEs &lGabarito, int subOpcao) {
    int id = rand() % contarLetrasLDE(lista) + 1;    // Pega id aleatorio da lista
    int tentativas = 0;
    bool perdeu = false;
    char letrasAcertadas;
    bool acertou = false;
    string tentativaDePalavra;
    No *temp = pesquisarLDE(lista, id);
    string palavra = temp->info;                   // Recebe palavra de acordo com id
    criaListaGab(lGabarito, temp);                  // Cria LDE gabarito com 5 posições com um CHAR em cada

    while (!acertou) {
        // Mostra a palavra com as letras acertadas
        cout << "Adivinhe a palavra: ";
        construirVisualizacao(lGabarito, acertou);
        cout << "Insira uma letra ou tente adivinhar a palavra completa: ";
        cin >> tentativaDePalavra;
        while (tentativaDePalavra.length() != 5) {    // Verifica se a palavra contém cinco letras.
            system("cls");
            cout << "Erro. A palavra deve conter cinco letras!" << endl;
            construirVisualizacao(lGabarito, acertou);
            cin >> tentativaDePalavra;
        }
        verificaJogo(lGabarito, tentativaDePalavra, acertou);
        tentativas++;
        switch(subOpcao) {
        case 1:
            if (tentativas == 6) {
                perdeu = true;
            }
            break;
        case 2:
            if (tentativas == 8) {
                perdeu = true;
            }
            break;
        case 3:
            if (tentativas == 10) {
                perdeu = true;
            }
            break;
        case 4:
            if (tentativas == 12) {
                perdeu = true;
            }
            break;
        }

        if (acertou == true) {
            system("cls");
            cout << "Parabéns, você venceu!" << endl;
            system("pause");
            limpaLDEs(lGabarito);  // Limpa a lista após acerto total
            return;
        } else if (acertou == false && perdeu == true){
            system("cls");
            cout << "Você perdeu, boa sorte na próxima!" << endl;
            system("pause");
            limpaLDEs(lGabarito);  // Limpa a lista após perder total
            return;
        }
    }
}

void mostrarMenu() {
    string palavra;
    LDE lista;
    LDEs lGabarito;
    limpaLDE(lista);
    limpaLDEs(lGabarito);
    string listaDePalavras = "palavras.txt";
    if (carregaPalavrasTXTnoLDE(lista, listaDePalavras)) {
        cout << "Palavras carregadas com sucesso" << endl;
    }
    int opcao;
    do {
        system("cls");
        std::cout << "##     ## ######## ##    ## ##     ##\n";
        std::cout << "###   ### ##       ###   ## ##     ##\n";
        std::cout << "#### #### ##       ####  ## ##     ##\n";
        std::cout << "## ### ## ######   ## ## ## ##     ##\n";
        std::cout << "##     ## ##       ##  #### ##     ##\n";
        std::cout << "##     ## ##       ##   ### ##     ##\n";
        std::cout << "##     ## ######## ##    ##  #######\n";
        cout << "1- JOGAR" << endl;
        cout << "2- REGRAS" << endl;
        cout << "3- LISTA DE PALAVRAS" << endl;
        cout << "4- SAIR" << endl;
        cin >> opcao;
        system("cls");
        switch (opcao) {
            case 1: {
                int subOpcao;
                cout << "\n===== Jogar =====" << endl;
                cout << "1. Palavra (6 tentativas)" << endl;
                cout << "2. Palavra (8 tentativas)" << endl;
                cout << "3. Palavra (10 tentativas)" << endl;
                cout << "4. Palavra (12 tentativas)" << endl;
                cout << "0 - retornar para o menu" << endl;
                cin >> subOpcao;
                switch (subOpcao) {
                    case 1:
                        jogar(lista, lGabarito, subOpcao);
                        break;
                    case 2:
                        // Implementar lógica para diferentes números de tentativas
                        break;
                    case 3:
                        // Implementar lógica para diferentes números de tentativas
                        break;
                    case 4:
                        // Implementar lógica para diferentes números de tentativas
                        break;
                    case 0:
                        mostrarMenu();
                        break;
                }
                break;
            }
            case 2:
                cout << "\n===== Regras =====" << endl;
                // Coloque o código para exibir as regras do jogo
                cout << "REGRAS" << endl;
                cout << "O jogo consiste em o usuario tentar advinhar uma ou maia palavras de 5 letras" << endl;
                cout << "porem, com um numero de tentativas em cada modo" << endl;
                cout << "1 palavra  (6 tentativas)" << endl;
                cout << "2 Palavras (8 tentativas)" << endl;
                cout << "3 Palavras (10 tentativas)" << endl;
                cout << "4 Palavras (12 tentativas)" << endl;
                system("pause");
                break;
            case 3:
                cout << "\n===== Lista =====" << endl;
                int opcaoLista;
                mostrarListaLDE(lista);
                cout << "Deseja remover ou inserir uma nova linha" << endl;
                cout << "[1] para inserir [2] para remover [3] para sair" << endl;
                cin >> opcaoLista;
                system("cls");
                switch (opcaoLista) {
                    case 1:
                        cout << "Digite a palavra que deseja inserir" << endl;
                        cin >> palavra;
                        while (palavra.length() != 5) {    // Verifica se a palavra contém cinco letras.
                            system("cls");
                            cout << "Erro. A palavra deve conter cinco letras!" << endl;
                            cin >> palavra;
                        }
                        inserirFinalLDE(lista, palavra, lista.fim->id++);
                        break;
                    case 2:
                        int linhaPalavra;
                        cout << "Qual linha você deseja remover?" << endl;
                        cin >> linhaPalavra;
                        while (linhaPalavra < 0 || linhaPalavra > contarLetrasLDE(lista)) {
                            cout << "Linha inválida! Digite novamente." << endl;
                            cin >> linhaPalavra;
                        }
                        removerPalavraLDE(lista, linhaPalavra);
                        break;
                    default:
                        cout << "Opção inválida!" << endl;
                }
                system("pause");
                break;
            case 4:
                cout << "Saindo do jogo..." << endl;
                break;
            default:
                cout << "Opção inválida! Tente novamente." << endl;
                system("pause");
        }
    } while (opcao != 4);
    passaListaGabaritoLDE(lista, listaDePalavras);
}

int main() {
    srand(time(NULL));
    system("cls");
    mostrarMenu();
    return 0;
}
