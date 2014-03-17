#include "sudoku.h";

int main() {

Sudoku teste;
teste.inicia_dados_arquivo("teste_certo.txt");
if(teste.verifica_horizontal()){return(0);}
if(teste.verifica_vertical()){return(0);}
if(teste.verifica_quadrado()){return(0);}

printf("O Sudoku não apresenta erro\n");
return(0);
}

