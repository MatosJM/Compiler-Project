%{ 
	#include <stdio.h>
	#include <ctype.h>
	#include <string.h>
	#include <stdlib.h>
	
	FILE *tok, *tab, *p_chave, *erro;
	int atrib, token;

	
	int getToken(char* lex);
	int getAtributo(char* lex);
	//int installID(char* lex, int token);
	void installAtrib(int token, int atr); //insere token e atributo na tabela de tokens
	void installToken(int token); //insere somente token  na tabela de tokens
	int getAscII(char *lex);
	
%}

/* definições regulares */
delim 				[ \t\n]
ws					{delim}+
letra 				[A-Za-z_]
digito 				[0-9]
id					{letra}({letra}|{digito})*

pontoevirgula 		[\;]
ponto 				[\.]
abrechave 			[\{]
fechachave 			[\}]
abrepar 			[\(]
fechapar 			[\)]
virgula 			[\,]
res_escopo 			[\:{2}] 
dois_pontos 		[:]
abre_col 			[\[]
fecha_col 			[\]]

numero				{digito}+(\.{digito}+)?(E[+-]?{digito}+)?
const_caract		\'(.)\'

op_rel_simples 		"<"|">"
opatr				"="
op_arit_simples 	[+\-] 
op_logic			"||"
op_mult				[\*\/\%]|"&&"
op_not				"!"
caract_desc			.  

%%
char 				{token = getToken(yytext); installToken(token);}		
class 				{token = getToken(yytext); installToken(token);}		
delete 				{token = getToken(yytext); installToken(token);}		
else 				{token = getToken(yytext); installToken(token);}		
float				{token = getToken(yytext); installToken(token);}
if					{token = getToken(yytext); installToken(token);}
int					{token = getToken(yytext); installToken(token);}
main				{token = getToken(yytext); installToken(token);}
new					{token = getToken(yytext); installToken(token);}
private				{token = getToken(yytext); installToken(token);}
public				{token = getToken(yytext); installToken(token);}
void				{token = getToken(yytext); installToken(token);}
while				{token = getToken(yytext); installToken(token);}
	
	
{ws}	
{id}				{atrib = getAtributo(yytext); token = getToken("ID"); installAtrib(token, atrib);}
{const_caract}		{atrib = getAtributo(yytext); token = getToken("CARACT"); installAtrib(token, atrib);}
{pontoevirgula}		{token = getAscII(yytext); installToken(token);}
{ponto}				{token = getAscII(yytext); installToken(token);}
{abrechave}			{token = getAscII(yytext); installToken(token);}
{fechachave}		{token = getAscII(yytext); installToken(token);}
{abrepar}			{token = getAscII(yytext); installToken(token);}
{fechapar}			{token = getAscII(yytext); installToken(token);}
{virgula}			{token = getAscII(yytext); installToken(token);}
{dois_pontos}		{token = getAscII(yytext); installToken(token);}
{abre_col}			{token = getAscII(yytext); installToken(token);}
{fecha_col}			{token = getAscII(yytext); installToken(token);}
{numero}			{atrib = atoi(yytext); token = getToken("NUM"); installAtrib(token, atrib);}
{op_rel_simples}	{atrib = getAscII(yytext); token = getToken("RELOP");installAtrib(token, atrib);}
{opatr}				{token = getAscII(yytext); installToken(token);}
"<="				{atrib = getToken("LE"); token = getToken("RELOP");installAtrib(token, atrib);}
">="				{atrib = getToken("GE"); token = getToken("RELOP");installAtrib(token, atrib);}
"=="				{atrib = getToken("EQ"); token = getToken("RELOP");installAtrib(token, atrib);}
"!="				{atrib = getToken("NE"); token = getToken("RELOP");installAtrib(token, atrib);}
"!"					{token = getAscII(yytext); installToken(token);}
{op_arit_simples}	{atrib = getAscII(yytext); token = getToken("ARITOP");installAtrib(token, atrib);}
"*"					{atrib = getAscII(yytext); token = getToken("MULTOP");installAtrib(token, atrib);}
"/"					{atrib = getAscII(yytext); token = getToken("MULTOP");installAtrib(token, atrib);}
"%"					{atrib = getAscII(yytext); token = getToken("MULTOP");installAtrib(token, atrib);}
"--"				{atrib = getToken("DECOP");  token = getToken("ARITOP");installAtrib(token, atrib);}
"++"				{atrib = getToken("INCOP");  token = getToken("ARITOP");installAtrib(token, atrib);}
"/="				{atrib = getToken("DE"); token = getToken("ARITOP");installAtrib(token, atrib);}
"*="				{atrib = getToken("ME"); token = getToken("ARITOP");installAtrib(token, atrib);}
"%="				{atrib = getToken("MODEQ"); token = getToken("ARITOP");installAtrib(token, atrib);}
"-="				{atrib = getToken("SE"); token = getToken("ARITOP");installAtrib(token, atrib);}
"&&"				{atrib = getToken("AND"); token = getToken("MULTOP");installAtrib(token, atrib);}
"||"				{atrib = getToken("OR"); token = getToken("LOGOP");installAtrib(token, atrib);}
{caract_desc}		{fprintf(erro,"erro! caractere %s desconhecido.\n",yytext);}

