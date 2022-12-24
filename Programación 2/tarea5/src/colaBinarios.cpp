#include "../include/colaBinarios.h"

#include <stdlib.h>

typedef struct nodoColaBinario *CBNodo;
struct nodoColaBinario{
    TBinario elem;
    CBNodo siguiente;
};

struct _rep_colaBinarios{
    CBNodo inicio, final;
};

TColaBinarios crearColaBinarios(){
    TColaBinarios res = new _rep_colaBinarios;
    res->inicio = res->final = NULL;
    return res;
}

TColaBinarios encolar(TBinario b, TColaBinarios c){
    CBNodo ins = new nodoColaBinario;
    ins->elem = b;
    ins->siguiente = NULL;
    if(c->inicio == NULL) c->inicio = ins;
    else c->final->siguiente = ins;
    c->final = ins;
    return c;
}

TColaBinarios desencolar(TColaBinarios c){
    if(!estaVaciaColaBinarios(c)){
        CBNodo borrar = c->inicio;
        c->inicio = c->inicio->siguiente;
        delete borrar;
    }
    return c;
}

void liberarColaBinarios(TColaBinarios c){
    while(c->inicio != NULL)
        desencolar(c);
    delete c;
}

bool estaVaciaColaBinarios(TColaBinarios c){
    return c->inicio == NULL;
}

TBinario frente(TColaBinarios c){
    return c->inicio->elem;
}