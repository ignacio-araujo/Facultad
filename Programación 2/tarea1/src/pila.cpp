#include "../include/pila.h"
#include "../include/lista.h"
#include "../include/utils.h"

#include <assert.h>

//Se define la representación de TPila.
//Se usan TLista y sus operaciones.
struct _rep_pila{
    TLista lst;
};

/*
    Se considera que la cima de la pila es el último elemento de la lista con la
    que se implementa, o sea, el que está en la posición longitud de lista.
    Esa es la posición pasada como parámetro al implementar 'cima' y 'desapilar'
    en las llamadas a las operaciones de lista 'infoLista' y 'remover'.
    En 'apilar' se pasa la posición longitud + 1 como parámetro de la operación
    'insertar'.
*/

TPila crearPila(){
    //Se obtiene memoria para la representación de la pila.
    TPila resultado = new _rep_pila;

    //(*resultado) es un registro de tipo _rep_pila.
    //Su único campo es lst, que debe ser inicializado.
    (*resultado).lst = crearLista();
    //De manera alternativa se puede usar el operador '->':
    //resultado->lst = crearLista();
    return resultado;
}

bool esVaciaPila(TPila p){
    return longitud((*p).lst) == 0;
}

TPila apilar(info_t nuevo, TPila p){
    (*p).lst = insertar(longitud((*p).lst) + 1, nuevo, (*p).lst);
    return p;
}

info_t cima(TPila p){
    assert(!esVaciaPila(p));
    return infoLista(longitud((*p).lst), (*p).lst);
}

TPila desapilar(TPila p){
    (*p).lst = remover(longitud((*p).lst), (*p).lst);
    return p;
}