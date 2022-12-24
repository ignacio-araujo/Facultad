#include "../include/mapping.h"
#include "../include/info.h"
#include "../include/cadena.h"

#include <stdlib.h>

struct _rep_mapping{
    TCadena *elems;
    nat tamanio;
    nat contador;
};

static nat hash(nat clave, nat tamanio){
    return clave % tamanio;
}

TMapping crearMap(nat M){
    TMapping res = new _rep_mapping;
    res->elems = new TCadena[M];
    res->tamanio = M;
    res->contador = 0;
    for(nat i = 0; i < M; i++)
        res->elems[i] = crearCadena();
    return res;
}

TMapping asociarEnMap(nat clave, double valor, TMapping map){
    nat pos = hash(clave, map->tamanio);
    
    TInfo dato = crearInfo(clave, valor);
    map->elems[pos] = insertarAlFinal(dato, map->elems[pos]);
    map->contador++;
    return map;
}

TMapping desasociarEnMap(nat clave, TMapping map){
    nat pos = hash(clave, map->tamanio);

    map->elems[pos] = removerDeCadena(siguienteClave(clave, inicioCadena(map->elems[pos]), map->elems[pos]) ,map->elems[pos]);
    map->contador--;
    return map;
}

bool existeAsociacionEnMap(nat clave, TMapping map){
    nat pos = hash(clave, map->tamanio);
    return siguienteClave(clave, inicioCadena(map->elems[pos]), map->elems[pos]) != NULL;
}

double valorEnMap(nat clave, TMapping map){
    nat pos = hash(clave, map->tamanio);
    return realInfo(infoCadena(siguienteClave(clave, inicioCadena(map->elems[pos]), map->elems[pos]), map->elems[pos]));
}

bool estaLlenoMap(TMapping map){
    return map->tamanio == map->contador;
}

void liberarMap(TMapping map){
    for(nat i = 0; i < map->tamanio; i++)
        liberarCadena(map->elems[i]);
    delete[] map->elems;
    delete map;
}
