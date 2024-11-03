#include "clientes.h"
#include "produtos.h"
#include "cores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int gerar_id_unico_produto(FILE *arquivo) {
  int id, id_existe;
  do {
    id = rand() % 90000 + 10000;
    id_existe = 0;

    rewind(arquivo);
    produto produto_atual;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                  &produto_atual.id, produto_atual.nome, &produto_atual.price,
                  &produto_atual.qtd) == 4) {
      if (produto_atual.id == id) {
        id_existe = 1;
        break;
      }
    }
  } while (id_existe);

  return id;
}

void Cadastrar_produto() {
  produto *novo_produto = malloc(sizeof(produto));
  if (novo_produto == NULL) {
    printf(VERMELHO "Erro ao alocar memória!\n" RESET);
    return;
  }

  int produto_existe = 0;
  int nova_quantidade = 0;

  FILE *arquivo = fopen("produtos.txt", "r+");
  if (arquivo == NULL) {
    printf(VERMELHO "Erro ao abrir o arquivo!\n" RESET);
    free(novo_produto);
    return;
  }

  printf(AZUL "Digite o nome do produto: " RESET);
  scanf(" %[^\n]", novo_produto->nome);
  printf(AZUL "Digite a quantidade do produto: " RESET);
  scanf("%d", &nova_quantidade);
  novo_produto->qtd = nova_quantidade;

  produto produto_atual;

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                &produto_atual.id, produto_atual.nome, &produto_atual.price,
                &produto_atual.qtd) == 4) {
    if (strcmp(produto_atual.nome, novo_produto->nome) == 0) {
      produto_existe = 1;
      novo_produto->qtd += produto_atual.qtd;
      novo_produto->id = produto_atual.id;
      novo_produto->price = produto_atual.price;
      break;
    }
  }

  if (!produto_existe) {
    novo_produto->id = gerar_id_unico_produto(arquivo);
    printf(AZUL "Digite o preço pago pelo produto: " RESET);
    scanf("%f", &novo_produto->price);
  }
  fclose(arquivo);

  FILE *arquivo_temp = fopen("produtos_temp.txt", "w");
  if (arquivo_temp == NULL) {
    printf(VERMELHO "Erro ao abrir o arquivo temporário para escrita!\n" RESET);
    free(novo_produto);
    return;
  }

  arquivo = fopen("produtos.txt", "r");
  if (arquivo != NULL) {
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                  &produto_atual.id, produto_atual.nome, &produto_atual.price,
                  &produto_atual.qtd) == 4) {
      if (strcmp(produto_atual.nome, novo_produto->nome) == 0) {
        fprintf(arquivo_temp, "ID: %d\n", novo_produto->id);
        fprintf(arquivo_temp, "Nome: %s\n", novo_produto->nome);
        fprintf(arquivo_temp, "Preço: %.2f\n", novo_produto->price);
        fprintf(arquivo_temp, "Quantidade: %d\n", novo_produto->qtd);
        fprintf(arquivo_temp, "\n");
      } else {
        fprintf(arquivo_temp, "ID: %d\n", produto_atual.id);
        fprintf(arquivo_temp, "Nome: %s\n", produto_atual.nome);
        fprintf(arquivo_temp, "Preço: %.2f\n", produto_atual.price);
        fprintf(arquivo_temp, "Quantidade: %d\n", produto_atual.qtd);
        fprintf(arquivo_temp, "\n");
      }
    }
    fclose(arquivo);
  } else {
    printf(VERMELHO "Erro ao abrir o arquivo para leitura!\n" RESET);
    fclose(arquivo_temp);
    free(novo_produto);
    return;
  }

  if (!produto_existe) {
    fprintf(arquivo_temp, "ID: %d\n", novo_produto->id);
    fprintf(arquivo_temp, "Nome: %s\n", novo_produto->nome);
    fprintf(arquivo_temp, "Preço: %.2f\n", novo_produto->price);
    fprintf(arquivo_temp, "Quantidade: %d\n", novo_produto->qtd);
    fprintf(arquivo_temp, "\n");
  }

  fclose(arquivo_temp);
  remove("produtos.txt");
  rename("produtos_temp.txt", "produtos.txt");

  printf(VERDE "Produto atualizado com sucesso! ID do produto: %d\n" RESET, novo_produto->id);

  free(novo_produto);
}

void alterar_estoque() {
  char nome_produto[100];
  int quantidade_alterada, alterar_preco;
  float novo_preco;

  FILE *arquivo = fopen("produtos.txt", "r");
  if (arquivo == NULL) {
    printf(VERMELHO "Erro ao abrir o arquivo!\n" RESET);
    return;
  }

  FILE *arquivo_temp = fopen("produtos_temp.txt", "w");
  if (arquivo_temp == NULL) {
    printf(VERMELHO "Erro ao criar arquivo temporário!\n" RESET);
    fclose(arquivo);
    return;
  }

  printf(AZUL "Digite o nome do produto que deseja alterar: " RESET);
  scanf(" %[^\n]", nome_produto);

  produto produto_atual;
  int produto_encontrado = 0;

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                &produto_atual.id, produto_atual.nome, &produto_atual.price,
                &produto_atual.qtd) == 4) {

    if (strcmp(produto_atual.nome, nome_produto) == 0) {
      produto_encontrado = 1;
      printf(AZUL "Produto encontrado: %s\n" RESET, produto_atual.nome);
      printf("Quantidade atual: %d\n", produto_atual.qtd);

      printf(AZUL "Digite a nova quantidade: " RESET);
      scanf("%d", &quantidade_alterada);
      produto_atual.qtd = quantidade_alterada;

      printf(AZUL "Deseja alterar o preço pago pelo produto? (1 - Sim, 0 - Não): " RESET);
      scanf("%d", &alterar_preco);
      if (alterar_preco) {
        printf(AZUL "Digite o novo preço: " RESET);
        scanf("%f", &novo_preco);
        produto_atual.price = novo_preco;
      }
    }

    fprintf(arquivo_temp, "ID: %d\nNome: %s\nPreço: %.2f\nQuantidade: %d\n\n",
            produto_atual.id, produto_atual.nome, produto_atual.price,
            produto_atual.qtd);
  }

  fclose(arquivo);
  fclose(arquivo_temp);

  if (produto_encontrado) {
    remove("produtos.txt");
    rename("produtos_temp.txt", "produtos.txt");
    printf(VERDE "Estoque atualizado com sucesso!\n" RESET);
  } else {
    printf(VERMELHO "Produto não encontrado!\n" RESET);
    remove("produtos_temp.txt");
  }
}

void visualizar_estoque() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf(VERMELHO "Erro ao abrir o arquivo de produtos!\n" RESET);
        return;
    }

    produto produto_atual;
    int opcao;

    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                  &produto_atual.id, produto_atual.nome, &produto_atual.price,
                  &produto_atual.qtd) == 4) {

        printf(AZUL "\nProduto ID: %d\n" RESET, produto_atual.id);
        printf("Nome: %s\n", produto_atual.nome);
        printf("Preço: %.2f\n", produto_atual.price);
        printf("Quantidade: %d\n", produto_atual.qtd);

        printf(AZUL "\nDigite 1 para o próximo produto ou 0 para parar: " RESET);
        scanf("%d", &opcao);

        if (opcao == 1) {
            continue;
        } else {
            printf(VERDE "Encerrando visualização.\n" RESET);
            break;
        }
    }

    fclose(arquivo);
    printf(VERDE "\nFim da lista de produtos.\n" RESET);
}
