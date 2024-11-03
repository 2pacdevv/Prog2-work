#include "catalogo.h"
#include "clientes.h"
#include "produtos.h"
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
    printf("Erro ao abrir o arquivo de catálogo!\n");
    return;
  }

  printf("Escolha uma opção:\n");
  printf("1. Pesquisar produto pelo nome\n");
  printf("2. Visualizar todos os produtos\n");
  printf("Digite a opção desejada: ");
  scanf("%d", &opcao);
  getchar();

  if (opcao == 1) {
    printf("Digite o nome do produto que deseja pesquisar: ");
    scanf(" %[^\n]", nome_produto);

    int produto_encontrado = 0;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      if (strstr(produto_atual.nome, nome_produto) != NULL) {
        printf("Produto encontrado:\n");
        printf("ID: %d\nNome: %s\nPreço: %.2f\n", produto_atual.id,
               produto_atual.nome, produto_atual.price);

        int continuar;
        do {
          printf("Digite 1 para continuar ou 0 para sair: ");
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
      printf("Produto não encontrado no catálogo.\n");
    }
  } else if (opcao == 2) {
    printf("Visualizando todos os produtos:\n");
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      printf("ID: %d\nNome: %s\nPreço: %.2f\n", produto_atual.id,
             produto_atual.nome, produto_atual.price);

      int continuar;
      do {
        printf("Digite 1 para continuar ou 0 para sair: ");
        scanf("%d", &continuar);
        if (continuar == 0) {
          fclose(arquivo);
          return;
        }
      } while (continuar != 1);
    }
  } else {
    printf("Opção inválida. Tente novamente.\n");
  }

  fclose(arquivo);
}
// funcoes outras ai

void Alterar_Catalogo() {
  int escolha;
  printf("Escolha oque deseja fazer: \n");
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
    excluir_produto_catalogo();
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

  FILE *arquivo = fopen("catalogo.txt", "a+");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de catálogo!\n");
    return;
  }

  printf("Digite o nome do produto: ");
  scanf(" %[^\n]", novo_produto.nome);
  printf("Digite o preço do produto: ");
  scanf("%f", &novo_produto.price);

  rewind(arquivo);
  produto_do_Catalogo produto_atual;

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                &produto_atual.id, produto_atual.nome,
                &produto_atual.price) == 3) {
    if (strcmp(produto_atual.nome, novo_produto.nome) == 0) {
      produto_existe = 1;
      break;
    }
  }

  if (produto_existe) {
    printf("Produto já cadastrado.\n");
  } else {

    novo_produto.id = gerar_id_unico_catalogo(arquivo);
    fprintf(arquivo, "ID: %d\nNome: %s\nPreço: %.2f\n\n", novo_produto.id,
            novo_produto.nome, novo_produto.price);
    printf("Produto cadastrado com sucesso! ID do produto: %d\n",
           novo_produto.id);
  }

  fclose(arquivo);
}

void alterar_produto_catalogo() {
  char nome_produto[60];
  int produto_encontrado = 0;
  produto_do_Catalogo produto_atual;

  FILE *arquivo = fopen("catalogo.txt", "r+");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de catálogo!\n");
    return;
  }

  printf("Digite o nome do produto que deseja alterar: ");
  scanf(" %[^\n]", nome_produto);

  long posicao = 0;
  while (
      !produto_encontrado &&
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
    printf("Produto encontrado!\n");
    printf("Nome atual: %s\nPreço atual: %.2f\n", produto_atual.nome,
           produto_atual.price);

    int opcao;
    printf("O que você deseja alterar?\n");
    printf("1. Nome\n");
    printf("2. Preço\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite o novo nome do produto: ");
      scanf(" %[^\n]", produto_atual.nome);
      break;
    case 2:
      printf("Digite o novo preço do produto: ");
      scanf("%f", &produto_atual.price);
      break;
    default:
      printf("Opção inválida. Nenhuma alteração realizada.\n");
      fclose(arquivo);
      return;
    }

    fprintf(arquivo, "ID: %d\nNome: %s\nPreço: %.2f\n\n", produto_atual.id,
            produto_atual.nome, produto_atual.price);

    printf("Produto atualizado com sucesso!\n");
  } else {
    printf("Produto não encontrado no catálogo.\n");
  }

  fclose(arquivo);
}

void excluir_produto_catalogo() {
  char nome_produto[60];
  int produto_encontrado = 0;
  produto_do_Catalogo produto_atual;

  FILE *arquivo = fopen("catalogo.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }

  FILE *arquivo_temp = fopen("temp_catalogo.txt", "w");
  if (arquivo_temp == NULL) {
    printf("Erro ao criar o arquivo\n");
    fclose(arquivo);
    return;
  }

  printf("Digite o nome do produto que deseja excluir: ");
  scanf(" %[^\n]", nome_produto);
  printf("Tem certeza que deseja exlcuir o produto %s? ", nome_produto);
  printf("1 - Sim\n");
  printf("0 - Não\n");
  int opcao5;
  scanf("%d", &opcao5);
  if (opcao5 == 1) {
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nPreço: %f\n\n",
                  &produto_atual.id, produto_atual.nome,
                  &produto_atual.price) == 3) {
      if (strcmp(produto_atual.nome, nome_produto) == 0) {
        produto_encontrado = 1;
        printf("Produto '%s' encontrado e será excluído.\n",
               produto_atual.nome);
        continue;
      }
      fprintf(arquivo_temp, "ID: %d\nNome: %s\nPreço: %.2f\n\n",
              produto_atual.id, produto_atual.nome, produto_atual.price);
    }

    if (!produto_encontrado) {
      printf("Produto não encontrado no catálogo.\n");
    } else {
      printf("Produto excluído com sucesso!\n");
    }

    fclose(arquivo);
    fclose(arquivo_temp);

    remove("catalogo.txt");
    rename("temp_catalogo.txt", "catalogo.txt");
  }

  else {
    printf("Operação cancelada.\n");
  }
}
