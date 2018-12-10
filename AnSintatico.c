#include "AnLexico.h"

void lex (FILE* file) {
	token=lexico(file);
}
// Operadores
int op_Rel(FILE* file){
	if((token.tipo==SN) &&
		(token.valor.codSN==sn_maior_igual
		|| token.valor.codSN==sn_maior
		|| token.valor.codSN==sn_igualdade
		|| token.valor.codSN==sn_menor_igual
		|| token.valor.codSN==sn_menor
		|| token.valor.codSN==sn_diferente)) {
       token=lexico(file);
       return 1; }
	else
	   return 0;
	   return 0;
}

void termo(FILE* file){
   fator(file);
   while(((token.valor.codSN==sn_and)
   		||(token.valor.codSN==sn_vezes)
   		||(token.valor.codSN==sn_divisao))
   		&&(token.tipo==SN)){
		token=lexico(file);
		fator(file);
		}
}

// Expressão Simples
void expr_Simp(FILE* file){

	 if((token.valor.codSN==sn_menos||token.valor.codSN==sn_mais)&&(token.tipo==SN)) token=lexico(file);
	 termo(file);

	 while(((token.valor.codSN==sn_menos)||(token.valor.codSN==sn_mais)||(token.valor.codSN==sn_or))&&(token.tipo==SN)){
		token=lexico(file);
		termo(file);
		}
}

void expr(FILE* file){
   expr_Simp(file);
   if(op_Rel(file)) expr_Simp(file);
}
// Atribuição
void atrib(FILE* file){
	if(token.tipo==ID){
		token=lexico(file);
		if((token.valor.codSN==sn_atribuicao)&&(token.tipo==SN)){
					token=lexico(file);
					expr(file);	 }
		else {
		  printf("Atencao: = esperado na linha %d",ctlinha);
	   	  system("PAUSE");
 		exit(1); 	}
	}
		 else {
		  printf("Atencao: ID esperado na linha %d",ctlinha);
	   	  system("PAUSE");
		exit(1); }
}

// fator
void fator(FILE* file){

	if((token.tipo==CTR)|| (token.tipo==CTL)|| (token.tipo==CTI)|| (token.tipo==CTC)) {
	    token=lexico(file);
	}
	else if(token.valor.codSN==sn_negacao && token.tipo==SN){
		token=lexico(file);
		fator(file);
	}

	else if(token.valor.codSN==sn_abre_parentese && token.tipo==SN){
		token=lexico(file);
		expr(file);
		if(!((token.valor.codSN==sn_fecha_parentese)&&(token.tipo==SN))){
			printf("Atencao: )  esperado na linha %d",ctlinha);
	 		system("PAUSE");
 	exit(1);
		}
		token=lexico(file);
	}

	else if(token.tipo==ID){
		token=lexico(file);
		if(token.valor.codSN==sn_abre_parentese && token.tipo==SN){
			token=lexico(file);
			if( token.valor.codSN!=sn_fecha_parentese){
		 	   	expr(file);
				while(token.valor.codSN==sn_virgula && token.tipo==SN){
					token=lexico(file);
					expr(file);
				}
			}
			if(token.valor.codSN==sn_fecha_parentese && token.tipo==SN) token=lexico(file);
			else{
				printf("Atencao: ) esperado na linha %d",ctlinha);
	            system("PAUSE");
 	exit(1);
	        }
		}
	}

	else{
   		printf("Atencao: fator esperado na linha %d",ctlinha);
        system("PAUSE");
 	exit(1);
	}

 }

