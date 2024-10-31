#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct produto{
int id;
char nome[60];
float price;
int qtd;
} produto;

void Cadastrar_produto();
int gerar_id_unico_produto();
void alterar_estoque();
void visualizar_estoque();