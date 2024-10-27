#include "clientes.h"
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Função para gerar um ID único de produto
int gerar_id_unico_produto() {
  static int id = 0; // Variável estática para incrementar ID
  return id;       // Retorna o ID incrementado
}

void Cadastrar_produto() {
  produto novo_produto;
  int produto_existe = 0;
  FILE *arquivo = fopen("produtos.txt", "a+");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  printf("Digite o nome do produto: ");
  scanf(" %[^\n]", novo_produto.nome);
  printf("Digite o preço do produto: ");
  scanf("%f", &novo_produto.price);
  printf("Digite a quantidade do produto: ");
  scanf("%d", &novo_produto.qtd);
  printf("Digite o fornecedor do produto: ");
  scanf(" %[^\n]", novo_produto.fornecedor);

  rewind(arquivo);
  produto produto_atual;

  // Verifica se o produto já existe
  while (fscanf(arquivo,
                "ID: %d\nNome: %[^\n]\nPreço: %f\nQuantidade: %d\nFornecedor: "
                "%[^\n]\n\n",
                &produto_atual.id, produto_atual.nome, &produto_atual.price,
                &produto_atual.qtd, produto_atual.fornecedor) == 5) {
    if (strcmp(produto_atual.nome, novo_produto.nome) == 0) {
      produto_existe = 1;
      break;
    }
  }
  fclose(arquivo);

  if (produto_existe) {
    printf("Produto já cadastrado.\n");
  } else {
    // Gera ID único para o produto
    novo_produto.id = gerar_id_unico_produto();

    // Reabre o arquivo em modo append
    arquivo = fopen("produtos.txt", "a+");
    if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo!\n");
      return;
    }

    // Grava as informações do novo produto no arquivo
    fprintf(arquivo,
            "ID: %d\nNome: %s\nPreço: %.2f\nQuantidade: %d\nFornecedor: %s\n\n",
            novo_produto.id, novo_produto.nome, novo_produto.price,
            novo_produto.qtd, novo_produto.fornecedor);
    printf("Produto cadastrado com sucesso! ID do produto: %d\n",
           novo_produto.id);
    fclose(arquivo);
  }
}
