#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 7
#define MAX_COMPRAS 10

typedef struct {
    int codigo;
    char nome[50];
    char marca[50];
    float preco_unitario;
} Produto;

typedef struct {
    int codigo_produto;
    int quantidade;
} ItemCompra;

typedef struct {
    ItemCompra itens[MAX_PRODUTOS];
    int total_itens;
    float faturamento_bruto;
} Compra;

Produto produtos[MAX_PRODUTOS] = {
    {1, "Camiseta", "Marca Xoxo", 30},
    {2, "Calça Jeans", "Marca Rasgados", 60},
    {3, "Sapato", "Marca Adidos", 80},
    {4, "Jaqueta", "Marca Rainy Day", 100},
    {5, "Saia", "Marca Cherry", 40},
    {6, "Blusa", "Marca CariocaMan", 35},
    {7, "Salto Alto", "Marca Bicho do Pé", 50}
};

void imprimir_produtos_disponiveis() {
    printf("\nProdutos disponíveis:\n");
    for (int i = 0; i < MAX_PRODUTOS; i++) {
        printf("Código: %d | Nome: %s | Marca: %s | Preço: R$%.2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].marca, produtos[i].preco_unitario);
    }
    printf("\n");
}

float calcular_preco_item(int codigo_produto, int quantidade) {
    float preco_item = 0;
    for (int i = 0; i < MAX_PRODUTOS; i++) {
        if (produtos[i].codigo == codigo_produto) {
            if (quantidade >= 3){
                preco_item = produtos[i].preco_unitario * quantidade * 0.9;
                printf("Desconto de 10%% aplicado.\n");
            }
            else{
                preco_item = produtos[i].preco_unitario * quantidade;
            }
            break;
        }
    }
    return preco_item;
}

// Função de comparação para qsort
int comparar_vendas(const void *a, const void *b) {
    const Compra *venda1 = (const Compra *)a;
    const Compra *venda2 = (const Compra *)b;

    if (venda1->faturamento_bruto > venda2->faturamento_bruto) {
        return -1; // Retorna -1 se a venda1 tiver um valor maior
    } else if (venda1->faturamento_bruto < venda2->faturamento_bruto) {
        return 1; // Retorna 1 se a venda1 tiver um valor menor
    } else {
        return 0; // Retorna 0 se forem iguais
    }
}

void salvar_compras_txt(Compra *compras, int num_compras) {
    FILE *arquivo_compras;
    arquivo_compras = fopen("compras.dat", "w");
    if (arquivo_compras == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de compras.\n");
        return;
    }

    for (int i = 0; i < num_compras; i++) {
        fprintf(arquivo_compras, "Venda %d - Total de itens: %d | Faturamento bruto: R$%.2f\n", i + 1, compras[i].total_itens, compras[i].faturamento_bruto);
    }

    fclose(arquivo_compras);
}

void carregar_compras_txt(Compra *compras, int *num_compras) {
    FILE *arquivo_compras;
    arquivo_compras = fopen("compras.dat", "r");
    if (arquivo_compras == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de compras.\n");
        return;
    }

    int i = 0;
    char linha[100]; // Ajuste o tamanho conforme necessário
    while (fgets(linha, sizeof(linha), arquivo_compras) != NULL && i < MAX_COMPRAS) {
        sscanf(linha, "Venda %*d - Total de itens: %d | Faturamento bruto: R$%f", &compras[i].total_itens, &compras[i].faturamento_bruto);
        i++;
    }

    *num_compras = i;

    fclose(arquivo_compras);
}

int main() {
    int total_unidades_vendidas = 0;
    float faturamento_total_bruto = 0;
    int numero_vendas = 0;
    char resposta;
    Compra compras[MAX_COMPRAS];
    int num_compras = 0;

    // Adicionando contadores para rastrear as vendas de cada produto
    int vendas_por_produto[MAX_PRODUTOS] = {0}; // Inicializa todos os contadores como zero

    printf("Bem-vindo à loja!\n");

    do {
        imprimir_produtos_disponiveis();

        Compra compra_atual;
        compra_atual.total_itens = 0;
        compra_atual.faturamento_bruto = 0;

        int codigo_produto, quantidade;
        do {
            printf("Insira o código do produto desejado: ");
            scanf("%d", &codigo_produto);
            if (codigo_produto < 1 || codigo_produto > MAX_PRODUTOS) {
                printf("Código de produto inválido. Por favor, tente novamente.\n");
            }
        } while (codigo_produto < 1 || codigo_produto > MAX_PRODUTOS);

        printf("Insira a quantidade que você quer: ");
        scanf("%d", &quantidade);

        compra_atual.itens[compra_atual.total_itens].codigo_produto = codigo_produto;
        compra_atual.itens[compra_atual.total_itens].quantidade = quantidade;
        compra_atual.total_itens += quantidade;

        total_unidades_vendidas += quantidade;

        // Atualizando o contador de vendas para o produto atual
        vendas_por_produto[codigo_produto - 1] += quantidade;

        float preco_item = calcular_preco_item(codigo_produto, quantidade);
        compra_atual.faturamento_bruto += preco_item;

        printf("Deseja adicionar mais um item ao carrinho (S/N)? ");
        scanf(" %c", &resposta);

        while (resposta == 'S' || resposta == 's') {
            do {
                printf("Insira o código do produto desejado: ");
                scanf("%d", &codigo_produto);
                if (codigo_produto < 1 || codigo_produto > MAX_PRODUTOS) {
                    printf("Código de produto inválido. Por favor, tente novamente.\n");
                }
            } while (codigo_produto < 1 || codigo_produto > MAX_PRODUTOS);

            printf("Insira a quantidade que você quer: ");
            scanf("%d", &quantidade);

            compra_atual.itens[compra_atual.total_itens].codigo_produto = codigo_produto;
            compra_atual.itens[compra_atual.total_itens].quantidade = quantidade;
            compra_atual.total_itens += quantidade;

            total_unidades_vendidas += quantidade;

            // Atualizando o contador de vendas para o produto atual
            vendas_por_produto[codigo_produto - 1] += quantidade;

            preco_item = calcular_preco_item(codigo_produto, quantidade);
            compra_atual.faturamento_bruto += preco_item;

            printf("Deseja adicionar mais um item ao carrinho (S/N)? ");
            scanf(" %c", &resposta);
        }
        
        compras[num_compras] = compra_atual;
        num_compras++;

        faturamento_total_bruto += compra_atual.faturamento_bruto;

        printf("\nResumo da compra:\n");
        printf("Total de itens: %d\n", compra_atual.total_itens);
        printf("Faturamento bruto: R$%.2f\n\n", compra_atual.faturamento_bruto);

        printf("Deseja iniciar uma nova compra (S/N)? ");
        scanf(" %c", &resposta);

    } while (resposta == 'S' || resposta == 's');

    printf("\n\nRelatório Final do Dia:\n");
    printf("Número de vendas realizadas hoje: %d\n", num_compras);
    printf("Número total de itens vendidos hoje: %d\n", total_unidades_vendidas);
    printf("Faturamento total bruto: R$%.2f\n", faturamento_total_bruto);

    // Determinando o produto mais vendido e o menos vendido
    int mais_vendido = 0;
    int menos_vendido = 0;

    for (int i = 1; i < MAX_PRODUTOS; i++) {
        if (vendas_por_produto[i] > vendas_por_produto[mais_vendido]) {
            mais_vendido = i;
        }
        if (vendas_por_produto[i] < vendas_por_produto[menos_vendido] && vendas_por_produto[i] > 0) {
            menos_vendido = i;
        }
    }
    
    // Ordenando as vendas em ordem decrescente de valor
    qsort(compras, num_compras, sizeof(Compra), comparar_vendas);

    printf("\nLista de vendas realizadas hoje (em ordem decrescente de valor):\n");
    for (int i = 0; i < num_compras; i++) {
        printf("Venda %d - Total de itens: %d | Faturamento bruto: R$%.2f\n", i + 1, compras[i].total_itens, compras[i].faturamento_bruto);
    }

    printf("Produto mais vendido: %s\n", produtos[mais_vendido].nome);
    printf("Produto menos vendido: %s\n", produtos[menos_vendido].nome);
    
    
    // Salvar as compras em um arquivo .dat
    salvar_compras_txt(compras, num_compras);

    // Carregar as compras de um arquivo .dat
    carregar_compras_txt(compras, &num_compras);

    return 0;
}