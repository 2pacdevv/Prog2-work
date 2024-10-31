#include "clientes.h"
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
int gerar_id_unico(FILE *arquivo) {
  int id;
  int id_existe;
  do {
    id = rand() % 90000 + 10000;

    id_existe = 0;

    rewind(arquivo);
    struct cliente cliente_atual;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                  cliente_atual.numero) == 4) {
      if (cliente_atual.id == id) {
        id_existe = 1;
        break;
      }
    }
  } while (id_existe);

  return id;
}

void Cadastrar_cliente() {
  struct cliente *novo_cliente = malloc(sizeof(struct cliente));
  float divida_total = 0.0;
  int cliente_existe = 0;

  FILE *arquivo = fopen("clientes.txt", "a+");
  // eu botei um verificador banzas, ele verifica se o arquivo abriu
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    free(novo_cliente);
    return;
  }

  printf("Digite o nome do cliente: ");
  scanf(" %[^\n]", novo_cliente->nome);
  printf("Digite a dívida do cliente: ");
  scanf("%f", &novo_cliente->divida);
  // esse comando rewind permite que o leitor volte desde o começo banza, ai lê
  // tudo de novo e adciona o valor a mais da divida que o usuario gastou
  rewind(arquivo);

  struct cliente cliente_atual;
  // nessa parte aqui marcelao, o code ta verificando se o cliente existe, caso
  // ele exista, a variavel cliente existe diz que ele existe
  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                cliente_atual.numero) == 4) {
    if (strcmp(cliente_atual.nome, novo_cliente->nome) == 0) {
      divida_total = cliente_atual.divida + novo_cliente->divida;
      novo_cliente->id = cliente_atual.id;
      cliente_existe = 1;
      break;
    }
  }
  fclose(arquivo);
  // aqui ele adciona o valor a divida total do cliente
  if (cliente_existe) {
    printf("Cliente já cadastrado. Atualizando a dívida...\n");
    novo_cliente->divida = divida_total;
  } else { // caso ele n exista, ele abre o arquivo novamente
    arquivo = fopen("clientes.txt", "a+");
    if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo!\n");
      free(novo_cliente);
      return;
    }
    // esse srand vc ja deve ter visto, ele serve pra inicializar a geração de
    // numeros aleatorios, como vamo chama a gerar id unico, é necessario
    srand(time(NULL));
    novo_cliente->id = gerar_id_unico(arquivo);
    printf("Digite o número do cliente: ");
    // se o cliente n existir, o usuario precisa adcionar o numero de telefone
    // desse cliente
    scanf("%s", novo_cliente->numero);
    fclose(arquivo);
  }
  // aqui ele abre um arquivo temporario e verifica se ele conseguiu tambem, eu
  // criei esse arquivo temporario porque era necessario pra ele manda pro
  // arquivo real
  FILE *arquivo_temp = fopen("clientes_temp.txt", "w");
  if (arquivo_temp == NULL) {
    printf("Erro ao abrir o arquivo temporário para escrita!\n");
    free(novo_cliente);
    return;
  }

  arquivo = fopen("clientes.txt", "r");
  if (arquivo != NULL) {
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                  cliente_atual.numero) == 4) {
      fprintf(arquivo_temp, "ID: %d\n", cliente_atual.id);
      fprintf(arquivo_temp, "Nome: %s\n", cliente_atual.nome);
      fprintf(arquivo_temp, "Dívida: %.2f\n",
              (strcmp(cliente_atual.nome, novo_cliente->nome) == 0)
                  ? novo_cliente->divida
                  : cliente_atual.divida);
      fprintf(arquivo_temp, "Número: %s\n\n", cliente_atual.numero);
    }
    fclose(arquivo);
  } else {
    printf("Erro ao abrir o arquivo para leitura!\n");
    fclose(arquivo_temp);
    free(novo_cliente);
    return;
  }

  if (!cliente_existe) {
    fprintf(arquivo_temp, "ID: %d\n", novo_cliente->id);
    fprintf(arquivo_temp, "Nome: %s\n", novo_cliente->nome);
    fprintf(arquivo_temp, "Dívida: %.2f\n", novo_cliente->divida);
    fprintf(arquivo_temp, "Número: %s\n\n", novo_cliente->numero);
  }

  fclose(arquivo_temp);
  remove("clientes.txt");
  rename("clientes_temp.txt", "clientes.txt");

  printf("Divida do cliente atualizada com sucesso! ID do cliente: %d\n",
         novo_cliente->id);

  free(novo_cliente);
}



