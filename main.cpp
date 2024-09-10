#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

enum cor{preto,amarelo,verde};

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
void limpaLDEs (LDEs &lGabarito){
  lGabarito.comeco = NULL;
  lGabarito.fim    = NULL;
}

void limpaLDE (LDE &lista){
  lista.comeco = NULL;
  lista.fim    = NULL;
}

bool inserirFinalLDE (LDEs &lGabarito, char textoGab, int i){  //lista Gabarito
  Nos* novo = new Nos();

  if(novo == NULL) return false;
  novo->id      = i;
  novo->info    = textoGab;
  novo->condicao= preto;
  novo->ant     = NULL;
  novo->prox    = NULL;

  if(lGabarito.comeco == NULL ){   //lista vazia
      lGabarito.comeco = novo;
      lGabarito.fim    = novo;
  } else {
      lGabarito.fim->prox = novo;
      novo->ant = lGabarito.fim;
      lGabarito.fim = novo;
    }
    return true;
 }

bool inserirFinalLDE (LDE &lista, string textoLista, int i){ //Lista txt
  No* novo = new No();

  if(novo == NULL) return false;
  novo->id    = i;
  novo->info  = textoLista;
  novo->ant   = NULL;
  novo->prox  = NULL;

  if(lista.comeco == NULL ){   //lista vazia
      lista.comeco = novo;
      lista.fim    = novo;
  } else {
      lista.fim->prox = novo;
      novo->ant = lista.fim;
      lista.fim = novo;
    }
    return true;
 }

void mostrarListaLDE (LDE &lista){    ///mostra a lista pro usuario
    No *aux = lista.comeco;
    while(aux!= NULL){
      cout<<aux->info <<" ";
      cout<<aux->id   <<" ";
      aux = aux->prox;
      cout<<endl;
    }
}

int contaListaLDE (LDE lista){
    int cont = 0;
    No *aux = lista.comeco;
    while (aux!=NULL){
      cont++;
      aux = aux->prox;
    }
    return cont;
}

bool carregaPalavrasTXTnoLDE(LDE &lista, string &listaDePalavras){
  ifstream listaPalavras("palavras.txt");
  if(!listaPalavras.is_open()){
    cout<<"erro ao abrir o arquivo para escrita!"<<endl;
    return false;
  }
  string palavra;
  int i = 1;
  while (getline(listaPalavras, palavra)){
    inserirFinalLDE(lista, palavra, i);
    i++;
  }
  listaPalavras.close();
  return true;
}

void passaListaGabaritoLDE(LDE &lista, string &listaDePalavras){
  ofstream listaPalavras ("palavras.txt",ios::trunc);
    if(!listaPalavras.is_open()){
    cout<<"erro ao abrir o arquivo para escrita!"<<endl;
    return;
  }
  No *aux = lista.comeco;
    while (aux != NULL){
      listaPalavras << aux-> info << endl;
      aux = aux->prox;
  }
  listaPalavras.close();
  cout<<"dados escritos com sucesso"<<endl;
}

No *pesquisarLDE(LDE lista, int linha){
  No *aux = lista.comeco;
  while(aux!=NULL){
    if(linha == aux->id) return aux;
    aux = aux->prox;
  }
  return NULL;
}

void pesquisarGabarito (LDEs &lGabarito, char letra, int letraNum, bool &acertou){
  Nos *aux = lGabarito.comeco;
  int win = 0;
  while(aux!=NULL){
    if(letra == aux->info){
      if(aux->id==letraNum){    //mesma casa da letra
        aux->condicao=verde;
      } else {
        aux->condicao=amarelo;     //casa diferente
      }
    }
    if(aux->condicao==verde){
      win++;
      if(win==5) acertou = true;
    }
    aux = aux->prox;
  }
}

void criaListaGab (LDEs &lGabarito,No *temp){
  string palavra = temp->info;
    for(int i = 0; i < 5;i++){   ///Separa a palavra letra por letra e depois inseri ela no arquivo
      char letra = palavra[i];
      inserirFinalLDE(lGabarito, letra, i);
  }
}

