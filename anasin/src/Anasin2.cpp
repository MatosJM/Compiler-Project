#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

FILE *tok, *tab, *pchave, *erro;//, *prog;

int lookahead, atrib, linha, linhaErro;
int tp[5];//vetor para armazenar os primeiros token de Tipos
int ft[6]; //vetor para armazenar os primeiros tokens de Fator
int finst[5];//vetor para armazenar os primeiros token de Inst

int nextToken(); //retorna o proximo token da tabela de tokens gerados pelo ANALEX
int getToken(char* lex); //retorna o token da palavra-chave  

char* getLex(); //retorna o lexema do lookahead;

void error(); //insere o erro e descricao na tabela de erros;
int errorEmpty(); //se a tabela de erros estiver vazia retorna 1, caso contrario retorna 0 
void reconhecer (int t); //funcao para casar o lookahead com os tokens da gramatica


void setTipos();
void setFirstInsts();	
void setFirstFator();

void programa();
void decClasse();
void rest();
void classe();
void corpoClasse();
void sessPriv();
void sessPubl();
void decs();
void rest2();
void dec();
void dec2();
void tipo();
void expr();
void rest3();
void exprSimples();
void rest4();
void termo();
void rest5();
void fator();
void fator2();
void fator3();
void listExprs();
void rest6();
void argumentos();
void rest7();
void impMetodos();
void rest8();
void metodo();
void metodo2();
void params();
void rest9();
void param();
void corpoMetodo();
void insts();
void rest10();
void inst();
void inst2();
void inst3();
void inst4();
void design();
void rest11();


int main(int argc, char *argv[]){
//	++argv, --argc;
	char command[200];
	atrib = 0;
	linha = 1;
	linhaErro = 1;
	lookahead = 0;
	if (argc > 0){
	//	prog = fopen(argv[0],"r"); //abre o programa de entrada no modo leitura
		sprintf(command, "lex.yy.exe %s", argv[1]);
		printf("\ncommand: %s\n", command);
		system(command);
		
	}else {
	//	prog = stdin;
		}
	tok=fopen("tabelaTokens.txt","r");
	erro=fopen("erro_anasin.txt","w");
	
	tab=fopen("tabela.txt","r");
	pchave=fopen("palavras_chave.txt","r");
	
	setTipos();
	setFirstInsts();
	setFirstFator();
	lookahead = nextToken();
	programa();
	if(errorEmpty()==1)	printf("\nLinguagem Reconhecida!", lookahead);
	fclose(tok);
	fclose(erro);
	fclose(tab);
	fclose(pchave);
	return 0;
}

int nextToken()
{ //retorna o primeiro token da tabela de tokens gerados pelo ANALEX
	int token = -1;
	char stringLinha[80];
	char *substring;
	substring = NULL;
	
	if(tok==NULL){
		printf("Erro na leitura do arquivo!\n");
		
	}else {
		fgets(stringLinha, 80, tok)!=NULL;
		
		//printf("stringLinha: %s", stringLinha); //para mostrar as linhas do arquivo TABELA DE SIMBOLOS
		substring = strtok(stringLinha, " \t");
		//printf("substring: %s\n", atr_substring); //mostra a substring extraida da linha
			//linha ++;
			token = atoi (substring);
			printf("\nNexToken: %d\t", token);
			substring = strtok(NULL, " \t");
			//printf("\tsubstring: %s \t", substring);
			linhaErro = linha;
			linha = atoi(substring);
			atrib = 0;
			
			substring = strtok(NULL, " \t");
			if(atoi(substring)>0){
				
				atrib = linha; //linha estava pegando o valor do atributo para ID e Constantes
				linha = atoi(substring); //valor de linha atualizado conforme esta na coluna da tabela de tokens
			}
			
		}
		printf("\tAtrib: %d \t", atrib);
	//printf("\nNexToken: %d\t\t", token);
	
	return token;
}

