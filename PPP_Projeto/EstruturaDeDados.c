#include "EstruturadeDados.h"
static Notas notas_raiz;
static Alunos alunos_raiz;
static Pautas pautas_raiz;

void escreve_pauta_cadeiras(FILE *f){
    Pautas pauta=pautas_raiz;
    Aluno alunos_inscritos_cadeira=NULL;
    struct binarioPauta s_escrever;
    double media=0;
    while(pauta!=NULL){
        alunos_inscritos_cadeira=pauta->infor_pauta;
        if(alunos_inscritos_cadeira==NULL){
            fprintf(stderr,"%s sem alunos inscritos.\n",pauta->cad);
        }
        else{
            while (alunos_inscritos_cadeira!=NULL){
                if(alunos_inscritos_cadeira->nota1<0 || alunos_inscritos_cadeira->nota1>20  || alunos_inscritos_cadeira->nota2<0 || alunos_inscritos_cadeira->nota2>20){//erro em alguma das notas
                    fprintf(stderr,"Erro a caregar nota do aluno %s para disciplina %s\n",alunos_inscritos_cadeira->nome,pauta->cad);
                }
                else {
                    media = alunos_inscritos_cadeira->nota1 + alunos_inscritos_cadeira->nota2;
                    s_escrever.media=media/2;
                    strcpy(s_escrever.nome,alunos_inscritos_cadeira->nome);
                    strcpy(s_escrever.cad,pauta->cad);
                    fwrite(&s_escrever, sizeof(s_escrever), 1, f);
                }
                alunos_inscritos_cadeira=alunos_inscritos_cadeira->aluno_seguinte;
            }
        }
        pauta=pauta->cad_Seguinte;
    }
}

void escreve_pauta_global(FILE *f){
    struct binarioGeral s_esrever;
    Alunos alunos_inscritos=alunos_raiz;
    Aluno pesquisa_aluno;
    Pautas cadeira_inscrito=NULL;
    int cadeiras_feitas,counter;
    double nota;
    while (alunos_inscritos!=NULL){
        cadeira_inscrito=pautas_raiz;
        if(cadeira_inscrito==NULL){
            fprintf(stderr,"Sem cadeiras no sistema");
        }else{
            cadeiras_feitas=0;
            counter=0;
            while (cadeira_inscrito!=NULL){
                pesquisa_aluno=cadeira_inscrito->infor_pauta;
                if(pesquisa_aluno!=NULL){
                    while (pesquisa_aluno!=NULL && pesquisa_aluno->num!=alunos_inscritos->aluno.num){
                        pesquisa_aluno=pesquisa_aluno->aluno_seguinte;
                    }
                    if(pesquisa_aluno!=NULL){
                        counter+=1;
                        nota=pesquisa_aluno->nota1+pesquisa_aluno->nota2;
                        nota=nota/2;
                        if(nota>=9.5){
                            cadeiras_feitas+=1;
                        }
                    }
                }
                cadeira_inscrito=cadeira_inscrito->cad_Seguinte;
            }
            if(counter!=10){
                fprintf(stderr,"Aluno %s não está inscrito ao numero de cadeiras certo\n", alunos_inscritos->aluno.nome);
            }
            if(cadeiras_feitas>=8){
                strcpy(s_esrever.aprovado,"APROVADO");
            }
            else{
                strcpy(s_esrever.aprovado,"REPROVADO");
            }
            strcpy(s_esrever.nome,alunos_inscritos->aluno.nome);
            fwrite(&s_esrever, sizeof(s_esrever), 1, f);
        }

        alunos_inscritos=alunos_inscritos->al_seguinte;
    }

}

int add_aluno(char*nome,long n_aluno){//Adiciona aluno a lista de alunos
    Alunos aux;
    Alunos temp=alunos_raiz,ant;

    //Obter espaço para um novo nó
    aux = (Alunos) malloc(sizeof(struct elem_fila_al));
    if (aux == NULL)
        //não há espaço
        return 1;

    aux->aluno.nome=(char*)malloc(sizeof(char)*strlen(nome));
    strcpy(aux->aluno.nome,nome);
    aux->aluno.num=n_aluno;

    if(alunos_raiz==NULL){
        alunos_raiz=aux;
        return 0;
    }

    // Procura onde inserir por ordem alfabetica
    while(temp!=NULL && strcmp(nome,temp->aluno.nome)>0){
        ant=temp;
        temp=temp->al_seguinte;
    }
    if(temp==alunos_raiz){
        temp->al_anterior=aux;
        aux->al_seguinte=temp;
        alunos_raiz=aux;
    }
    else {
        aux->al_anterior=ant;
        ant->al_seguinte=aux;
        aux->al_seguinte=temp;
    }
    if(aux->al_seguinte!=NULL){
        aux->al_seguinte->al_anterior=aux;
    }
    return 0;
}

int add_nota(double nota,char*nome,long n_aluno){
    Notas aux = (Notas) malloc(sizeof(struct elem_fila_nota));
    if (aux == NULL)
        //não há espaço
        return 1;
    aux->cadeira.nome =(char*)malloc(sizeof(char)*strlen(nome));
    strcpy(aux->cadeira.nome,nome);
    aux->cadeira.num=n_aluno;
    aux->cadeira.nota=nota;
    if (notas_raiz == NULL) {
        notas_raiz = aux;
    }
    else{
        Notas temp=notas_raiz;
        while (temp->nota_seguinte != NULL)
            temp = temp->nota_seguinte;
        temp->nota_seguinte = aux;
        aux->nota_anterior = temp;
    }
    return 0;
}

