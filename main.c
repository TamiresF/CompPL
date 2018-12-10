#include <stdio.h>
#include <stdlib.h>

#include "AnLexico.h"

int ctlinha = 1; //contador de linhas

int main()
{
  Token token;
    char arq[20];
  FILE* file;

  printf ("Nome do arquivo: ");
  gets (arq); //armazena nome do arquivo

    if ((file = fopen (arq, "r")) == NULL) {
    printf ("\nErro ao abrir arquivo.\n");
        system("pause");
        exit(0);
  }
  Identificadores tbID[2000];
  lex(file);
  while (!feof (file)) {
    sintatico(file);
  }

  if (fclose(file)) {
    printf ("\nErro ao fechar o arquivo.");
    system("pause");
    exit(0);
  }
}

void imprime(Token token) {

  switch (token.tipo) {

  case PR:
    printf ("\n\n<PR, %s ,%d>", token.lexema, token.valor.codPR);
    break;

  case ID:
    printf ("\n\n<ID, %s>",token.lexema);
    break;

  case CTL:
    printf ("\n\n<CTL, %s , %d>", tbCstring[token.valor.posLiteral], token.valor.posLiteral);
    break;

  case CTI:
    printf ("\n\n<CTI,%d>", token.valor.valorInt);
    break;

  case CTC:
    if (token.valor.valorInt == -1) printf ("\n\n<CTC,  >");
    else if (token.valor.valorInt == '\n') printf ("\n\n<CTC,\n>");
    else if (token.valor.valorInt == '\0') printf ("\n\n<CTC,\0>");
    else printf ("\n\n<CTC, %c>", token.valor.valorInt);
    break;

  case SN:
    printf ("\n\n<SN, %s ,%d>", token.lexema, token.valor.codSN);
    break;

  case CTR:
    printf ("\n\n<CTR, %f>", token.valor.valorReal);
    break;

  case TKI:

    break;
  }
}
//tabela palavras reservadas
char tbPR [13][13] = {
  "Char",
  "Int",
  "Real",
  "Bool",
  "If",
  "Else",
  "While",
  "func",
  "noparam",
  "pl",
  "return",
  "fwd",
  "For"
};
//tabela sinais
char tbSN [20][3] = {
  "==", "=",  ".and.", ".or.", ";",
  ",",  "(",  ")",  "{",  "}"
  "#", ".not.",  "+",  "-",  "*",
  "/",  "<=", "<",  ">=", ">",
};

char tbCstring [255][255];   //tabela para guardar as constantes literais
int posFinal = -1;           //indica a posição da ultima constante inserida