int getToken(char* lex)
{ //retorna o token da palavra-chave
	int token = -1;
	char stringLinha[80];
	char *substring;
	char *temp;
	
	fseek(pchave,0,SEEK_SET);
	
	while(fgets(stringLinha, 80, pchave)!=NULL && token ==-1)
	{
		//printf("stringLinha: %s", stringLinha); //para mostrar as linhas do arquivo TABELA DE SIMBOLOS
		substring = strtok(stringLinha, " \t");
		//printf("substring: %s\n", substring); //mostra a substring extraida da linha
		
		if(strcmp(substring, lex)==0)
		{
			temp = substring;
			//printf("Tokenlex: %s", substring); //mostra a substring extraida da linha
			substring = strtok(NULL, " \t");
			substring = strtok(NULL, " \t");
			token = atoi(substring);
			if(lookahead==token){
				printf("Tokenlex: %s", temp);
			}
			
		}
	
	}
	
	return token;
}

int getAscII(char *lex){
	char aux[1], ch; //caratere;
	int ret; //retorno;
	
	strcpy(aux,lex);
	ch = aux[0];
	ret = (int)ch;
	if(lookahead == ret){
		printf("AscIIlex: %s", lex);
	}
	
	return ret;
}

char* getLex(){ //retorna o lexema do token
	char* rLex=NULL;
	char stringLinha3[80];
	char *substring3;
	char *temp3;
	//printf("Atrib GetLex: %d\n", atrib);
	//printf("Lookahead GetLex: %d\n", lookahead);
	if(atrib>0) //retorna o lexema na tabela de simbolos
	{
		fseek(tab,0,SEEK_SET);
		while(rLex==NULL && fgets(stringLinha3, 80, tab)!=NULL)
		{
			substring3 = strtok(stringLinha3, " \t");
			temp3 = substring3;
			substring3 = strtok(NULL, " \t");
		//	printf("atrib>0: temp3 =  %s\n", temp3);
		//	printf("atrib>0: Substring3 =  %s\n", substring3);
			if(atoi(substring3)==atrib)
			{
				rLex = temp3;
			//	strcpy(rLex,temp3);
			}
			
		}
	}
	else if(atrib==0 && lookahead <=256) //retorna o lexema da tabela ASCII
	{
		int u = 0;
	//	char aux = (char)lookahead;
		char aux[2];
		
		//printf("aux: %c\n", aux);
		
		u = sprintf(aux,"%c", (char)lookahead);
		rLex = aux;
		//strcpy(rLex,aux2);
	//	printf("rLexaux: %s\n", aux2);
	}
	else if(atrib==0 && lookahead >256) //retorna o lexema da tabela de palavras-chave
	{
		fseek(pchave,0,SEEK_SET);
		while(rLex==NULL && fgets(stringLinha3, 80, pchave)!=NULL)
		{
		//	printf("stringLinha: %s", stringLinha3); //para mostrar as linhas do arquivo TABELA DE SIMBOLOS
			substring3 = strtok(stringLinha3, " \t");
		//	printf("substring: %s\n", substring3); //mostra a substring extraida da linha
			temp3 = substring3;
			substring3 = strtok(NULL, " \t");
			substring3 = strtok(NULL, " \t");
		//	printf("atrib==0: temp3 =  %s\n", temp3);
		//	printf("atrib==0: Substring3 =  %s\n", substring3);
			if(atoi(substring3)==lookahead) //true quando o token esta na tabela de palavras-chave
			{	
				rLex = temp3;
				//strcpy(rLex, temp3);	
			}
		}
	}
	printf("rLex: %s\n", rLex);
	return rLex;
}

void error()
{
	char result[15];
	strcpy(result, getLex());
	
 //printf("Result: %s\n", result);
 if(lookahead>0){
 	printf("Linha: %d: Erro! antes de %s\n", linhaErro, result);
 	fprintf(erro, "Linha: %d: Erro! antes de %s\n", linhaErro, result);
 }else{
 	printf("Linha: %d: Erro!\n", linhaErro);
 	fprintf(erro, "Linha: %d: Erro!\n", linhaErro);
 }
 
}

void reconhecer (int t)
{ if (lookahead == t) lookahead = nextToken();
  else error (); 
}

