#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct firstCome {
    int unidadeTempo;
    int processo;
    struct firstCome *prox;
} fcfs;


typedef struct {
    fcfs* inicio;
    fcfs* fim;
} extremidades;


typedef struct firstJob {
    int processo;
    int unidadeTempo;
    struct firstJob *prox;
} fj;



typedef struct roundRobin {
    int processo;
    int unidadeTempo;
    struct roundRobin *prox;
} rr;

typedef struct {
    rr* inicio;
    rr* fim;
} roundEx;


int trinta() {
    int i = rand() % 10 + 1;
    if(i >= 1 && i <= 3){
        return 1;
    } else { 
        return 0;
    }
}


int geraAleatorio(){
    int i = rand() % 20 + 1;
    return i;
}


//-------------------------------------------------------------------
//Funções para o firstJob
//First Job funciona


void inserirOrdenado(fj **lista, int novoProcesso, int novaUnidadeTempo) {
    fj *novoNo = (fj *)malloc(sizeof(fj));
    if (!novoNo) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->processo = novoProcesso;
    novoNo->unidadeTempo = novaUnidadeTempo;
    novoNo->prox = NULL;

    if (*lista == NULL) {
        // Inserir no início
        novoNo->prox = *lista;
        *lista = novoNo;
    } else if (novaUnidadeTempo < (*lista)->unidadeTempo) {
        fj* atual = (*lista)->prox;
        if(novaUnidadeTempo < atual->unidadeTempo) {
            (*lista)->prox = novoNo;
            novoNo->prox = atual;
        } else {
            fj *atual = *lista;
            while (atual->prox != NULL && atual->prox->unidadeTempo < novaUnidadeTempo) {
            atual = atual->prox;
        }
        }
    }  else {
        // Encontrar posição de inserção
        fj *atual = *lista;
        while (atual->prox != NULL && atual->prox->unidadeTempo < novaUnidadeTempo) {
            atual = atual->prox;
        }

        // Inserir no meio ou no final
        novoNo->prox = atual->prox;
        atual->prox = novoNo;
    }
}



/*
void inserirOrdenado(fj **lista, int novoProcesso, int novaUnidadeTempo) {
    fj *novoNo = (fj *)malloc(sizeof(fj));
    if (!novoNo) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->processo = novoProcesso;
    novoNo->unidadeTempo = novaUnidadeTempo;
    novoNo->prox = NULL;

    if (*lista == NULL || novaUnidadeTempo < (*lista)->unidadeTempo) {
        // Inserir no início
        novoNo->prox = *lista;
        *lista = novoNo;
    } else {
        // Encontrar posição de inserção
        fj *atual = *lista;
        while (atual->prox != NULL && atual->prox->unidadeTempo < novaUnidadeTempo) {
            atual = atual->prox;
        }

        // Inserir no meio ou no final
        novoNo->prox = atual->prox;
        atual->prox = novoNo;
    }
}
*/

int firstJob() {
    int iteracoes = 0;
    int numero_processos = 1;
    int _30;
    int criado = 0;
    int processo_criado = 0, tempo_criado = 0;
    fj *lista = (fj*)malloc(sizeof(fj));
    lista->processo = numero_processos;
    lista->unidadeTempo = geraAleatorio();
    lista->prox = NULL;
    printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)\tAção: Criação do processo:(%d, %d)\n", 
    iteracoes, lista->processo, lista->unidadeTempo, lista->processo, lista->unidadeTempo);
    iteracoes++;
    numero_processos++;
    
    while(iteracoes < 100) {
        _30 = trinta();
        if(_30 == 1){
            fj *node = (fj*)malloc(sizeof(fj));
            tempo_criado = geraAleatorio();
            inserirOrdenado(&lista, numero_processos, tempo_criado);
            processo_criado = numero_processos;
            numero_processos++;
            criado = 1;
            free(node);
            }

        lista->unidadeTempo--;
        if(lista->prox != NULL) {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(%d, %d)",  
            iteracoes, lista->processo, lista->unidadeTempo,lista->prox->processo, lista->prox->unidadeTempo);
        } else {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)",  
            iteracoes, lista->processo, lista->unidadeTempo);
        }
        if(criado == 1) {
            printf("\tAção: Criação do processo (Id:%d, Tempo:%d)\n", processo_criado, tempo_criado);
            criado = 0;
        }   else {
            printf("\tAção: Nenhuma\n");
        }
        //lista->unidadeTempo--;

        if (lista->unidadeTempo == 0) {
            fj *temp = lista;
            lista = lista->prox;
            free(temp);  // Liberar espaço de memória do processo finalizado
        }   
        iteracoes++;
    }
    
    return numero_processos-1;
}




//----------------------------------------------
// Funções para o roundRobin
void desenfileirar(roundEx* fila) {
    if (fila->inicio == NULL) {
        printf("Fila vazia\n");
        return;
    }

    rr* remove = fila->inicio;
    fila->inicio = fila->inicio->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(remove);
}


void enfileirar(roundEx* fila, int processo, int tempo) {
    rr* novo = (rr*)malloc(sizeof(rr));
    novo->unidadeTempo = tempo;
    novo->processo = processo;
    novo->prox = NULL;

    if (fila->fim == NULL) {
        fila->fim = novo;
        fila->inicio = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }

    // Adicionamos este trecho para garantir que fila->inicio->prox seja definido corretamente
    if (fila->inicio->prox == NULL) {
        fila->inicio->prox = novo;
    }
}



