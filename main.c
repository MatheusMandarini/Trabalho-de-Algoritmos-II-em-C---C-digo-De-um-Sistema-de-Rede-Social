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
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    Usuario *lista_usuarios = NULL;
    Publicacao *lista_publicacoes = NULL; /*uma pilha de publicações para ser colocada no arquivo da mais recente para a mais velha*/
    FILE *arq_input, *arq_feed, *arq_recomendacoes, *arq_relatorio;
    int opcao, id;
    char buffer[200];
    if(argc > 2) {
        printf("Erro de comando\n");
        return 1;
    }
    if(argc == 2) {
        strcpy(buffer, argv[1]);
    }
    
    /*Código para manipulação da rede social seria implementado aqui*/
    arq_input = fopen(buffer, "r");
    if(arq_input == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }
    /*arquivo para mostrar as informações dos cadastrados ao final do codigo*/
    arq_relatorio = fopen("relatorio.txt", "w");
    if(arq_relatorio == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }
    /*arquivo para mostrar o feed de todas as publicações que ocorreu durante o codigo*/
    arq_feed = fopen("feed_geral.txt", "w");
    if(arq_feed == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }
    /*arquivo para mostrar as recomendações de todos os restantes cadastrados em um arquivo*/
    arq_recomendacoes = fopen("recomendacoes.txt", "w");
    if(arq_recomendacoes == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }
    do {
        printf("\n=========================================\nMenu:\n");
        printf("\t1. Adicionar usuário\n");
        printf("\t2. Atualiza Usuário\n");
        printf("\t3. Remover usuário\n");
        printf("\t4. Publicar\n");
        printf("\t5. Adicionar amigo\n");
        printf("\t6. Listar amigos\n");
        printf("\t7. Recomendação de Amizade\n");
        printf("\t8. Remove Amizade\n");
        printf("\t9. Listar usuários\n");
        printf("\t10. Ver Feed do Usuário\n");
        printf("\t0. Sair\n");
        printf("Escolha uma opção: ");
        fscanf(arq_input, "%d", &opcao);
        printf("%d\n=========================================\n", opcao);
        switch(opcao) {
            case 1:
                adicionar_usuario(&lista_usuarios, arq_input);
                break;
            case 2:
                atualiza(&lista_usuarios, arq_input);
                break;
            case 3:
                printf("Digite o ID do usuário a ser removido: ");
                fscanf(arq_input, "%d", &id);
                printf("%d\n", id);
                remover_usuario(&lista_usuarios, id, &lista_publicacoes);
                break;
            case 4:
                publicar(&lista_usuarios, arq_input, &lista_publicacoes);
                break;
            case 5:
                adicionar_amigo(lista_usuarios, arq_input);
                break;
            case 6:
                lista_amigos(lista_usuarios, arq_input);
                break;
            case 7:
                recomedacao_por_id(lista_usuarios, arq_input);
                break;
            case 8:
                remove_amigo(&lista_usuarios, arq_input);
                break; 
            case 9:
                listar_usuarios(lista_usuarios);    
                break;
            case 10:
                feed_usuario(lista_usuarios, lista_publicacoes, arq_input);    
                break; 
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
            printf("\n");
        }
        if(feof(arq_input) && opcao != 0) {
            printf("Fim do arquivo de entrada alcançado.\n");
        }
    } while(opcao != 0 && !feof(arq_input));
    
    relatorio(lista_publicacoes, arq_feed, lista_usuarios, arq_recomendacoes, arq_relatorio);
    libera_memoria(lista_usuarios, lista_publicacoes);
    fclose(arq_feed);
    fclose(arq_input);
    fclose(arq_relatorio);
    fclose(arq_recomendacoes);

    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> e0a228d1c93717d2084a409c34d71ca40a639c45