int errorEmpty()
{
	int ret = 1;
	char stringLinha2[80];
	//fseek(erro,0,SEEK_SET);
	freopen("erro_anasin.txt","r", erro);

	rewind(erro);

	while(fgets(stringLinha2, 80, erro)!=NULL && ret ==1)
	{
		ret = 0;
	}
//	printf("\nret: %d", ret);
	return ret;
}

void setTipos(){
	printf("\n-----Setando os tipos ------------------------\n");
	tp[0] = getToken("char");
	tp[1] = getToken("int");
	tp[2] = getToken("float");
	tp[3] = getToken("void");
	tp[4] = getToken("ID");
	
}

void setFirstInsts(){
	printf("\n-----Setando os parametros de FirstInst ------\n");
	finst[0] = getToken("ID");
	finst[1] = getToken("if");
	finst[2] = getToken("while");
	finst[3] = getAscII("{");
	finst[4] = getToken("delete");
}
		
void setFirstFator(){
	printf("\n-----Setando os parametros de FirstFator ------\n");
	ft[0] = getToken("ID");
	ft[1] = getToken("NUM");
	ft[2] = getAscII("(");
	ft[3] = getAscII("!");
	ft[4] = getToken("CARACT");
	ft[5] = getToken("new");
}
	

void programa()
{	
	decClasse();
	impMetodos();
	/**
	t = getToken("class");
	if(lookahead==t){
		decClasse();
		impMetodos();
	}else error();
	/**/
}
void decClasse()
{
	//t = getToken("class"); //CLASS
	/**/
	//if(lookahead==t){
		classe();
		rest();
//	}else error();
	
	/**/
}
void rest(){
	//if(lookahead==t){
	if(lookahead==getToken("class")){
		classe();
		rest();
	}
}

void classe(){
//	if(lookahead==t){
	if(lookahead==getToken("class")){
		reconhecer(lookahead);
		reconhecer(getToken("ID"));
		corpoClasse();
		reconhecer(getAscII(";"));	
	}else error();
}

void corpoClasse(){
	if(lookahead==getAscII("{")){
		reconhecer(lookahead);
		sessPriv();
		sessPubl();
		reconhecer(getAscII("}"));	
	}
}

void sessPriv(){
	if(lookahead==getToken("private")){
		reconhecer(lookahead);
		reconhecer(getAscII(":"));
		decs();
	}
}
void sessPubl(){
	if(lookahead==getToken("public")){
		reconhecer(lookahead);
		reconhecer(getAscII(":"));
		decs();
	}
}

void decs(){
	
	/**
	printf("\n-----setando os tipos ------");
	tp[0] = getToken("char");
	tp[1] = getToken("int");
	tp[2] = getToken("float");
	tp[3] = getToken("void");
	tp[4] = getToken("ID");
	/**/
	/**/
	for(int i=0; i<5; i++){
		if(lookahead==tp[i]){
			dec();
			rest2();
			break;	
		}else if(i==5){
			error();
		}
	}
	/**/

}

void rest2(){
	/**
	tp[0] = getToken("char");
	tp[1] = getToken("int");
	tp[2] = getToken("float");
	tp[3] = getToken("void");
	tp[4] = getToken("ID");
	/**/
	/**/
	for(int i=0; i<5; i++){
		if(lookahead==tp[i]){
			dec();
			rest2();
			break;	
		}
	}
	/**/
	
}
void dec(){
	tipo();
	reconhecer(getToken("ID"));
	dec2();
	
}

void dec2(){
	//printf("\n------dec2 ------------------\n");
	//printf("lookahead: %d\n", lookahead);
	if(lookahead==getAscII(";")){
		reconhecer(lookahead);
	}else if(lookahead==getAscII("[")){
		reconhecer(lookahead);
		reconhecer(getToken("NUM"));
		reconhecer(getAscII("]"));
		reconhecer(getAscII(";"));
		
	}else if (lookahead==getAscII("(")){
		reconhecer(lookahead);
		argumentos();
		reconhecer(getAscII(")"));
		reconhecer(getAscII(";"));
	}else error();
}

