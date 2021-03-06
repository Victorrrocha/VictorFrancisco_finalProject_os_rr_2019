/*
    CÓDIGO BASEADO NO PROBLEMA "The Big Bang Theory",
    UTILIZANDO THREADS E SEMAFOROS, CÓDIGO APLICADO 
    ALGORITMO DO BARBEIRO.

    BY: Francisco Nascimento and Victor Rocha.
*/





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
sem_t mutex_raj;



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

// SERVE PARA VER DISPONIBILISADE DO PERSONAGEM E O TEMPO QUE ELE VAI COMER
struct disp{
    int disp;
    time_t segundos;
    int tempo;
};



/*  DECLARANDO AS DIPONIBILIDADES DOS PERSONAGEM 
    E A CRIAÇÃO DA FILA.
*/
disp dispp[6];
personagens fila;
int raj_ver;


/*
	VETOR DOS 6 (SEIS) PERSONAGEM E JUNTO COM OS PERSONAGEM QUE TEM 
	MAIS PRIORIDADE QUE ELE MESMO, ESSES PERSONAGENS SÃO OS QUE SÃO 
	APRESENTADOS NO PROBLEMA "The Big Bang Theory".
*/
personagem elenco[6] = {{"Sheldon", "Leonard", "Penny"},
                        {"Amy", "Leonard", "Penny"},
                        {"Leonard", "Howard", "Bernadette"},
                        {"Penny", "Howard", "Bernadette"},
                        {"Howard", "Sheldon", "Amy"},
                        {"Bernadette", "Sheldon", "Amy"},
                        };



/// ------- DECLARAÇÕES --------------
void imprimi_fila();
void trocar_people(int p, int e);
void ir_comer(personagem people);
personagem remove_personagem();
int raj_identifica();
void* raj (void* arg);
void* ver_disp(void* arg);
void* micro_ondas(void* arg);
int sort_decrescente();
void add_personagem();
/// -----------------------------------



/*
	TEM O OBJETIVO DE INFORMAR AO VETOR DE DISPONIBILIDADE "dispp"
	OS DADOS DO PERSONAGEM COMO DIZER QUE FOI COMER, HORARIO ATUAL
	E O TEMPO EM (3-6) SEGUNDOS QUE PERMANECERÁ COMENDO. 
*/
void ir_comer(personagem people){

    // 'valor_rand' RECEBE UM VALOR ALEATORIO ENTRE(3-6)
    int valor_rand;
    srand(time(0));
    valor_rand = rand()%(7-3+1)+3;

    time_t agora;


    // LIBERANDO O PERSONAGEM PARA QUE POSSA ENTRAR NA FILA
    for(int i = 0; i < 6; i++){
        if(elenco[i].Nome == people.Nome){
            dispp[i].segundos = time(&agora);
            dispp[i].tempo = valor_rand;
            dispp[i].disp = 2;

            break;
        }
    }
    
}


/*  THREAD TEM O OBJETIVO DE OBSERVAR CONSTANTIMENTE
    SE OS PERSONAGENS QUE FORAM COMER PASSOU O TEMPO 
    DETERMINADO DE (3-6)SEGUNDOS, PARA POSTERIORMENTE
    LIBERA-LO PARA PODER ENTRAR NA FILA NOVAMENTE
*/
void* ver_disp(void* arg){
    time_t agora;
    while(true){
        for(int i = 0; i < 6; i++){
            if(dispp[i].disp == 2 && time(&agora) >= (dispp[i].segundos + dispp[i].tempo)){
                dispp[i].disp = 0; 
                sem_wait(&mutex);
                cout << elenco[i].Nome << " vai comer " << endl;
                sem_post(&mutex);
            }
        }
    }
    pthread_exit(NULL);
}




/* TROCA DE POSIÇÃO DOIS PERSONAGENS DA FILA PREVIAMENTE INDICADO COMO 'p' E 'e'
   FUNÇÃO USADA NA ORDENAÇÃO
*/
void trocar_people(int p, int e){
    personagem aux;
    aux = fila.fila[p];
    fila.fila[p] = fila.fila[e];
    fila.fila[e] = aux;
}



// REMOVE O PRÓXIMO PERSONAGEM DA FILA
personagem remove_personagem (){
	cout << fila.fila[fila.tam-1].Nome << " começa a esquentar algo" << endl;
    fila.tam--;
    return fila.fila[fila.tam];
}



// IMPRIMI QUEM ESTA NA FILA NAQUELE MOMENTO
void imprimi_fila(){
    for(int i = 0; i < fila.tam; i++)
        cout << fila.fila[i].Nome << endl;
}



