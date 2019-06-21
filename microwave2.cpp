#include <iostream>
#include <unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>

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
    int usos_de_microondas;
};

struct personagens{
    personagem fila[7];
    int tam;
};

personagens fila;

personagem elenco[6] = {{"Sheldon", "Leonard", "Penny", 4},
                        {"Amy", "Leonard", "Penny", 5},
                        {"Leonard", "Howard", "Bernadette", 4},
                        {"Penny", "Howard", "Bernadette", 6},
                        {"Howard", "Sheldon", "Amy", 5},
                        {"Bernadette", "Sheldon", "Amy", 4},
                        };

void add_personagem(personagem people);
                        
void sort_decrescente(personagens fila){
    cout << "------ sort fila----------" << endl;
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

void vai_comer(personagem person){
    if(person.usos_de_microondas > 0){
        if(person.usos_de_microondas%2 == 0)
        {
            sleep(3);
        }else
        {
            sleep(4);
        }
        add_personagem(person);
        person.usos_de_microondas -= 1;
    }
}


void* micro_ondas(void* arg){
    cout << "------micro ondas--------" << endl;
    string nome_aux;
    while(1){
        if (fila.tam != 0){
            //printf("Função micro ondas entrou!----------------\n");
            sem_wait(&mutex);
            cout << fila.fila[fila.tam-1].Nome << " começa a esquentar algo:" << fila.tam-1 << endl;
            sleep(1); // o tempo dele usando
            fila.tam -= 1;
            nome_aux = fila.fila[fila.tam].Nome;
            sem_post(&mutex);
            cout << nome_aux << " vai comer "<< endl;
            vai_comer(fila.fila[fila.tam-1]);
        }
    }
    pthread_exit(NULL);
}

void add_personagem(personagem people){
    cout << "--------add personagem---------" << endl;
    cout << people.Nome << " quer usar o forno" << endl;
    sem_wait(&mutex);
    fila.fila[fila.tam++] = people;
    sem_post(&peoples);
    sem_post(&mutex);
}

/// -------- funções aleatórias de espera ---------- ////



int main()
{
    int tam = 3;

    sem_init(&peoples, TRUE, 0);
    sem_init(&mutex, TRUE, 1);

    pthread_t m;

    // TESTE MANUAL
    // ---------------------------------------
    add_personagem(elenco[0]);
    add_personagem(elenco[1]);
    add_personagem(elenco[2]);

    sem_wait(&mutex);
    sort_decrescente(fila);
    sem_post(&mutex);
    // -----------------------------------------



    pthread_create(&m, NULL, micro_ondas, NULL);



    // TESTE 2, APOS O INICIO DA THREAD
    // -----------------------------------------
    sleep(4);
    
    add_personagem(elenco[4]);
    sleep(2);
    add_personagem(elenco[3]);
    sleep(2);
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