void tipo(){
	/**
	int tp[5]; //variavel local transformada em variavel global;
	tp[0] = getToken("char");
	tp[1] = getToken("int");
	tp[2] = getToken("float");
	tp[3] = getToken("void");
	tp[4] = getToken("ID");
	/**/
	for(int i=0; i<5; i++){
		if(lookahead==tp[i]){
			reconhecer(tp[i]);
			break;	
		}else if(i==5){
			error();
		}
	}	
		
}

void expr(){
	//printf("\n------------------expr-----------------\n");
	//int ft[6]; //variavel local transformada em variavel global;
	
	/**
	ft[0] = getToken("ID");
	ft[1] = getToken("NUM");
	ft[2] = getAscII("(");
	ft[3] = getAscII("!");
	ft[4] = getToken("CARACT");
	ft[5] = getToken("new");
	/**/
//	for(int i=0; i<6; i++){
//		if(lookahead==ft[i]){
			exprSimples();
			rest3();
//			break;	
//		}//else if(i==6)error();
		
//	}	
}

void rest3(){
	if(lookahead==getToken("RELOP")){
		reconhecer(lookahead);
		exprSimples();
		rest3();	
	}
}

void exprSimples(){
	//for(int i=0; i<6; i++){
	//	if(lookahead==ft[i]){
			termo();
			rest4();
	//		break;	
	//	}//else if(i==6)error();
		
	//}	
	
	
}
void rest4(){
	if(lookahead==getToken("ARITOP")){
		reconhecer(lookahead);
		termo();
		rest4();	
	}else if (lookahead==getToken("LOGOP")){
		reconhecer(lookahead);
		termo();
		rest4();
	}
}

void termo(){
	fator();
	rest5();
}

void rest5(){
	if(lookahead==getToken("MULTOP")){
		reconhecer(lookahead);
		fator();
		rest5();	
	}
}
void fator(){
	
	if(lookahead==getToken("ID")){
		design();
		fator2();	
	}else if(lookahead==getToken("NUM")){
		reconhecer(lookahead);
	}else if(lookahead==getAscII("(")){
		reconhecer(lookahead);
		expr();
		reconhecer(getAscII(")"));
	}else if(lookahead==getAscII("!")){
		reconhecer(lookahead);
		fator();
	}else if(lookahead==getToken("CARACT")){
		reconhecer(lookahead);
	}else if(lookahead==getToken("new")){
		reconhecer(lookahead);
		reconhecer(getToken("ID"));
		reconhecer(getAscII("("));
		fator3();
	}else error();
	

	
}

void fator2(){
	if(lookahead==getAscII("(")){
		reconhecer(lookahead);
		listExprs();
		reconhecer(getAscII(")"));	
	}else if(lookahead==getAscII("[")){
		reconhecer(lookahead);
		listExprs();
		reconhecer(getAscII("]"));
	}
}

void fator3(){
	if(lookahead==getAscII("(")){
		listExprs();
		reconhecer(getAscII(")"));	
	}else if(lookahead==getAscII(")")){
		reconhecer(lookahead);
	}else error();	
}

void listExprs(){
	expr();
	rest6();
}

void rest6(){
	if(lookahead==getAscII(",")){
		reconhecer(lookahead);
		expr();
		rest6();	
	}
}

void argumentos(){
	tipo();
	rest7();
}

void rest7(){
	if(lookahead==getAscII(",")){
		reconhecer(lookahead);
		tipo();
		rest7();	
	}
}

void impMetodos(){
	metodo();
	rest8();
}

void rest8(){
	for(int i=0; i<5; i++){
		if(lookahead==tp[i]){
			metodo();
			rest8();
			break;	
		}
	}
}

void metodo(){
	tipo();
	metodo2();
}

