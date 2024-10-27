#include "clientes.h"
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int escolha = -1;
// função main banzas, da um naipe ae, vou tentar deixar ela o mais limpa
// possivel
int main() {
  printf("Bem-vindo ao aplicativo de administração de Pub\n");

  while (escolha != 0) {
    tela_inicial();
    scanf("%d", &escolha);
    if (escolha == 0)
      break;
    if (escolha == 1) {
      Cadastrar_cliente();
    }
    if (escolha == 2) {
      verificar_cliente();
    }
    if (escolha == 3) {
      Cadastrar_produto();
    }
  }
  return 0;
}