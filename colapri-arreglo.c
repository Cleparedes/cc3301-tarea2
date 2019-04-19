#include <stdio.h>
#include <stdlib.h>
#include "colapri.h"
#include "colapri-arreglo.h"

/* La idea principal de una colapri es que no se sabe
 * que' almacena el arreglo.  Podria almacenar strings,
 * punteros a personas, punteros a longs e incluso longs,
 * pero el codigo de este archivo no cambia.
 *
 * Lo unico que se requiere es que el que cree la cola de prioridades
 * con nuevaColaPri debe suministrar su tamaño maximo
 * y una funcion 'comp' que sirva para comparar prioridades.
 */

typedef struct {
  ColaPriOps *ops;
  CompPri comp;
  int nmax, n;
  void **elems;
} ColaPriArr; /* ColaPriArr no es un puntero, ColaPri si lo es */

static void agregar(ColaPri colapri, void *a);
static void* extraer(ColaPri colapri);
static void* mejor(ColaPri colapri);
static int tamano(ColaPri colapri);
static void destruir(ColaPri colapri); /* debe estar vacia */
static IterColaP iterador(ColaPri colapri);


/*******************************************
 * Implementacion de la cola de prioridades
 *******************************************/
static ColaPriOps colapri_arr_ops= {
  agregar, extraer, mejor, tamano, destruir, iterador
};

ColaPri nuevaColaPriArreglo(int nmax, CompPri comp) {
  ColaPriArr *cp= malloc(sizeof(ColaPriArr));
  cp->ops= &colapri_arr_ops;
  cp->comp= comp;
  cp->nmax= nmax;
  cp->n= 0;
  cp->elems= malloc(nmax*sizeof(void *));
  return (ColaPri)cp;
}

/* Los elementos estan ordenados de mejor a peor prioridad */
static void agregar(ColaPri colapri, void *a) {
  ColaPriArr *cp= (ColaPriArr*)colapri;
  int k;
  if (cp->n==cp->nmax) {
    fprintf(stderr, "Error fatal: cola de prioridades llena\n");
    exit(1);
  }
  for (k= 0; k<cp->n; k++) {
    if ((*cp->comp)(a, cp->elems[k])<0)
      break;
  }
  /* se desplaza el arreglo para insertar el nuevo elemento en k */
  void *prev= a;
  for ( ; k<cp->n; k++) {
    void *temp= cp->elems[k];
    cp->elems[k]= prev;
    prev= temp;
  }
  cp->elems[k]= prev;
  cp->n++;
}

static void* mejor(ColaPri colapri) {
  ColaPriArr *cp= (ColaPriArr*)colapri;
  return cp->elems[0];
}

static int tamano(ColaPri colapri) {
  ColaPriArr *cp= (ColaPriArr*)colapri;
  return cp->n;
}

static void* extraer(ColaPri colapri) {
  ColaPriArr *cp= (ColaPriArr*)colapri;
  void *res= cp->elems[0];
  int k;
  for (k= 1; k<cp->n; k++)
    cp->elems[k-1]= cp->elems[k];
  cp->n--;
  return res;
}

static void destruir(ColaPri colapri) {
  ColaPriArr *cp= (ColaPriArr*)colapri;
  free(cp->elems);
  free(cp);
}

/*******************************************
 * Implementacion de la cola de prioridades
 *******************************************/

typedef struct {
  IterColaPOps *ops;
  ColaPriArr *cp;
  int k;
} IterColaPArr;

static int continua(IterColaP iter);
static void* proximo(IterColaP iter);
static void destruirIter(IterColaP iter);

static IterColaPOps iterOps= { continua, proximo, destruirIter };

IterColaP iterador(ColaPri colapri) {
  IterColaPArr *iter= malloc(sizeof(IterColaPArr));
  iter->ops= &iterOps;
  iter->cp= (ColaPriArr*)colapri;
  iter->k= 0;
  return (IterColaP)iter;
}

static int continua(IterColaP itercp) {
  IterColaPArr *iter= (IterColaPArr*)itercp;
  return iter->k < iter->cp->n;
}

static void *proximo(IterColaP itercp) {
  IterColaPArr *iter= (IterColaPArr*)itercp;
  return iter->cp->elems[iter->k++];
}

static void destruirIter(IterColaP iter) {
  free(iter);
}
