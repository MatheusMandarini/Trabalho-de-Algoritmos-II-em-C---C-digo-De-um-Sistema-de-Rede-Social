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

void adicionar_usuario(Usuario **lista_usuarios, FILE *arq) {
    char nome[50];
    int idade, id;
    int fecha_procedimento;
    char cidade[50];
    Usuario *novo_usuario, *q, *p;
    char ch;
    fecha_procedimento=0;

    /*Leitura do arquivo para cada dado*/
    fscanf(arq, "%d %49s %d %49s", &id, nome, &idade, cidade);
    
    /*verifica se leu tudo corretamente e se não tem mais nada na linha*/
    ch = fgetc(arq);
    if(ch != '\n' && ch != EOF) {
        /* Limpa o restante da linha até o final */
        printf("Aviso: Nome / Nome da cidade maior que o permitido de 50 caracteres ou formato inválido na entrada.\n");
        while (ch != '\n' && ch != EOF) {
            ch = fgetc(arq);
        }
        fecha_procedimento=1; /*indica que houve erro na leitura dos dados*/
    }

    /*Mostra os dados lidos*/
    if(fecha_procedimento!=1)
        printf("Nome: %s\nIdade: %d\nCidade: %s\nID: %d\n", nome, idade, cidade, id);
    else
        printf("\nFalha na leitura dos dados do usuario com o ID: %d.\n", id);
    
        /*verificação de idade negativa*/
    if(idade <0){
        printf("\nIdade inválida! Deve ser um número não negativo.\n");
        fecha_procedimento=1;
    }
    
    /*verificação de ID negativo ou maior que 9999*/
    if(id <0){
        printf("\nID inválido! Deve ser um número não negativo.\n");
        fecha_procedimento=1;
    }
    
    /*verificação de ID maior que 9999*/
    if(id > 9999){
        printf("\nID inválido! Deve ser um número menor que 10000.\n");
        fecha_procedimento=1;
    }
    
    /*verificação se tem ID igual ja adicionado, caso ja cadastrado, le de novo até nao ter outro cadastrado*/
    q = *lista_usuarios;
    while(q != NULL) {
        if(q->id == id) {
            printf("O ID %d ja esta cadastrado!", id);
            fecha_procedimento=1;
            q=NULL; /*sai do while*/
        }
        else {
            q = q->prox;   
        }
    }

    /*Se não houve erro na leitura dos dados, adiciona o usuário na lista*/
    if(fecha_procedimento!=1){
        /*Abre malloc e verifica se não esta NULL*/
        novo_usuario = (Usuario *)malloc(sizeof(Usuario));
        if(novo_usuario == NULL) {
            printf("Erro de alocação de memória!\n");
        }
        else {
            /*Adiciona Na lista em ordem crescente alfabetica*/
            strcpy(novo_usuario->nome, nome);
            novo_usuario->idade = idade;
            strcpy(novo_usuario->cidade, cidade);
            novo_usuario->id = id;
            novo_usuario->amigos = NULL;
            novo_usuario->prox = NULL;
            novo_usuario->publicacoes = NULL;

            /*Verifica se não tem ninguem na lista ou que em ordem alfabetica é menor que o primeiro da lista*/
            if(*lista_usuarios == NULL || strcmp(novo_usuario->nome, (*lista_usuarios)->nome) < 0) {
                novo_usuario->prox = *lista_usuarios;
                *lista_usuarios = novo_usuario;
            } else {
                p = *lista_usuarios;
                /*while anda até encontra que o proximo seja maior em ordem alfabetica que o que quer adicionar*/
                while(p->prox != NULL && strcmp(p->prox->nome, novo_usuario->nome) < 0) {
                    p = p->prox;
                }
                novo_usuario->prox = p->prox;
                p->prox = novo_usuario;
            }
            printf("Usuário %s adicionado com sucesso!\n", nome);
        }
    } 

}
/*Função para listar todos os usuários cadastrados*/
void listar_usuarios(Usuario *lista_usuarios) {
    Usuario *p = lista_usuarios;
    
    if(p == NULL) {
        printf("Nenhum usuário cadastrado.\n");
   
    }
    else{
        printf("Lista de usuários cadastrados:\n");
        while(p != NULL) {
            printf("\tNome: %s\n\tIdade: %d\n\tCidade: %s\n\tID: %d\t\n=========================================\n", p->nome, p->idade, p->cidade, p->id);
            p = p->prox;
        }
    }
}
/*função recursiva de busca*/
Usuario* busca_usuario(Usuario *lista_usuarios, int id) {
    /*Caso base: lista vazia (usuário não encontrado) */
    if (lista_usuarios == NULL) {
        return NULL;
    }

    /* Caso base: usuário encontrado*/
    if (lista_usuarios->id == id) {
        return lista_usuarios;
    }

    /* Chamada recursiva para o próximo nó da lista*/
    return busca_usuario(lista_usuarios->prox, id);
}
/*Função para remover um usuário da lista*/
void remover_usuario(Usuario **lista_usuarios, int id, Publicacao **lista_publicacoes) {
    Usuario *p, *q;
    Usuario *ant = NULL;
    Publicacao *pub_atual;
    Publicacao *pub_ant = NULL;
    Amigo *r, *antA;
    
    antA=NULL;
    p = *lista_usuarios;
    
    /*Procura o usuário na lista*/
    while(p != NULL && p->id != id) {
        ant = p;
        p = p->prox;
    }
    
    if(p == NULL) {
        printf("Usuário com ID %d não encontrado.\n", id);
    }
    else{
        
        /*Remove o usuário da lista*/
        if(ant == NULL) {
            *lista_usuarios = p->prox;
        } else {
            ant->prox = p->prox;
        }
        
        /*Remove esse usuário das listas de amigos de outros usuários*/
        q = *lista_usuarios;
        while(q != NULL) {
            
            antA = NULL;
            r = q->amigos;
            while(r != NULL) {
                
                if(r->id == id) {
                    if(antA == NULL) {
                        q->amigos = r->prox; /*caso for o primeiro da lista de amigos a ser removido*/
                    } else {
                        antA->prox = r->prox; /*caso for no meio da lista*/
                    }
                    free(r);
                    r=NULL; /*sai do while*/
                
                }
                else {
                    antA = r;
                    r = r->prox; /*anda para o proximo amigo adicionado*/
                
                }
            
            }
            
            q = q->prox; /*passa pro proximo usuario para fazer a limpa desse usuario sendo removido de todos amigos que tinha*/
        
        }
        
        printf("Usuário %s(ID: %d) removido com sucesso.\n", p->nome, id);
        free(p);
        
        /*remover a publicação do usuario removido*/
        pub_atual = *lista_publicacoes;
        pub_ant = NULL;
        while(pub_atual != NULL) {
            if(pub_atual->id == id) {
                if(pub_ant == NULL) {
                    *lista_publicacoes = pub_atual->prox; /*caso for o primeiro da lista de publicações a ser removido*/
                
                } else {
                    pub_ant->prox = pub_atual->prox; /*caso for no meio da lista*/
                
                }
                
                free(pub_atual);
                if(pub_ant == NULL) {
                    pub_atual = *lista_publicacoes; /*reinicia o ponteiro caso tenha removido o primeiro*/
                
                } else {
                    pub_atual = pub_ant->prox; /*continua do ponto onde parou*/
                
                }
            } else {
                pub_ant = pub_atual;
                pub_atual = pub_atual->prox; /*anda para a próxima publicação*/
            
            }
        
        }
        
    }

}
/*Função para adicionar uma publicação de um usuário*/
void publicar(Usuario **lista_usuarios, FILE *arq, Publicacao **lista_publicacoes) {
    Usuario *p = *lista_usuarios;
    int fecha_procedimento;
    char c;
    char publicacao[200];
    int id;
    Publicacao *nova_pub, *temp_pub;

    fecha_procedimento=0;
    
    /* lê id e a publicação (uma palavra) na mesma linha */
    printf("Digite o ID do usuário que fará a publicação: ");
    
    if (fscanf(arq, "%d ", &id) == 1) {
        
        printf("%d\n", id);
        
        fgets(publicacao, sizeof(publicacao), arq); /* lê a publicação até o final da linha */
        
        /*caso há publicação maior que o permitido, limpa o restante da linha*/
        c = publicacao[strlen(publicacao) - 1];
        if (c != '\n' && c != EOF) {
            printf("\nErro: Publicação maior que o permitido de 200 caracteres ou formato inválido na entrada.\n");
            while (c != '\n' && c != EOF) {
                c = fgetc(arq);
            }
            fecha_procedimento=1; /*indica que houve erro na leitura dos dados*/
        }

        /*busca o usuário pelo ID*/
        p= busca_usuario(*lista_usuarios, id);  
        if (p == NULL) {
            printf("Usuário com ID %d não encontrado.\n", id);
        }
        else{
            if(fecha_procedimento!=1){
                /*temp_pub para armazenar na lista geral de publicações*/
                temp_pub = (Publicacao *)malloc(sizeof(Publicacao));
                
                if (temp_pub == NULL) {
                    printf("Erro de alocação de memória!\n");
                }
                
                strcpy(temp_pub->publicacao, publicacao);
                temp_pub->id = id;
                strcpy(temp_pub->nome, p->nome);
                
                if (*lista_publicacoes==NULL) /*pilha para armazenar as publicações por ordem de tempo*/
                {
                    temp_pub->prox = NULL;
                    *lista_publicacoes = temp_pub;
                }
                else{
                    temp_pub->prox = *lista_publicacoes;
                    *lista_publicacoes = temp_pub;
                }

                /* armazena a última publicação no campo publicacoes (estrutura atual) */
                /*nova_pub para armazenar na lista de publicações do usuário*/
                nova_pub = (Publicacao *)malloc(sizeof(Publicacao));
                if (nova_pub == NULL) {
                    printf("Erro de alocação de memória!\n");
                }
                
                strcpy(nova_pub->publicacao, publicacao);
                
                /* insere no início da lista de publicações como uma pilha, por ser em ordem de "tempo, o primeiro a ser mostrado sera a mais recente"*/
                nova_pub->prox = NULL;
                nova_pub->prox = p->publicacoes;
                p->publicacoes = nova_pub;
                printf("\n-----------------------------------------\nO/A usuário/a %s (ID: %d) publicou:\n\t%s-----------------------------------------", p->nome, p->id, publicacao);
                printf("\n");
            }
        }
    }
}
/*Função para adicionar um amigo a um usuário*/
void adicionar_amigo(Usuario *lista_usuarios, FILE *arq) {
    Usuario *p, *q;
    int id, id_amigo, ja_sao_amigos=0;
    Amigo *novo_amigo, *r, *novo_amigo2;

    printf("Digite os IDs dos usuários que se tornarão amigos (separados por espaço): ");
    
    /* lê os dois ids na mesma linha: id id_amigo */
    if (fscanf(arq, "%d %d", &id, &id_amigo) == 2){
        printf("%d %d\n", id, id_amigo);

        p = lista_usuarios;
        while (p != NULL && p->id != id) 
            p = p->prox;
        if (p == NULL) {
            printf("Usuário com ID %d não encontrado.\n", id);
        }
        else
        {
            q = lista_usuarios;
            while (q != NULL && q->id != id_amigo)
                q = q->prox;
            if (q == NULL) {
                printf("Usuário com ID %d não encontrado.\n", id_amigo);
            }
            else
            {
                /*verifica se ja são amiogos*/
                r = p->amigos;
                while (r != NULL) {
                    if (r->id == id_amigo) {
                        printf("%s e %s ja sao amigos!\n", p->nome, q->nome);
                        ja_sao_amigos = 1; /* são amigos */
                        r=NULL; /*sai do while*/
                    }
                    else {
                        r = r->prox;
                    }
                }
                if (ja_sao_amigos!=1) /*se não são amigos, adiciona a amizade*/
                {
                    
                    /* cria amigo para p com nome de q */
                    novo_amigo = (Amigo *)malloc(sizeof(Amigo));
                    if (novo_amigo == NULL) {  
                        printf("Erro de alocação de memória!\n");
                    }
                    strcpy(novo_amigo->nome, q->nome);
                    novo_amigo->id = id_amigo;
                    novo_amigo->prox = NULL;

                    /* insere ordenado por nome na lista de amigos de p */
                    if (p->amigos == NULL || strcmp(novo_amigo->nome, p->amigos->nome) < 0) { /* insere no início */
                        novo_amigo->prox = p->amigos;
                        p->amigos = novo_amigo;
                    } else { /* insere no meio ou fim */
                        
                        /*caso os nomes forem iguais sera ordenado por ordem descrescente de id*/
                        if (strcmp(novo_amigo->nome, p->amigos->nome) == 0)
                        {
                            if (novo_amigo->id < p->amigos->id) /*insere no inicio*/
                            {
                                novo_amigo->prox = p->amigos;
                                p->amigos = novo_amigo;
                            }
                            else{
                                r = p->amigos;
                                while (r->prox != NULL && strcmp(r->prox->nome, novo_amigo->nome) == 0 && r->prox->id < novo_amigo->id) /* encontra posição correta */
                                    r = r->prox;
                                novo_amigo->prox = r->prox;
                                r->prox = novo_amigo;
                            }
                            
                        }
                        else{
                            r = p->amigos;
                            while (r->prox != NULL && strcmp(r->prox->nome, novo_amigo->nome) < 0) /* encontra posição correta */
                                r = r->prox;
                            novo_amigo->prox = r->prox;
                            r->prox = novo_amigo;
                        }
                    }
                    
                    /* cria amigo recíproco para q com nome de p */
                    novo_amigo2 = (Amigo *)malloc(sizeof(Amigo));
                    if (novo_amigo2 == NULL) { 
                        printf("Erro de alocação de memória!\n"); 
                    }
                    strcpy(novo_amigo2->nome, p->nome);
                    novo_amigo2->id = p->id;
                    novo_amigo2->prox = NULL;

                    if (q->amigos == NULL || strcmp(novo_amigo2->nome, q->amigos->nome) < 0) {
                        novo_amigo2->prox = q->amigos;
                        q->amigos = novo_amigo2;
                    } else {
                        /*caso os nomes forem iguais sera ordenado por ordem descrescente de id*/
                        if (strcmp(novo_amigo2->nome, q->amigos->nome) == 0)
                        {
                            if (novo_amigo2->id < q->amigos->id) /*insere no inicio*/
                            {
                                novo_amigo2->prox = q->amigos;
                                q->amigos = novo_amigo2;
                            }
                            else{
                                r = q->amigos;
                                while (r->prox != NULL && strcmp(r->prox->nome, novo_amigo2->nome) == 0 && r->prox->id < novo_amigo2->id) /* encontra posição correta */
                                    r = r->prox;
                                novo_amigo2->prox = r->prox;
                                r->prox = novo_amigo2;
                            }
                        }
                        else{
                            r = q->amigos;
                            while (r->prox != NULL && strcmp(r->prox->nome, novo_amigo2->nome) < 0) 
                                r = r->prox;
                            novo_amigo2->prox = r->prox;
                            r->prox = novo_amigo2;
                        }
                    }

                    printf("Amizade adicionada entre %s e %s.\n", p->nome, q->nome);
                }
            }
            
        }
        
    }   
    else {
        printf("Erro ao ler os IDs dos usuários.\n");
    }
}

