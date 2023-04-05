/*
 * mapeamento.h
 *
 *  Created on: 9 de mar de 2023
 *      Author: rodem
 */

#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include <stdio.h>
#include <stdlib.h>
#include "meuio.h"

#define TAMANHO_COMANDO 16

//Criando o tipo Funcao, uma função que recebe uma string e não retorna nada.
typedef void (*Funcao)(char*);

//Estrutura para armazenar comandos como uma lista(Sim não existe listas em C...)
struct pos_comandos{
    struct pos_comandos * prox; //Ponteiro para o próximo
    char *comando; //Texto que representa o comando
    Funcao f; //Função a ser executada pelo comando
};

//Dando o apelido para ponteiro para a estrutura acima.
typedef struct pos_comandos* Posicao;


//Função que recebe uma lista de comandos e um texto e apartir do texto
//procura o comando na lista de comandos, considerando a primeira palavra
void procura_comando(Posicao lista, char *texto){
//    criando string para salvar o texto do comando
    char comando[TAMANHO_COMANDO];

//    scanf de string, pegar a primeira palavra do texto e salvar na string
//    comando.
    sscanf(texto,"%s",comando);

//    interar toda a lista procurando o comando e quando acha o executa
    Posicao p;
    for(p = lista; p->prox != NULL; p = p->prox){
//        comparando o os comandos
        if(0 == strcmp(comando,p->prox->comando)){
//            executando a função do comando
            p->prox->f(texto);
            return;
        }
    }

//    mini tratamento de erro
    print("Comando não encontrado [", 1);
    print(comando, 1);
    print("]\n\r", 1);
}


//Cria uma lista vazia
Posicao inicia_lista_comandos(){
    //equivalente a um new
    Posicao p = (Posicao) malloc(sizeof(struct pos_comandos));
    p->prox = NULL;
    return p;
}

//função que adiciona um novo comando a lista de comandos
void adiciona_comando(Posicao cabeca, char * comando, Funcao f){

//    pegnado o menor tamanho para criar a string, o tamanho do
//    comando, ou a constante TAMANHO_COMANDO
    int t = strlen(comando);
    t = t < TAMANHO_COMANDO ? t : TAMANHO_COMANDO;

//    new
    Posicao p = (Posicao) malloc(sizeof(struct pos_comandos));

//    new na string
    p->comando = (char*) malloc(t * (sizeof(char)));

//    Não podemos passar a string atravez de =, temos que copiar
    strcpy(p->comando,comando);

    //set
    p->f = f;
    p->prox = NULL;

//    interando lista até o final dela
    Posicao aux = cabeca;
    while(aux->prox != NULL)
        aux = aux->prox;
    aux->prox = p;
}


//Função para transformar int em string, ignore e use sprintf, printf em string.
void int_to_str(int n, char* ptr){
    int aux,i = 0,valor = n >0 ? n: (-1 * n);
    char temp;
    *ptr++ = 0;
    do{
        ++i;
        aux = valor%10;
        valor/=10;
        *ptr++ = '0' + aux;
    }while(valor);
    if(n<0){
        *ptr= '-';
        ++i;
    }else
        --ptr;
    aux = i/2;
    for(valor=aux; valor >= 0; valor--){
        temp = *(ptr - (valor));
        *(ptr - (valor)) = *(ptr - (i - valor));
        *(ptr - (i - valor)) = temp;
    }
}





#endif /* MAPEAMENTO_H_ */
