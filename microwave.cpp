#include <iostream>
#include <unistd.h>
#include<pthread.h>
#include<semaphore.h>


using namespace std;
#define TAM 7
#define TRUE 1
#define VAZIO "vazio"

sem_t mutex;
sem_t micro;
sem_t peoples;




struct personagem{
    string Nome;
    // este personagem não tem preferência em relação a estes de baixo
    string depoisDele;
    string depoisDela;
};

struct personagens{
    personagem fila[7];
    int tam;
};

personagens fila;

personagem elenco[6] = {{"Sheldon", "Leonard", "Penny"},
                        {"Amy", "Leonard", "Penny"},
                        {"Leonard", "Howard", "Bernadette"},
                        {"Penny", "Howard", "Bernadette"},
                        {"Howard", "Sheldon", "Amy"},
                        {"Bernadette", "Sheldon", "Amy"},
                        };

// o while foi para ter maior controle na hora de interar, ta bem autoexplicativo, se trocar ele volta do inicio dos próximos de p 
// quando não troca, ele pode continuar, se vc achar um jeito de fazer isso só com for, nem me fala pq eu vou ficar com raiva de mim kkkkkkk
                        
void sort_decrescente(personagens fila){
	personagem aux;
	bool houveTroca = true;
	int p,e;
		p = fila.tam - 1;
		while(p > 0){
			e = p - 1;
			while(e >= 0){

				if(fila.fila[p].depoisDele == fila.fila[e].Nome || fila.fila[p].depoisDela == fila.fila[e].Nome)
                {
                    aux = fila.fila[p];
                    fila.fila[p] = fila.fila[e];
                    fila.fila[e] = aux;
                    houveTroca = true;
                    cout << fila.fila[p].Nome << " TROCOU COM " << fila.fila[e].Nome << endl;
                    e = p - 1;
                }
                else{
                	cout << fila.fila[p].Nome << " NAO TROCOU COM " << fila.fila[e].Nome << endl;
                	e--; 
                }
			}
			p--;
		}

    for(int i = 0; i < fila.tam; i++){
	    cout << fila.fila[i].Nome << endl;
    }
}

void sort_crescente(personagens fila){
	personagem aux;
	bool houveTroca = true;
	int p,e;
		p = 0;
		while(p < fila.tam - 1){
			e = 1;
			while(e < fila.tam){

				if(fila.fila[p].depoisDele == fila.fila[e].Nome || fila.fila[p].depoisDela == fila.fila[e].Nome)
                {
                    aux = fila.fila[p];
                    fila.fila[p] = fila.fila[e];
                    fila.fila[e] = aux;
                    houveTroca = true;
                    cout << fila.fila[p].Nome << " TROCOU COM " << fila.fila[e].Nome << endl;
                    e = p + 1;
                }
                else{
                	cout << fila.fila[p].Nome << " NAO TROCOU COM " << fila.fila[e].Nome << endl;
                	e++; 
                }
			}
			p++;
		}
    for(int i = 0; i < fila.tam; i++)
	    cout << fila.fila[i].Nome << endl;

}



void* micro_ondas(void* arg){
    string nome_aux;
    while(1){
        if (fila.tam != 0){
            //printf("Função micro ondas entrou!----------------\n");
            sem_wait(&mutex);
            cout << fila.fila[fila.tam-1].Nome << " começa a esquentar algo:" << fila.tam-1 << endl;
            fila.tam -= 1;
            nome_aux = fila.fila[fila.tam].Nome;
            sem_post(&mutex);
            cout << nome_aux << " vai comer "<< endl;
        }
    }
    pthread_exit(NULL);
}

void add_personagem(personagem people){
    cout << people.Nome << " quer usar o forno" << endl;
    sem_wait(&mutex);
    fila.fila[fila.tam++] = people;
    sem_post(&peoples);
    sem_post(&mutex);
}

int main()
{
    int tam = 3;

    sem_init(&peoples, TRUE, 0);
    sem_init(&mutex, TRUE, 1);

    pthread_t m;

    // TESTE MANUAL
    // ---------------------------------------
    add_personagem(elenco[4]);
    add_personagem(elenco[0]);
    add_personagem(elenco[1]);

    sem_wait(&mutex);
    sort_decrescente(fila);
    sem_post(&mutex);
    // -----------------------------------------



    pthread_create(&m, NULL, micro_ondas, NULL);



    // TESTE 2, APOS O INICIO DA THREAD
    // -----------------------------------------
    sleep(4);
    
    add_personagem(elenco[2]);
    add_personagem(elenco[3]);
    add_personagem(elenco[5]);

    sem_wait(&mutex);
    sort_decrescente(fila);
    sem_post(&mutex);
    // -----------------------------------------


    sleep(5);
    if (fila.tam == 0)
        printf("Fim de teste: A fila esta vazia!\n");
    else{
        sem_wait(&mutex);
        cout << "Fim de test: "<< fila.fila[fila.tam].Nome << " é o proximo da fila.\n "<< endl;
        sem_post(&mutex);
    }

    while(1){};


    return 0;
}
