/*
 Projeto feito para PPP com listas duplamente ligadas
 O programa aceita mais que 2 input na linha de comandos, desde que o nome dado paras os ficheiros de saida seja aceite
 Os ficheiros de entrada sao pedidos ao utilizador pelo stdin, e se um não conseguir ser lido é pedido até ser aceite
 O programa aceita mais parametros nos ficheiros dos que os pedidos, mas não faz nada com eles
 Em ambos os ficheiros se houver menos parametros que é pedido a linha é ignorada
 Nos nomes aceita qualquer um, visto que o que nos importa será o numero do aluno
 Nos numeros só aceita positivos, e verifica se de facto é numero, se tiver um caracter a meio aceita o numero até esse caracter, se tiver caracter no inciio vai indicar erro
 Se for decimal tem de ser com . se for virgula não o vai reconhece
 Notas maior que 20 transforma em 20
 No ficheiro das disciplinas se um aluno não tiver no ficheiro de alunos, vai dar print a uma mensagem de erro, e se houver algum erro nessa linha, nao se vai adicionar a lista
 Verifica se a prova é 1 ou 2, se for >2 assume que é a prova 2, se menor <1 assume que é prova 1
 No caso de um aluno não estar inscrito a 10 cadeiras, vai imprimir tal mensagem, mas vai calcular se foi aprovado ou não na mesma
 Se por acaso o aluno so tiver a nota de um dos testes, irá assumir que é 0 no teste em falta

 */

#include "EstruturadeDados.h"

void leitura_alunos(FILE *f);
void leitura_notas(FILE *f);
void remove_espacos_inicio(char*);
void remove_espacos(char*);
void check_number(char*);

int main(int argc, char *argv[]) {
    FILE* foP,*foG,*fi_alunos=NULL,*fi_cadeiras=NULL;
    char nome_ficheiro[MAX];
    if(argc>3){
        fprintf(stderr,"Demasiados inputs na linha de comandos.\nO programa vai prosseguir na mesma.\n");
    }
    else if(argc<3){
         fprintf(stderr,"Menos inputs na linha de comandos.\nO programa vai parar pois não vai conseguir escrever nos ficheiros.\n");
         exit(-1);
     }
    foP=fopen(argv[1],"wb");
    if(foP==NULL){
        fprintf(stderr,"Erro a abrir ficheiro de escrita. Fechando o programa.");
        exit(-1);
    }
    foG=fopen(argv[2],"wb");
    if(foG==NULL){
        fprintf(stderr,"Erro a abrir ficheiro de escrita. Fechando o programa.");
        exit(-1);
    }
    printf("Nome do ficheiro dos alunos: ");
    fgets(nome_ficheiro,MAX,stdin);
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    fi_alunos=fopen(nome_ficheiro,"r");
    while(fi_alunos==NULL){
        fprintf(stderr,"Erro a abrir ficheiro alunos, tente outra vez: ");
        fgets(nome_ficheiro,MAX,stdin);
        nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
        fi_alunos=fopen(nome_ficheiro,"r");
    }
    printf("Nome do ficheiro das cadeiras: ");
    fgets(nome_ficheiro,MAX,stdin);
    nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
    fi_cadeiras=fopen(nome_ficheiro,"r");
    while (fi_cadeiras==NULL){
        fprintf(stderr,"Erro a abrir ficheiro cadeiras, tente outra vez: ");
        fgets(nome_ficheiro,MAX,stdin);
        nome_ficheiro[strlen(nome_ficheiro)-1]='\0';
        fi_cadeiras=fopen(nome_ficheiro,"r");
    }
    //carregar ficheiros para listas ligadas
    leitura_alunos(fi_alunos);
    leitura_notas(fi_cadeiras);
    add_pauta();
    //escrever output
    escreve_pauta_cadeiras(foP);
    escreve_pauta_global(foG);

    fclose(fi_cadeiras);
    fclose(fi_alunos);
}


void leitura_alunos(FILE *f) {//Função que vai ler ficheiro de alunos
    char buffer[50], *token, *token2, *token3, *tail;
    int counter = 1;
    long temp,n_aluno = 0;
    while (fgets(buffer, MAX, f)) {
        token = strtok(buffer, "#");//Nome
        token2 = strtok(NULL, "#");//Nº
        token3 = strtok(NULL, "#");//A mais
        if (token3 != NULL) {
            fprintf(stderr, "Dados a mais na linha %d ficheiro Alunos\n", counter);
        }
        if (token == NULL || token2 == NULL) {
            fprintf(stderr, "Erro na linha %d ficheiro Alunos, dados não se encontram separados, ou não se encontram dados na linha\n",counter);
        } else {
            remove_espacos_inicio(token);
            remove_espacos(token2);
            if (token2[strlen(token2) - 1] == '\n') {
                token2[strlen(token2) - 1] = '\0';
            }
            check_number(token2);
            temp = strtol(token2, &tail, 0);
            errno = 0;
            if (errno) {//verifica se o numero de aluno não da transbordo
                fprintf(stderr, "Transbordo em long na linha %d ficheiro Alunos\n", counter);
            }
            else if (temp == 0 && token2[0] != '0') {//se temp for igual a 0 e o char inicial nao for 0 é pq deu erro a fazer a conversao
                fprintf(stderr, "Erro! Numero de aluno não é um long na linha: %d ficheiro Alunos\n", counter);
                errno=1;
            }
            else if(temp==0 && token2[0]=='0'){//temp é 0 e o char inicial é 0, logo nãp houve erro
                n_aluno=0;
            }
            else{
                n_aluno=temp;
            }
            if (errno == 0) {
                if(add_aluno(token, n_aluno)==1) {//Adiciona o aluno
                    fprintf(stderr,"Erro a adicionar aluno\n");
                }
            }
            counter += 1;
        }
    }
}


