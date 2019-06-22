#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>


using namespace std;
#define TRUE 1
#define VAZIO "vazio"

sem_t mutex;
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

/// ------- DECLARAÇÕES --------------
void mostrar_fila();
void trocar_people(int p, int e);
void ir_comer(personagem people);
void remove_personagem();
//int raj_escolhe_verifica();
//void* raj (void* arg);
void sort_decrescente();
void sort_crescente();
void* micro_ondas(void* arg);
void add_personagem();
void ir_comer(personagem people);
/// -----------------------------------



/* RECEBE UM PERSONAGEM QUE ACABOU DE USAR O MICROWAVE
   E ESPERA UM TEMPO ALEATORIO ENTRE(3-6) SEGUNDOS,
   DEPOIS O PERSONAGEM FICA DISPONIVEL PARA ENTRAR 
   NA FILA NOVAMENTE
*/
void ir_comer(personagem people){

    
    // 'valor_rand' RECEBE UM VALOR ALEATORIO ENTRE(3-6)
    int valor_rand;
    srand(time(0));
    valor_rand = rand()%(6-3+1)+3;


    cout << people.Nome << " vai comer " << endl;

    // OCORRE UM PAUSA EM SEGUNDOS DO TAMANHO DO 'valor_rand'
    sleep(valor_rand);


    // LIBERANDO O PERSONAGEM PARA QUE POSSA ENTRAR NA FILA
    for(int i = 0; i < 6; i++){
        if(elenco[i].Nome == people.Nome){
            sem_wait(&mutex);
            disp[i] = 0;
            sem_post(&mutex);
            break;
        }
    }

}




/* TROCA DE POSIÇÃO DOIS PERSONAGENS DA FILA PREVIAMENTE INDICADO COMO 'p' E 'e'
   FUNÇÃO USADA NA ORDENAÇÃO
*/
void trocar_people(int p, int e){
    personagem aux;
    aux = fila.fila[p];
    fila.fila[p] = fila.fila[e];
    fila.fila[e] = aux;
    cout << fila.fila[p].Nome << " TROCOU COM " << fila.fila[e].Nome << endl;
}



// IMPRIMI QUAM ESTA NA FILA NAQUELE MOMENTO
void imprimi_fila(){
    for(int i = 0; i < fila.tam; i++)
        cout << fila.fila[i].Nome << endl;
}



// o while foi para ter maior controle na hora de interar, ta bem autoexplicativo, se trocar ele volta do inicio dos próximos de p 
// quando não troca, ele pode continuar, se vc achar um jeito de fazer isso só com for, nem me fala pq eu vou ficar com raiva de mim kkkkkkk                     
void sort_decrescente(){
    personagem aux;
    int p,e;
    p = fila.tam - 1;
    while(p > 0){
        e = p - 1;
        while(e >= 0){
            if(fila.fila[p].depoisDele == fila.fila[e].Nome || fila.fila[p].depoisDela == fila.fila[e].Nome)
            {
                trocar_people( p, e);
                e = p - 1;
            }
            else{
                cout << fila.fila[p].Nome << " NAO TROCOU COM " << fila.fila[e].Nome << endl;
                e--; 
            }
        }
    p--;
    }
    imprimi_fila();
}

void sort_crescente(){
    personagem aux;
    int p,e;
        p = 0;
        while(p < fila.tam - 1){
            e = 1;
            while(e < fila.tam){
                if(fila.fila[p].depoisDele == fila.fila[e].Nome || fila.fila[p].depoisDela == fila.fila[e].Nome)
                {
                    trocar_people( p, e);
                    e = p + 1;
                }
                else{
                    cout << fila.fila[p].Nome << " NAO TROCOU COM " << fila.fila[e].Nome << endl;
                    e++; 
                }
            }
            p++;
        }
    imprimi_fila();

}



// REMOVE O PROXIMO DA FILA PARA USAR O MICROWAVE
void* micro_ondas(void* arg){

    personagem aux;
    while(1){
        if (fila.tam != 0){
            

            // REMOVENDO O PROXIMO DA FILA
            sem_wait(&mutex);
            sem_wait(&peoples);
            cout << fila.fila[fila.tam-1].Nome << " começa a esquentar algo" << endl;
            fila.tam -= 1;
            aux = fila.fila[fila.tam];
            sleep(1);
            sem_post(&mutex);


            // INDO COMER APOS TER USANDO O MICROWAVE
            ir_comer(aux);


        }
    }

    pthread_exit(NULL);
}



void add_personagem(){


    int valor_rand;


    /* PROCURA UM PERSONAGEM DE FORMA ALEATORIA
       QUE NÃO ESTEJA NA FILA, USANDO O MICROWARE OU COMENDO
    */
    srand(time(0));
    do
    {
        valor_rand = rand()%6;
    }while(disp[valor_rand] != 0);


    // APÓS ENCONTRADO ALGUEM LIVRE ADICIONA-O NA FILA
    sem_wait(&mutex);
    disp[valor_rand] = 1;
    cout << elenco[valor_rand].Nome << " quer usar o forno" << endl;
    fila.fila[fila.tam++] = elenco[valor_rand];
    sem_post(&peoples);
    sort_decrescente();
    sem_post(&mutex);
    
}



int main()
{
    int tam = 3;

    sem_init(&peoples, TRUE, 0);
    sem_init(&mutex, TRUE, 1);
    sem_init(&mutex2, TRUE, 1);

    pthread_t m;


    add_personagem();
    add_personagem();
    add_personagem();


    pthread_create(&m, NULL, micro_ondas, NULL);

    sleep(20); // PAUSA PRA TER CERTEZA QUE TUDO FOI FEITO ANTES QUE CONTINUE
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