/* Esto se necesita para usar random */
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "colapri.h"
#include "colapri-arreglo.h"

/* Error es igual a printf, solo que termina la ejecución del proceso */
void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  exit(1);
}

/*
 * test_colapri: test unitario generico.
 * no se sabe como esta implementada la cola de prioridades.
 * Puede ser implementada con arreglos, listas enlazadas o un heap.
 */

void test_colapri(ColaPri cola, void *perro, void *gato, void *pez) {
  cola->ops->agregar(cola, perro);
  if (cola->ops->mejor(cola)!=perro)
    error("perro debio ser el mejor elemento\n");
  if (cola->ops->tamano(cola)!=1)
    error("el tamano de la cola debio ser 1\n");
  cola->ops->agregar(cola, gato);
  if (cola->ops->mejor(cola)!=gato)
    error("gato debio ser el mejor elemento\n");
  cola->ops->agregar(cola, pez);
  if (cola->ops->mejor(cola)!=gato)
    error("gato debio ser el mejor elemento\n");
  if (cola->ops->tamano(cola)!=3)
    error("el tamano de la cola debio ser 3\n");
  if (cola->ops->extraer(cola)!=gato)
    error("se debio extraer el gato\n");
  if (cola->ops->tamano(cola)!=2)
    error("el tamano de la cola debio ser 2\n");

  {
    IterColaP iter= cola->ops->iterador(cola);
    int k= 0;
    void *elems[2];
    while (iter->ops->continua(iter)) {
      if (k>=2)
        error("continua debio entregar falso\n");
      elems[k++]= iter->ops->proximo(iter);
    }
    if (elems[0]!=perro)
      error("el iterador debio entregar primero el perro\n");
    if (elems[1]!=pez)
      error("el iterador debio entregar luego el pez\n");
    iter->ops->destruir(iter);
  }

  if (cola->ops->extraer(cola)!=perro)
    error("se debio extraer el perro\n");
  if (cola->ops->extraer(cola)!=pez)
    error("se debio extraer el pez\n");
}

int comp_lls(void *a, void *b) {
  long long x= (long long)a;
  long long y= (long long)b;
  return (int)(x-y);
}

#define N 10000
#define M 1000

int irand(int max) {
  int r;
  do {
    r= (int) ((double)random() / RAND_MAX * max); /* random en [0, max[ */
  } while (r>=max); /* con mala suerte podria ocurrir */
  return r;
}

int main() {
  /* Test unitario de una cola de strings */
  ColaPri cola= nuevaColaPriArreglo(M, (CompPri)strcmp);
  test_colapri(cola, "perro", "gato", "pez");

  /* Test unitario de una cola de long longs vestidos como punteros */
  ColaPri cola2= nuevaColaPriArreglo(M, comp_lls);
  test_colapri(cola2, (void*)2, (void*)1, (void*)3);

  /* Se repite el primer test unitario */
  test_colapri(cola, "juan", "diego", "pedro");
  cola->ops->destruir(cola);

  /* Test intensivo: se agregan N elementos a 2 colas: cola2 y cola3.
   * Cuando alcanzan el tamano M se extrae un elemento para mantener
   * el tamano en M.  Los elementos extraidos se comparan con
   * los que se extrajeron de 2 colas de referencia: ref2 y ref3.
   */

  /* Se reutiliza cola2, que deberia estar vacia */
  ColaPri ref2= nuevaColaPriArreglo(M, comp_lls);
  ColaPri cola3= nuevaColaPriArreglo(M, comp_lls);
  ColaPri ref3= nuevaColaPriArreglo(M, comp_lls);

  /* Se agregan M elementos */
  int k;
  for (k=0; k<M; k++) {
    long long r1= irand(N), r2= irand(N);
    ref2->ops->agregar(ref2, (void*)r1);
    cola2->ops->agregar(cola2, (void*)r1);
    ref3->ops->agregar(ref3, (void*)r2);
    cola3->ops->agregar(cola3, (void*)r2);
  }

  /* Se extrae y agregan N-M elementos */
  for ( ; k<N; k++) {
    long long r1= irand(N), r2= irand(N);
    if (ref2->ops->extraer(ref2)!=cola2->ops->extraer(cola2))
      error("la cola cola2 no entrego el mismo elemento que ref2\n");
    ref2->ops->agregar(ref2, (void*)r1);
    cola2->ops->agregar(cola2, (void*)r1);

    if (ref3->ops->extraer(ref3)!=cola3->ops->extraer(cola3))
      error("la cola cola3 no entrego el mismo elemento que ref3\n");
    ref3->ops->agregar(ref3, (void*)r2);
    cola3->ops->agregar(cola3, (void*)r2);
  }

  /* La cola2 tiene M elementos.  Se itera sobre sus elementos
   * comparando con lo que entrega la cola de referencia ref2.
   */
  IterColaP iter_ref2= ref2->ops->iterador(ref2);
  IterColaP iter_cola2= cola2->ops->iterador(cola2);

  while (iter_ref2->ops->continua(iter_ref2)) {
    if (!iter_cola2->ops->continua(iter_cola2))
      error("iter_cola2 debio haber continuado\n");
    if (iter_ref2->ops->proximo(iter_ref2)!=
          iter_cola2->ops->proximo(iter_cola2))
      error("iter_cola2 entrego un elemento distinto de iter_ref2\n");
  }
  if (iter_cola2->ops->continua(iter_cola2))
    error("iter_cola2 debio terminar\n");
  iter_ref2->ops->destruir(iter_ref2);

  /* La cola3 tiene M elementos.  Se itera sobre sus elementos
   * comparando con lo que entrega la cola de referencia ref3.
   */
  IterColaP iter_ref3= ref3->ops->iterador(ref3);
  IterColaP iter_cola3= cola3->ops->iterador(cola3);

  while (iter_ref3->ops->continua(iter_ref3)) {
    if (!iter_cola3->ops->continua(iter_cola3))
      error("iter_cola3 debio haber continuado\n");
    if (iter_ref3->ops->proximo(iter_ref3)!=
          iter_cola3->ops->proximo(iter_cola3))
      error("iter_cola3 entrego un elemento distinto de iter_ref3\n");
  }
  if (iter_cola3->ops->continua(iter_cola3))
    error("iter_cola3 debio terminar\n");
  iter_ref3->ops->destruir(iter_ref3);

  /* Se extraen todos los elementos de las colas y se verifica
   * que todas queden vacias.
   */
  while (ref2->ops->tamano(ref2)!=0) {
    if (cola2->ops->tamano(cola2)<=0)
      error("cola2 debio contener al menos un elemento\n");
    if (ref2->ops->extraer(ref2)!=cola2->ops->extraer(cola2))
      error("cola2 no entrego el mismo elemento que ref2\n");
    if (cola3->ops->tamano(cola3)<=0)
      error("cola3 debio contener al menos un elemento\n");
    if (ref3->ops->extraer(ref3)!=cola3->ops->extraer(cola3))
      error("cola3 no entrego el mismo elemento que ref3\n");
  }
  if (cola2->ops->tamano(cola2)!=0)
    error("cola2 debio estar vacia\n");
  if (cola3->ops->tamano(cola3)!=0)
    error("cola3 debio estar vacia\n");

  /* Se prueban los destructores */
  ref2->ops->destruir(ref2);
  ref3->ops->destruir(ref3);
  cola2->ops->destruir(cola2);
  cola3->ops->destruir(cola3);

  printf("Perfecto.  Todo funciona bien.\n");
  return 0;
}
