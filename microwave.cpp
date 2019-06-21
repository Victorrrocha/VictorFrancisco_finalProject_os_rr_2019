#include <iostream>
#include<pthread.h>
#include<semaphore.h>

using namespace std;
#define TAM 7
#define TRUE 1

sem_t mutex;
sem_t micro;
sem_t peoples;




struct personagem{
    string Nome;
    // este personagem não tem preferência em relação a estes de baixo
    string depoisDele;
    string depoisDela;
};

personagem fila[7];

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

void* micro_ondas(void* arg){
    while(TRUE){
        sem_wait(&peoples);
        sem_wait(&mutex);
        printf("Função micro ondas entrou!----------------\n");
        for(int i = 0; i < TAM; i++){
            printf("%s Usando micro ondas!\n", fila[0].Nome);
            if(fila[i].Nome[0] == '\0'){
                fila[i-1].Nome[0] = '\0';
                break;
            }
        }
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}
    


void iniciar_fila(){
    for(int i = 0; i < TAM; i++){
        fila[i].Nome[0] = '\0';
        fila[i].depoisDele[0] = '\0';
        fila[i].depoisDela[0] = '\0';
    }
}

int main()
{
    int tam = 3;

    sem_init(&peoples, TRUE, 6);
    sem_init(&mutex, TRUE, 1);

    pthread_t m;

    iniciar_fila();
    sem_wait(&mutex);
    fila[0] = elenco[4];
    fila[1] = elenco[0];
    fila[2] = elenco[1];
    sem_post(&peoples);
    sem_post(&peoples);
    sem_post(&peoples);
    //TESTE COM LOOP, NAO ESQUECE DE TROCAR O TAMANHO DO VETOR
    //fila[3] = elenco[2];
    //fila[4] = elenco[3];
    //fila[5] = elenco[5];

    sort_crescente(fila, tam);
    sem_post(&mutex);

    pthread_create(&m, NULL, micro_ondas, NULL);



    return 0;
}
