#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//tabela palavras reservadas
typedef enum palReservada {
	Char,
	Int,
	Real,
	Bool,
	If,
	Else,
	While,
	func,
	noparam,
	pl,
	Return,
	fwd,
	For
}PalReservada;

//tabela sinal
typedef enum sinal{
	sn_igualdade,
	sn_atribuicao,
	sn_and,
	sn_or,
	sn_ponto_virgula,
	sn_virgula,
	sn_abre_parentese,
	sn_fecha_parentese,
	sn_abre_chave,
	sn_fecha_chave,
	sn_diferente,
	sn_negacao,
	sn_mais,
	sn_menos,
	sn_vezes,
	sn_divisao,
	sn_menor_igual,
	sn_menor,
	sn_maior_igual,
	sn_maior
}Sinal;

typedef enum categoria {
	PR,  //palavra reservada
	ID,  //identificador
	CTL, //cadeiacon
	CTI, //intcon
	CTC, //caraccon
	SN,  //sinal
	CTR,  //realcon
	TKI,	//Token Invalido
	EF		//Final de Arquivo
}Categoria;

//Estrutura do token
struct token {
	Categoria tipo;
	char lexema[31];
	union campo {
		PalReservada	codPR;
		Sinal			codSN;
		int				valorInt;
		double			valorReal;
		int				posLiteral;
	} valor;
};
typedef struct token Token;

struct ids {
	char nome[30];
	char tipo[13];
	char escopo[8];
	char categoria[6];
};
typedef struct ids Identificadores;

extern char tbPR [13][13];         //tabela de palavras-reservadas
extern char tbSN [20][3];          //tabela de simbolos de sinais
extern char tbCstring [255][255];  //tabela de constantes strings
extern int posFinal;               //indica posição da ultima string inserida na tabela de literais
extern int ctlinha;                // contador de linha

//Assinatura das funcoes
Token token;
Token lexico(FILE*);
int buscarPR(char []);
int inserirTbCstring(char []);
void imprime(Token);


