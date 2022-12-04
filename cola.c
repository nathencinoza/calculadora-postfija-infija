/* ENCINOZA VILELA, NATHALIA LUCIA
   PADRON: 106295                  */

#include "cola.h"
#include<stdlib.h>
#include<stdio.h>
typedef struct nodo {
	void** dato; 
	struct nodo *sig;
} nodo_t; 

struct cola {
	nodo_t *prim; 
	nodo_t *ult;	
};

nodo_t *nodo_crear(void* dato) {
	nodo_t *nodo = malloc(sizeof(nodo_t)); 
	if (nodo == NULL) {
		return NULL;
	}
	nodo->dato = dato;
	nodo->sig = NULL;
	return nodo;
}

cola_t *cola_crear(void) {
	cola_t *cola = malloc(sizeof(cola_t)); 
	if (cola == NULL) {
		return NULL;
	}
	cola->prim = NULL; 
	cola->ult = NULL; 
	return cola;
}

bool cola_esta_vacia(const cola_t *cola) {
	return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
	nodo_t *nodo = nodo_crear(valor);
	if (cola_esta_vacia(cola)) {
		cola->prim = nodo; 
		cola->ult = nodo;
		return true;
	}
	cola->ult->sig = nodo; 
	cola->ult = cola->ult->sig;
	return true;
}

void *cola_ver_primero(const cola_t *cola) {
	if (cola_esta_vacia(cola)) {
		return NULL; 
	}
	return cola->prim->dato;
}

void *cola_desencolar(cola_t *cola) {
	nodo_t *aux; void* x;
	if (cola_esta_vacia(cola)) {
		return NULL;
	}
	x = cola->prim->dato;
	aux = cola->prim;
	cola->prim = cola->prim->sig; 
	free(aux);
	return x;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
	if (destruir_dato != NULL) {
		while(!cola_esta_vacia(cola)) {
		destruir_dato(cola->prim->dato);
		cola_desencolar(cola);
		}
		free(cola);
	}
	else {
		while(!cola_esta_vacia(cola)) {
			cola_desencolar(cola);
		}
		free(cola);
	}	
}