int tela_inicial() {
  int opcao;

  printf("-----Menu Principal-----\n");
  printf("Digite a funcionalidade que você deseja: \n");
  printf("0 - Sair\n");
  printf("1 - Atender Cliente\n");
  printf("2 - Verificar cliente\n");
  printf("3 - Cadastrar Produto\n");
  printf("4 - Alterar estoque\n");
  printf("5 - Visualizar estoque\n");
  printf("6 - Catálogo\n");
  printf("7 - Alterar Catálogo\n");
  printf("8 - Emitir Relatório de vendas do dia\n");

  printf("Opção: ");
  scanf("%d", &opcao);

  switch (opcao) {
    case 0:
        break;
    case 1:
      Cadastrar_cliente();
      return tela_inicial(); 
      break;
    case 2:
      verificar_cliente();
      return tela_inicial(); 
      break;
    case 3:
      Cadastrar_produto();
      return tela_inicial(); 
      break;
    case 4:
      alterar_estoque();
      return tela_inicial(); 
      break;
    case 5:
      visualizar_estoque();
      return tela_inicial(); 
      break;
    case 6:
      printf("Alterando catálogo...\n");
      return tela_inicial(); 
      break;
    case 7:
      printf("Emitindo relatório de vendas...\n");
      return tela_inicial(); 
      break;
    default:
      printf("Opção inválida. Tente novamente.\n");
     return tela_inicial(); 
  }

}


void alterar_dados_cliente(int id_cliente) {
  struct cliente cliente_atual;
  FILE *arquivo = fopen("clientes.txt", "r");
  FILE *arquivo_temp = fopen("clientes_temp.txt", "w");
  int encontrado = 0;

  if (arquivo == NULL || arquivo_temp == NULL) {
    printf("Erro ao abrir os arquivos!\n");
    return;
  }

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                cliente_atual.numero) == 4) {
    if (cliente_atual.id == id_cliente) {
      encontrado = 1;
      printf("Cliente encontrado. O que você deseja alterar?\n");
      printf("1. Nome\n");
      printf("2. Número\n");
      printf("Escolha uma opção: ");

      int opcao;
      scanf("%d", &opcao);
      getchar();

      if (opcao == 1) {
        printf("Novo Nome: ");
        fgets(cliente_atual.nome, sizeof(cliente_atual.nome), stdin);
        cliente_atual.nome[strcspn(cliente_atual.nome, "\n")] = 0;
      } else if (opcao == 2) {
        printf("Novo Número: ");
        fgets(cliente_atual.numero, sizeof(cliente_atual.numero), stdin);
        cliente_atual.numero[strcspn(cliente_atual.numero, "\n")] = 0;
      } else {
        printf("Opção inválida!\n");
      }
    }

    fprintf(arquivo_temp, "ID: %d\n", cliente_atual.id);
    fprintf(arquivo_temp, "Nome: %s\n", cliente_atual.nome);
    fprintf(arquivo_temp, "Dívida: %.2f\n", cliente_atual.divida);
    fprintf(arquivo_temp, "Número: %s\n\n", cliente_atual.numero);
  }

  if (!encontrado) {
    printf("Cliente com ID %d não encontrado.\n", id_cliente);
  }

  fclose(arquivo);
  fclose(arquivo_temp);

  if (remove("clientes.txt") != 0) {
    perror("Erro ao excluir o arquivo clientes.txt");
  } else {
    if (rename("clientes_temp.txt", "clientes.txt") != 0) {
      perror("Erro ao renomear o arquivo temporário");
    } else {
      printf("Dados do cliente alterados com sucesso!\n");
    }
  }
}

void limpar_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void verificar_cliente() {
  char nome_procurado[60];
  struct cliente cliente_atual;
  int encontrou = 0;
  int opcao = -1;

  FILE *arquivo = fopen("clientes.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  limpar_buffer();

  printf("Digite o nome do cliente que deseja procurar: ");
  fgets(nome_procurado, 60, stdin);
  nome_procurado[strcspn(nome_procurado, "\n")] = 0;

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                cliente_atual.numero) == 4) {
    if (strstr(cliente_atual.nome, nome_procurado) != NULL) {
      encontrou = 1;
      printf("\nCliente encontrado:\n");
      printf("ID: %d\nNome: %s\nDívida: %.2f\nNúmero: %s\n", cliente_atual.id,
             cliente_atual.nome, cliente_atual.divida, cliente_atual.numero);

      int escolha2 = -1;
      while (escolha2 != 0) {
        printf("Digite o que quer fazer: \n");
        printf("1 - Avançar\n");
        printf("9 - Sair\n");
        printf("2 - Selecionar Cliente\n");
        scanf("%d", &opcao);
        limpar_buffer();

        if (opcao == 9) {
          fclose(arquivo);
          return;
        } 
        else if (opcao == 1) {
          break;
        } else if (opcao == 2) {
          printf("Menu do cliente %s\n", cliente_atual.nome);
          printf("1 - Alterar dado\n");
          printf("2 - Dar Baixa na Divida\n");
          printf("3 - Deletar Cliente\n");
          printf("0 - Sair\n");
          scanf("%d", &escolha2);
          if (escolha2 == 1) {
            alterar_dados_cliente(cliente_atual.id);
          } else if (escolha2 == 2) {
            dar_baixa_na_divida(cliente_atual.id);
          } else if (escolha2 == 3) {
            printf("Você tem certeza que deseja excluir o cliente %s?\n",
                   cliente_atual.nome);
            printf("1 - Sim\n");
            printf("2 - Não\n");
            int *confirmar = malloc(sizeof(int));
            scanf("%d", confirmar);
            if (confirmar[0] == 1) {
              excluir_cliente(cliente_atual.id);
            } else {
              return;
            }
          }

        } else {
          printf("Opção inválida. Tente novamente.\n");
        }
      }
    }
  }

  if (!encontrou) {
    printf("Cliente com nome '%s' não encontrado.\n", nome_procurado);
  }

  fclose(arquivo);
}

