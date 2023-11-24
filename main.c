// Alunas: Ana Flávia e Isabella - 2B - Ciência da Computação

#include <stdio.h>

#define const 50

struct Produtos {
  int codigo_produto;
  double preco_produto;
  char descricao_produto[const];
}; // precisamos delimitar o máximo de caracteres da string

struct Vendedores {
  int codigo_vendedor;
  char nome_vendedor[const];
};

struct Vendas {
  int codigo_vendedor;
  int codigo_produto_venda;
  int unidades_vendidas;
};

int main() {

  FILE *totais;
  totais = fopen("totais.txt", "w");
  
  // -----------------------------------PRODUTOS----------------------------------------- //

  FILE *catalogo_produtos; // ponteiro para o arquivos
  catalogo_produtos = fopen("produtos.txt", "r"); // abrindo o arquivo

  if (catalogo_produtos == NULL) {
    printf("Error opening file.\n");
    return 1; // Caso haja erro
  }

  int total_linhas_produto; // armazenando o primeiro número como total de
                            // linhas do arquivo
  fscanf(catalogo_produtos, "%d\n", &total_linhas_produto);

  struct Produtos produtos[total_linhas_produto]; // transformando struct em um vetor

  fprintf(totais, "\n");

  for (int i = 0; i < total_linhas_produto; i++) {
    fscanf(catalogo_produtos, "%d", &produtos[i].codigo_produto);
    fscanf(catalogo_produtos, "%lf", &produtos[i].preco_produto);
    fscanf(catalogo_produtos, "%s", &produtos[i].descricao_produto);

    fprintf(totais, "[%d] %d %.2lf %s\n", i, produtos[i].codigo_produto,
           produtos[i].preco_produto, produtos[i].descricao_produto);
  } // o for percorre o arquivo e armazena cada número em uma variável do struct

  fprintf(totais, "\n\n");

  // -------------------------------VENDEDORES-----------------------------------------//

  FILE *lista_vendedores; // ponteiro para o arquivo
  lista_vendedores = fopen("vendedores.txt", "r"); // abrindo o arquivo

  if (lista_vendedores == NULL) {
    printf("Error opening file.\n");
    return 1; // Caso haja erro
  }

  int total_linhas_vendedor; // armazenando o primeiro número como total de
                             // linhas do arquixo
  fscanf(lista_vendedores, "%d\n", &total_linhas_vendedor);

  struct Vendedores vendedores[total_linhas_vendedor]; // transformando struct em um vetor

  for (int i = 0; i < total_linhas_vendedor; i++) {
    fscanf(lista_vendedores, "%d", &vendedores[i].codigo_vendedor);
    fscanf(lista_vendedores, "%s", &vendedores[i].nome_vendedor);

    fprintf(totais, "[%d] %d %s\n", i, vendedores[i].codigo_vendedor,
           vendedores[i].nome_vendedor);
  } // o for percorre o arquivo e armazena cada número em uma variável do struct

  fprintf(totais, "\n\n");

  // -------------------------------VENDAS-------------------------------------------//

  FILE *log_vendas; // ponteiro para o arquivo
  log_vendas = fopen("vendas.txt", "r"); // abrindo o arquivo

  if (log_vendas == NULL) {
    printf("Error opening file.\n");
    return 1; // Caso haja erro
  }

  int total_linhas_venda; // armazenando o primeiro número como total de linhas
                          // do arquixo
  fscanf(log_vendas, "%d\n", &total_linhas_venda);

  struct Vendas vendas[total_linhas_venda]; // transformando struct em um vetor

  for (int i = 0; i < total_linhas_venda; i++) {
    fscanf(log_vendas, "%d %d %d", &vendas[i].codigo_vendedor,
           &vendas[i].codigo_produto_venda, &vendas[i].unidades_vendidas);
    fprintf(totais, "[%d] %d %d %d\n", i, vendas[i].codigo_vendedor,
           vendas[i].codigo_produto_venda, vendas[i].unidades_vendidas);
  } // o for percorre o arquivo e armazena cada número em uma variável do struct

  fprintf(totais, "\n\n");

  fclose(catalogo_produtos);
  fclose(lista_vendedores);
  fclose(log_vendas);

  // -------------------------------CALCULO TOTAIS---------------------------------------//

  double total = 0.0;
  double preco_produto = 0.0;
  double soma_produto = 0.0;

  // inicializando um vetor para armazenar o total do vendedor
  // o tamanho do vetor é a quantidade de linhas do arquivo vendedores
  double total_vendedor[total_linhas_vendedor];

  // inicializando um vetor para armazenar o total do produto
  // o tamanho do vetor é a quantidade de linhas do arquivo produtos
  double total_produto[total_linhas_produto];

  for (int i = 0; i < total_linhas_venda; i++) { // o for percorre vendas e armazena o codigo do produto, a quantidade e o codigo do vendedor em variaveis
    int codigo_produto_venda = vendas[i].codigo_produto_venda;
    int unidades_vendidas = vendas[i].unidades_vendidas;
    int codigo_vendedor_venda = vendas[i].codigo_vendedor;

    // esse for vai percorrer produtos e comparar com o codigo tirado de vendas
    for (int j = 0; j < total_linhas_produto; j++) {

      int codigo_produto = produtos[j].codigo_produto;

      // caso o codigo do produto seja igual ao da venda ele identifica o valor
      // e cacula a soma de cada produto
      if (codigo_produto_venda == codigo_produto) {
        preco_produto = produtos[j].preco_produto;
        soma_produto = preco_produto * unidades_vendidas;
      }
    }
    // calculo do total geral
    total += soma_produto;

    // esse for percorre o arquivo dos vendedores
    for (int v = 0; v < total_linhas_vendedor; v++) {

      // caso o codigo do vendedor do arquivo de venda seja igual ao do arquivo
      // do vendedor vai somar o valor do produto no total desse vendedor
      if (codigo_vendedor_venda == vendedores[v].codigo_vendedor) {
        total_vendedor[v] += soma_produto;
      }
    }

    // esse for percorre o arquivo dos produtos
    for (int n = 0; n < total_linhas_produto; n++) {

      // caso o codigo do produto do arquivo de venda seja igual ao do arquivo
      // do produto vai somar o valor no total desse produto
      if (codigo_produto_venda == produtos[n].codigo_produto) {
        total_produto[n] += soma_produto;
      }
    }
  }

  // Print do total geral
  fprintf(totais, "Total geral vendido: %.2lf\n", total);

  fprintf(totais, "\n");

  // Print do total de cada produto
  fprintf(totais, "Total de vendas de cada produto:\n");
  for (int n = 0; n < total_linhas_produto; n++) {
    fprintf(totais, "Produto %d (%s): R$%.2lf\n", produtos[n].codigo_produto,
            produtos[n].descricao_produto, total_produto[n]);
  }

  fprintf(totais, "\n");

  // Print do total de cada vendedor
  fprintf(totais, "Total de vendas de cada vendedor:\n");
  for (int v = 0; v < total_linhas_vendedor; v++) {
    fprintf(totais, "Vendedor %d (%s): R$%.2lf\n",
            vendedores[v].codigo_vendedor, vendedores[v].nome_vendedor,
            total_vendedor[v]);
  }

  fclose(totais);

  return 0;
}