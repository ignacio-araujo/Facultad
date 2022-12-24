#include "../include/colCadenas.h"
#include "../include/cadena.h"

#include <stdlib.h>

struct _rep_colCadenas{
    TCadena *elems;
    nat tamanio;
};

TColCadenas crearColCadenas(nat M){
    TColCadenas res = new _rep_colCadenas;
    res->elems = new TCadena[M];
    res->tamanio = M;
    for(nat i = 0; i < M; i++)
        res->elems[i] = crearCadena();
    return res;
}

TColCadenas insertarEnColCadenas(TInfo info, nat pos, TColCadenas col){
    if(pos < col->tamanio){
        if(esVaciaCadena(col->elems[pos])) col->elems[pos] = insertarAlFinal(info, col->elems[pos]);
        else col->elems[pos] = insertarAntes(info, inicioCadena(col->elems[pos]), col->elems[pos]);
    }
    return col;
}

bool estaEnColCadenas(nat dato, nat pos, TColCadenas col){
    bool res = false;
    TLocalizador cursor = inicioCadena(col->elems[pos]);
    while(!res && cursor != NULL){
        if(natInfo(infoCadena(cursor, col->elems[pos])) == dato)
            res = true;
        cursor = siguiente(cursor, col->elems[pos]);
    }
    return res;
}

TInfo infoEnColCadenas(nat dato, nat pos, TColCadenas col){
    TCadena aux = col->elems[pos];
    return infoCadena(siguienteClave(dato, inicioCadena(aux), aux), aux);
}

TColCadenas removerDeColCadenas(nat dato, nat pos, TColCadenas col){
    col->elems[pos] = removerDeCadena(siguienteClave(dato, inicioCadena(col->elems[pos]), col->elems[pos]), col->elems[pos]);
    return col;
}

void liberarColCadenas(TColCadenas col){
    for(nat i = 0; i < col->tamanio; i++)
        liberarCadena(col->elems[i]);
    delete[] col->elems;
    delete col;
}