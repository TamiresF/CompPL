#include "AnLexico.h"

//analisador lexico
Token analex(FILE* fp) {

	char literal[255]; //armazena temporariamente a constante literal
	char num[20];		//armazena temporariamente a constante inteira e real
	int c;					//caractere lido do arquivo de entrada (codigo-fonte)
	int estadoAtual = 0;	//indica o estado atual no Automato Finito
	int b = 0;				//indica a proxima posicao ou do token.lexema ou do num ou do literal que sera inserido um caractere
	Token token;

//Identificação dos estados do Automato Finito
while (1) {
       switch (estadoAtual) {

		case 0: //estado inicial no AF
			c = fgetc(fp); //le o proximo caractere

			if (c == ' ' || c == '\t')  estadoAtual = 47;                  //filtra os espacos em branco e tabulações
			else if (isalpha(c))		estadoAtual = 1;                   //trata palavras-reservadas e identificadores
			else if (isdigit(c))		estadoAtual = 3;	               //trata constantes inteiras e reais
			else if (c == '\'')			estadoAtual = 6;                   //trata constantes caracteres APOSTROFO
			else if (c == '\"')			estadoAtual = 10;	               //trata constantes literais ASPAS
			else if (c == '<')			estadoAtual = 35;	               //trata <= ou <
			else if (c == '>')			estadoAtual = 38;	               //trata >= ou >
			else if (c == '=')			estadoAtual = 41;	               //trata == ou =
			else if (c == '(')			estadoAtual = 15;	               //trata (
			else if (c == ')')			estadoAtual = 16;	               //trata )
			else if (c == '/')			estadoAtual = 31;	               //filtra os comentarios e trata / operador divisão
			else if (c == '{')			estadoAtual = 17;	               //trata {
			else if (c == '}')			estadoAtual = 18;	               //trata }
			else if (c == ',')			estadoAtual = 21;	               //trata ,
			else if (c == ';')			estadoAtual = 22;	               //trata ;
			else if (c == '+')			estadoAtual = 23;	               //trata +
			else if (c == '-')			estadoAtual = 24;	               //trata -
			else if (c == '*')			estadoAtual = 25;	               //trata *
			else if (c == '\n')			estadoAtual = 29;	               //filtra as quebras de linha do arquivo (ENTER)
			else if (c == '#')			estadoAtual = 26;	               //trata # ou diferente
			else if (c == '.')          estadoAtual = 50;                 //trata .and. .or. .not.
			else				        estadoAtual = 49;	               //ERRO: caractere invalido

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

		//AF para delimitadores
        case 47:
			c = fgetc(fp); //le o proximo caractere

			if (c == ' ' || c == '\t') estadoAtual = 47; //filtra os espacos em branco e tabulacoes
			else estadoAtual = 48;
			break;

		case 48:
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)
			estadoAtual = 0;
			break;
        //Fim AF para delimitadores

        //AF para palavras-reservadas ou identificadores
		case 1:
			token.lexema[b] = c;
			c = fgetc(fp); //le o proximo caractere
			b++;

			if (isalnum(c)) estadoAtual = 1;
			else {
				token.lexema[b] = '\0'; //acrescenta o terminador nulo '\0'
				estadoAtual = 2;
			}
			break;

		case 2:
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			int p = buscarTabPR(token.lexema); //a cadeia coletada em token.lexema e buscada na tabela de simbolos de palavras-reservadas

			if (p == -1) token.tipo = ID;	//um identificador (ID)
			else {							//uma palavra-reservada (PR)
				token.tipo = PR;
				token.valor.codPR = p; //p e o indice (enum) da entrada da tabela de simbolos de palavras-reservadas para a cadeia em token.lexema
			}
			return token;
			break;
		//Fim AF para palavras-reservadas ou identificadores

		//AF para constantes reais e inteiras
		case 3:
			num[b] = c;

			c = fgetc(fp); //le o proximo caractere

			b++;

			if (isdigit(c)) estadoAtual = 3;
			else if (c == '.') {
				num[b] = c; //acrescenta o ponto decimal .
				b++;
				estadoAtual = 4;
			}
			else {
				num[b] = '\0'; //acrescenta o terminador nulo '\0'
				estadoAtual = 13;
			}
			break;

		case 4:
			c = fgetc(fp); //le o proximo caractere

			if (isdigit(c)) estadoAtual = 5;
			break;

		case 5:
			num[b] = c;

			c = fgetc(fp); //le o proximo caractere

			b++;

			if (isdigit(c))	estadoAtual = 5;
			else {
				num[b] = '\0'; //acrescenta o terminador nulo '\0'
				estadoAtual = 14;
			}
			break;

		case 14:            //constante REAL
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = CTR;
			token.valor.valorReal = atof(num);
			return token;
			break;

		case 13:           //constante Inteira
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = CTI;
			token.valor.valorInt = atoi(num);
			return token;
			break;
		//Fim AF para constantes reais e inteiras

		//AF para constantes caracteres
		case 6:
			c = fgetc(fp); //le o proximo caractere

            if (isprint(c) && c != '\'') estadoAtual = 8;
			else if (c == '\'') {
				token.valor.valorInt = -1; //ausencia de qualquer caractere
				estadoAtual = 7;
			}
			break;

		case 8:
			token.valor.valorInt = c;

			c = fgetc(fp); //le o proximo caractere

			if (c == '\'') estadoAtual = 7;
			break;

		case 7:
			token.tipo = CTC;
			return token;
			break;
		//Fim AF para constantes caracteres

		//AF para constantes literais
		case 10:
			c = fgetc(fp); //le o proximo caractere

			if (isprint(c) && c != '\"' && c != '\n') estadoAtual = 11;
			break;

		case 11:
			literal[b] = c;

			c = fgetc(fp); //le o proximo caractere

			b++;

			if (isprint(c) && c != '\"' && c != '\n') estadoAtual = 11;
			else if (c == '\"') {
				literal[b] = '\0'; //acrescenta o terminador nulo '\0'
				estadoAtual = 12;
			}
			break;

		case 12:
			token.tipo = CTL;
			token.valor.posLiteral = inserirTabCTL(literal);
			return token;
			break;
		//Fim AF para constantes literais

		//AF para <= ou <
		case 35:
			c = fgetc(fp); //le o proximo caractere

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
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = SN;
			strcpy (token.lexema, "<");
			token.valor.codSN = sn_menor;
			return token;
			break;
		//Fim AF para <= ou <

		//AF para >= ou >
		case 38:
			c = fgetc(fp); //le o proximo caractere

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
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = SN;
			strcpy (token.lexema, ">");
			token.valor.codSN = sn_maior;
			return token;
			break;
		//Fim AF para >= ou >

		//AF para == ou =
		case 41:
			c = fgetc(fp); //le o proximo caractere

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
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = SN;
			strcpy (token.lexema, "=");
			token.valor.codSN = sn_atribuicao;
			return token;
			break;
		//Fim AF para == ou =

		//AF para (
		case 15:
			token.tipo = SN;
			strcpy (token.lexema, "(");
			token.valor.codSN = sn_abre_parentese;
			return token;
			break;
		//Fim AF para (

        //AF para )
		case 16:
			token.tipo = SN;
			strcpy (token.lexema, ")");
			token.valor.codSN = sn_fecha_parentese;
			return token;
			break;
		//Fim AF para )

		//AF para comentarios ou /
		case 31:
			c = fgetc(fp); //le o proximo caractere

			if (c == '/') estadoAtual = 32;
			else estadoAtual = 46;
			break;

		case 32:
			c = fgetc(fp); //le o proximo caractere

			if (c == '\n') estadoAtual = 0;
			else estadoAtual = 32;
			break;

		case 46:
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)

			token.tipo = SN;
			strcpy (token.lexema, "/");
			token.valor.codSN = sn_divisao;
			return token;
			break;
		//Fim AF para comentarios ou /

		 //AF para {
		case 17:
			token.tipo = SN;
			strcpy (token.lexema, "{");
			token.valor.codSN = sn_abre_chave;
			return token;
			break;
		//Fim AF para {

		//AF para }
		case 18:
			token.tipo = SN;
			strcpy (token.lexema, "}");
			token.valor.codSN = sn_fecha_chave;
			return token;
			break;
		//Fim AF para }


		//AF para ,
		case 21:
			token.tipo = SN;
			strcpy (token.lexema, ",");
			token.valor.codSN = sn_virgula;
			return token;
			break;
		//Fim AF para ,

		//AF para ;
		case 22:
			token.tipo = SN;
			strcpy (token.lexema, ";");
			token.valor.codSN = sn_ponto_virgula;
			return token;
			break;
		//Fim AF para ;

		//AF para +
		case 23:
			token.tipo = SN;
			strcpy (token.lexema, "+");
			token.valor.codSN = sn_mais;
			return token;
			break;
		//Fim AF para +

		//AF para -
		case 24:
			token.tipo = SN;
			strcpy (token.lexema, "-");
			token.valor.codSN = sn_menos;
			return token;
			break;
		//Fim AF para -

		//AF para *
		case 25:
			token.tipo = SN;
			strcpy (token.lexema, "*");
			token.valor.codSN = sn_vezes;
			return token;
			break;
		//Fim AF para *

		//AF para quebra de linha
		case 29:
			contLinha++;
			c = fgetc(fp); //le o proximo caractere

			if (c == '\n') estadoAtual = 29; //filtra as quebras de linha (ENTER)
			else estadoAtual = 30;
			break;

		case 30:
			ungetc(c, fp); //devolve caractere a entrada (arquivo-fonte)
			estadoAtual = 0;
			break;
		//Fim AF para quebra de linha

		//AF para # diferente
		case 26:
			token.tipo = SN;
			strcpy (token.lexema, "#");
			token.valor.codSN = sn_diferente;
			return token;
			break;
		//Fim AF para #

		}
	}
}