%%

int main(int argc, char *argv[]){
	++argv, --argc;
	if (argc > 0){
		yyin = fopen(argv[0],"r"); //abre o programa de entrada no modo leitura
	}else {
		yyin = stdin;
		}
	//tok=fopen("token.txt","w");
	//tab=fopen("tabela.txt","w");
	
	tok=fopen("tabelaTokens.txt","w");
	erro=fopen("erro.txt","w");
	tab=NULL;
	p_chave=fopen("palavras_chave.txt","r");
	
	yylex();
	
	fclose(tok);
	if(tab!=NULL){
		fclose(tab);
	}

	fclose(p_chave);
	fclose(erro);
	return 0;
}

int yywrap(){
	return 1;
}

int getToken(char* lex){
	int token = -1;
	char stringLinha[50];
	char *substring;
	
	fseek(p_chave,0,SEEK_SET);
	
	while(fgets(stringLinha, 80, p_chave)!=NULL && token ==-1){
		//printf("stringLinha: %s", stringLinha); //para mostrar as linhas do arquivo TABELA DE SIMBOLOS
		substring = strtok(stringLinha, " \t");
		//printf("substring: %s\n", substring); //mostra a substring extraida da linha
		
		if(strcmp(substring, lex)==0){
			
			substring = strtok(NULL, " \t");
			substring = strtok(NULL, " \t");
			token = atoi(substring);
				
		}
	
	}
	
	return token;
}

int getAtributo(char* lex){
	int atr = -1;
	int linha = 0;
	
	char atr_stringLinha[50];
	char *atr_substring;
	
	//printf("lex: %s\n", lex);
	atr_substring = NULL;

	
	
	if(tab==NULL){
		tab = fopen("tabela.txt","w");
		
	}else {
		freopen("tabela.txt","a+", tab);
		rewind(tab);
		
		while(fgets(atr_stringLinha, 80, tab)!=NULL && atr ==-1){
		//printf("stringLinha: %s", atr_stringLinha); //para mostrar as linhas do arquivo TABELA DE SIMBOLOS
		atr_substring = strtok(atr_stringLinha, " \t");
		//printf("substring: %s\n", atr_substring); //mostra a substring extraida da linha
			linha ++;
			if(strcmp(atr_substring, lex)==0){
				atr_substring = strtok(NULL, " \t");
				atr = atoi(atr_substring);
			}
			
		}
	}
	
	
	
	if(atr==-1){ //se nao encontrou o atributo, entao insere o ID na tabela e retorna atributo
		if(atr_substring!=NULL){
			atr_substring = strtok(NULL, " \t");
			//atr = atoi(atr_substring)+1;
			atr = linha + 1;
		}else{
			atr = linha+1;
		}
		
		fprintf(tab, "%s \t %d \n", lex, atr);
	}
	//fclose(tab);
	return atr;
}

void installAtrib(int token, int atr){
	
	//fseek(tab,0,SEEK_END);
	fprintf(tok, "%d \t %d\n", token, atr); 	
}

void installToken(int token){

	//fseek(tab,0,SEEK_END);
	fprintf(tok, "%d \t \n", token); 	
}

int getAscII(char *lex){
	char aux[1], ch; //caratere;
	int ret; //retorno;
	
	strcpy(aux,lex);
	ch = aux[0];
	ret = (int)ch;
	//printf("lex: %s  aux[0]: %c ch: %c  ret: %d", lex, aux[0], ch, ret);
	return ret;
}