void dar_baixa_na_divida(int id_cliente) {
  struct cliente cliente_atual;
  FILE *arquivo = fopen("clientes.txt", "r");
  FILE *arquivo_temp = fopen("clientes_temp.txt", "w");
  int encontrado = 0;

  if (arquivo == NULL || arquivo_temp == NULL) {
    printf("Erro ao abrir os arquivos!\n");
    return;
  }

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                cliente_atual.numero) == 4) {
    if (cliente_atual.id == id_cliente) {
      encontrado = 1;
      printf("Cliente encontrado. Digite o valor que foi pago: \n");

      float valor_pago;
      scanf("%f", &valor_pago);

      if (valor_pago < 0) {
        printf("Valor pago inválido. Deve ser maior ou igual a 0.\n");
        fclose(arquivo);
        fclose(arquivo_temp);
        return;
      }

      cliente_atual.divida -= valor_pago;

      if (cliente_atual.divida < 0) {
        cliente_atual.divida = 0;
      }
    }

    // Escreve os dados no arquivo temporário
    fprintf(arquivo_temp, "ID: %d\n", cliente_atual.id);
    fprintf(arquivo_temp, "Nome: %s\n", cliente_atual.nome);
    fprintf(arquivo_temp, "Dívida: %.2f\n", cliente_atual.divida);
    fprintf(arquivo_temp, "Número: %s\n\n", cliente_atual.numero);
  }

  fclose(arquivo);
  fclose(arquivo_temp);

  if (!encontrado) {
    printf("Cliente com ID %d não encontrado.\n", id_cliente);
  } else {
    printf("Dívida atualizada. Dívida atual do cliente: %.2f\n",
           cliente_atual.divida);
  }

  if (remove("clientes.txt") != 0) {
    perror("Erro ao remover o arquivo clientes.txt");
  } else {
    if (rename("clientes_temp.txt", "clientes.txt") != 0) {
      perror("Erro ao renomear o arquivo temporário");
    } else {
      printf("Dados do cliente atualizados com sucesso!\n");
    }
  }
}

void excluir_cliente(int id_cliente) {
  struct cliente cliente_atual;
  int encontrado = 0;

  FILE *arquivo = fopen("clientes.txt", "r");
  FILE *arquivo_temp = fopen("clientes_temp.txt", "w");

  if (arquivo == NULL || arquivo_temp == NULL) {
    printf("Erro ao abrir os arquivos!\n");
    return;
  }

  while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida,
                cliente_atual.numero) == 4) {

    if (cliente_atual.id != id_cliente) {
      fprintf(arquivo_temp, "ID: %d\n", cliente_atual.id);
      fprintf(arquivo_temp, "Nome: %s\n", cliente_atual.nome);
      fprintf(arquivo_temp, "Dívida: %.2f\n", cliente_atual.divida);
      fprintf(arquivo_temp, "Número: %s\n\n", cliente_atual.numero);
    } else {
      encontrado = 1;
    }
  }

  fclose(arquivo);
  fclose(arquivo_temp);

  if (encontrado) {
    if (remove("clientes.txt") != 0) {
      perror("Erro ao remover o arquivo clientes.txt");
    } else if (rename("clientes_temp.txt", "clientes.txt") != 0) {
      perror("Erro ao renomear o arquivo temporário");
    } else {
      printf("Cliente com ID %d excluído com sucesso!\n", id_cliente);
      return;
    }
  } else {
    printf("Cliente com ID %d não encontrado.\n", id_cliente);
    remove("clientes_temp.txt");
  }
}