int buscarTabPR(char lexema[31]) {

    int i;
	for (i = 0; i < 12; i++) {
		if (strcmp (tabPR[i], lexema) == 0) return i; //caso iguais, retorna a posicao da palavra-reservada na tabPR
	}

	return -1; //nao e uma palavra reservada
}

int inserirTabCTL (char literal[255]) {

	if (posUltimaCTL != -1) {

         int i = 0;
		 while (i <= posUltimaCTL) {
		       if (strcmp (tabCTL[i], literal) == 0) return i; //caso iguais, retorna a posicao da constante literal na tabCTL
               i++;
         }
	}
        posUltimaCTL++;
		strcpy (tabCTL[posUltimaCTL], literal); //inseri nova constante literal
		return posUltimaCTL; //retorna a posicao da constante literal na tabCTL
}

void imprimirToken(Token token) {

	switch (token.tipo) {

	case PR:
		printf ("\n\n<PR,%s,%d>", token.lexema, token.valor.codPR);
		break;

	case ID:
		printf ("\n\n<ID, %s>",token.lexema);
		break;

	case CTL:
		printf ("\n\n<CTL,%s,%d>", tabCTL[token.valor.posLiteral], token.valor.posLiteral);
		break;

	case CTI:
		printf ("\n\n<CTI,%d>", token.valor.valorInt);
		break;

	case CTC:
		if (token.valor.valorInt == -1) printf ("\n\n<CTC,  >");
		else if (token.valor.valorInt == '\n') printf ("\n\n<CTC,\n>");
		else if (token.valor.valorInt == '\0') printf ("\n\n<CTC,\0>");
		else printf ("\n\n<CTC,%c>", token.valor.valorInt);
		break;

	case SN:
		printf ("\n\n<SN,%s,%d>", token.lexema, token.valor.codSN);
		break;

	case CTR:
		printf ("\n\n<CTR,%f>", token.valor.valorReal);
		break;
	}
}