void leitura_notas(FILE *f) {//Função que vai ler ficheiro de cadeiras
    char buffer[50], *token, *token2, *token3, *token4, *token5, *tail;
    int counter = 1, prova, erro;
    double nota;
    long n_aluno,temp;
    while (fgets(buffer, MAX, f)) {
        erro = 0;
        token = strtok(buffer, "#");//Nº
        token2 = strtok(NULL, "#");//Cadeira
        token3 = strtok(NULL, "#");//Prova
        token4 = strtok(NULL, "#");//Nota
        token5 = strtok(NULL, "#");//A mais
        if (token5 != NULL) {//se tiver palavras a mais
            fprintf(stderr, "Dados a mais na linha %d ficheiro Notas\n", counter);
        }
        if (token == NULL || token2 == NULL || token3 == NULL || token4 == NULL) {// se nao tiver suficiente
            fprintf(stderr, "Erro na linha %d ficheiro Notas, dados não se encontram separados, ou não se encontram dados na linha\n",
                    counter);
        } else {
            remove_espacos(token);
            check_number(token);
            remove_espacos_inicio(token2);
            if (token4[strlen(token4) - 1] == '\n') {
                token4[strlen(token4) - 1] = '\0';
            }
            check_number(token3);
            //ver se numero é long ou nao
            errno = 0;
            temp = strtol(token, &tail, 0);
            if (errno) {
                fprintf(stderr, "Transbordo em long na linha %d ficheiro Notas\n", counter);
                erro=1;
            }
            else if (temp == 0 && token3[0] != '0') {
                fprintf(stderr, "Erro! Numero de aluno não é um long na linha: %d ficheiro Notas\n", counter);
                erro=1;
            }
            else if(temp==0 && token3[0]=='0'){
                n_aluno=0;
            }
            else{
                n_aluno=temp;
            }
            //transformar nota em double
            errno=0;
            nota=strtod(token4,&tail);
            if(errno) {
                fprintf(stderr, "Transbordo em double na linha %d ficheiro Notas\n", counter);
            }
            else if (nota == 0 && token4[0] != '0') {
                fprintf(stderr, "Erro! Nota não é um double na linha: %d ficheiro Notas\n", counter);
                erro=1;
            }
            if(nota<0){
                fprintf(stderr,"Erro a carregar nota na linha %d ficheiro Notas, nota<0, transformada em 0",counter);
                nota=0.0;
            }
            else if(nota>20){
                fprintf(stderr,"Erro a carregar nota na linha %d ficheiro Notas, nota>20, transformada em 20",counter);
                nota=20.0;
            }
            //transformar prova em int
            errno=0;
            temp = strtol(token3, &tail, 0);
            if (errno) {
                fprintf(stderr,"Transbordo em long na linha %d ficheiro Notas\n",counter);
            }
            else if (temp <= INT_MAX && temp >= INT_MIN) {
                prova = (int)temp;
            } else {
                printf("Transbordo em int na linha %d ficheiro Notas\n", counter);
                erro=1;
            }
            if (temp == 0 && token3[0]!='0') {
                fprintf(stderr, "Erro na linha %d ficheiro Notas, segundo parametro não é um inteiro\n", counter);
                erro=1;
            }
            if (erro != 1) {//Adiciona as cadeiras
                if(prova<1){
                    prova=1;
                }
                else if(prova>2){
                    prova=2;
                }
                if(add_nota(nota, token2, n_aluno)==1){
                    fprintf(stderr,"Erro a adiconar cadeira\n");
                }
            }
            counter += 1;
        }
    }
}

void check_number(char * string){//Verifica se só há numeros na string do numero
    for(int i=0;i<strlen(string);i++){
        if(string[i]<'0' || string[i]>'9'){
            fprintf(stderr,"Erro a ler numero, caracter que não digito lido. Numero %s so vai ser lido até á casa %d\n",string,i);
        }
    }
}

void remove_espacos_inicio(char* string){//Retira os espaços no inicio da string
    int count=0,flag=0;
    while (string[count]==' '){
        flag=1;//tem espaços
        string[count]=string[count+1];
        count++;//ver em que caracter começa o espacço
    }
    if(flag==1){
        for(int i=0;i<strlen((string));i++){
            string[i]=string[count++];//substituir de inicio o caracter com espaço por caracters sem espaço
        }
        string[count]='\0';
    }
}

void remove_espacos(char* string){//Retira todos os espaços da string
    int count = 0;
    for (int i = 0; i < strlen(string); i++)
        if (string[i] != ' ')
            string[count++] = string[i];
    string[count]='\0';
}