/*função auxiliar para empilhar os nomes dos amigos em comum na recomendação*/
void empilhar_nomes(char pilha_nomes[][100], const char *nome, int *topo, Amigo *amigo_p) {
    
    /*verifica limite da pilha para evitar overflow*/
    /*se o limite estourar da um aviso de erro e mostra o ID da recomedação que deu erro*/
    if (*topo <= 199) {
        (*topo)++;
        strcpy(pilha_nomes[*topo], nome);
    }
    else {
        printf("\nErro: Limite de amigos em comum excedido para o amigo %s (ID: %d).\n", amigo_p->nome, amigo_p->id);
    }
}

/*função auxiliar para printar os nomes dos amigos em comum na recomendação*/
void printar_nomes(char pilha_nomes[][100], int *topo, FILE *arq) {
    int i;

    for (i = *topo; i >= 0; i--) {
        fprintf(arq, " %s", pilha_nomes[i]);
        if (i > 0) {
            fprintf(arq, ", ");
        }
    }
    /*zera o topo da pilha após imprimir*/
    *topo = -1;

}

/*função para converter int para string*/
void int_para_string(int numero, char *str) {
    int i = 0, j;
    char temp;

    do {
        str[i++] = (numero % 10) + '0';
        numero /= 10;
    } while (numero > 0);

    str[i] = '\0'; 

    /*inverte a string*/
    for (j = 0; j < i / 2; j++) {
        temp = str[j];                
        str[j] = str[i - j - 1];      
        str[i - j - 1] = temp;        
    }
}

