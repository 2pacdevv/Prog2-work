#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int escolha = 0;
struct produto{
    char nome[30];
    float valor_de_compra;
    float valor_de_venda;
    int quantidade_no_estoque;
    char numero_do_fornecedor[]
};
void tela_incial(){
    printf("Bem vindo ao aplicativo de administração de Pub\n");
    printf("Digite a funcionalidade que você deseja: \n");
    printf("1 - Cadastrar divida do cliente\n");
    printf("2 - Verificar cliente\n");
    printf("3 - Cadastrar Produto\n");
    printf("4 - Alterar estoque\n");
    printf("5 - Emitir Relátorio de vendas do dia\n");
}
int main(){
    tela_incial();
    scanf("%d", &escolha);
}