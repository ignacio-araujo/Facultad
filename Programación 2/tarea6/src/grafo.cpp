#include "../include/grafo.h"
#include "../include/avl.h"
#include "../include/mapping.h"

#include <stdlib.h>

struct _rep_grafo{
    nat vertices;
    TMapping aristas;
    TAvl *vecinos;
};

static nat hash(nat v1, nat v2, nat N){
    nat min, max;
    if(v1 < v2){
        min = v1;
        max = v2;
    }
    else{
        min = v2;
        max = v1;
    }
    return (min-1)*N+max-1;
}

TGrafo crearGrafo(nat N, nat M){
    TGrafo res = new _rep_grafo;
    res->vertices = N;
    res->aristas = crearMap(M);
    res->vecinos = new TAvl[N];
    for(nat i = 0; i < N; i++)
        res->vecinos[i] = crearAvl();
    return res;
}

nat cantidadVertices(TGrafo g){
    return g->vertices;
}

bool hayMParejas(TGrafo g){
    return estaLlenoMap(g->aristas);
}

TGrafo hacerVecinos(nat v1, nat v2, double d, TGrafo g){
    nat clave = hash(v1, v2, g->vertices);
    g->aristas = asociarEnMap(clave, d, g->aristas);
    nat pos1 = v1-1;
    nat pos2 = v2-1;
    g->vecinos[pos1] = insertarEnAvl(v2, g->vecinos[pos1]);
    g->vecinos[pos2] = insertarEnAvl(v1, g->vecinos[pos2]);
    return g;
}

bool sonVecinos(nat v1, nat v2, TGrafo g){
    nat clave = hash(v1, v2, g->vertices);
    return existeAsociacionEnMap(clave, g->aristas);
}

double distancia(nat v1, nat v2, TGrafo g){
    nat clave = hash(v1, v2, g->vertices);
    return valorEnMap(clave, g->aristas);
}

TIterador vecinos(nat v, TGrafo g){
    nat pos = v-1;
    return enOrdenAvl(g->vecinos[pos]);
}

void liberarGrafo(TGrafo g){
    liberarMap(g->aristas);
    for(nat i = 0; i < g->vertices; i++)
        liberarAvl(g->vecinos[i]);
    delete[] g->vecinos;
    delete g;
}