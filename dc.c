#include "pila.h"
#include "strutil.h"
#include "calc_helper.h"
#include<stdio.h> 
#include<stdlib.h>  
#include<math.h>

bool verificar(pilanum_t* pila, calc_num arr[], int operandos) {
	int i = 0; calc_num op;
	while(i != operandos) {
		if (!desapilar_num(pila, &op)) {
			return false;
		}
		arr[i] = op;
		i++;
	}
	return true;
}

void calculadora_postfija(char* linea, pilanum_t* pila) {
	struct calc_token token;
	char** string = dc_split(linea);
	int i  = 0;
	bool ok = true;
	while(string[i] && ok) {
		ok = true;
		if (!calc_parse(string[i], &token)) {
			printf("ERROR\n");
			ok = false;
			break;
		}
		else if(token.type == TOK_NUM) {
			apilar_num(pila, token.value);
		}
		else if(token.type == TOK_OPER) {
			calc_num resultado, arr[token.oper.num_operandos];
			switch(token.oper.op) {
				case OP_ADD:
					if (verificar(pila, arr, token.oper.num_operandos)) {
						resultado = arr[0] + arr[1]; 
						apilar_num(pila, resultado);
					}
					else {
						printf("ERROR\n");
						ok = false;
					}
				break;

				case OP_SUB:
					if (verificar(pila, arr, token.oper.num_operandos)) {
						resultado = arr[1] - arr[0]; 
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;
				
				case OP_MUL: 
					if (verificar(pila, arr, token.oper.num_operandos)) {
						resultado = arr[0] * arr[1]; 
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;

				case OP_DIV:
					if (verificar(pila, arr, token.oper.num_operandos) && (arr[0] != 0)) {
						resultado = arr[1] / arr[0]; 
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;
	
				case OP_POW:
					if (verificar(pila, arr, token.oper.num_operandos) && (arr[0] >= 0)) {
						double res = pow((double)(arr[1]), (double)(arr[0]));
						resultado = (calc_num) res;
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;
			
				case OP_LOG:
					if (verificar(pila, arr, token.oper.num_operandos) && (arr[0] >= 2)) {
						double x = log((double)arr[0]);
						double y = log((double)arr[1]);
						double res = (y/x);
						resultado = (calc_num) res;
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;

				case OP_RAIZ: 
					if (verificar(pila, arr, token.oper.num_operandos) && (arr[0] >= 0)) {
						double res = sqrt((double)arr[0]);
						resultado =(calc_num) res;
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;
			
				case OP_TERN:
					if (verificar(pila, arr, token.oper.num_operandos)) {
						resultado = arr[2] ? arr[1] : arr[0];
						apilar_num(pila, resultado);
					}
					else {
						ok = false;
						printf("ERROR\n");
					}
				break;
			}
		}
		i++;
	}
	if (ok == true) {
		calc_num resultado = 0;
		desapilar_num(pila, &resultado);
		if (pila_esta_vacia(pila)) {
			printf("%ld\n", resultado);
		}
		else {
			printf("ERROR\n");
		}
	}
	free_strv(string);
}
int main() {
	pilanum_t* pila = pilanum_crear();
	char* linea = NULL;
	size_t tam =0;

	while(getline(&linea, &tam, stdin) != EOF) {
		calculadora_postfija(linea, pila);
	}
	pilanum_destruir(pila);
	free(linea); 
	return 0;
}
