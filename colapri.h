
/* ColaPri: cola de prioridades generica (puntero) */
typedef struct colapri *ColaPri;

/* IterColaP: iterador de una cola de prioridades (puntero) */
typedef struct itercolap *IterColaP;

/* CompPri: comparador de prioridades */
typedef int (*CompPri)(void *a, void *b);

/* ColaPriOps: tabla de operaciones de una cola de prioridad */
typedef struct {
  void (*agregar)(ColaPri cola, void *a);
  void* (*extraer)(ColaPri cola);
  void* (*mejor)(ColaPri cola);
  int (*tamano)(ColaPri cola);
  void (*destruir)(ColaPri cola); /* debe estar vacia */
  IterColaP (*iterador)(ColaPri cola);
} ColaPriOps;

/* El unico campo visible de una cola es su tabla de operaciones */
struct colapri {
  ColaPriOps *ops;
};

/* Tabla de operaciones para un iterador */
typedef struct {
  int (*continua)(IterColaP iter);
  void* (*proximo)(IterColaP iter);
  void (*destruir)(IterColaP iter);
} IterColaPOps;

struct itercolap {
  IterColaPOps *ops;
};

