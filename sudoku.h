#include <pthread.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

struct pacote{
int ** end_matrix;
int num;
int erro;
};

struct dica_x{
int pos_x;
int pos_y;
struct sugestao *sugestao;
struct dica_x *next;
};

struct estrutura_dicas{
int ** end_matrix;
bool *dicas;
int num;
};

struct sugestao{
int num;
struct sugestao *next;
};


class Sudoku {

public:
void inicia_dados_arquivo(char * arquivo);
void inicia_dados_arquivo_x(char * arquivo);
bool verifica_horizontal();
bool verifica_vertical();
bool verifica_quadrado();
struct estrutura_dicas * acha_dica_coluna();
struct estrutura_dicas * acha_dica_linha();
struct estrutura_dicas * acha_dica_quadrado();
void fornece_dicas();
void imprime_dicas();
void resolve();
void rearranja();
Sudoku();


/////////////////////////////////

private:
static void* verifica_horizontal_thread(void* v);
static void* verifica_vertical_thread(void* v);
static void* verifica_quadrado_thread(void* v);
static void* acha_dica_coluna_thread(void* v);
static void* acha_dica_linha_thread(void* v);
static void* acha_dica_quadrado_thread(void* v);
void insere_vetor_x(struct dica_x * vetor_x,int i,int j);
void apaga_referencias(int x,int y,int num);
int ** matrix;
bool iniciado;
struct dica_x * vetor_de_x;




};

#endif // SUDOKU_H_INCLUDED