void metodo2(){
	//printf("metodo2: -------------------\n");
	//printf("lookahead: %d\n", lookahead);
	if(lookahead==getToken("ID")){
		reconhecer(lookahead);
		reconhecer(getToken("RESESC"));
		reconhecer(getToken("ID"));
		reconhecer(getAscII("("));
		params();
		reconhecer(getAscII(")"));
		corpoMetodo();
	}else if(lookahead==getToken("main")){
		reconhecer(lookahead);
		reconhecer(getAscII("("));
		reconhecer(getAscII(")"));
		corpoMetodo();
	}else error();
}

void params(){
	for(int i=0; i<5; i++){
		if(lookahead==tp[i]){
			param();
			rest9();
			break;	
		}else if(i==5) error();
	}
}


void rest9(){
	if(lookahead==getAscII(",")){
		reconhecer(lookahead);
		param();
		rest9();	
	}
}

void param(){
	tipo();
	reconhecer(getToken("ID"));
}

void corpoMetodo(){
	if(lookahead==getAscII("{")){
		reconhecer(lookahead);
		insts();
		reconhecer(getAscII("}"));	
	}else error();
}

void insts(){
	/**
	finst[0] = getToken("ID");
	finst[1] = getToken("if");
	finst[2] = getToken("while");
	finst[3] = getAscII("{");
	finst[4] = getToken("delete");
	/**/
	
	for(int i=0; i<5; i++){
		if(lookahead==finst[i]){
			//printf("entrei em insts\n");
			inst();
			rest10();
			break;	
		}else if(i==5) error();
	}	
}

void rest10(){
	//printf("entrei em rest10\n");
	for(int i=0; i<5; i++){
		if(lookahead==finst[i]){
			inst();
			rest10();
			break;	
		}
	}	
}

void inst(){
	//printf("--------------------------inst ---------------\n");
	for(int i=0; i<5; i++){
		if(lookahead==finst[i]){
			switch (i){
				case 0:{
					design();
					inst2();	
					break;
				}
				case 1:{
					reconhecer(lookahead);
					reconhecer(getAscII("("));
					expr();
					reconhecer(getAscII(")"));
					inst();
					inst4();
					
					break;
				}
				case 2:{
					reconhecer(lookahead);
					reconhecer(getAscII("("));
					expr();
					reconhecer(getAscII(")"));
					inst();
					break;
				}case 3:{
					reconhecer(lookahead);
					inst();
					reconhecer(getAscII("{"));
					break;
				}case 4:{
					reconhecer(lookahead);
					design();
					reconhecer(getAscII(";"));
					break;
				}
				default: error();
			}
		}else if(i==5) error();
	}	
}

void inst2(){
	//printf("--------------------------inst2 ---------------\n");
	if(lookahead==getAscII(";")){
		reconhecer(lookahead);	
	}else if(lookahead==getAscII("=")){
		reconhecer(lookahead);
		expr();
		reconhecer(getAscII(";"));	
	}else if(lookahead==getAscII("[")){
		reconhecer(lookahead);
		exprSimples();
		reconhecer(getAscII("]"));
		reconhecer(getAscII("="));
		expr();	
		reconhecer(getAscII(";"));	
	}else if(lookahead==getAscII("(")){
		reconhecer(lookahead);
		inst3();
	}//else error();
}

void inst3(){
	//printf("--------------------------inst3 ---------------\n");
	for(int i=0; i<6; i++){
		if(lookahead==ft[i]){
			listExprs();
			reconhecer(getAscII(")"));
			reconhecer(getAscII(";"));
			break;	
		}
	}	
	
	if(lookahead==getAscII(")")){
		reconhecer(lookahead);
		reconhecer(getAscII(";"));	
	}else error();
}

void inst4(){
//	printf("--------------------------inst4 ---------------\n");
	if(lookahead==getToken("else")){
		reconhecer(lookahead);
		inst();	
	}
}

void design(){
//	printf("--------------------------design ---------------\n");
	if(lookahead==getToken("ID")){
		reconhecer(lookahead);
		rest11();	
	}else error();
	
}

void rest11(){
//	printf("\n--------------------------rest11 ---------------\n");

	if(lookahead==getAscII(".")){
		reconhecer(lookahead);
		reconhecer(getToken("ID"));
		rest11();	
	}
}

