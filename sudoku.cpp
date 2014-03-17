#include "sudoku.h"

//////////////////////Verifica//////////////////////////////////

Sudoku::Sudoku(){
int i;
matrix = (int **)malloc(9*sizeof(int *));
for(i=0;i<9;i++){matrix[i] = (int *)malloc(9*sizeof(int ));}
}

void Sudoku::inicia_dados_arquivo(char * arquivo){
int i,j;
ifstream myfile;

myfile.open (arquivo, ios::in);
if(myfile.good()){
for (i=0;i<9;i++)
for (j=0;j<9;j++){myfile >> matrix[i][j];}
iniciado = true;
}else{cout << "\nErro ao abrir o programa.\n";}
}

void* Sudoku::verifica_horizontal_thread(void* v){
int i,l;
bool tick[10]={0,0,0,0,0,0,0,0,0,0};
struct pacote *box;
box = (struct pacote *) v;

for(i=0;i<9;i++){
     l = box->end_matrix[box->num][i];
        //printf("id %d, i= %d numero %d\n",box->num,i,l);
    if(tick[l]){
        box->erro = l;
    break;}
    else{tick[l] = true;}
}
return NULL;
}

bool Sudoku::verifica_horizontal(){
 int i;
 bool ja_errou = false;
 pthread_t thr[9];
 struct pacote pacote[10];

 for(i=0;i<9;i++){
    pacote[i].num = i;
    pacote[i].end_matrix = matrix;
    pacote[i].erro = -1;
    if (pthread_create(&thr[i], NULL, verifica_horizontal_thread,(void *) &pacote[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

for(i=0;i<9;i++){
    pthread_join(thr[i],NULL);
    if(ja_errou == false && pacote[i].erro != -1 ){
    printf("\nA linha %d contem duas ocorrencias do numero %d.\n",i+1,pacote[i].erro);
    ja_errou = true;
    }
}

return(ja_errou);
}

bool Sudoku::verifica_vertical(){
 int i;
 bool ja_errou=false;
 pthread_t thr[9];
 struct pacote pacote[10];

 for(i=0;i<9;i++){
    pacote[i].num = i;
    pacote[i].end_matrix = matrix;
    pacote[i].erro = -1;
    if (pthread_create(&thr[i], NULL, verifica_vertical_thread,(void *) &pacote[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

for(i=0;i<9;i++){
    pthread_join(thr[i],NULL);
    if(ja_errou == false && pacote[i].erro != -1 ){
    printf("\nA coluna %d contem duas ocorrencias do numero %d.\n",i+1,pacote[i].erro);
    ja_errou = true;
    }
}

return(ja_errou);
}

void* Sudoku::verifica_vertical_thread(void* v){
int i,l;
bool tick[10]={0,0,0,0,0,0,0,0,0,0};
struct pacote *box;
box = (struct pacote *) v;

for(i=0;i<9;i++){
     l = box->end_matrix[i][box->num];
        //printf("id %d, i= %d numero %d\n",box->num,i,l);
    if(tick[l]){
        box->erro = l;
    break;}
    else{tick[l] = true;}
}
return NULL;
}

bool Sudoku::verifica_quadrado(){
 int i;
 bool ja_errou=false;
 pthread_t thr[9];
 struct pacote pacote[10];

 for(i=0;i<9;i++){
    pacote[i].num = i;
    pacote[i].end_matrix = matrix;
    pacote[i].erro = -1;
    if (pthread_create(&thr[i], NULL, verifica_quadrado_thread,(void *) &pacote[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

for(i=0;i<9;i++){
    pthread_join(thr[i],NULL);
    if(ja_errou == false && pacote[i].erro != -1 ){
    printf("\nO quadrado %d contem duas ocorrencias do numero %d.\n",i+1,pacote[i].erro);
    ja_errou = true;
    }
}

return(ja_errou);
}

void* Sudoku::verifica_quadrado_thread(void* v){
int i,j,l,plus_coluna,plus_linha;
bool tick[10]={0,0,0,0,0,0,0,0,0,0};
struct pacote *box;
box = (struct pacote *) v;


plus_coluna= (box->num)%3;
plus_linha= (box->num)/3;

for(i=0;i<3;i++){
for(j=0;j<3;j++){

     l = box->end_matrix[plus_linha*3+i][plus_coluna*3+j];
        //printf("id %d, i= %d numero %d\n",box->num,i,l);
    if(tick[l]){
        box->erro = l;
    break;}
    else{tick[l] = true;}
}}
return NULL;
}

///////////////////////Dica////////////////////////////////////

void Sudoku::inicia_dados_arquivo_x(char * arquivo){
int i,j,k2;
char k;
ifstream myfile;
vetor_de_x = (struct dica_x *)malloc(sizeof(struct dica_x));
vetor_de_x->next=NULL;
myfile.open (arquivo, ios::in);
if(myfile.good()){
for (i=0;i<9;i++)
for (j=0;j<9;j++){
        myfile >> k;
        if(k == 'X'){
                matrix[i][j] = -1;
                insere_vetor_x(vetor_de_x,i,j);
        }
        else{
                k2 = (int)k;
                matrix[i][j] = k2-48;}
}
iniciado = true;
}else{cout << "\nErro ao abrir o programa.\n";}

}

void Sudoku::insere_vetor_x(struct dica_x * vetor_x,int i,int j){

while(vetor_x->next != NULL){vetor_x = vetor_x->next;}

vetor_x->pos_x = i;
vetor_x->pos_y = j;
vetor_x->sugestao = NULL;
vetor_x->next = (struct dica_x *)malloc(sizeof(struct dica_x));
vetor_x = vetor_x->next;
vetor_x->next = NULL;
}

struct estrutura_dicas * Sudoku::acha_dica_coluna(){
int i;
pthread_t thr[9];
struct estrutura_dicas *vetor_dicas;
vetor_dicas = (struct estrutura_dicas *)malloc(9*sizeof(struct estrutura_dicas));

 for(i=0;i<9;i++){
    vetor_dicas[i].end_matrix = matrix;
    vetor_dicas[i].num = i;
    vetor_dicas[i].dicas=(bool*)calloc(9,sizeof(bool));
    if (pthread_create(&thr[i], NULL, acha_dica_coluna_thread,(void *) &vetor_dicas[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

for(i=0;i<9;i++){pthread_join(thr[i],NULL);}

return(vetor_dicas);
}

void* Sudoku::acha_dica_coluna_thread(void* v){
int i,l;
struct estrutura_dicas *box;
box = (struct estrutura_dicas *) v;


for(i=0;i<9;i++){

     l = box->end_matrix[i][box->num];
     if(l == -1){continue;}
     else{box->dicas[l-1]=true;}
}
for(i=0;i<9;i++){
 if(box->dicas[i]){box->dicas[i]=false;}else{box->dicas[i]=true;/*printf("Coluna%d:%d\n",box->num,i+1);*/}
}
return NULL;
}

struct estrutura_dicas * Sudoku::acha_dica_linha(){
int i;
pthread_t thr[9];
struct estrutura_dicas *vetor_dicas;
vetor_dicas = (struct estrutura_dicas *)malloc(9*sizeof(struct estrutura_dicas));

 for(i=0;i<9;i++){
    vetor_dicas[i].end_matrix = matrix;
    vetor_dicas[i].num = i;
    vetor_dicas[i].dicas=(bool*)calloc(9,sizeof(bool));
    if (pthread_create(&thr[i], NULL, acha_dica_linha_thread,(void *) &vetor_dicas[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

for(i=0;i<9;i++){pthread_join(thr[i],NULL);}

return(vetor_dicas);

}

void* Sudoku::acha_dica_linha_thread(void* v){
int i,l;
struct estrutura_dicas *box;
box = (struct estrutura_dicas *) v;


for(i=0;i<9;i++){

     l = box->end_matrix[box->num][i];
     if(l == -1){continue;}
     else{box->dicas[l-1]=true;}
}
for(i=0;i<9;i++){
 if(box->dicas[i]){box->dicas[i]=false;}else{box->dicas[i]=true;/*printf("Linha%d:%d\n",box->num,i+1);*/}
}
return NULL;
}

struct estrutura_dicas * Sudoku::acha_dica_quadrado(){
int i;
pthread_t thr[9];
struct estrutura_dicas *vetor_dicas;
vetor_dicas = (struct estrutura_dicas *)malloc(9*sizeof(struct estrutura_dicas));

 for(i=0;i<9;i++){
    vetor_dicas[i].end_matrix = matrix;
    vetor_dicas[i].num = i;
    vetor_dicas[i].dicas=(bool*)calloc(9,sizeof(bool));
    if (pthread_create(&thr[i], NULL, acha_dica_quadrado_thread,(void *) &vetor_dicas[i]))
    printf("Erro na criacao da thread %d \n",i);
 }

 for(i=0;i<9;i++){pthread_join(thr[i],NULL);}

return(vetor_dicas);

}

void* Sudoku::acha_dica_quadrado_thread(void* v){
int i,j,l;
struct estrutura_dicas *box;
box = (struct estrutura_dicas *) v;
int plus_coluna= (box->num)%3;
int plus_linha= (box->num)/3;

for(i=0;i<3;i++){
for(j=0;j<3;j++){

     l = box->end_matrix[plus_linha*3+i][plus_coluna*3+j];
     if(l == -1){continue;}
     else{box->dicas[l-1]=true;}
}}

for(i=0;i<9;i++){
 if(box->dicas[i]){box->dicas[i]=false;}else{box->dicas[i]=true;/*printf("Quadrado%d:%d\n",box->num,i+1);*/}
}

return NULL;
}

void Sudoku::fornece_dicas(){
int i,j,k;
int plus_coluna;
int plus_linha;
struct estrutura_dicas *a,*b,*c;
struct dica_x * proximo;
struct sugestao  *u,*p;

a = acha_dica_coluna();
b = acha_dica_linha();
c = acha_dica_quadrado();

//int teste=0;
for(i=0;i<9;i++){
for(j=0;j<9;j++){
   // teste=0;

    if(a[i].dicas[j]){
    proximo = vetor_de_x;
    while(proximo->next != NULL){
     //       teste++;
    if(proximo->pos_y == i){
    u = (struct sugestao *)malloc(sizeof(struct sugestao));
    u->next=NULL;
    u->num= j;
    //printf("i:%d j:%d teste:%d\n",i+1,j+1,teste);
    p = proximo->sugestao;
    if(p == NULL){proximo->sugestao = u;}else{
    while(p->next != NULL){p = p->next;}
    p->next = u;}
    }
    proximo = proximo->next;
    }
}


    if(b[i].dicas[j]){
    proximo = vetor_de_x;
    while(proximo->next != NULL){
    if(proximo->pos_x == i){
    u = (struct sugestao *)malloc(sizeof(struct sugestao));
    u->next=NULL;
    u->num= j;
    p = proximo->sugestao;
    if(p == NULL){proximo->sugestao = u;}else{
    while(p->next != NULL){p = p->next;}
    p->next = u;}
    }
    proximo = proximo->next;
    }
}



    if(c[i].dicas[j]){
    plus_coluna= (i)%3;
    plus_linha= (i)/3;
    plus_coluna*= 3;
    plus_linha*=3;
    proximo = vetor_de_x;
    while(proximo->next != NULL){
    if((proximo->pos_x >= plus_linha) && (proximo->pos_x < plus_linha+3) && (proximo->pos_y >= plus_coluna) && (proximo->pos_y < plus_coluna+3) ){
    u = (struct sugestao *)malloc(sizeof(struct sugestao));
    u->next=NULL;
    u->num= j;
    p = proximo->sugestao;
    if(p == NULL){proximo->sugestao = u;}else{
    while(p->next != NULL){p = p->next;}
    p->next = u;}
    }
    proximo = proximo->next;
    }
}

}








}}

void Sudoku::imprime_dicas(){
int i,j,k,vetor[9],espaco;
struct dica_x * a;
struct sugestao * b;
a = vetor_de_x;

//for(i=0;i<9;i++){vetor[i]=0;}

for(i=0;i<9;i++){
        printf("\n");
for(j=0;j<9;j++){
        espaco = 4;
if(matrix[i][j] != -1){printf(" %d ",matrix[i][j]); if(espaco > 0){for(espaco=espaco;-1<espaco;espaco--){printf(" ");}} continue;}
printf("(");
a = vetor_de_x;
while(a->pos_x != i || a->pos_y != j){a = a->next;}
b = a->sugestao;
while(b != NULL){
   //vetor[b->num]++;
  printf("%d",b->num);
  espaco--;
  b = b->next;
}

/*
for(k=0;k<9;k++){
        if(vetor[k] == 3){printf("%d",k+1);espaco--;}
        vetor[k]=0;
}*/

printf(")");
espaco +=1;
if(espaco > 0){for(espaco=espaco;-1<espaco;espaco--){printf(" ");}}




}}
}

///////////////////////Resolve//////////////////////////////////

void Sudoku::rearranja(){
int i,j,k,vetor[9];
struct dica_x * a;
struct sugestao * b,*c;
a = vetor_de_x;

for(i=0;i<9;i++){vetor[i]=0;}

for(i=0;i<9;i++){
for(j=0;j<9;j++){
if(matrix[i][j] != -1){continue;}
a = vetor_de_x;
while(a->pos_x != i || a->pos_y != j){a = a->next;}
b = a->sugestao;
while(b != NULL){
   vetor[b->num]++;
  c = b;
  b = b->next;
  free(c);
}
a->sugestao = NULL;

for(k=0;k<9;k++){
        if(vetor[k] == 3){
        b = (struct sugestao *)malloc(sizeof(struct sugestao));
        b->num = k+1;
        b->next = NULL;
        if(a->sugestao == NULL){a->sugestao = b;}else{
        c = a->sugestao;
        while(c->next != NULL){c = c->next;}
        c->next = b;
        }}
        vetor[k]=0;
}


}}
}

void Sudoku::apaga_referencias(int x,int y,int num){
struct dica_x * a;
struct sugestao * b,*c;
a = vetor_de_x;
int plusx,plusy,index,plus_linha,plus_coluna;


while(a->next != NULL){
      if(a->pos_x == x && a->pos_y == y){a = a->next;continue;}

      if(a->pos_x == x){
      b = a->sugestao;
      if(b != NULL){
      if(b->next == NULL){
      if(b->num == num){free(b);a->sugestao == NULL;}
      }else{
       if(b->num == num){a->sugestao = b->next; free(b);}else{
       c = b->next;
       while(c != NULL){
       if(c->num == num){b->next = c->next;}
       b= b->next;
       if(b == NULL){break;}
       c = b->next;
      }}}}}


      if(a->pos_y == y){
      b = a->sugestao;
      if(b != NULL){
      if(b->next == NULL){
      if(b->num == num){free(b);a->sugestao == NULL;}
      }else{
       if(b->num == num){a->sugestao = b->next; free(b);}else{
       c = b->next;
       while(c != NULL){
       if(c->num == num){b->next = c->next;}
       b= b->next;
       if(b == NULL){break;}
       c = b->next;
      }}}}}


plusx = x/3;
plusy = y/3;
plusy *= 3;
plusx *= 3;

      if((a->pos_x > plusx) && (a->pos_x < plusx+3) && (a->pos_y > plusy) && (a->pos_y < plusy+3) ){
      b = a->sugestao;
      if(b != NULL){
      if(b->next == NULL){
      if(b->num == num){free(b);a->sugestao == NULL;}
      }else{
       if(b->num == num){a->sugestao = b->next; free(b);}else{
       c = b->next;
       while(c != NULL){
       if(c->num == num){b->next = c->next;}
       b= b->next;
       if(b == NULL){break;}
       c = b->next;
      }}}}}




        a = a->next;
        }
//imprime_dicas();
//getchar();

}

void Sudoku::resolve(){
struct dica_x * a;
struct sugestao * b;
Sudoku vetor[20];
pthread_t thr[20];
int controle,i,j;
bool flag;
recomeco:
flag = true;
while(flag){
controle = 0;
a = vetor_de_x;
flag = false;
while(a->next != NULL){

b = a->sugestao;
if(b != NULL){
    controle++;
if(b->next == NULL){
 flag = true;
 matrix[a->pos_x][a->pos_y] = b->num;
 free(b);
 a->sugestao = NULL;
 apaga_referencias(a->pos_x,a->pos_y,matrix[a->pos_x][a->pos_y]);
}}
a = a->next;
}}
///////////////////////////////////////////////////////////////////////////
if(controle >  0){


a = vetor_de_x;
flag = false;
while(a->next != NULL){

b = a->sugestao;
if(b != NULL){
 matrix[a->pos_x][a->pos_y] = b->num;
 free(b);
 a->sugestao = NULL;
 apaga_referencias(a->pos_x,a->pos_y,matrix[a->pos_x][a->pos_y]);
goto recomeco;
}
a = a->next;
}
}



}


