#include<stdio.h> 
#include<stdlib.h>  
#include "calc_helper.h"
#include "pila.h"
#include "cola.h"

void calculadora_infija(char* linea) {
	struct calc_token token;
	char** string = infix_split(linea);
	int i  = 0;
	pila_t* pila = pila_crear();
	cola_t* cola = cola_crear();
	while(string[i]) {
		if (!calc_parse(string[i], &token)) {
			printf("ERROR\n");
			break;
		}
		else if (token.type == TOK_NUM) {
			cola_encolar(cola, string[i]);
		}
		else if (token.type == TOK_OPER) {
			if (!pila_esta_vacia(pila)) {
				char* tope = pila_ver_tope(pila);
				struct calc_token token_aux = token;
				calc_parse(tope, &token_aux);
				while (((token_aux.oper.precedencia > token.oper.precedencia) || (token_aux.oper.precedencia == token.oper.precedencia
				 && token.oper.asociatividad == ASSOC_LEFT)) && token_aux.type != TOK_LPAREN) {
					cola_encolar(cola, pila_desapilar(pila));
					tope = pila_ver_tope(pila);
					if (tope == NULL) {
						break;
					}
					calc_parse(tope, &token_aux);
				}
			}
			pila_apilar(pila, string[i]);
		}
		else if (token.type == TOK_LPAREN) {
			pila_apilar(pila, string[i]);
		}
		else if (token.type == TOK_RPAREN) {
			char* tope = pila_ver_tope(pila);
			struct calc_token token_aux = token; 
			calc_parse(tope, &token_aux);
			while (token_aux.type != TOK_LPAREN) {
				cola_encolar(cola, pila_desapilar(pila));	
				tope = pila_ver_tope(pila);
				calc_parse(tope, &token_aux);
			}
			if (token_aux.type == TOK_LPAREN) {
				pila_desapilar(pila);
			}
		}
		i++;
	}
	while (!pila_esta_vacia(pila)) {
		cola_encolar(cola, pila_desapilar(pila));
	}
	while (!cola_esta_vacia(cola)) {
		printf("%s ", (char*)cola_desencolar(cola));
	}
	printf("\n");
	cola_destruir(cola,free); 
	pila_destruir(pila);
	free_strv(string);
}

int main() {
	char* linea = NULL;
	size_t tam =0;

	while(getline(&linea, &tam, stdin) != EOF) {
		calculadora_infija(linea);
	}
	free(linea); 
	return 0;
}