void tipo(FILE* file){
	if((token.tipo==PR) && ( (token.valor.codPR==Char)||(token.valor.codPR==Int)||(token.valor.codPR==Real)||(token.valor.codPR==Bool) ))
	     token=lexico(file);
	else {
	 printf("Atencao: Tipo esperado na linha %d",ctlinha);
	 system("PAUSE");
 	exit(1);
	 }
}
// tipos_Param
void tipos_Param(FILE* file){
	if((token.tipo==PR)&&(token.valor.codPR==noparam)) token=lexico(file);
    else{
	 	tipo(file);
 		if(token.tipo==ID){
		   token=lexico(file);
		   while((token.valor.codSN==sn_virgula)&&(token.tipo==SN)){
                token=lexico(file);
				tipo(file);
				if(token.tipo==ID)
				      token=lexico(file);
				else{
					 printf("Atencao: ID esperado na linha %d",ctlinha);
		  			 system("PAUSE");
 	exit(1);
				 }
			}
	   	}
	    else{
		 	printf("Atencao: ID esperado na linha %d",ctlinha);
			system("PAUSE");
 	exit(1);
		}
	}
}
// Cmd
void cmd(FILE* file){
	if((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN)) token=lexico(file);

	else if((token.valor.codPR==For)&&(token.tipo==PR)){
   	   token=lexico(file);
	   if((token.valor.codSN==sn_abre_parentese)&&(token.tipo==SN)){
				token=lexico(file);
			if(token.tipo!= SN || token.valor.codSN!=sn_ponto_virgula) atrib(file);
			if((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN)) token=lexico(file);
			else{
                printf("Atencao: ; esperado na linha %d",ctlinha);
                system("PAUSE");
 				exit(1);
            }

			if(!((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN))) expr(file);

			if((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN))token=lexico(file);
			else{
				 printf("Atencao: ; esperado na linha %d ",ctlinha);
                 system("PAUSE");
 				 exit(1);
            }
			if(!((token.valor.codSN==sn_fecha_parentese)&&(token.tipo==SN))) atrib(file);
			if((token.valor.codSN==sn_fecha_parentese)&&(token.tipo==SN)) token=lexico(file);
			else{
				printf("Atencao: ) esperado na linha %d",ctlinha);
                system("PAUSE");
 				exit(1);
            }
			cmd(file);
		}
		else{
	 	  printf("Atencao: ( esperado na linha %d",ctlinha);
          system("PAUSE");
 			exit(1);
    	}
	}
// Construção IF
	else if((token.valor.codPR==If)&&(token.tipo==PR)){
		token=lexico(file);
		if((token.valor.codSN==sn_abre_parentese)&&(token.tipo==SN)){
			token=lexico(file);
			expr(file);
 		 	if((token.valor.codSN==sn_fecha_parentese)&&(token.tipo==SN)){
 				token=lexico(file);
				cmd(file);
					if((token.valor.codPR==Else)&&(token.tipo==PR)){
					token=lexico(file);
					cmd(file); } }
		 	else{
			    printf("Atencao: ) esperado na linha %d",ctlinha);
			    system("PAUSE");
 				exit(1); }
        }
		else{
			 printf("Atencao: (  esperado na linha %d",ctlinha);
			 system("PAUSE");
 			exit(1);
	  	}
  	}

	else if((token.valor.codPR==Return)&&(token.tipo==PR))  {

	   	token=lexico(file);
		if(!((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN))) expr(file);
		if((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN))token=lexico(file);
		else{
	 	printf("Atencao: ; esperado linha %d",ctlinha);
            system("PAUSE");
 	exit(1);
        }
	}

	else if(token.tipo==ID){

		token=lexico(file);
		if((token.valor.codSN==sn_abre_parentese)&&(token.tipo==SN)){
		  	token=lexico(file);
			if(!((token.tipo==SN)&&(token.valor.codSN==sn_fecha_parentese))){
  				expr(file);
				while((token.valor.codSN==sn_virgula)&&(token.tipo==SN)){
					token=lexico(file);
					expr(file);
				}
	  		}
	  		token=lexico(file);
			if((token.valor.codSN==sn_ponto_virgula)&&(token.tipo==SN)){ token=lexico(file); }
	  		else{
				printf("Atencao: ;  esperado na linha %d",ctlinha);
           		system("PAUSE");
 				exit(1);
     		}
		}
		else if (token.valor.codSN==sn_atribuicao && token.tipo==SN) {
			token=lexico(file);
			expr(file);
			if(token.valor.codSN==sn_ponto_virgula && token.tipo==SN) token=lexico(file);
	  		else{
				printf("Atencao: ;  esperado na linha %d",ctlinha);
           		system("PAUSE");
 				exit(1);
     		}
		}
	 	else{
	 	  printf("Atencao: ( esperado na linha %d",ctlinha);
          system("PAUSE");
 		  exit(1);
      	}
 	}

  	else if((token.valor.codPR==While)&&(token.tipo==PR)){

       token=lexico(file);
       if((token.valor.codSN==sn_abre_parentese)&&(token.tipo==SN)){
     		token=lexico(file);
 	 		expr(file);

 			if((token.valor.codSN==sn_fecha_parentese)&&(token.tipo==SN)){
 				token=lexico(file);
				cmd(file);
         	}
        	else{
         	     printf("Atencao: ) esperado na linha %d",ctlinha);
                 system("PAUSE");
 				exit(1);
            }
     	}
 		else{
	     	printf("Atencao: (  esperado na linha %d",ctlinha);
		    system("PAUSE");
 			exit(1);
    	}
  	}

	else if ((token.valor.codSN==sn_abre_chave)&&(token.tipo==SN)){
		token=lexico(file);
		while(!((token.valor.codSN==sn_fecha_chave)&&(token.tipo==SN))) cmd(file);
		if((token.valor.codSN==sn_fecha_chave)&&(token.tipo==SN)) token=lexico(file);
		else{
			printf("Atencao: } esperado na linha %d",ctlinha);
            system("PAUSE");
 			exit(1);
        }
	}

  	else {
  		printf("\n Token Erro: %s \n",token.lexema);
	printf("\n Token Erro: %i \n",token.valor.codPR);
  		printf("Atencao: Comando esperado na linha %d",ctlinha);
    	system("PAUSE");
 		exit(1);
    }

}

