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
//banza essa função é a de atender o cliente, oberva ela
void Cadastrar_cliente() {
    struct cliente *novo_cliente = malloc(sizeof(struct cliente));
    float divida_total = 0.0;
    int cliente_existe = 0;

    FILE *arquivo = fopen("clientes.txt", "a+");  
    //eu botei um verificador banzas, ele verifica se o arquivo abriu
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        free(novo_cliente);
        return;
    }

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novo_cliente->nome);
    printf("Digite a dívida do cliente: ");
    scanf("%f", &novo_cliente->divida);
//esse comando rewind permite que o leitor volte desde o começo banza, ai lê tudo de novo e adciona o valor a mais da divida que o usuario gastou
    rewind(arquivo);
    
    struct cliente cliente_atual;
    //nessa parte aqui marcelao, o code ta verificando se o cliente existe, caso ele exista, a variavel cliente existe diz que ele existe
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
        if (strcmp(cliente_atual.nome, novo_cliente->nome) == 0) {
            divida_total = cliente_atual.divida + novo_cliente->divida;
            novo_cliente->id = cliente_atual.id;  
            cliente_existe = 1;
            break;  
        }
    }
    fclose(arquivo);
//aqui ele adciona o valor a divida total do cliente
    if (cliente_existe) {
        printf("Cliente já cadastrado. Atualizando a dívida...\n");
        novo_cliente->divida = divida_total;  
    } else { //caso ele n exista, ele abre o arquivo novamente
        arquivo = fopen("clientes.txt", "a+");  
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            free(novo_cliente);
            return;
        }
        //esse srand vc ja deve ter visto, ele serve pra inicializar a geração de numeros aleatorios, como vamo chama a gerar id unico, é necessario
        srand(time(NULL)); 
        novo_cliente->id = gerar_id_unico(arquivo);
        printf("Digite o número do cliente: ");
        //se o cliente n existir, o usuario precisa adcionar o numero de telefone desse cliente
        scanf("%s", novo_cliente->numero);
        fclose(arquivo);
    }
    //aqui ele abre um arquivo temporario e verifica se ele conseguiu tambem, eu criei esse arquivo temporario porque era necessario pra ele manda pro arquivo real
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

    printf("Divida do cliente atualizada com sucesso! ID do cliente: %d\n", novo_cliente->id);

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


void alterar_dados_cliente(int id_cliente) {
    struct cliente cliente_atual;
    FILE *arquivo = fopen("clientes.txt", "r");
    FILE *arquivo_temp = fopen("clientes_temp.txt", "w");
    int encontrado = 0;

    if (arquivo == NULL || arquivo_temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    // Lê todos os clientes e altera os dados do cliente desejado
    while (fscanf(arquivo, "ID: %d\nNome: %[^\n]\nDívida: %f\nNúmero: %s\n\n",
                  &cliente_atual.id, cliente_atual.nome, &cliente_atual.divida, cliente_atual.numero) == 4) {
        if (cliente_atual.id == id_cliente) {
            encontrado = 1;
            printf("Cliente encontrado. O que você deseja alterar?\n");
            printf("1. Nome\n");
            printf("2. Número\n");
            printf("Escolha uma opção: ");
            
            int opcao;
            scanf("%d", &opcao);
            getchar(); // Limpa o buffer do newline

            if (opcao == 1) {
                // Ler novo nome
                printf("Novo Nome: ");
                fgets(cliente_atual.nome, sizeof(cliente_atual.nome), stdin);
                cliente_atual.nome[strcspn(cliente_atual.nome, "\n")] = 0; // Remove a nova linha
            } else if (opcao == 2) {
                // Ler novo número
                printf("Novo Número: ");
                fgets(cliente_atual.numero, sizeof(cliente_atual.numero), stdin);
                cliente_atual.numero[strcspn(cliente_atual.numero, "\n")] = 0; // Remove a nova linha
            } else {
                printf("Opção inválida!\n");
            }
        }

        // Escreve os dados do cliente no arquivo temporário
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
    
    // Tenta remover o arquivo original e renomear o arquivo temporário
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

            int escolha2 = -1;
            while (escolha2 != 0) {
                printf("Digite o que quer fazer: \n");
                printf("0 - Voltar\n");
                printf("1 - Avançar\n");
                printf("9 - Sair\n");
                printf("2 - Selecionar Cliente\n");
                scanf("%d", &opcao);
                limpar_buffer(); 

                if (opcao == 9) { 
                    fclose(arquivo); // Fechar arquivo antes de sair
                    return;
                } else if (opcao == 0) {
                    fclose(arquivo);
                    return; // Voltar da função
                } else if (opcao == 1) {
                    break; // Avançar para o próximo cliente
                } else if (opcao == 2) {
                    printf("Menu do cliente %s\n", cliente_atual.nome);
                    printf("1 - Alterar dado\n");
                    printf("2 - Dar Baixa na Divida\n");
                    scanf("%d", &escolha2);
                    if (escolha2 == 1) {
                        alterar_dados_cliente(cliente_atual.id); // Chama a função de alteração
                    }
                    // Aqui você pode implementar a funcionalidade de dar baixa na dívida se necessário
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
