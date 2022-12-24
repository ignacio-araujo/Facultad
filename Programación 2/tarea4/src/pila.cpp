#include "../include/pila.h"

#include <stdlib.h>

typedef struct nodoPila *PNodo;
struct nodoPila{
    nat elem;
    PNodo siguiente;
};

struct _rep_pila{
    PNodo inicio, final;
    int cantElementos;
    int maximo;
};


TPila crearPila(nat tamanio){
    TPila res = new _rep_pila;
    res->inicio = res->final = NULL;
    res->cantElementos = 0;
    res->maximo = tamanio;
    return res;
}

TPila apilar(nat num, TPila p){
    if(!estaLlenaPila(p)){
        PNodo aux = new nodoPila;
        aux->elem = num;
        if(estaVaciaPila(p)) p->final = aux;
        else aux->siguiente = p->inicio;
        p->inicio = aux;
        p->cantElementos++;
    }
    return p;
}

TPila desapilar(TPila p){
    if(!estaVaciaPila(p)){
        PNodo aux = p->inicio;
        p->inicio = p->inicio->siguiente;
        delete aux;
        p->cantElementos--;
    } 
    return p;
}

void liberarPila(TPila p){
    while(!estaVaciaPila(p)){
        PNodo aux = p->inicio;
        p->inicio = p->inicio->siguiente;
        p->cantElementos--;
        delete aux;
    }
    delete p;
}

bool estaVaciaPila(TPila p){
    return p->cantElementos == 0;
}

bool estaLlenaPila(TPila p){
    return p->cantElementos == p->maximo;
}

nat cima(TPila p){
    return p->inicio->elem;
}