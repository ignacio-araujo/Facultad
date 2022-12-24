#include "../include/iterador.h"
#include "../include/info.h"

#include <stdlib.h>

typedef struct nodoIterador *TNodo;
struct nodoIterador{
    nat elem;
    TNodo siguiente;
};

struct _rep_iterador{
    bool reiniciado;
    TNodo inicio, final, actual;
};


TIterador crearIterador(){
    TIterador iterador = new _rep_iterador;
    iterador->reiniciado = false;
    iterador->inicio = iterador->final = iterador->actual = NULL;
    return iterador;
}


TIterador agregarAIterador(nat elem, TIterador iter){
    if(!iter->reiniciado){
        TNodo agregar = new nodoIterador;
        agregar->elem = elem;
        agregar->siguiente = NULL;
        if(iter->inicio == NULL) iter->inicio = agregar;
        else iter->final->siguiente = agregar;
        iter->final = agregar;
    }
    return iter;
}


TIterador reiniciarIterador(TIterador iter){
    if(iter->inicio != NULL) iter->actual = iter->inicio;
    iter->reiniciado = true;
    return iter;
}


TIterador avanzarIterador(TIterador iter){
    if(estaDefinidaActual(iter)){
        if(iter->actual == iter->final) iter->actual = NULL;
        else iter->actual = iter->actual->siguiente;
    }
    return iter;
}


nat actualEnIterador(TIterador iter){
    return iter->actual->elem;
}


bool estaDefinidaActual(TIterador iter){
    if(!iter->reiniciado) return false;
    return iter->actual != NULL;
}


void liberarIterador(TIterador iter){
    TNodo aux;
    while(iter->inicio != NULL){
        aux = iter->inicio;
        iter->inicio = iter->inicio->siguiente;
        delete aux;
    }
    delete iter;
}