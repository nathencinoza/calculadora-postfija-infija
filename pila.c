/* ENCINOZA VILELA, NATHALIA LUCIA
   PADRON: 106295                  */

#include "pila.h"
#include <stdlib.h>
#include<stdio.h>
#define CAPACIDAD_INICIAL 10
#define CANTIDAD_INICIAL 0
#define FACTOR_REDIM 2


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t *pila_crear(void) {
	pila_t *pila = malloc(sizeof(pila_t));
	if (pila == NULL) {
		return NULL;
	}
	pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	pila->cantidad = CANTIDAD_INICIAL; 
	pila->capacidad = CAPACIDAD_INICIAL; 
	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila); 
}

bool redimensionar(pila_t *pila, size_t valor_a_redimensionar) {
	void *datos_nuevos = realloc(pila->datos, sizeof(void*) * valor_a_redimensionar);
	if (datos_nuevos == NULL) {
		return false; 
	}
	pila->capacidad = valor_a_redimensionar;
	pila->datos = datos_nuevos;
	return true;
}
// Devuelve verdadero si la pila no tiene elementos apilados, false en caso contrario.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0; 
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor) {
	if (pila->cantidad == pila->capacidad) {
		if (!redimensionar(pila, pila->capacidad * FACTOR_REDIM)) {
			return false;
		}
	}
	pila->cantidad++;
	pila->datos[pila->cantidad-1] = valor;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void *pila_ver_tope(const pila_t *pila) {
	if (pila->cantidad == 0) {
		return NULL; 
	}
	return pila->datos[pila->cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void *pila_desapilar(pila_t *pila) { 
	if (pila->cantidad == 0) {
		return NULL;
	}
	else if ((pila->capacidad) >= (pila->cantidad*4)) {
		redimensionar(pila, pila->capacidad / FACTOR_REDIM);
	}

	void* tope = pila->datos[pila->cantidad-1];
	pila->cantidad--;
	return tope;
}
