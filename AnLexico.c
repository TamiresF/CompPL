#include "AnLexico.h"

//Analisador lexico
Token lexico(FILE* fp) {

  char literal[255];      //guarda a constante literal
  char num[20];       //guarda a constante inteira e real
  int c;          //caractere lido do arquivo de entrada (codigo-fonte)
  int estadoAtual = 0;  //indica o estado atual no AF
  int b = 0;        //contador para as tabelas temporarias
  Token token;

while (1) {
       switch (estadoAtual) {

    case 0:            //estado inicial no AF
      c = fgetc(fp); //lê o proximo caractere

      if (c == ' ' || c == '\t')  estadoAtual = 0;//filtra delimitadores
      else if (isalpha(c))    estadoAtual = 1;//se ler uma letra
      else if (isdigit(c))    estadoAtual = 3;//se ler um digito
      else if (c == '\'')     estadoAtual = 6;
      else if (c == '\"')     estadoAtual = 10;
      else if (c == '<')      estadoAtual = 35;
      else if (c == '>')      estadoAtual = 38;
      else if (c == '=')      estadoAtual = 41;
      else if (c == '(')      estadoAtual = 15;
      else if (c == ')')      estadoAtual = 16;
      else if (c == '/')      estadoAtual = 31;
      else if (c == '{')      estadoAtual = 17;
      else if (c == '}')      estadoAtual = 18;
      else if (c == '|')      estadoAtual = 19;
      else if (c == ',')      estadoAtual = 21;
      else if (c == ';')      estadoAtual = 22;
      else if (c == '+')      estadoAtual = 23;
      else if (c == '-')      estadoAtual = 24;
      else if (c == '*')      estadoAtual = 25;
      else if (c == '\n')     estadoAtual = 29;
      else if (c == '#')      estadoAtual = 27;
      else if (c == '.')      estadoAtual = 50;
      else if (c==EOF)      estadoAtual = 48;
      else                estadoAtual = 47;//caractere invalido

      break;

      //AF para .and. .not. .or.
        case 50:
            token.lexema[b] = c;
			c = fgetc(fp); //le o proximo caractere
			b++;

			if (isalnum(c)) estadoAtual = 51;
			break;

        case 51:
            token.lexema[b] = c;
			c = fgetc(fp); //le o proximo caractere
			b++;

			if (isalnum(c)) estadoAtual = 51;
			else {
				token.lexema[b] = '\0'; //acrescenta o terminador nulo '\0'
				estadoAtual = 2;
			}
			break;
    //Fim AF para .and. .not. .or.

    //Palavras reservadas ou identificadores
    case 1:
      token.lexema[b] = c;
      c = fgetc(fp);
      b++;

      if (isalnum(c) || c == '_') estadoAtual = 1;
      else {
        token.lexema[b] = '\0'; //encerra
        estadoAtual = 2;
      }
      break;

    case 2:
      ungetc(c, fp); //devolve o caracter

      int p = buscarPR(token.lexema); //função compara os valores que foram guardados com as palavras reservadas

      if (p == -1) token.tipo = ID; //ID
      else {              //PR
        token.tipo = PR;
        token.valor.codPR = p;
      }
      return token;
      break;


    //constantes reais e inteiras
    case 3:
      num[b] = c;

      c = fgetc(fp);

      b++;

      if (isdigit(c)) estadoAtual = 3;
      else if (c == '.') {
        num[b] = c; //ponto decimal .
        b++;
        estadoAtual = 4;
      }
      else {
        num[b] = '\0'; //encerra
        estadoAtual = 13;
      }
      break;

    case 4:
      c = fgetc(fp);

      if (isdigit(c)) estadoAtual = 5;
      break;

    case 5:
      num[b] = c;

      c = fgetc(fp);

      b++;

      if (isdigit(c)) estadoAtual = 5;
      else {
        num[b] = '\0';
        estadoAtual = 14;
      }
      break;

    case 14:
      ungetc(c, fp);

      token.tipo = CTR;
      token.valor.valorReal = atof(num);
      return token;
      break;

    case 13:
      ungetc(c, fp);

      token.tipo = CTI;
      token.valor.valorInt = atoi(num);
      return token;
      break;


    //constantes caracteres
    case 6:
      c = fgetc(fp);
      if (c == '\\') estadoAtual = 8;
      else if (isprint(c) && c != '\'') estadoAtual = 9;
      else if (c == '\'') {
        token.valor.valorInt = -1;
        estadoAtual = 7;
      }
      break;

    case 8:
      c = fgetc(fp);

      if (c == 'n') {
        c = '\n';
        estadoAtual = 9;
      }
      else if (c == '0') {
        c = '\0';
        estadoAtual = 9;
      }
      break;

    case 9:
      token.valor.valorInt = c;

      c = fgetc(fp);

      if (c == '\'') estadoAtual = 7;
      break;

    case 7:
      token.tipo = CTC;
      return token;
      break;


    //constantes literais
    case 10:
      c = fgetc(fp);

      if (isprint(c) && c != '\"' && c != '\n') estadoAtual = 11;
      break;

    case 11:
      literal[b] = c;

      c = fgetc(fp);
      b++;

      if (isprint(c) && c != '\"' && c != '\n') estadoAtual = 11;
      else if (c == '\"') {
        literal[b] = '\0';
        estadoAtual = 12;
      }
      break;

    case 12:
      token.tipo = CTL;
      token.valor.posLiteral = inserirTbCstring(literal);
      return token;
      break;


    //<= ou <
    case 35:
      c = fgetc(fp);

      if (c == '=') estadoAtual = 36;
      else estadoAtual = 37;
      break;

    case 36:
      token.tipo = SN;
      strcpy (token.lexema, "<=");
      token.valor.codSN = sn_menor_igual;
      return token;
      break;

    case 37:
      ungetc(c, fp);
      token.tipo = SN;
      strcpy (token.lexema, "<");
      token.valor.codSN = sn_menor;
      return token;
      break;


    //>= ou >
    case 38:
      c = fgetc(fp);

      if (c == '=') estadoAtual = 39;
      else estadoAtual = 40;
      break;

    case 39:
      token.tipo = SN;
      strcpy (token.lexema, ">=");
      token.valor.codSN = sn_maior_igual;
      return token;
      break;

    case 40:
      ungetc(c, fp);

      token.tipo = SN;
      strcpy (token.lexema, ">");
      token.valor.codSN = sn_maior;
      return token;
      break;


    //== ou =
    case 41:
      c = fgetc(fp);

      if (c == '=') estadoAtual = 42;
      else estadoAtual = 45;
      break;

    case 42:
      token.tipo = SN;
      strcpy (token.lexema, "==");
      token.valor.codSN = sn_igualdade;
      return token;
      break;

    case 45:
      ungetc(c, fp);

      token.tipo = SN;
      strcpy (token.lexema, "=");
      token.valor.codSN = sn_atribuicao;
      return token;
      break;

    //( abre parentese
    case 15:
      token.tipo = SN;
      strcpy (token.lexema, "(");
      token.valor.codSN = sn_abre_parentese;
      return token;
      break;


        //) fecha parentese
    case 16:
      token.tipo = SN;
      strcpy (token.lexema, ")");
      token.valor.codSN = sn_fecha_parentese;
      return token;
      break;


    //comentarios ou /
    case 31:
      c = fgetc(fp);

      if (c == '*') estadoAtual = 32;
      else estadoAtual = 46;
      break;

    case 32:
      c = fgetc(fp);

      if (c == '*') estadoAtual = 33;
      else if (c==EOF) estadoAtual = 34;
      else estadoAtual = 32;
      break;

    case 33:
      c = fgetc(fp);
      if (c == '*') estadoAtual = 33;
      else if (c==EOF) estadoAtual = 34;
      else if (c == '/') estadoAtual = 34;
      else estadoAtual = 32;
      break;

    case 34:
      estadoAtual = 0;
      break;

    case 46:
      ungetc(c, fp);

      token.tipo = SN;
      strcpy (token.lexema, "/");
      token.valor.codSN = sn_divisao;
      return token;
      break;
    //comentarios ou /

     //{ abre chaves
    case 17:
      token.tipo = SN;
      strcpy (token.lexema, "{");
      token.valor.codSN = sn_abre_chave;
      return token;
      break;


    //} fecha chaves
    case 18:
      token.tipo = SN;
      strcpy (token.lexema, "}");
      token.valor.codSN = sn_fecha_chave;
      return token;
      break;


    //  ||
    case 19:
      c = fgetc(fp);

      if (c == '|') estadoAtual = 20;
      break;

    case 20:
      token.tipo = SN;
      strcpy (token.lexema, "||");
      token.valor.codSN = sn_or;
      return token;
      break;


    // virgula
    case 21:
      token.tipo = SN;
      strcpy (token.lexema, ",");
      token.valor.codSN = sn_virgula;
      return token;
      break;


    // ponto e virgula
    case 22:
      token.tipo = SN;
      strcpy (token.lexema, ";");
      token.valor.codSN = sn_ponto_virgula;
      return token;
      break;


    // mais +
    case 23:
      token.tipo = SN;
      strcpy (token.lexema, "+");
      token.valor.codSN = sn_mais;
      return token;
      break;


    // menos -
    case 24:
      token.tipo = SN;
      strcpy (token.lexema, "-");
      token.valor.codSN = sn_menos;
      return token;
      break;


    // *
    case 25:
      token.tipo = SN;
      strcpy (token.lexema, "*");
      token.valor.codSN = sn_vezes;
      return token;
      break;


    //quebra de linha
    case 29:
      ctlinha++;
      c = fgetc(fp);

      if (c == '\n') estadoAtual = 29;
      else estadoAtual = 30;
      break;

    case 30:
      ungetc(c, fp);
      estadoAtual = 0;
      break;


    // #
    case 27:
      token.tipo = SN;
      strcpy (token.lexema, "#");
      token.valor.codSN = sn_diferente;
      return token;
      break;

    case 47:
      printf("\nToken %c invalido na linha: %i", c,ctlinha);
      token.tipo = TKI;
      return token;
      break;

    case 48:
      printf("\nfinal de arquivo\n");
      token.tipo = EOF;
      return token;
      break;
    }
  }
}

int buscarPR(char lexema[31]) {

    int i;
  for (i = 0; i < 13; i++) {
    if (strcmp (tbPR[i], lexema) == 0) return i; //caso iguais, retorna a posicao da palavra-reservada na tabela
  }

  return -1; // se não é PR é um identificador
}


int inserirTbCstring (char literal[255]) {
int i = 0;
  while (i <= posFinal) {
        if (strcmp (tbCstring[i], literal) == 0) return i; //verifica se a constante já esta na tabela e retorna a posição
        i++;
 }
        posFinal++;
    strcpy (tbCstring[posFinal], literal); //insere nova constante
    return posFinal; //retorna a posicao
}



