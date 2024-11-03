#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct produto_do_Catalogo{
  int id;
  char nome[60];
  float price;
} produto_do_Catalogo;
void Alterar_Catalogo();
void cadastrar_produto_catalogo();
int gerar_id_unico_catalogo(FILE *arquivo);
void alterar_produto_catalogo();
void excluir_produto();
void visualizar_catalogo();