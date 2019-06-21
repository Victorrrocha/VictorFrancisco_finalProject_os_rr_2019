#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>


using namespace std;
#define TRUE 1
#define VAZIO "vazio"

sem_t mutex;
sem_t micro;
sem_t peoples;
sem_t mutex2;




struct personagem{
    string Nome;
    // este personagem não tem preferência em relação a estes de baixo
    string depoisDele;
    string depoisDela;
};

struct personagens{
    personagem fila[6];
    int tam;
};

personagens fila;
int disp[6];

personagem elenco[6] = {{"Sheldon", "Leonard", "Penny"},
                        {"Amy", "Leonard", "Penny"},
                        {"Leonard", "Howard", "Bernadette"},
                        {"Penny", "Howard", "Bernadette"},
                        {"Howard", "Sheldon", "Amy"},
                        {"Bernadette", "Sheldon", "Amy"},
                        };

void ir_comer(personagem people){
    srand(time(0));
    int valor_rand;
    valor_rand = rand()%(6-3+1)+3;
    cout << people.Nome << " vai comer " << endl;
    sleep(valor_rand);
    for(int i = 0; i < 6; i++){
        if(elenco[i].Nome == people.Nome){
            sem_wait(&mutex2);
            disp[i] = 0;
            sem_post(&mutex2);
            break;
        }
    }

}

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
    personagem aux;
    while(1){
        if (fila.tam != 0){
            sem_wait(&mutex);
            cout << fila.fila[fila.tam-1].Nome << " começa a esquentar algo" << endl;
            fila.tam -= 1;
            aux = fila.fila[fila.tam];
            sleep(1);
            sem_wait(&peoples);
            ir_comer(aux);
            sem_post(&mutex);
            // indo comer
            
        }
    }
    pthread_exit(NULL);
}

void add_personagem(){
    srand(time(0));
    int valor_rand;
    while(1){
        valor_rand = rand()%6;
        if(disp[valor_rand] == 0){

            
            sem_wait(&mutex);
            cout << elenco[valor_rand].Nome << " quer usar o forno" << endl;
            sem_wait(&mutex2);
            disp[valor_rand] = 1;
            sem_post(&mutex2);
            fila.fila[fila.tam++] = elenco[valor_rand];
            sem_post(&peoples);
            sem_post(&mutex);
            break;
        }
    }


}



int main()
{
    int tam = 3;

    sem_init(&peoples, TRUE, 0);
    sem_init(&mutex, TRUE, 1);
    sem_init(&mutex2, TRUE, 1);

    pthread_t m;

    pthread_create(&m, NULL, micro_ondas, NULL);

    /*// TESTE MANUAL
    // ---------------------------------------
    
    add_personagem();
    add_personagem();
    add_personagem();
    
    sem_wait(&mutex);
    sort_decrescente(fila);
    sem_post(&mutex);
    // -----------------------------------------
    
    // TESTE 2, APOS O INICIO DA THREAD
    // -----------------------------------------
    //sleep(4);
    add_personagem();
    add_personagem();
    add_personagem();
    sem_wait(&mutex);
    sort_decrescente(fila);
    sem_post(&mutex);
    // -----------------------------------------*/

    while(1){
        add_personagem();
    }


    sleep(50);
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