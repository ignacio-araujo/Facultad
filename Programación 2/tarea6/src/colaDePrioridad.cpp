#include "../include/colaDePrioridad.h"
#include "../include/info.h"

#include <stdlib.h>
#include <stdio.h>

struct _rep_colaDePrioridad{
    TInfo *heap;
    int *posiciones; //pos del índice, si no existe en heap entonces -1
    nat tope, tamanio;
};

static void filtrarDescendiente(nat indice, TColaDePrioridad &cp);
static void filtrarAscendente(nat indice, TColaDePrioridad &cp);

TColaDePrioridad crearCP(nat N){
    TColaDePrioridad res = new _rep_colaDePrioridad;
    res->heap = new TInfo[N];
    res->posiciones = new int[N];
    res->tope = N;
    res->tamanio = 0;
    for(nat i = 0; i<N; i++){
        res->heap[i] = NULL;
        res->posiciones[i] = -1;
    }
    return res;
}

nat rangoCP(TColaDePrioridad cp){
    return cp->tope;
}

TColaDePrioridad insertarEnCP(nat elem, double valor, TColaDePrioridad cp){
    cp->heap[cp->tamanio] = crearInfo(elem, valor);
    cp->posiciones[elem-1] = cp->tamanio;
    cp->tamanio++;
    if(cp->tamanio > 1) filtrarAscendente(cp->tamanio-1, cp);
    return cp;
}

bool estaVaciaCP(TColaDePrioridad cp){
    return cp->tamanio == 0;
}

nat prioritario(TColaDePrioridad cp){
    return natInfo(cp->heap[0]);
}

TColaDePrioridad eliminarPrioritario(TColaDePrioridad cp){
    cp->posiciones[natInfo(cp->heap[0])] = -1;
    liberarInfo(cp->heap[0]);
    cp->heap[0] = cp->heap[cp->tamanio-1];
    cp->tamanio--;
    filtrarDescendiente(0, cp);
    return cp;
}

bool estaEnCP(nat elem, TColaDePrioridad cp){
    if(estaVaciaCP(cp)) return  false;
    return cp->posiciones[elem-1] != -1;
}

double prioridad(nat elem, TColaDePrioridad cp){
    nat pos = cp->posiciones[elem-1];
    return realInfo(cp->heap[pos]);
}

TColaDePrioridad actualizarEnCP(nat elem, double valor, TColaDePrioridad cp){
    int pos = cp->posiciones[elem-1];
    TInfo aux = crearInfo(elem, valor);
    liberarInfo(cp->heap[pos]);
    cp->heap[pos] = aux;
    filtrarAscendente(cp->tamanio-1, cp);
    return cp;
}

void liberarCP(TColaDePrioridad cp){
    for(nat i = 0; i < cp->tamanio; i++)
        if(cp->heap[i] != NULL) liberarInfo(cp->heap[i]);
    delete[] cp->heap;
    delete[] cp->posiciones;
    delete cp;
}

void filtrarDescendiente(nat indice, TColaDePrioridad &cp){
    if(!estaVaciaCP(cp) && indice > 0){
        nat indiceDerecho = 2*indice+2;
        nat indiceIzquierdo = 2*indice+1;
        nat indiceHijoMenor = indice;
        
        if(indiceIzquierdo >= cp->tamanio && indiceDerecho < cp->tamanio)
            indiceHijoMenor = indiceDerecho;
        else if(indiceDerecho >= cp->tamanio && indiceIzquierdo < cp->tamanio)
            indiceHijoMenor = indiceIzquierdo;

        if(indice != indiceHijoMenor && realInfo(cp->heap[indice]) > realInfo(cp->heap[indiceHijoMenor])){
            TInfo aux = cp->heap[indice];
            cp->heap[indice] = cp->heap[indiceHijoMenor];
            cp->heap[indiceHijoMenor] = aux;
            
            cp->posiciones[natInfo(cp->heap[indice])-1] = indiceHijoMenor;
            cp->posiciones[natInfo(cp->heap[indiceHijoMenor])-1] = indice;

            filtrarDescendiente(indiceHijoMenor, cp);
        }
    }
}

void filtrarAscendente(nat indice, TColaDePrioridad &cp){
    if(!estaVaciaCP(cp) && indice > 0){
        nat indicePadre = (indice-1)/2;
        if(realInfo(cp->heap[indicePadre]) > realInfo(cp->heap[indice])){
            TInfo aux = cp->heap[indice];
            cp->heap[indice] = cp->heap[indicePadre];
            cp->heap[indicePadre] = aux;
            
            cp->posiciones[natInfo(cp->heap[indice])-1] = indicePadre;
            cp->posiciones[natInfo(cp->heap[indicePadre])-1] = indice;

            filtrarAscendente(indicePadre, cp);
        }
    }
}