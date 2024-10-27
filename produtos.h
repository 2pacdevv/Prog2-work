#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct produto{
int id;
char nome[60];
float price;
int qtd;
char fornecedor[60];
} produto;

void Cadastrar_produto();
int gerar_id_unico_produto();