/*
	TEM O OBJETIVO DE ORDENAR A FILA DE ACORDO COM SUA PRIORIDADE DE FORMA DECRESCENTE, SENDO O ULTIMO DA FILA O PRÓXIMO 
	À USAR O MICROWAVE ( FOI FEITO ASSIM PARA FACILITAR A REMOÇÃO ). MAS COMO O PROBLEMA "The Big Bang Theory" MOSTRA,
	CASO ESTEJA 3 (TRÊS) PERSONAGENS DE PRIORIDADES DIFERENTES NA FILA OCORRE UM LOOP INFINITO, POIS ESTES NÃO ENTRAM
	EM CONSENSO SOBRE A ORDEM FINAL DE COMO FICARÁ A FILA.
*/
int sort_decrescente(){

    personagem aux;
    int p,e;
    p = fila.tam - 1;
    while(p > 0){
        e = p - 1;
        while(e >= 0){


            /*  VERIFICA SE A FUNÇÃO 'raj' SETOU 'raj_ver' COM VALOR 2
            	APÓS, VERIFICA SE HÁ CONFLITO NA FILA, SENDO VERDADE.
            */
            if (raj_ver == 2){
                if (raj_identifica() == 1){

                    sem_wait(&mutex_raj);
                    raj_ver = 0;
                    sem_post(&mutex_raj);

                    sem_wait(&peoples);
                    aux = remove_personagem ();
                    ir_comer(aux);

                    return 0;
                }
            }


            // OCORRE UMA TROCA DE PERSONAGEM CASO UM DE PRIORIDADE MAIR ESTEJA À FRENTE
            if(fila.fila[p].depoisDele == fila.fila[e].Nome || fila.fila[p].depoisDela == fila.fila[e].Nome)
            {
                trocar_people( p, e);
                e = p - 1;
            }
            else{
                e--; 
            }
        }
    p--;
    }

    sem_wait(&mutex_raj);
    raj_ver = 0;
    sem_post(&mutex_raj);
    return 1;
}



// REMOVE O PROXIMO NA FILA PARA USAR O MICROWAVE
void* micro_ondas(void* arg){

    personagem aux;
    while(1){
        if (fila.tam != 0){
            

            sleep(1);
            sem_wait(&peoples);
            sem_wait(&mutex);

            // REMOVENDO O PROXIMO DA FILA
            aux = remove_personagem ();

            sem_post(&mutex);

            // INDO COMER
            sem_wait(&mutex);
            ir_comer(aux);
            sem_post(&mutex);


        }
    }

    pthread_exit(NULL);
}



/* VERFICA SE OS PERSONAGENS QUE ESTÃO NAQUELE INSTANTE NA FILA, 
   SE À PELO MENOS 3(TRÊS) DO MESMO TIPO, SE FOR VERDADE A FUNÇÃO 
   RETORNARÁ '1' CASO CONTRARIO RETORNARÁ '0'
*/
int raj_identifica(){
    int tipo01 = 0, tipo02 = 0, tipo03 = 0;
    for(int i = 0; i < fila.tam; i++){
        if (fila.fila[i].Nome == elenco[0].Nome || fila.fila[i].Nome == elenco[1].Nome)
            tipo01 = 1;
        if (fila.fila[i].Nome == elenco[2].Nome || fila.fila[i].Nome == elenco[3].Nome)
            tipo02 = 1;
        if (fila.fila[i].Nome == elenco[4].Nome || fila.fila[i].Nome == elenco[5].Nome)
            tipo03 = 1;
    }
    if ((tipo01 + tipo02 + tipo03) == 3){
    	cout << "Raj detectou um deadlock, liberando " << fila.fila[fila.tam-1].Nome << endl;
        return 1;
    }
    else return 0;
}



/* A CADA 5 SEGUNDOS A FUNÇÃO 'raj'	verifica se 'raj_ver' É IGUAL À 0,
   CASO FOR VERDADE A VARIAVEL 'raj_ver' RECEBE 2
*/
void* raj(void* arg){
    while(true){
    	sleep(5);
        sem_wait(&mutex_raj);
        if (raj_ver == 0)
            raj_ver = 2;
        sem_post(&mutex_raj);
    }
}



/*  ADICIONA UM NOVO PERSONAGEM À FILA DE FORMA ALEATÓRIA,
    ADICIONA SOMENTE PERSONAGENS QUE JA NÃO ESTEJA NA FILA,
    USANDO O MICRONDAS OU ESTEJA COMENDO 
*/
void add_personagem(){


    int valor_rand;


    /* PROCURA UM PERSONAGEM DE FORMA ALEATORIA
       QUE NÃO ESTEJA NA FILA, USANDO O MICROWARE OU COMENDO
    */
    srand(time(0));
    do 
    {
        valor_rand = rand()%6;
    }while(dispp[valor_rand].disp != 0);


    // APÓS ENCONTRADO ALGUEM LIVRE ADICIONA-O NA FILA
    sem_wait(&mutex);
    dispp[valor_rand].disp = 1;
    cout << elenco[valor_rand].Nome << " quer usar o forno" << endl;
    fila.fila[fila.tam++] = elenco[valor_rand];
    while (sort_decrescente() == 0){}
    sem_post(&peoples);
    sem_post(&mutex);

}



int main()
{
	// INICIALIZANDO SEMAFOROS E DECLARANDO THREADS-------
    sem_init(&peoples, TRUE, 0);
    sem_init(&mutex, TRUE, 1);
    sem_init(&mutex_raj, TRUE, 1);

    struct tm *data_hora_atual;


    
    pthread_t m,r,v;
    pthread_create(&r, NULL, raj, NULL);
    pthread_create(&m, NULL, micro_ondas, NULL);
    pthread_create(&v, NULL, ver_disp, NULL);
    // ---------------------------------------------------

    // ADICIONANDO DE FORMA INDEFINIDA PERSONAGENS À FILA
    while (true){ sleep(0.95); add_personagem(); }
   

    return 0;
}