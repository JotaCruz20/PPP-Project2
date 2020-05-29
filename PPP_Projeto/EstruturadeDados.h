#ifndef PROJETO_ESTRUTURADEDADOS_H
#define PROJETO_ESTRUTURADEDADOS_H
#define MAX 100
#include <errno.h>
#include <zconf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct infor_aluno{
    char* nome;
    long num;
};

struct infor_cadeira{
    long num;
    double nota;
    char* nome;
};

typedef struct elem_fila_al* Alunos;
typedef struct elem_fila_nota* Notas;
typedef struct elem_fila_pauta* Pautas;
typedef struct infor_pauta * Aluno;

struct elem_fila_al {
    struct infor_aluno aluno;
    Alunos al_anterior;
    Alunos al_seguinte;
};
struct elem_fila_nota {
    struct infor_cadeira cadeira;
    Notas nota_anterior;
    Notas nota_seguinte;
};

struct infor_pauta{
    char* nome;
    long num;
    double nota1;
    double nota2;
    Aluno aluno_seguinte;
    Aluno aluno_anterior;
};

struct binarioPauta{
    double media;
    char nome[MAX];
    char cad[MAX];
};

struct binarioGeral{
    char nome[MAX];
    char aprovado[MAX];
};

struct elem_fila_pauta{
    char* cad;
    Aluno infor_pauta;
    Pautas cad_Seguinte;
};

int add_nota(double,char*,long);
int add_aluno(char*,long);
void escreve_pauta_cadeiras(FILE *f);
void escreve_pauta_global(FILE *f);
int add_pauta();


#endif //PROJETO_ESTRUTURADEDADOS_H