int roundRobin() {
    int iteracoes = 0;
    int numero_processos = 1;
    int _30;
    int criado = 0;
    int tempo_criado;
    int processo_criado = 0;
    int _6 = 0;

    roundEx lista;
    lista.fim = NULL;
    lista.inicio = NULL;

    enfileirar(&lista, numero_processos, geraAleatorio());
    printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)\tAção: Criação do processo:(%d, %d)\n", 
    iteracoes, lista.inicio->processo, lista.inicio->unidadeTempo, lista.inicio->processo, lista.inicio->unidadeTempo);
    iteracoes++;
    numero_processos++;
    
    while(1) {
        _30 = trinta();
        if(_30 == 1){
            tempo_criado = geraAleatorio();
            enfileirar(&lista, numero_processos, tempo_criado);
            numero_processos++;
            criado = 1;
            }

        lista.inicio->unidadeTempo--;
        _6++;

        if(lista.inicio->prox != NULL) {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(%d, %d)",  
            iteracoes, lista.inicio->processo, lista.inicio->unidadeTempo,lista.inicio->prox->processo, lista.inicio->prox->unidadeTempo);
        } else {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)",  
            iteracoes, lista.inicio->processo, lista.inicio->unidadeTempo);
        }
        if(criado == 1) {
            printf("\tAção: Criação do processo (Id:%d, Tempo:%d)\n", (numero_processos-1), tempo_criado);
            criado = 0;
        }   else {
            printf("\tAção: Nenhuma\n");
        }
        //lista->unidadeTempo--;

        if (lista.inicio->unidadeTempo == 0) {
            desenfileirar(&lista);
            _6 = 0;
        }   
        if (_6 == 6) {
            enfileirar(&lista, lista.inicio->processo, lista.inicio->unidadeTempo);
            desenfileirar(&lista);
            _6 = 0;
        }
        iteracoes++;
    }
    
    return numero_processos-1;
}







//-----------------------------------------------------------------
// Funções para o firstCome
// First Come funcionando


void enfileirarF(extremidades* fila, int processo, int tempo) {
    fcfs* novo = (fcfs*)malloc(sizeof(fcfs));
    novo->unidadeTempo = tempo;
    novo->processo = processo;
    novo->prox = NULL;

    if (fila->fim == NULL) {
        fila->fim = novo;
        fila->inicio = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }

    // Adicionamos este trecho para garantir que fila->inicio->prox seja definido corretamente
    if (fila->inicio->prox == NULL) {
        fila->inicio->prox = novo;
    }
}

void desenfileirarF(extremidades* fila) {
    if (fila->inicio == NULL) {
        printf("Fila vazia\n");
        return;
    }

    fcfs* remove = fila->inicio;
    fila->inicio = fila->inicio->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(remove);
}




int firstCome() {

    extremidades fila;
    fila.inicio = NULL;
    fila.fim = NULL;

    int iteracoes = 0;
    int numero_processos = 1;
    int _30;
    int criado = 0;
    int processo_criado = 0, tempo_criado;
    tempo_criado = geraAleatorio();
    enfileirarF(&fila, numero_processos, tempo_criado);

    printf("fila.inicio%d,%d\n", fila.inicio->processo, fila.inicio->unidadeTempo);
    numero_processos++;

    printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)\tAção: Nenhuma\n", iteracoes, fila.inicio->processo, fila.inicio->unidadeTempo);

    printf("Lakaka");
    iteracoes++;
    //TODO: Loop infinito (?)
    while(1) {
        _30 = trinta();
        if(_30 == 1){
            tempo_criado = geraAleatorio();
            enfileirarF(&fila, numero_processos, tempo_criado);
            numero_processos++;
            criado = 1;
        }

        //printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\n",  iteracoes, fila[tamanho].processo, fila[tamanho].unidadeTempo);
        fila.inicio->unidadeTempo--;
        if(fila.inicio->prox != NULL) {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(%d, %d)",  
            iteracoes, fila.inicio->processo, fila.inicio->unidadeTempo,fila.inicio->prox->processo, fila.inicio->prox->unidadeTempo);
        } else {
            printf("Iteração:%d\tID_processo:%d\tUnidadeTempoRestante:%d\tPróximoProcesso:(NULL, NULL)",  
            iteracoes, fila.inicio->processo, fila.inicio->unidadeTempo);
        }
        if(criado == 1) {
            printf("\tAção: Criação do processo (Id:%d, Tempo:%d)\n", (numero_processos-1), tempo_criado);
            criado = 0;
        }   else {
            printf("\tAção: Nenhuma\n");
        }
        
        if(fila.inicio->unidadeTempo == 0) {
            desenfileirarF(&fila);
        }
        iteracoes++;
    }
    
    return numero_processos-1;
}





int main() {
    srand(time(NULL));
    int define = 1;
    int lakaka = 0;
    while(define != 0){
        printf("\nEscolha o método desejado:\n");
        printf("1- First Come, First Served\n");
        printf("2- Shortest Job First\n");
        printf("3- Round-Robin\n");
        printf("0- Sair do Programa\n");
        
        scanf("%d", &define);

        switch (define) {
            case 1:
                lakaka = firstCome();
                break;
            case 2:
                lakaka = firstJob();
                break;
            case 3:
                lakaka = roundRobin();
                break;
            case 0:
                printf("Saindo do programa");
                break;
            default:
                printf("\nValor incorreto, tente novamente!\n");
        
        } 
        }
        




    return 0;
}

