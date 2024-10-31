#include "clientes.h"
#include "produtos.h"
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
    printf("Erro ao alocar memória!\n");
    return;
  }

  int produto_existe = 0;
  int nova_quantidade = 0;

  FILE *arquivo = fopen("produtos.txt", "r+");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    free(novo_produto);
    return;
  }

  printf("Digite o nome do produto: ");
  scanf(" %[^\n]", novo_produto->nome);
  printf("Digite a quantidade do produto: ");
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
    printf("Digite o preço pago pelo produto: ");
    scanf("%f", &novo_produto->price);
  }
  fclose(arquivo);

  FILE *arquivo_temp = fopen("produtos_temp.txt", "w");
  if (arquivo_temp == NULL) {
    printf("Erro ao abrir o arquivo temporário para escrita!\n");
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
    printf("Erro ao abrir o arquivo para leitura!\n");
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

  printf("Produto atualizado com sucesso! ID do produto: %d\n", novo_produto->id);

  free(novo_produto);
}


void alterar_estoque() {
  char nome_produto[100];
  int quantidade_alterada, alterar_preco;
  float novo_preco;

  FILE *arquivo = fopen("produtos.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  FILE *arquivo_temp = fopen("produtos_temp.txt", "w");
  if (arquivo_temp == NULL) {
    printf("Erro ao criar arquivo temporário!\n");
    fclose(arquivo);
    return;
  }

  printf("Digite o nome do produto que deseja alterar: ");
  scanf(" %[^\n]", nome_produto);

  produto produto_atual;
  int produto_encontrado = 0;

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                &produto_atual.id, produto_atual.nome, &produto_atual.price,
                &produto_atual.qtd) == 4) {

    if (strcmp(produto_atual.nome, nome_produto) == 0) {
      produto_encontrado = 1;
      printf("Produto encontrado: %s\n", produto_atual.nome);
      printf("Quantidade atual: %d\n", produto_atual.qtd);

      printf("Digite a nova quantidade: ");
      scanf("%d", &quantidade_alterada);
      produto_atual.qtd = quantidade_alterada;

      printf("Deseja alterar o preço pago pelo produto? (1 - Sim, 0 - Não): ");
      scanf("%d", &alterar_preco);
      if (alterar_preco) {
        printf("Digite o novo preço: ");
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
    printf("Estoque atualizado com sucesso!\n");
  } else {
    printf("Produto não encontrado!\n");
    remove("produtos_temp.txt");
  }
}


void visualizar_estoque() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos!\n");
        return;
    }

    produto produto_atual;
    int opcao;
    long posicao_anterior = 0;

    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\n\n",
                  &produto_atual.id, produto_atual.nome, &produto_atual.price,
                  &produto_atual.qtd) == 4) {

        printf("\nProduto ID: %d\n", produto_atual.id);
        printf("Nome: %s\n", produto_atual.nome);
        printf("Preço: %.2f\n", produto_atual.price);
        printf("Quantidade: %d\n", produto_atual.qtd);

        printf("\nDigite 1 para o próximo produto ou 0 para parar: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            continue;
        } 
     
        else {
            printf("Encerrando visualização.\n");
            break;
        }
    }

    fclose(arquivo);
    printf("\nFim da lista de produtos.\n");
}


