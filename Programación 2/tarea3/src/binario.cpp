#include "../include/binario.h"

#include <stdlib.h>
#include <stdio.h>

struct _rep_binario{
    TInfo elem ;
    TBinario izq , der ;
};

static nat max(nat a, nat b);
static bool esAvlAux(TBinario b, int &i);
static void imprimirBinarioAux(TBinario b, int contador);
static double sumaUltimosPositivosAux(nat i, TBinario b, nat &cont);

TBinario crearBinario(){
    return NULL;
}

TBinario insertarEnBinario(TInfo i, TBinario b){
    if(b == NULL){
        b = new _rep_binario;
        b->izq = b->der = NULL;
        b->elem = i;
    }
    else if(natInfo(i) > natInfo(b->elem))
        b->der = insertarEnBinario(i, b->der);
    else 
        b->izq = insertarEnBinario(i, b->izq);
    return b;
}

TInfo mayor(TBinario b){
    TInfo res = b->elem;
    if(b->der != NULL) res = mayor(b->der);
    return res;
}

TBinario removerMayor(TBinario b){
    if(b->der == NULL){
        TBinario aux = b;
        if(b->izq == NULL) b = NULL;
        else b = b->izq;
        delete aux;
    }
    else b->der = removerMayor(b->der);
    return b;
}

TBinario removerDeBinario(nat elem, TBinario b){
    if(b != NULL){
        if(natInfo(b->elem) == elem){
            if(b->der == NULL && b->izq == NULL){
                liberarInfo(b->elem);
                delete b;
                b = NULL;
            }
            else if(b->der == NULL){
                TBinario aux = b->izq;
                liberarInfo(b->elem);
                delete b;
                b = aux;
            }
            else if(b->izq == NULL){
                TBinario aux = b->der;
                liberarInfo(b->elem);
                delete b;
                b = aux;
            }
            else{
                liberarInfo(b->elem);
                b->elem = mayor(b->izq);
                b->izq = removerMayor(b->izq);
            }
        }
        else if(elem < natInfo(b->elem))
            b->izq = removerDeBinario(elem, b->izq);
        else
            b->der = removerDeBinario(elem, b->der);
    }
    return b;
}

void liberarBinario(TBinario b){
    if(b != NULL){
        liberarBinario(b->izq);
        liberarBinario(b->der);
        liberarInfo(b->elem);
        delete b;
        b = NULL;
    }
}

bool esVacioBinario(TBinario b){
    return b == NULL;
}

static bool esAvlAux(TBinario b, int &i){
    if(b == NULL) return true;
    else{
        int altura_izquierda = i+1;
        int altura_derecha = i+1;

        bool izquierda = esAvlAux(b->izq, altura_izquierda);
        if(!izquierda) return false;
        bool derecha = esAvlAux(b->der, altura_derecha);
        if(!derecha) return false;

        i = max(altura_izquierda, altura_derecha);
        return abs(altura_izquierda-altura_derecha) <= 1;
    }
}

bool esAvl(TBinario b){
    int i = 0;
    return esAvlAux(b, i);
}

TInfo raiz(TBinario b){
    return b->elem;
}

TBinario izquierdo(TBinario b){
    return b->izq;
}

TBinario derecho(TBinario b){
    return b->der;
}

TBinario buscarSubarbol(nat elem, TBinario b){
    TBinario res;
    if(b == NULL) res = NULL;
    else{
        if(natInfo(b->elem) == elem) res = b;
        else if(natInfo(b->elem) > elem) res = buscarSubarbol(elem, b->izq);
        else res = buscarSubarbol(elem, b->der);
    }
    return res;
}

static nat max(nat a, nat b){
    if(a > b) return a;
    else return b;
}

nat alturaBinario(TBinario b){
    if(b == NULL) return 0;
    return max(alturaBinario(b->izq), alturaBinario(b->der)) + 1;
}

nat cantidadBinario(TBinario b){
    nat res = 0;
    if(b != NULL)
        res = 1 + cantidadBinario(b->izq)+cantidadBinario(b->der);
    return res;
}

double sumaUltimosPositivosAux(nat i, TBinario b, nat &cont){
    double res = 0;
    if(b != NULL && cont < i){
        res = sumaUltimosPositivosAux(i, b->der, cont); //No controlamos cont porque se hará al llamarse de vuelta
        if(cont < i && realInfo(b->elem) > 0){ //Acá sí porque podria haber cambiado con la llamada anterior
            res = res+realInfo(b->elem);
            cont++;
        }
        res = res+sumaUltimosPositivosAux(i, b->izq, cont);
    }
    return res;
}

double sumaUltimosPositivos(nat i, TBinario b){
    nat cont = 0;
    return sumaUltimosPositivosAux(i, b, cont);
}

TCadena linealizacion(TBinario b){
    TCadena res = NULL;
    if(b != NULL){
        res = crearCadena();
        TCadena izquierda = linealizacion(b->izq);
        TCadena derecha = linealizacion(b->der);
        if(izquierda != NULL) res = insertarSegmentoDespues(izquierda, finalCadena(res), res);
        res = insertarAlFinal(copiaInfo(b->elem), res);
        if(derecha != NULL) res = insertarSegmentoDespues(derecha, finalCadena(res), res);
    }
    return res;
}

TBinario menores(double cota, TBinario b){
    TBinario res = NULL;
    if(b != NULL){
        TBinario izquierda = menores(cota, b->izq);
        TBinario derecha = menores(cota, b->der);

        if(realInfo(b->elem) < cota){
            res = new _rep_binario;
            res->elem = copiaInfo(b->elem);
            res->der = derecha;
            res->izq = izquierda;
        }
        else if(derecha == NULL) res = izquierda;
        else if(izquierda == NULL) res = derecha;
        else{
            TInfo aux = mayor(b->izq);
            res = new _rep_binario;
            res->elem = copiaInfo(aux);
            res->der = derecha;
            res->izq = izquierda;
            removerMayor(izquierda);
        }
    }
    return res;
}

static void imprimirBinarioAux(TBinario b, int contador){
    if(b != NULL){
        imprimirBinarioAux(b->der, contador+1);

        for (int i = 0; i < contador; i++)
            printf("%c", '-');
        ArregloChars info = infoATexto(b->elem);
        printf("%s\n", info);
        delete[] info;

        imprimirBinarioAux(b->izq, contador+1);
    }
}

void imprimirBinario(TBinario b){
    printf("\n");
    imprimirBinarioAux(b, 0);
}