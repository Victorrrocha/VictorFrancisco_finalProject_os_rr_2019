#include <iostream>

using namespace std;


struct personagem{
    string Nome;
    // este personagem não tem preferência em relação a estes de baixo
    string depoisDele;
    string depoisDela;
};

personagem elenco[6] = {{"Sheldon", "Leonard", "Penny"},
                        {"Amy", "Leonard", "Penny"},
                        {"Leonard", "Howard", "Bernadette"},
                        {"Penny", "Howard", "Bernadette"},
                        {"Howard", "Sheldon", "Amy"},
                        {"Bernadette", "Sheldon", "Amy"},
                        };

// o while foi para ter maior controle na hora de interar, ta bem autoexplicativo, se trocar ele volta do inicio dos próximos de p 
// quando não troca, ele pode continuar, se vc achar um jeito de fazer isso só com for, nem me fala pq eu vou ficar com raiva de mim kkkkkkk

void sort_decrescente(personagem *fila, int tam){
	personagem aux;
	bool houveTroca = true;
	int p,e;
		p = tam - 1;
		while(p > 0){
			e = p - 1;
			while(e >= 0){

				if(fila[p].depoisDele == fila[e].Nome || fila[p].depoisDela == fila[e].Nome)
                {
                    aux = fila[p];
                    fila[p] = fila[e];
                    fila[e] = aux;
                    houveTroca = true;
                    cout << fila[p].Nome << " TROCOU COM " << fila[e].Nome << endl;
                    e = p - 1;
                }
                else{
                	cout << fila[p].Nome << " NAO TROCOU COM " << fila[e].Nome << endl;
                	e--; 
                }
			}
			p--;
		}

	cout << fila[0].Nome << endl;
    cout << fila[1].Nome << endl;
    cout << fila[2].Nome << endl;
    cout << fila[3].Nome << endl;
    cout << fila[4].Nome << endl;
    cout << fila[5].Nome << endl;

}

void sort_crescente(personagem *fila, int tam){
	personagem aux;
	bool houveTroca = true;
	int p,e;
		p = 0;
		while(p < tam - 1){
			e = 1;
			while(e < tam){

				if(fila[p].depoisDele == fila[e].Nome || fila[p].depoisDela == fila[e].Nome)
                {
                    aux = fila[p];
                    fila[p] = fila[e];
                    fila[e] = aux;
                    houveTroca = true;
                    cout << fila[p].Nome << " TROCOU COM " << fila[e].Nome << endl;
                    e = p + 1;
                }
                else{
                	cout << fila[p].Nome << " NAO TROCOU COM " << fila[e].Nome << endl;
                	e++; 
                }
			}
			p++;
		}

	cout << fila[0].Nome << endl;
    cout << fila[1].Nome << endl;
    cout << fila[2].Nome << endl;
    //cout << fila[3].Nome << endl;
    //cout << fila[4].Nome << endl;
    //cout << fila[5].Nome << endl;

}

int main()
{
    personagem fila[6];
    int tam = 6;

    fila[0] = elenco[4];
    fila[1] = elenco[0];
    fila[2] = elenco[1];
    //TESTE COM LOOP, NAO ESQUECE DE TROCAR O TAMANHO DO VETOR
    fila[3] = elenco[2];
    fila[4] = elenco[3];
    fila[5] = elenco[5];

    sort_decrescente(fila, tam);



    return 0;
}
