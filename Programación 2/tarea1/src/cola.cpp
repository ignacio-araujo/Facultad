#include "../include/cola.h"
#include "../include/lista.h"
#include "../include/utils.h"


struct _rep_cola{
    TLista lst;
};

TCola crearCola(){
    TCola resultado = new _rep_cola;

    resultado->lst = crearLista();
    return resultado;
}

bool esVaciaCola(TCola c){
    return longitud(c->lst) == 0;
}

TCola encolar(info_t nuevo, TCola c){
    c->lst = insertar(longitud(c->lst) + 1, nuevo, c->lst);
    return c;
}

info_t frente(TCola c){
    return infoLista(1, c->lst);
}

TCola desencolar(TCola c){
    c->lst = remover(1, c->lst);
    return c;
}