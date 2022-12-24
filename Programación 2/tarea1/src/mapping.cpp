#include "../include/mapping.h"
#include "../include/lista.h"
#include "../include/utils.h"

struct _rep_mapping{
    TLista lst;
};

TMapping crearMapping(){
    TMapping resultado = new _rep_mapping;

    resultado->lst = crearLista();
    return resultado;
}

TMapping asociar(nat clave, double valor, TMapping m){
    if(posNat(clave, m->lst) == 0){
        info_t map;
        map.natural = clave;
        map.real = valor;

        m->lst = insertar(longitud(m->lst) + 1, map, m->lst);
    }
    return m;
}

bool esClave(nat clave, TMapping m){
    return posNat(clave, m->lst) != 0;
}

double valor(nat clave, TMapping m){
    return infoLista(posNat(clave, m->lst), m->lst).real;
}

TMapping desasociar(nat clave, TMapping m){
    m->lst = remover(posNat(clave, m->lst), m->lst);
    return m;
}