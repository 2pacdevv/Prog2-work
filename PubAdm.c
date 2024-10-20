#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct cliente {
    int id;  
    char nome[30];
    float divida;
    char numero[11];
};

int escolha = -1;

int gerar_id_unico(FILE *arquivo) {
    int id;
    int id_existe;
    do {
        id = rand() % 90000 + 10000; 
        
        id_existe = 0;

        rewind(arquivo);
        struct cliente cliente_atual;
        while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                      &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
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
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        free(novo_cliente);
        return;
    }
    


    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novo_cliente->nome);
    printf("Digite a dívida do cliente: ");
    scanf("%f", &novo_cliente->divida);

    rewind(arquivo);
    struct cliente cliente_atual;
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
        if (strcmp(cliente_atual.nome, novo_cliente->nome) == 0) {
            divida_total = cliente_atual.divida + novo_cliente->divida;
            cliente_existe = 1;
            break;  
        }
    }
    fclose(arquivo);

    if (cliente_existe) {
        printf("Cliente já cadastrado. Atualizando a dívida...\n");
        novo_cliente->divida = divida_total;
    } else {
        printf("Digite o número do cliente: ");
        scanf("%s", novo_cliente->numero);
        srand(time(NULL)); 
         novo_cliente->id = gerar_id_unico(arquivo);
    }

    FILE *arquivo_temp = fopen("clientes_temp.txt", "w");
    if (arquivo_temp == NULL) {
        printf("Erro ao abrir o arquivo temporário para escrita!\n");
        free(novo_cliente);
        return;
    }

    arquivo = fopen("clientes.txt", "r");
    if (arquivo != NULL) {
        while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                      &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
            fprintf(arquivo_temp, "ID: %d\n", cliente_atual.id);
            fprintf(arquivo_temp, "Nome: %s\n", cliente_atual.nome);
            fprintf(arquivo_temp, "Dívida: %.2f\n", (strcmp(cliente_atual.nome, novo_cliente->nome) == 0) ? novo_cliente->divida : cliente_atual.divida);
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

    printf("Cliente cadastrado/atualizado com sucesso! ID: %d\n", novo_cliente->id);

    free(novo_cliente);
}

void tela_inicial() {
    printf("Digite a funcionalidade que você deseja: \n");
    printf("1 - Atender Cliente\n");
    printf("2 - Verificar cliente\n");
    printf("3 - Cadastrar Produto\n");
    printf("4 - Alterar estoque\n");
    printf("5 - Emitir Relatório de vendas do dia\n");
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
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
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
        if (strstr(cliente_atual.nome, nome_procurado) != NULL) {
            encontrou = 1;
            printf("\nCliente encontrado:\n");
            printf("ID: %d\nNome: %s\nDívida: %.2f\nNúmero: %s\n", 
                   cliente_atual.id, cliente_atual.nome, cliente_atual.divida, cliente_atual.numero);

            printf("Digite 1 para avançar para o próximo cliente, 0 para voltar ao cliente anterior, ou 9 para sair: ");
            scanf("%d", &opcao);
            limpar_buffer(); 

            if (opcao == 9) { 
                break;
            } else if (opcao == 0) {
                rewind(arquivo);
                encontrou = 0;
            }
        }
    }

    if (!encontrou) {
        printf("Cliente com nome '%s' não encontrado.\n", nome_procurado);
    }

    fclose(arquivo);
}









//função main banzas, da um naipe ae, vou tentar deixar ela o mais limpa possivel
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
    }
    return 0;
}
