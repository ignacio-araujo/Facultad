#include "../include/avl.h"

#include <stdlib.h>

struct _rep_avl{
    nat dato;
    _rep_avl *izq, *der;
    nat altura;
};

static nat max(TAvl a, TAvl b);
static void rotarDerecha(TAvl &avl);
static void rotarIzquierda(TAvl &avl);
static void enOrdenAvlAux(TAvl avl, TIterador &it);
static TAvl arregloAAvlAux(ArregloNats elems, int inicio, int final);
static TAvl avlMinAux(nat h, nat &contador);


TAvl crearAvl(){
    return NULL;
}

bool estaVacioAvl(TAvl avl){
    return avl == NULL;
}

TAvl insertarEnAvl(nat elem, TAvl avl){
    if(avl == NULL){
        avl = new _rep_avl;
        avl->izq = avl->der = NULL;
        avl->dato = elem;
        avl->altura = 1;
        return avl;
    }
    if(elem > avl->dato)
        avl->der = insertarEnAvl(elem, avl->der);
    else
        avl->izq = insertarEnAvl(elem, avl->izq);

    avl->altura = 1+max(avl->izq, avl->der);
    
    int FB = max(avl->izq, avl->izq) - max(avl->der, avl->der); //Usamos max para el caso en el que uno de los punteros sea null, porque max ya nos devuelve 0
    
    //Rotacion derecha
    if(FB < -1 && elem > avl->der->dato)
        rotarIzquierda(avl);
    //Rotacion izquierda
    else if(FB > 1 && elem < avl->izq->dato)
        rotarDerecha(avl);
    //Rotacion derecha-izquierda
    else if(FB < -1 && elem < avl->der->dato){
        rotarDerecha(avl->der);
        rotarIzquierda(avl);
    }
    //Rotacion izquierda-derecha
    else if(FB > 1 && elem > avl->izq->dato){
        rotarIzquierda(avl->izq);
        rotarDerecha(avl);
    }
    return avl;
}

TAvl buscarEnAvl(nat elem, TAvl avl){
    TAvl res;
    if(avl == NULL) res = NULL;
    else{
        if(avl->dato == elem) res = avl;
        else if(avl->dato > elem) res = buscarEnAvl(elem, avl->izq);
        else res = buscarEnAvl(elem, avl->der);
    }
    return res;
}

nat raizAvl(TAvl avl){
    return avl->dato;
}

TAvl izqAvl(TAvl avl){
    return avl->izq;
}

TAvl derAvl(TAvl avl){
    return avl->der;
}

nat cantidadEnAvl(TAvl avl){
    nat res = 0;
    if(avl != NULL)
        res = 1+cantidadEnAvl(avl->izq)+cantidadEnAvl(avl->der);
    return res;
}

nat alturaDeAvl(TAvl avl){
    if(avl == NULL) return 0;
    else return avl->altura;
}

void enOrdenAvlAux(TAvl avl, TIterador &it){
    if(avl != NULL){
        enOrdenAvlAux(avl->izq, it);
        it = agregarAIterador(avl->dato, it);
        enOrdenAvlAux(avl->der, it);
    }    
}

TIterador enOrdenAvl(TAvl avl){
    TIterador res = crearIterador();
    enOrdenAvlAux(avl, res);
    res = reiniciarIterador(res);
    return res;
}


TAvl arregloAAvlAux(ArregloNats elems, int inicio, int final){
    if(inicio > final) return NULL;
    else{
        TAvl res = new _rep_avl;
        nat mitad = (inicio+final)/2;
        res->dato = elems[mitad];
        res->izq = arregloAAvlAux(elems, inicio, mitad-1);
        res->der = arregloAAvlAux(elems, mitad+1, final);
        res->altura = 1+max(res->izq, res->der);
        return res;
    }
}

TAvl arregloAAvl(ArregloNats elems, nat n){
    return arregloAAvlAux(elems, 0, n-1);
}

TAvl avlMinAux(nat h, nat &contador){//Explicacion https://imgur.com/nJCxnS2
    if(h == 0) return NULL;
    else if(h == 1){
        TAvl nodo = new _rep_avl;
        nodo->altura = 1;
        nodo->izq = nodo->der = NULL;
        nodo->dato = contador;
        contador++;
        return nodo;
    }
    else{
        TAvl nodo = new _rep_avl;
        nodo->altura = h;
        nodo->izq = avlMinAux(h-1, contador);
        nodo->dato = contador;
        contador++;
        nodo->der = avlMinAux(h-2, contador);
        return nodo;
    }
}

TAvl avlMin(nat h){
    nat contador = 1;
    return avlMinAux(h, contador);
}

void liberarAvl(TAvl avl){
    if(avl != NULL){
        liberarAvl(avl->izq);
        liberarAvl(avl->der);
        delete avl;
        avl = NULL;
    }
}

static nat max(TAvl a, TAvl b){
    if(a == NULL && b == NULL) return 0;
    else if(a == NULL && b != NULL) return b->altura;
    else if(a != NULL && b == NULL) return a->altura;
    else if(a->altura > b->altura) return a->altura;
    else return b->altura;
}

void rotarIzquierda(TAvl &x){
    TAvl y = x->der ;
    TAvl avl2 = y->izq ;
    //Rotamos
    y-> izq = x ;
    x-> der = avl2 ;

    x-> altura = 1+max(x->izq, x->der);
    y-> altura = 1+max(y->izq, y->der);

    x = y ;
}

void rotarDerecha(TAvl &y){
    TAvl x = y->izq ;
    TAvl avl3 = x->der ;
    //Rotamos
    x-> der = y ;
    y-> izq = avl3 ;
    
    y-> altura = 1+max(y->izq, y->der);
    x-> altura = 1+max(x->izq, x->der);
    
    y = x;
}
