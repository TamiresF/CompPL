#include <stdio.h>
#include <stdlib.h>

#include "AnLexico.h"

int contLinha = 1; //contador de linhas

int main()
{
	Token token;
    char arq[100];
	FILE* fp;

	printf ("Arquivo de Entrada: ");
	gets (arq);
	fflush (stdin);

    if ((fp = fopen (arq, "r")) == NULL) {
		printf ("\n\tErro na abertura de arquivo.\n");
        system("pause");
        exit(0);
	}

	while (!feof (fp)) {

		token = analex(fp); //analisador lexico (retorna proximo token)

		imprimirToken(token); //exibi as informacoes do token
	}

	if (fclose(fp)) { //retorna zero se a operação de fechamento for bem-sucedida
		printf ("\n\tErro ao fechar o arquivo.");
		system("pause");
		exit(0);
	}
}

char tabPR [100][100] = { //tabela de simbolos de palavras-reservadas
	"pl",	"var",	"endvar",	"prog",
	"endprog", "Char",	"Int",	"Real",
	"Bool",	"noparam", "enffunc", "fwd",
	"proc", "endproc", "If", "endif",
	"While", "endwhile", "For", "endfor",
	"Return", "call", "keybord", "display"
};

char tabSN [20][3] = { //tabela de simbolos de sinais
	"#",	".not.",	"+",	"-",	"*",
	"/",	"<=",	"<",	">=",	">",
	"==",	"=",	".and.",	".or.",	";",
	",",	"(",	")",	"{",	"}"
};

char tabCTL [255][255];   //tabela de simbolos de constantes literais
int posUltimaCTL = -1;    //indica a posicao da ultima constante literal inserida na tabCTL

