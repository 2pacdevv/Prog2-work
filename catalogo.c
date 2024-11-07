#include "catalogo.h"
#include "clientes.h"
#include "produtos.h"
#include "cores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void visualizar_catalogo() {
  int opcao;
  char nome_produto[60];
  produto_do_Catalogo produto_atual;

  FILE *arquivo = fopen("catalogo.txt", "r");
  if (arquivo == NULL) {
    printf(VERMELHO "Erro ao abrir o arquivo de catálogo!\n" RESET);
    return;
  }

  printf(AZUL "Escolha uma opção:\n" RESET);
  printf("1. Pesquisar produto pelo nome\n");
  printf("2. Visualizar todos os produtos\n");
  printf("Digite a opção desejada: ");
  scanf("%d", &opcao);
  getchar();

  if (opcao == 1) {
    printf(AZUL "Digite o nome do produto que deseja pesquisar: " RESET);
    scanf(" %[^\n]", nome_produto);

    int produto_encontrado = 0;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      if (strstr(produto_atual.nome, nome_produto) != NULL) {
        printf(VERDE "Produto encontrado:\n" RESET);
        printf("ID: %d\nNome: %s\nPreço: %.2f\n", produto_atual.id,
               produto_atual.nome, produto_atual.price);

        int continuar;
        do {
          printf(AZUL "Digite 1 para continuar ou 0 para sair: " RESET);
          scanf("%d", &continuar);
          if (continuar == 0) {
            fclose(arquivo);
            return;
          }
        } while (continuar != 1);
        produto_encontrado = 1;
      }
    }
    if (!produto_encontrado) {
      printf(VERMELHO "Produto não encontrado no catálogo.\n" RESET);
    }
  } else if (opcao == 2) {
    printf(AZUL "Visualizando todos os produtos:\n" RESET);
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      printf("ID: %d\nNome: %s\nPreço: %.2f\n", produto_atual.id,
             produto_atual.nome, produto_atual.price);

      int continuar;
      do {
        printf(AZUL "Digite 1 para continuar ou 0 para sair: " RESET);
        scanf("%d", &continuar);
        if (continuar == 0) {
          fclose(arquivo);
          return;
        }
      } while (continuar != 1);
    }
  } else {
    printf(VERMELHO "Opção inválida. Tente novamente.\n" RESET);
  }

  fclose(arquivo);
}

void Alterar_Catalogo() {
  int escolha;
  printf(AZUL "Escolha o que deseja fazer: \n" RESET);
  printf("1 - Cadastrar Produto da loja\n");
  printf("2 - Alterar Produto da loja\n");
  printf("3 - Excluir Produto da loja\n");
  printf("0 - Sair\n");
  scanf("%d", &escolha);
  if (escolha == 1) {
    cadastrar_produto_catalogo();
  } else if (escolha == 2) {
    alterar_produto_catalogo();
  } else if (escolha == 3) {
    excluir_produto();
  } else {
    printf(VERMELHO "Opção inválida\n" RESET);
  }
}

int gerar_id_unico_catalogo(FILE *arquivo) {
  int id, id_existe;
  produto_do_Catalogo produto_atual;

  do {
    id = rand() % 90000 + 10000;
    id_existe = 0;

    rewind(arquivo);
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      if (produto_atual.id == id) {
        id_existe = 1;
        break;
      }
    }
  } while (id_existe);

  return id;
}

void cadastrar_produto_catalogo() {
    produto_do_Catalogo novo_produto;
    int produto_existe = 0;

    printf(AZUL "Digite o nome do produto: " RESET);
    scanf(" %[^\n]", novo_produto.nome);

    FILE *arquivo = fopen("catalogo.txt", "r");
    if (arquivo == NULL) {
        printf(VERMELHO "Erro ao abrir o arquivo de catálogo!\n" RESET);
        return;
    }

    produto_do_Catalogo produto_atual;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
        if (strcmp(produto_atual.nome, novo_produto.nome) == 0) {
            produto_existe = 1;
            break;
        }
    }
    fclose(arquivo);

    if (produto_existe) {
        printf(VERMELHO "Produto já cadastrado.\n" RESET);
        return;
    }

    printf(AZUL "Digite o preço do produto: " RESET);
    scanf("%f", &novo_produto.price);

    arquivo = fopen("catalogo.txt", "a");
    if (arquivo == NULL) {
        printf(VERMELHO "Erro ao abrir o arquivo de catálogo!\n" RESET);
        return;
    }

    novo_produto.id = gerar_id_unico_catalogo(arquivo);
    fprintf(arquivo, "ID: %d\nNome: %s\nPreço: %.2f\n\n", novo_produto.id,
            novo_produto.nome, novo_produto.price);

    printf(VERDE "Produto cadastrado com sucesso! ID do produto: %d\n" RESET, novo_produto.id);

    fclose(arquivo);
}