/*esse procedimento recomendará as sugestões de amigos para todos os usuarios, e para recomendar esses amigos, ele terá que ter pelos menos um amigo em 
comum para ser recomendado, e mostrará quantos amigos em comum eles possuem e esse amigo a ser recomendado não pode ja ser amigo dele*/
void recomedacoes_geral(Usuario *lista_usuarios, FILE *arq) {
    Usuario *p, *q;
    Amigo *amigo_p, *amigo_q;
    int amigos_em_comum, ja_sao_amigos, apareceu;
    char string_id[10];
    char pilha_nomes[200][100]; /*pilha para armazenar os nomes dos amigos em comum e seus ID respectivamente*/
    int topo = -1; /*topo da pilha*/
    char nome_amigo[100];
    
    p = lista_usuarios;
    
    if(p==NULL)
    {
        fprintf(arq, "RECOMENDAÇÕES DE AMIGOS\n");
        fprintf(arq, "=========================================================\n");
        fprintf(arq, "Nenhum usuário cadastrado para recomendações de amigos.\n");
        fprintf(arq, "=========================================================\n");
    }
    else{

        fprintf(arq, "RECOMENDAÇÕES DE AMIGOS\n");
        while(p != NULL) {
            /* cabeçalho por usuário (caixa) */
            fprintf(arq, "\n+----------------------+--------+--------------------+-----------------------+\n");
            fprintf(arq, "| Recomendações para %-46.60s (ID: %d) |\n", p->nome, p->id);
            fprintf(arq, "+----------------------+--------+--------------------+-----------------------+\n");
            fprintf(arq, "| %-20s | %-6s | %-15s | %-21s |\n",
                "Nome", "ID", "N° Amigos em comum", "Amigos em comum");
            fprintf(arq, "+----------------------+--------+--------------------+-----------------------+\n");

            q = lista_usuarios;
            apareceu = 0;
            while(q != NULL) {
                if(p->id != q->id) { /*não pode recomendar ele mesmo*/
                    /*verifica se ja são amigos*/
                    amigo_p = p->amigos;
                    ja_sao_amigos = 0;
                    while(amigo_p != NULL) {
                        if(amigo_p->id == q->id) { /*caso ja forem amigos, ja sai do while e parte para a proxima*/
                            ja_sao_amigos = 1;
                            amigo_p = NULL; /*sai do while*/
                        }
                        else{
                            amigo_p = amigo_p->prox;
                        }        
                    }
                    if(ja_sao_amigos == 0) {
                        /*conta amigos em comum*/
                        amigos_em_comum = 0; /*zera contador*/
                        amigo_p = p->amigos; /*reinicia amigo_p para o começo da lista de amigos de p*/
                        while(amigo_p != NULL) { /*percorre a lista de amigos de p*/
                            amigo_q = q->amigos;
                            while(amigo_q != NULL) { /*percorre a lista de amigos de q*/
                                if(amigo_p->id == amigo_q->id) { /*achou amigo em comum*/
                                    amigos_em_comum++; /*incrementa contador*/
                                    apareceu = 1;
                                    amigo_q = NULL; /*sai do while de amigos de q*/
                                    int_para_string(amigo_p->id, string_id); /*converte o id do amigo em comum para string*/
                                    
                                    strcpy(nome_amigo, amigo_p->nome); /*copia o nome do amigo em comum para a string*/
                                    strcat(nome_amigo, " (ID: ");
                                    
                                    strcat(nome_amigo, string_id); /*adiciona o id do amigo em comum na string*/
                                    strcat(nome_amigo, ")");
                                    empilhar_nomes(pilha_nomes, nome_amigo, &topo, amigo_p); /*empilha o nome do amigo em comum*/  
                                }
                                else /*continua procurando na lista de amigos de q*/
                                {
                                    amigo_q = amigo_q->prox;
                                }
                            }
        
                            amigo_p = amigo_p->prox; /*continua procurando na lista de amigos de p*/
                        }
                        if(amigos_em_comum > 0) { /*se tiver pelo menos um amigo em comum, mostra a recomendação*/
                            fprintf(arq, "| %-20.20s | %-6d | %-18d |",
                            q->nome, q->id, amigos_em_comum);
                            printar_nomes(pilha_nomes, &topo, arq);
                            fprintf(arq, " \n");
                        } 
                    }
                    
                }
                q = q->prox;
            }
            if (apareceu==0)
            {
                fprintf(arq, "| %-77s |\n", "Nenhuma recomendação disponível.");
            }
            fprintf(arq, "+----------------------+--------+--------------------+-----------------------+\n");
            p = p->prox;
        }
    }
}