bool removerPalavraLDE (LDE &lista, int linha){
  No *aux = pesquisarLDE(lista, linha);
  if(aux==NULL)return false;

    if(aux==lista.comeco){
      if(aux==lista.fim){
        lista.comeco = NULL;
        lista.fim    = NULL;
      } else {
        lista.comeco = aux->prox;
        lista.comeco->ant = NULL;
      }
    }
    else if(aux==lista.fim){
      lista.fim =  aux->ant;
      lista.fim->ant = NULL;
    } else {
      No *ant = aux->ant;
      No *prox = aux->prox;
      ant->prox = prox;
      prox->ant =  ant;
    }
    delete aux;
    return true;
}

// Função para construir a visualização da palavra com as letras acertadas
char construirVisualizacao(LDEs &lGabarito, bool &acertou) {
  Nos *aux = lGabarito.comeco;
  while(aux!=NULL){
    if(aux->condicao==verde){
      cout<<(aux->info);
    } else if(aux->condicao==amarelo){  //depois tem que implementar a logica da cor aqui nessas funções
      cout<<(aux->info);
    }
     else {
     cout<<('_');
    }
    aux=aux->prox;
  }
}

void verificaJogo(LDEs lGabarito, string tentativaDePalavra, bool &acertou){
    for(int i = 0;i <5;i++){
    char letra = tentativaDePalavra[i];
    pesquisarGabarito(lGabarito, letra, i,acertou);
  }
}

int contarLetrasLDE(LDE lista){
  int nLinhas = 0;
  No *aux = lista.comeco;
  while( aux != NULL){
    nLinhas++;
    aux = aux->prox;
  }
  return nLinhas;
}


void jogar(LDE lista, LDEs &lGabarito) {
    int id = rand()% contarLetrasLDE(lista) + 1;    // Pega id aleatorio da lista
    char letrasAcertadas;
    bool acertou = false;
    string tentativaDePalavra;
    No *temp = pesquisarLDE(lista, id);
    string palavra = temp->info;                   //Recebe palavra de acordo com id
    criaListaGab(lGabarito,temp);                  //Cria LDE gabarito com 5 posições com um CHAR em cada

        while (acertou!=true) {
            // Mostra a palavra com as letras acertadas
            cout << "Adivinhe a palavra: " << construirVisualizacao(lGabarito, acertou) << endl;
            cout << "Insira uma letra ou tente adivinhar a palavra completa: ";
            cin >> tentativaDePalavra;
            while(tentativaDePalavra.length()!=5){    ///Verifica se a palavra contem cinco letras.
              system("cls");
              cout<<"erro. a palavra deve conter cinco letras!"<<endl;
              cin>>tentativaDePalavra;
            }
            verificaJogo(lGabarito, tentativaDePalavra, acertou);

          }

        if(acertou==true){
          system("cls");
          cout<<"Parabens voce venceu!"<<endl;
        } else if(acertou==false){
          cout<<"Suas tentativas expiraram. Derrota!"<<endl;
        }
}

