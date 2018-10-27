#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//tabela palavras reservadas da linguagem PL
typedef enum palReservada {
    pl,
    var,
    endvar,
    prog,
    endprog,
	Char,
	Int,
	Real,
	Bool,
	noparam,
	enffunc,
	fwd,
	proc,
	endproc,
	se,
    endif,
	While,
	endwhile,
	For,
	endfor,
	Return,
	call,
	keybord,
	display
}PalReservada;

//tabela sinal
typedef enum sinal{
	sn_diferente,
	sn_negacao,
	sn_mais,
	sn_menos,
	sn_vezes,
	sn_divisao,
	sn_menor_igual,
	sn_menor,
	sn_maior_igual,
	sn_maior,
	sn_igualdade,
	sn_atribuicao,
	sn_and,
	sn_or,
	sn_ponto_virgula,
	sn_virgula,
	sn_abre_parentese,
	sn_fecha_parentese,
	sn_abre_chave,
	sn_fecha_chave
}Sinal;

typedef enum categoria {
	PR, ID, CTL,
	CTI, CTC, SN,
	CTR
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

extern char tabPR [100][100];        //tabela de simbolos de palavras-reservadas
extern char tabSN [20][3];         //tabela de simbolos de sinais
extern char tabCTL [255][255];	   //tabela de simbolos de constantes literais
extern int posUltimaCTL;		   //indica a posicao da ultima constante literal inserida na tabCTL
extern int contLinha;              //contador de linhas

//Assinatura das funcoes
FILE* abrirArq ();
void pausa ();
Token analex(FILE*);
void erro (int);
int buscarTabPR(char []);
int inserirTabCTL(char []);
void imprimirToken(Token);
void fecharArq (FILE*);