/*esse procedimento recomendará as sugestões de amigos para um usuário específico, e para recomendar esses amigos, ele terá que ter pelos menos um amigo em 
comum para ser recomendado, e mostrará quantos amigos em comum eles possuem e esse amigo a ser recomendado não pode ja ser amigo dele*/
void recomedacao_por_id(Usuario *lista_usuarios, FILE *arq) {
    Usuario *p, *q;
    Amigo *amigo_p, *amigo_q;
    int amigos_em_comum, ja_sao_amigos, apareceu=0;
    int id;
    
    printf("Digite o ID da pessoa que gostaria de receber recomendações de amigos: ");
    fscanf(arq, "%d", &id);
    
    printf("%d\n", id);
    p = busca_usuario(lista_usuarios, id);
    
    if(p == NULL) {
        printf("\nUsuário com ID %d não encontrado.\n", id);
    }
    else{
        printf("Recomendações de amigos para %s:\n", p->nome);
        
        q = lista_usuarios;
        while(q != NULL) {
            if(p->id != q->id) { /*não pode recomendar ele mesmo*/
                
                /*verifica se ja são amigos*/
                amigo_p = p->amigos;
                ja_sao_amigos = 0;
                while(amigo_p != NULL) {
                    if(amigo_p->id == q->id) { /*caso ja forem amigos, ja sai do while e parte para a proxima*/
                        ja_sao_amigos = 1;
                        amigo_p = NULL; /*sai do while*/
                    
                    }
                    else{
                        amigo_p = amigo_p->prox;
                    
                    }        
                
                }
                if(ja_sao_amigos == 0) {
                    
                    /*conta amigos em comum*/
                    amigos_em_comum = 0; /*zera contador*/
                    amigo_p = p->amigos; /*reinicia amigo_p para o começo da lista de amigos de p*/
                    while(amigo_p != NULL) { /*percorre a lista de amigos de p*/
                        
                        amigo_q = q->amigos;
                        while(amigo_q != NULL) { /*percorre a lista de amigos de q*/
                            
                            if(amigo_p->id == amigo_q->id) { /*achou amigo em comum*/
                                amigos_em_comum++; /*incrementa contador*/
                                apareceu = 1;
                                amigo_q = NULL; /*sai do while de amigos de q*/
                            
                            }
                            else /*continua procurando na lista de amigos de q*/
                            {
                                amigo_q = amigo_q->prox;
                            
                            }
                        
                        }
                        amigo_p = amigo_p->prox; /*continua procurando na lista de amigos de p*/
                    
                    }
                    if(amigos_em_comum > 0) { /*se tiver pelo menos um amigo em comum, mostra a recomendação*/
                        printf("\t%s (ID: %d) - Amigos em comum: %d\n", q->nome, q->id, amigos_em_comum);
                    
                    }
                
                }
            
            }
            q = q->prox;
        
        }
        if (apareceu==0)
        {
            printf("\tNenhuma recomendação disponível.\n");
        
        }
    
    }

}
/*Função para listar os amigos de um usuário*/
void lista_amigos(Usuario *lista_usuarios, FILE *arq) {
    Usuario *p = lista_usuarios;
    Amigo *r;
    int id;
    
    p = lista_usuarios;
    
    printf("Digite o ID da pessoa que gostaria de listar as amizades: ");
    fscanf(arq, "%d", &id);
    printf("%d\n", id);
    if(p == NULL) {
        printf("Nenhum usuário cadastrado.\n");
    
    }
    else{
        
        while(p!=NULL && p->id!=id)
        {
            p=p->prox;
        }
        if(p==NULL)
        {
            printf("ID inexistente\n");
        
        }
        else
        {
            
            r=p->amigos;
            if(r==NULL){
                printf("Usuário Não Possui Amigos\n");
            }
            else{
                printf("Amizades de %s(ID: %d):\n", p->nome, p->id);
                
                while (r!=NULL)
                {
                    printf("\tNome: %s (ID: %d)\n", r->nome, r->id);
                    r=r->prox;
                
                }
            
            }
        
        }
   
    }

}
/*Função para liberar toda a memória alocada dinamicamente*/
void libera_memoria(Usuario *lista_usuarios, Publicacao *lista_publicacoes) {
    Usuario *p = lista_usuarios;
    Usuario *temp_usuario;
    Amigo *r, *temp_amigo;
    Publicacao *s, *temp_pub;
    Publicacao *temp_publicacao;
    
    /*Libera lista de publicações geral*/
    while(lista_publicacoes != NULL) {
        temp_publicacao = lista_publicacoes;
        lista_publicacoes = lista_publicacoes->prox;
        free(temp_publicacao);
    }
    
    /*Libera lista de usuários*/
    while(p != NULL) {
        /*Libera lista de amigos*/
        r = p->amigos;
        while(r != NULL) {
            temp_amigo = r;
            r = r->prox;
            free(temp_amigo);
        }
        /*Libera lista de publicações*/
        s = p->publicacoes;
        while(s != NULL) {
            temp_pub = s;
            s = s->prox;
            free(temp_pub);
        }
        /*Libera usuário*/
        temp_usuario = p;
        p = p->prox;
        free(temp_usuario);
    }

}
/*Escreve as publicações no arquivo de saída da mais recente para a mais velha*/
void relatorio(Publicacao *lista_publicacoes, FILE *arq_feed, Usuario *lista_usuarios, FILE *arq_recomendacoes, FILE *arq_relatorio) {
    Publicacao *p;
    Publicacao *q;
    Usuario *u;
    int contador_amigos, contador_publicacoes;
    Amigo *r;
    Publicacao *s;
    Usuario *temp;
    int max_nome, max_cidade; /* tamanhos mínimos do cabeçalho*/
    int total_linha; /*tamanho total da linha para o relatório*/
    int i;

    q=NULL;
    p = lista_publicacoes;
    u = lista_usuarios;
    temp = lista_usuarios;

    /*calcular para nao estourar linha da tabela*/
    max_nome = 20; /*tamanho fixo para o nome*/
    max_cidade = 15; /*tamanho fixo para a cidade*/
    while (temp != NULL) {
        if (strlen(temp->nome) > max_nome) /*caso para nao estourar a tabela*/
            max_nome = strlen(temp->nome);
        if (strlen(temp->cidade) > max_cidade) /*caso para nao estourar a tabela*/
            max_cidade = strlen(temp->cidade);
        temp = temp->prox;
    }
    total_linha = max_nome + max_cidade + 52;
    
    /*no arq_relatorio ira mostrar todos os usuarios cadastrados e suas informações: idade, cidade e tambem o numero de amigos e de publicações cada um fez, e será mostrada em formato de uma tebela*/
    fprintf(arq_relatorio, "RELATÓRIO DE USUÁRIOS\n");
    if(u == NULL) {
        fprintf(arq_relatorio, "============================================\n");
        fprintf(arq_relatorio, "Nenhum usuário cadastrado.\n");
        fprintf(arq_relatorio, "============================================\n");
    }
    else
    {
        for (i = 0; i < total_linha; i++) 
            fprintf(arq_relatorio, "-");
        fprintf(arq_relatorio, "\n");
        
        /* Cabeçalho ampliado com ID, Idade e Cidade */
        fprintf(arq_relatorio, "| %-*s | %-6s | %-7s | %-*s | %-7s | %-12s |\n",
            max_nome, "Nome",
            "ID",
            "Idade",
            max_cidade, "Cidade",
            "N Amigos",
            "N Publicações");
        for (i = 0; i < total_linha; i++) 
            fprintf(arq_relatorio, "-");
        fprintf(arq_relatorio, "\n");
        while(u != NULL) {
            /*conta numero de amigos*/
            contador_amigos = 0;
            r = u->amigos;
            while(r != NULL) {
                contador_amigos++;
                r = r->prox;
            }
            /*conta numero de publicações*/
            contador_publicacoes = 0;
            s = u->publicacoes;
            while(s != NULL) {
                contador_publicacoes++;
                s = s->prox;
            }
            /* imprime também ID, idade e cidade */
            fprintf(arq_relatorio, "| %-*s | %-6d | %-2d anos | %-*s | %-7d  | %-12d  |\n",
                max_nome, u->nome,
                u->id,
                u->idade,
                max_cidade, u->cidade,
                contador_amigos,
                contador_publicacoes);
            u = u->prox;
        }
        for (i = 0; i < total_linha; i++) 
            fprintf(arq_relatorio, "-");
        fprintf(arq_relatorio, "\n");
    }

    
    /*no arq_recomendacoes ira mostrar as recomendações de amigos para todos os usuarios*/
    recomedacoes_geral(lista_usuarios, arq_recomendacoes);
    
    /*no arq_feed ira mostrar todas as publicações feitas na rede social, da mais recente para a mais velha*/
    if(p == NULL) {
        fprintf(arq_feed, "FEED GERAL\n============================================\n");
        fprintf(arq_feed, "Nenhuma publicação realizada.\n============================================\n");
    }
    else{
        fprintf(arq_feed, "FEED GERAL\n============================================\n");
        /*mostrar em ordem do mais recente para a mensagem mais velha, porem, se tiver mensagens seguidas da mesma pessoa, não ira mostrar
        o nome e o id da pessoa de novo, mostrara só o post*/
        while(p != NULL) {
            if (q==NULL) /*primeira publicação*/
            {
                fprintf(arq_feed, "Usuario: %s (ID: %d)\nPost:\n\t%s", p->nome, p->id, p->publicacao);
                
            }
            else{
                if (p->id==q->id) /*mesma pessoa que postou a anterior*/
                {
                    fprintf(arq_feed, "Post:\n\t%s", p->publicacao);
                   
                }
                else /*pessoa diferente*/
                {
                    fprintf(arq_feed, "============================================\nUsuario: %s (ID: %d)\nPost:\n\t%s", p->nome, p->id, p->publicacao);
                    
                }
            }
            
            q=p;
            p = p->prox;
        }
        fprintf(arq_feed, "____________________________________________\n");
    }
}

