#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct cliente {
  int id;
  char nome[30];
  float divida;
  char numero[12];
};
int gerar_id_unico(FILE *arquivo);
void Cadastrar_cliente();
void tela_inicial();
void alterar_dados_cliente(int id_cliente);
void limpar_buffer();
void verificar_cliente();
void dar_baixa_na_divida(int id_cliente);
int gerar_id_unico(FILE *arquivo);
void excluir_cliente(int id_cliente);