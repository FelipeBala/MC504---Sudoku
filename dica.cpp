#include "sudoku.h";
int main(int argc, char *argv[] ) {

Sudoku teste;
teste.inicia_dados_arquivo_x(argv[1]);
teste.fornece_dicas();
teste.rearranja();
teste.imprime_dicas();
return(0);
}