/*procedimento para remover o amigo de suas amizades*/
void remove_amigo(Usuario **lista_usuarios, FILE *arq) {
    int id, id_amigo;
    Usuario *p, *q;
    Amigo *r, *ant=NULL;
    printf("Digite o ID do usuário que deseja remover um amigo: ");
    fscanf(arq, "%d", &id);
    printf("%d\n", id);
    printf("Digite o ID do amigo a ser removido: ");
    fscanf(arq, "%d", &id_amigo);
    printf("%d\n", id_amigo);
    p = busca_usuario(*lista_usuarios, id);
    q = busca_usuario(*lista_usuarios, id_amigo);
    if (p == NULL) {
        printf("Usuário com ID %d não encontrado.\n", id);
       
    }
    else
    {    
        if (q == NULL) {
            printf("Usuário com ID %d não encontrado.\n", id_amigo);
        }
        else{
            /*remove amigo de p*/
            r = p->amigos;
            ant = NULL;
            while (r != NULL && r->id != id_amigo) {
                ant = r;
                r = r->prox;
            }
            if (r == NULL) {
                printf("%s e %s não são amigos.\n", p->nome, q->nome);
            } else {
                if (ant == NULL) {
                    p->amigos = r->prox; /*remover o primeiro amigo da lista*/
                } else {
                    ant->prox = r->prox; /*remover amigo do meio ou fim da lista*/
                }
                free(r);
                printf("Amizade removida entre %s e %s.\n", p->nome, q->nome);
            }
            /*remove amigo de q*/
            r = q->amigos;
            ant = NULL;
            while (r != NULL && r->id != id) {
                ant = r;
                r = r->prox;
            }
            if (r != NULL) {
                if (ant == NULL) {
                    q->amigos = r->prox; /*remover o primeiro amigo da lista*/
                } else {
                    ant->prox = r->prox; /*remover amigo do meio ou fim da lista*/
                }
                free(r);
            }
        }
    }
    
}
/*procedimento para ver todos as publicações do Usuario + publicaçoes dos seus amigos*/
void feed_usuario(Usuario *lista_usuarios, Publicacao *lista_publicacoes, FILE *arq) {

    int id;
    Usuario *p;
    Publicacao *pub_atual;
    Amigo *amigo_p;
    int encontrou;
    printf("Digite o ID do usuário para ver o feed: ");
    fscanf(arq, "%d", &id);
    printf("%d\n", id);
    p = busca_usuario(lista_usuarios, id);
    if (p == NULL) {
        printf("\nUsuário com ID %d não encontrado.\n", id);
    } else {
        printf("\nFeed de %s (ID: %d):\n", p->nome, p->id);
        pub_atual = lista_publicacoes;
        encontrou = 0;  
        while (pub_atual != NULL) {
            /*verifica se a publicação é do próprio usuário ou de um amigo*/
            if (pub_atual->id == p->id) {
                printf("\t%s (ID: %d) publicou: %s\n", pub_atual->nome, pub_atual->id, pub_atual->publicacao);
                encontrou = 1;
            } else {
                /*verifica se é de um amigo*/
                amigo_p = p->amigos;
                while (amigo_p != NULL) {
                    if (amigo_p->id == pub_atual->id) {
                        printf("\t%s (ID: %d) publicou: %s\n", pub_atual->nome, pub_atual->id, pub_atual->publicacao);
                        encontrou = 1;
                        amigo_p = NULL; /*sai do while*/
                    }
                    else {
                        /*continua procurando*/
                        amigo_p = amigo_p->prox;
                    }
                }
            }
            pub_atual = pub_atual->prox;
        }
        if (encontrou==0) {
            printf("\tNenhuma publicação disponível no feed.\n");
        }
    }
}
/*procedimento para atualizar usuario*/
void atualiza(Usuario **lista_usuarios, FILE *arq) {
    int id, idade;
    char nome[50], cidade[50];
    Usuario *p;
    printf("Digite o ID do usuário a ser atualizado: ");
    fscanf(arq, "%d", &id);
    printf("%d\n", id);
    p = busca_usuario(*lista_usuarios, id);
    if (p == NULL) {
        printf("Usuário com ID %d não encontrado.\n", id);
        fscanf(arq, "%*[^\n]"); /* Limpa a linha restante */
    } else {
        printf("Digite o novo nome: ");
        fscanf(arq, "%49s", nome);
        printf("%s\n", nome);
        printf("Digite a nova idade: ");
        fscanf(arq, "%d", &idade);
        printf("%d\n", idade);
        printf("Digite a nova cidade: ");
        fscanf(arq, "%49s", cidade);
        printf("%s\n", cidade);
        
        strcpy(p->nome, nome);
        p->idade = idade;
        strcpy(p->cidade, cidade);
        
        printf("Usuário atualizado com sucesso.\n");
    }

}