#include "EstruturadeDados.h"

int main(){
    struct binarioPauta pauta;
    struct binarioGeral aluno;
    char nome_ficheiro[MAX],buffer[MAX];
    FILE *f1,*f2,*f1o,*f2o;
    printf("Nome do ficheiro de pautas: ");
    fgets(nome_ficheiro,MAX,stdin);
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    f1=fopen(nome_ficheiro,"rb");
    printf("Nome do ficheiro para onde quer escrever: ");
    fgets(nome_ficheiro,MAX,stdin);
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    f1o=fopen(nome_ficheiro,"w");
    while(f1==NULL){
        fprintf(stderr,"Erro a abrir ficheiro, escreva o nome outra vez: ");
        fgets(nome_ficheiro,MAX,stdin);
        nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
        f1=fopen(nome_ficheiro,"rb");
    }
    /*while (fread(buffer, sizeof(char),MAX,f1)){
        printf("%s\n",buffer);
    }*/
    while (fread(&pauta, sizeof(pauta),1,f1)){
        fprintf(f1o,"Cadeira:%s Aluno:%s Nota Final:%.2lf\n",pauta.cad,pauta.nome,pauta.media);
    }
    printf("\nNome do ficheiro da pauta geral : ");
    fgets(nome_ficheiro,MAX,stdin);
    printf("\n");
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    f2=fopen(nome_ficheiro,"rb");
    printf("Nome do ficheiro para onde quer escrever: ");
    fgets(nome_ficheiro,MAX,stdin);
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    f2o=fopen(nome_ficheiro,"w");
    while(f2==NULL){
        fprintf(stderr,"Erro a abrir ficheiro, escreva o nome outra vez: ");
        fgets(nome_ficheiro,MAX,stdin);
        printf("\n");
        nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
        f2=fopen(nome_ficheiro,"rb");
    }
    while (fread(&aluno, sizeof(aluno),1,f2)){
        fprintf(f2o,"Aluno:%s Estado:%s\n",aluno.nome,aluno.aprovado);
    }

}