void alterar_produto_catalogo() {
  char nome_produto[60];
  int produto_encontrado = 0;
  produto_do_Catalogo produto_atual;

  FILE *arquivo = fopen("catalogo.txt", "r+");
  if (arquivo == NULL) {
    printf(VERMELHO "Erro ao abrir o arquivo de catálogo!\n" RESET);
    return;
  }

  printf(AZUL "Digite o nome do produto que deseja alterar: " RESET);
  scanf(" %[^\n]", nome_produto);

  long posicao = 0;
  while (!produto_encontrado &&
         (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n", &produto_atual.id,
                 produto_atual.nome, &produto_atual.price) == 3)) {

    if (strcmp(produto_atual.nome, nome_produto) == 0) {
      produto_encontrado = 1;
      fseek(arquivo, posicao, SEEK_SET);
    } else {
      posicao = ftell(arquivo);
    }
  }

  if (produto_encontrado) {
    printf(VERDE "Produto encontrado!\n" RESET);
    printf("Nome atual: %s\nPreço atual: %.2f\n", produto_atual.nome,
           produto_atual.price);

    int opcao;
    printf(AZUL "O que você deseja alterar?\n" RESET);
    printf("1. Nome\n");
    printf("2. Preço\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf(AZUL "Digite o novo nome do produto: " RESET);
      scanf(" %[^\n]", produto_atual.nome);
      break;
    case 2:
      printf(AZUL "Digite o novo preço do produto: " RESET);
      scanf("%f", &produto_atual.price);
      break;
    default:
      printf(VERMELHO "Opção inválida. Nenhuma alteração realizada.\n" RESET);
      fclose(arquivo);
      return;
    }

    fprintf(arquivo, "ID: %d\nNome: %s\nPreço: %.2f\n\n", produto_atual.id,
            produto_atual.nome, produto_atual.price);

    printf(VERDE "Produto atualizado com sucesso!\n" RESET);
  } else {
    printf(VERMELHO "Produto não encontrado no catálogo.\n" RESET);
  }

  fclose(arquivo);
}



void excluir_produto() {
    char nome_produto[60];
    produto_do_Catalogo produto_encontrado;
    FILE *arquivo = fopen("catalogo.txt", "r");
    FILE *temp = fopen("temp_catalogo.txt", "w");
    int encontrado = 0;

    if (arquivo == NULL || temp == NULL) {
        printf(VERMELHO "Erro ao abrir o arquivo de catálogo.\n" RESET);
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    printf(AZUL "Digite o nome do produto que deseja excluir: " RESET);
    scanf(" %[^\n]", nome_produto);

    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_encontrado.id, produto_encontrado.nome,
                  &produto_encontrado.price) == 3) {
        if (strcmp(produto_encontrado.nome, nome_produto) == 0) {
            encontrado = 1;
            printf(VERDE "Produto encontrado: %s (ID: %d, Preço: %.2f)\n" RESET,
                   produto_encontrado.nome, produto_encontrado.id, produto_encontrado.price);
            
            int confirmar;
            printf(AZUL "Tem certeza que deseja excluir este produto? (1 para sim, 0 para não): " RESET);
            scanf("%d", &confirmar);

            if (confirmar == 1) {
                printf(VERDE "Produto '%s' excluído com sucesso.\n" RESET, nome_produto);
                continue;
            } 
            else {
                printf(VERDE "Exclusão cancelada para o produto '%s'.\n" RESET, nome_produto);
            }
        }
        fprintf(temp, "ID: %d\nNome: %s\nPreço: %.2f\n\n", produto_encontrado.id,
                produto_encontrado.nome, produto_encontrado.price);
    }

    if (!encontrado) {
        printf(VERMELHO "Produto '%s' não encontrado no catálogo.\n" RESET, nome_produto);
    }

    fclose(arquivo);
    fclose(temp);

    remove("catalogo.txt");
    rename("temp_catalogo.txt", "catalogo.txt");
}

