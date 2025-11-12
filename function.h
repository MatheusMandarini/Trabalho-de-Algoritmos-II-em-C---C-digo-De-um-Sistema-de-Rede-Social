/************************************************
* *
* Nome do(a) estudante: Matheus Mandarini de Menezes *
* Trabalho Prático *
* Disciplinas: Algoritmos e Programação II *
* Laboratório de Algoritmos e Programação II *
* Professores: Ivone e Ronaldo *
* Data: 14/11/2025 *
* Versão: 1.0 *
* Descrição: Um sistema simplificado de uma Rede Social, onde pode fazer cadastro de pessoas, fazer publicações, ter amizades e recomendações de amigos para usuários *  
* *
*************************************************/
#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>

/*Registro de amigos encadeado para cada usuário*/
typedef struct amigo {
    char nome[50];
    int id;
    struct amigo *prox;
} Amigo;
/*Registro de piblicacoes encadeado para cada usuário*/
typedef struct pub {
    char publicacao[200];
    int id;
    char nome[50];
    struct pub *prox;
} Publicacao;
/*Registro de usuários*/
typedef struct usuario {
    char nome[50];
    int idade;
    char cidade[50];
    Publicacao *publicacoes;
    int id;
    Amigo *amigos;
    struct usuario *prox;
} Usuario;

void adicionar_usuario(Usuario **lista_usuarios, FILE *arq);

void listar_usuarios(Usuario *lista_usuarios);

void remover_usuario(Usuario **lista_usuarios, int id, Publicacao **lista_publicacoes);

void publicar(Usuario **lista_usuarios, FILE *arq, Publicacao **lista_publicacoes);

void adicionar_amigo(Usuario *lista_usuarios, FILE *arq);

void recomedacoes_geral(Usuario *lista_usuarios, FILE *arq);

void recomedacao_por_id(Usuario *lista_usuarios, FILE *arq);

void lista_amigos(Usuario *lista_usuarios, FILE *arq);

void libera_memoria(Usuario *lista_usuarios, Publicacao *lista_publicacoes);

void relatorio(Publicacao *lista_publicacoes, FILE *arq_feed, Usuario *lista_usuarios, FILE *arq_recomendacoes, FILE *arq_relatorio);

void remove_amigo(Usuario **lista_usuarios, FILE *arq);

void feed_usuario(Usuario *lista_usuarios, Publicacao *lista_publicacoes, FILE *arq);

void atualiza(Usuario **lista_usuarios, FILE *arq);

void lista_publicacoes_usuario(Usuario *lista_usuarios, FILE *arq);

#endif