void prog(FILE* file){

	if (token.tipo == PR &&
	   (token.valor.codPR==Real
	   	||token.valor.codPR==Bool
	   	||token.valor.codPR==Int
	   	||token.valor.codPR==Char)) {

		token=lexico(file);
		if(token.tipo==ID) token=lexico(file);
		else {
			printf("Atencao: ID esperado na linha %d",ctlinha);
   	         system("PAUSE");
 			exit(1);
		}
		if(token.valor.codSN==sn_abre_parentese && token.tipo==SN) {
			token=lexico(file);
			tipos_Param(file);
			if(token.valor.codSN==sn_fecha_parentese && token.tipo==SN) token=lexico(file);
			else {
				printf("Atencao: ) esperado na linha %d",ctlinha);
	   	        system("PAUSE");
 				exit(1);
			}
			if(token.valor.codSN==sn_abre_chave && token.tipo==SN) token=lexico(file);
			else {
				printf("Atencao: { esperado na linha %d",ctlinha);
	   	        system("PAUSE");
 				exit(1);
			}

			while (token.tipo==PR && (token.valor.codPR==Real||token.valor.codPR==Char||token.valor.codPR==Bool||token.valor.codPR==Int)) {
				token=lexico(file);
				if(token.tipo==ID) token=lexico(file);
				else {
					printf("Atencao: ID esperado na linha %d",ctlinha);
		   	        system("PAUSE");
 	exit(1);
				}
				while (token.tipo==SN && token.valor.codSN==sn_virgula) {
					token=lexico(file);
					if(token.tipo==ID) token=lexico(file);
					else {
						printf("Atencao: ID esperado na linha %d",ctlinha);
 		   	         	system("PAUSE");
 	exit(1);
					}
				}
				if(token.valor.codSN==sn_ponto_virgula && token.tipo==SN) token=lexico(file);
				else {
					printf("Atencao: ; esperado na linha %d",ctlinha);
		   	        system("PAUSE");
 	exit(1);
				}
			}

			while ( (token.tipo==PR && (token.valor.codPR==While ||token.valor.codPR==If||token.valor.codPR==Return||token.valor.codPR==For))
					|| token.tipo==ID
					|| (token.tipo==SN && (token.valor.codSN==sn_ponto_virgula||token.valor.codSN==sn_abre_chave)) ) {
				cmd(file);
			}

			if(token.valor.codSN==sn_fecha_chave && token.tipo==SN) token=lexico(file);
			else {
				printf("Atencao: } esperado na linha %d",ctlinha);
	   	        system("PAUSE");
 	exit(1);
			}
		}

		else if (token.tipo== SN && (token.valor.codSN==sn_ponto_virgula || token.valor.codSN==sn_virgula)) {
			while (token.tipo!= SN || token.valor.codSN!=sn_ponto_virgula) {
				if(token.valor.codSN==sn_virgula && token.tipo==SN) token=lexico(file);
				else{
					 printf("Atencao: , esperado na linha %d",ctlinha);
					 system("PAUSE");
 	exit(1);
				}
				if(token.tipo==ID) token=lexico(file);
				else {
					printf("Atencao: ID esperado na linha %d",ctlinha);
	   	         	system("PAUSE");
 	exit(1);
				}
			}
			if(token.valor.codSN==sn_ponto_virgula && token.tipo==SN) token=lexico(file);
			else {
				printf("Atencao: ; esperado na linha %d",ctlinha);
	   	        system("PAUSE");
 	exit(1);
			}
		}

		else {
			printf("Atencao: tipo invalido linha %d",ctlinha);
			system("PAUSE");
 	exit(1);
		}
	}

	else if((token.valor.codPR==func)&&(token.tipo==PR)) {
		token=lexico(file);
		if(token.tipo==ID) token=lexico(file);
		else {
			printf("Atencao: ID esperado na linha %d",ctlinha);
   	        system("PAUSE");
 	exit(1);
		}
		if((token.valor.codSN==sn_abre_parentese)&&(token.tipo==SN)) token=lexico(file);
		else {
			printf("Atencao: ( esperado na linha %d",ctlinha);
   	        system("PAUSE");
 	exit(1);
		}
		tipos_Param(file);
		if(token.valor.codSN==sn_fecha_parentese && token.tipo==SN) token=lexico(file);
		else {
			printf("Atencao: ) esperado na linha %d",ctlinha);
   	        system("PAUSE");
 	exit(1);
		}
		if(token.valor.codSN==sn_abre_chave && token.tipo==SN) token=lexico(file);
		else {
			printf("Atencao: { esperado na linha %d",ctlinha);
   	        system("PAUSE");
 	exit(1);
		}
		while (token.tipo==PR && (token.valor.codPR==Int||token.valor.codPR==Char||token.valor.codPR==Real||token.valor.codPR==Bool)) {
				token=lexico(file);
				if(token.tipo==ID) token=lexico(file);
				else {
					printf("Atencao: ID esperado na linha %d",ctlinha);
		   	        system("PAUSE");
 	exit(1);
				}
				while (token.tipo == SN && token.valor.codSN==sn_virgula) {
					token=lexico(file);
					if(token.tipo==ID) token=lexico(file);
					else {
						printf("Atencao: ID esperado na linha %d",ctlinha);
 		   	         	system("PAUSE");
 	exit(1);
					}
				}
				if(token.valor.codSN==sn_ponto_virgula && token.tipo==SN) token=lexico(file);
				else {
					printf("Atencao: ; esperado na linha %d",ctlinha);
		   	        system("PAUSE");
 	exit(1);
				}
		}

		while ((token.tipo==PR && (token.valor.codPR==If ||token.valor.codPR==While||token.valor.codPR==For||token.valor.codPR==Return))||token.tipo==ID||(token.tipo==SN && (token.valor.codSN==sn_abre_chave||token.valor.codSN==sn_ponto_virgula))) {
			cmd(file);
		}

		if(token.valor.codSN==sn_fecha_chave && token.tipo==SN) token=lexico(file);
		else {
			printf("Atencao: } esperado na linha %d",ctlinha);
   	        system("PAUSE");
 	exit(1);
		}
	}
	else {
		printf("Atencao: prog esperado linha %d",ctlinha);
		system("PAUSE");
 	exit(1);
		}
}
void sintatico(FILE* file){
    prog(file);
}