int add_pauta() {
    Notas aux_notas = notas_raiz;
    Alunos aux_alunos = alunos_raiz;
    Pautas aux_pauta, add_pauta, ant_pauta = NULL;
    Aluno pesquisa_aluno, ant_aluno, add_aluno;
    if (aux_alunos == NULL) {
        fprintf(stderr, "Sem alunos no sistema\n");
        return 1;
    }
    if (aux_notas == NULL) {
        fprintf(stderr, "Sem cadeiras no sistema\n");
        return 1;
    }
    while (aux_notas != NULL) {
        aux_pauta = pautas_raiz;
        aux_alunos = alunos_raiz;
        while (aux_alunos != NULL && aux_alunos->aluno.num != aux_notas->cadeira.num) {//Procurar aluno q se pretende inserir nota
            aux_alunos = aux_alunos->al_seguinte;
        }
        if (aux_alunos == NULL) {//Aluno nao existe
            fprintf(stderr, "Erro a inserir aluno %lu na Pauta.\n", aux_notas->cadeira.num);
        } else {
            if (aux_pauta == NULL) {//Ainda não há cadeiras nas Pautas
                //inserir a cadeira nas pautas
                add_pauta = (Pautas) malloc(sizeof(struct elem_fila_nota));
                add_pauta->cad = (char *) malloc(sizeof(char) * strlen(aux_notas->cadeira.nome));
                strcpy(add_pauta->cad, aux_notas->cadeira.nome);
                //inserir o aluno na cadeira
                add_pauta->infor_pauta = (Aluno) malloc(sizeof(struct infor_pauta));
                add_pauta->infor_pauta->nome = (char *) malloc(sizeof(char) * strlen(aux_alunos->aluno.nome));
                strcpy(add_pauta->infor_pauta->nome, aux_alunos->aluno.nome);
                add_pauta->infor_pauta->num = aux_alunos->aluno.num;
                add_pauta->infor_pauta->nota1 = aux_notas->cadeira.nota;
                pautas_raiz = add_pauta;
            } else {//Já existem cadeiras nas pautas
                while (aux_pauta != NULL && strcmp(aux_pauta->cad, aux_notas->cadeira.nome) != 0) {//Verificar se cadeira existe ou nao
                    ant_pauta = aux_pauta;
                    aux_pauta = aux_pauta->cad_Seguinte;
                }
                if (aux_pauta == NULL) {//Cadeira nao existe
                    //adicionar cadeira
                    add_pauta = (Pautas) malloc(sizeof(struct elem_fila_nota));
                    add_pauta->cad = (char *) malloc(sizeof(char) * strlen(aux_notas->cadeira.nome));
                    strcpy(add_pauta->cad, aux_notas->cadeira.nome);
                    ant_pauta->cad_Seguinte = aux_pauta;
                    //inserir o aluno na cadeira
                    aux_alunos = alunos_raiz;
                    add_pauta->infor_pauta = (Aluno) malloc(sizeof(struct infor_pauta));
                    add_pauta->infor_pauta->nome = (char *) malloc(sizeof(char) * strlen(aux_alunos->aluno.nome));
                    strcpy(add_pauta->infor_pauta->nome, aux_alunos->aluno.nome);
                    add_pauta->infor_pauta->num = aux_alunos->aluno.num;
                    add_pauta->infor_pauta->nota1 = aux_notas->cadeira.nota;
                    ant_pauta->cad_Seguinte = add_pauta;
                } else {//Cadeira ja existe no sistema
                    pesquisa_aluno = aux_pauta->infor_pauta;
                    while (pesquisa_aluno != NULL && pesquisa_aluno->num != aux_notas->cadeira.num) {//procurar se o aluno existe na cadeira
                        ant_aluno = pesquisa_aluno;
                        pesquisa_aluno = pesquisa_aluno->aluno_seguinte;
                    }
                    if (pesquisa_aluno != NULL) {//aluno ja existe, vamos inserir a segunda nota
                        pesquisa_aluno->nota2 = aux_notas->cadeira.nota;
                    } else {//aluno nao existe vamos adicionar
                        pesquisa_aluno = aux_pauta->infor_pauta;
                        while (pesquisa_aluno != NULL && strcmp(aux_alunos->aluno.nome, pesquisa_aluno->nome) > 0) {//Inserir aluno por ordem alfabetica
                            ant_aluno = pesquisa_aluno;
                            pesquisa_aluno = pesquisa_aluno->aluno_seguinte;
                        }
                        add_aluno = (Aluno) malloc(sizeof(struct infor_pauta));
                        add_aluno->nome = (char *) malloc(sizeof(char) * strlen(aux_alunos->aluno.nome));
                        strcpy(add_aluno->nome, aux_alunos->aluno.nome);
                        add_aluno->num = aux_notas->cadeira.num;
                        add_aluno->nota1 = aux_notas->cadeira.nota;
                        if (pesquisa_aluno == aux_pauta->infor_pauta) {//Insere no inicio da lista
                            pesquisa_aluno->aluno_anterior = add_aluno;
                            add_aluno->aluno_seguinte = pesquisa_aluno;
                            aux_pauta->infor_pauta = add_aluno;
                        } else {//Inserir no meio ou no fim
                            add_aluno->aluno_anterior = ant_aluno;
                            ant_aluno->aluno_seguinte = add_aluno;
                            add_aluno->aluno_seguinte = pesquisa_aluno;
                        }
                        if (add_aluno->aluno_seguinte != NULL) {//apontar o seguinte para si
                            add_aluno->aluno_seguinte->aluno_anterior = add_aluno;
                        }
                    }
                }
            }
        }
        aux_notas = aux_notas->nota_seguinte;
    }
    return 0;
}
