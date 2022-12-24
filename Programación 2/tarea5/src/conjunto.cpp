#include "../include/conjunto.h"
#include "../include/avl.h"
#include "../include/usoTads.h"

#include <stdlib.h>

struct _rep_conjunto{
    TAvl elems;
    nat min;
    nat max;
};

TConjunto crearConjunto(){
    return NULL;
}

TConjunto singleton(nat elem){
    TConjunto res = new _rep_conjunto;
    res->elems = crearAvl();
    res->elems = insertarEnAvl(elem, res->elems);
    res->min = res->max = elem;
    return res;
}

TConjunto unionDeConjuntos(TConjunto c1, TConjunto c2){
    TIterador i1 = iteradorDeConjunto(c1);
    TIterador i2 = iteradorDeConjunto(c2);
    TIterador i3 = enAlguno(i1, i2);
    nat tamanio = cantidadEnIterador(i3); 
    reiniciarIterador(i3);

    TConjunto res = NULL;
    if(tamanio > 0){
        ArregloNats elems = new nat[tamanio];
        for(nat i = 0; i<tamanio; i++){
            elems[i] = actualEnIterador(i3);
            i3 = avanzarIterador(i3);
        }
        res = arregloAConjunto(elems, tamanio);
        delete[] elems;
    }
    liberarIterador(i1);
    liberarIterador(i2);
    liberarIterador(i3);
    return res;
}

TConjunto diferenciaDeConjuntos(TConjunto c1, TConjunto c2){
    TIterador i1 = iteradorDeConjunto(c1);
    TIterador i2 = iteradorDeConjunto(c2);
    TIterador i3 = soloEnA(i1, i2);
    nat tamanio = cantidadEnIterador(i3);
    reiniciarIterador(i3);

    TConjunto res = NULL;
    if(tamanio > 0){
        ArregloNats elems = new nat[tamanio];
        for(nat i = 0; i<tamanio; i++){
            elems[i] = actualEnIterador(i3);
            i3 = avanzarIterador(i3);
        }
        res = arregloAConjunto(elems, tamanio);
        delete[] elems;
    }
    liberarIterador(i1);
    liberarIterador(i2);
    liberarIterador(i3);
    return res;
}

bool perteneceAConjunto(nat elem, TConjunto c){
    if(c == NULL) return false;
    return buscarEnAvl(elem, c->elems) != NULL;
}

bool estaVacioConjunto(TConjunto c){
    return c == NULL;
}

nat cardinalidad(TConjunto c){
    if(c == NULL) return 0;
    return cantidadEnAvl(c->elems);
}

nat minimo(TConjunto c){
    return c->min;
}

nat maximo(TConjunto c){
    return c->max;
}

TConjunto arregloAConjunto(ArregloNats elems, nat n){
    TConjunto res = new _rep_conjunto;
    res->elems = arregloAAvl(elems, n);
    res->min = elems[0];
    res->max = elems[n-1];
    return res;
}

TIterador iteradorDeConjunto(TConjunto c){
    if(c == NULL){
        TIterador res = crearIterador();
        res = reiniciarIterador(res);
        return res;
    }
    return enOrdenAvl(c->elems);
}

void liberarConjunto(TConjunto c){
    if(c != NULL){
        liberarAvl(c->elems);
        delete c;
    }
}