void mostrarMenu() {
    string palavra;
    LDE lista;
    LDEs lGabarito;
    limpaLDE(lista);
    limpaLDEs(lGabarito);
    string listaDePalavras = "palavras.txt";
    if(carregaPalavrasTXTnoLDE(lista, listaDePalavras)){
      cout<<"palavras carregadas com sucesso"<<endl;
    }
    int opcao;
    do {
    cout << "          ___  __  __   ___   __           " << endl;
    cout << "            | |   | |     | |  | \\ | | | | | \\ / |  | \\       " << endl;
    cout << "            | |   | |---- | |__| | | | | | | | | |  | |          " << endl;
    cout << "            ||   ||__ ||  \\\\ || || || \\|_|/       " << endl;
    cout << "                                                                 " << endl;
    cout << "1.             _   __   __   __   __             " << endl;
    cout << "              | | / |  | \\ | | __ | |  | | | |  | \\          " << endl;
    cout << "          _   | | | |  | | | |  | | | |_| | | |_| |            " << endl;
    cout << "         |||| \\||/ |||| ||  || ||  \\\\         " << endl;
    cout << "                                                                 " << endl;
    cout << "2.      __   __  __   __   __   __       " << endl;
    cout << "       | |  | \\ | |     | | __ | |  | \\ | |  | | / |         " << endl;
    cout << "       | |_| | | |---- | |  | | | || | | |_| | '------.      " << endl;
    cout << "       ||  \\\\ ||_ |||| ||  \\\\ ||  ||  _|/  " << endl;
    cout << "                                                                 " << endl;
    cout << "3.          _       __  ___  ___  __               " << endl;
    cout << "           | |       | |  / |        | |   | |  | |              " << endl;
    cout << "           | |   _   | |  '------.   | |   | |__| |              " << endl;
    cout << "           |||| ||_  _|/   ||   ||  |_|              " << endl;
    cout << "                                                                 " << endl;
    cout << "4.            __   __  __  ___                     " << endl;
    cout << "             / |      | |  | |  | |  | |  | \\                   " << endl;
    cout << "             '------. | |_| |  | |  | |_| |                    " << endl;
    cout << "              _|/ ||  || ||_ ||  \\\\                  " << endl;
    cin>>opcao;
      system("cls");
        switch (opcao) {
            case 1: {
                int subOpcao;
                cout << "\n===== Jogar =====" << endl;
                cout << "1. Palavra (6  tentativa)" << endl;
                cout << "2. Palavra (8  tentativas)" << endl;
                cout << "3. Palavra (10 tentativas)" << endl;
                cout << "4. Palavra (12 tentativas)" << endl;
                cout << "Escolha uma opção: ";
                cin >> subOpcao;
                switch (subOpcao){

                case 1:
                system("cls");
                jogar(lista, lGabarito);
                break;

                case 2:
                  ///jogo
                break;

                case 3:
                  ///jogo
                break;

                case 4:
                  ///jogo
                break;
                }
                break;
            }
            case 2:
                cout << "\n===== Regras =====" << endl;
                // Coloque o código para exibir as regras do jogo
                cout << "As regras serão exibidas aqui." << endl;
                break;
            case 3:
                cout << "\n===== Lista =====" << endl;
                int opcaoLista;
                mostrarListaLDE(lista);
                cout << "Deseja remover ou inserir uma nova linha" << endl;
                cout << "[1] para inserir [2] para remover [3] para sair"<<endl;
                cin>>opcaoLista;
                system("cls");
                switch(opcaoLista){

                  case 1:
                  cout<<"digite a palavra que deseja inserir"<<endl;
                  cin>>palavra;
                    while(palavra.length()!=5){    ///Verifica se a palavra contem cinco letras.
                     system("cls");
                     cout<<"erro. a palavra deve conter cinco letras!"<<endl;
                     cin>>palavra;
                  }
                  inserirFinalLDE(lista,palavra,lista.fim->id++);
                  break;

                  case 2:
                    int linhaPalavra;
                    cout<<"qual linha voce deseja remover?"<<endl;
                    cin>>linhaPalavra;
                    while(linhaPalavra<0||linhaPalavra>contarLetrasLDE(lista)){
                      cout<<"linha invalida! digite novamente."<<endl;
                      cin>>linhaPalavra;
                    }
                    removerPalavraLDE(lista,linhaPalavra);
                    break;
                    default:
                    cout<<"opção invalida!"<<endl;
                }
                break;
            case 4:
                cout << "Saindo do jogo..." << endl;
                break;
            default:
                cout << "Opção inválida! Tente novamente." << endl;
        }
        cout << endl;
    } while (opcao < 0 && opcao > 4);
    passaListaGabaritoLDE(lista, listaDePalavras);
}

//int contarLinhasTXT (string &listaDePalavras){
//  ifstream listaPalavras("palavras.txt");
//  if(!listaPalavras.is_open()){
//    cout<<"erro ao abrir o arquivo para escrita!"<<endl;
//    return 0;
//  }
//  string linha;
//  int numeroDeLinhas =1;
//  while(getline(listaPalavras, linha)){
//    numeroDeLinhas++;
//  }
//  listaPalavras.close();
//  return numeroDeLinhas;
//}


int main(){
  srand (time(NULL));
  system("cls");
  mostrarMenu();
  return 0;
}
