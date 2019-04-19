#include <stdio.h>
#include <stdlib.h>
#include "colapri.h"
#include "colapri-lista.h"

typedef struct lista{
    ColaPriOps *ops;
    CompPri comp;
    void *v;
    struct lista *sig;
} ColaPriLis;

static void agregar(ColaPri colapri, void *a);
static void *extraer(ColaPri colapri);
static void *mejor(ColaPri colapri);
static int tamano(ColaPri colapri);
static void destruir(ColaPri colapri);
static IterColaP iterador(ColaPri colapri);

static ColaPriOps colapri_lis_ops = {
    agregar, extraer, mejor, tamano, destruir, iterador
};

ColaPri nuevaColaPriLista(CompPri comp){
    ColaPriLis *cp = malloc(sizeof(ColaPriLis));
    cp->ops = &colapri_lis_ops;
    cp->comp = comp;
    cp->v = malloc(sizeof(void*));
    cp->sig = NULL;
    return (ColaPri) cp;
}

static void agregar(ColaPri colapri, void *a){
    ColaPriLis *cp = (ColaPriLis*) colapri;
    while(cp->sig != NULL){
        if((*cp->sig->comp)(a, cp->sig->v)<0)
            break;
        cp = cp->sig;
    }
    ColaPriLis *ncp = (ColaPriLis*) nuevaColaPriLista(cp->comp);
    ncp->v = a;
    ncp->sig = cp->sig;
    cp->sig = ncp;
}

static void *extraer(ColaPri colapri){
    ColaPriLis *cp = (ColaPriLis*) colapri;
    void *cbz = cp->sig->v;
    cp->sig = cp->sig->sig;
    return cbz;
}

static void *mejor(ColaPri colapri){
    ColaPriLis *cp = (ColaPriLis*) colapri;
    return cp->sig->v;
}

static int tamano(ColaPri colapri){
    ColaPriLis *cp = (ColaPriLis*) colapri;
    int c = -1;
    while(cp != NULL){
        c++;
        cp = cp->sig;
    }
    return c;
}

static void destruir(ColaPri colapri){
    ColaPriLis *cp = (ColaPriLis*) colapri;
    if(cp->sig != NULL) 
        destruir((ColaPri) cp->sig);
    free(cp);
}

typedef struct {
    IterColaPOps *ops;
    ColaPriLis *cp;
} IterColaPLis;

static int continua(IterColaP itercp);
static void *proximo(IterColaP itercp);
static void destruirIter(IterColaP itercp);

static IterColaPOps iterOps = {
    continua, proximo, destruirIter
};

static IterColaP iterador(ColaPri colapri){
    IterColaPLis *iter = malloc(sizeof(IterColaPLis));
    iter->ops = &iterOps;
    iter->cp = (ColaPriLis*) colapri;
    return (IterColaP) iter;
}

static int continua(IterColaP itercp){
    IterColaPLis *iter = (IterColaPLis*) itercp;
    return iter->cp->sig != NULL;
}

static void *proximo(IterColaP itercp){
    IterColaPLis *iter = (IterColaPLis*) itercp;
    void *cbz = iter->cp->sig->v;
    iter->cp = iter->cp->sig;
    return cbz;
}

static void destruirIter(IterColaP itercp){
    free(itercp);
}