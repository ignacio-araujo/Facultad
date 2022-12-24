#include "../include/usoTads.h"
#include "../include/cadena.h"
#include "../include/pila.h"
#include "../include/colaBinarios.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static bool esCaminoAux(TLocalizador loc, TCadena c, TBinario b);


nat cantidadEnIterador(TIterador it){
    nat res = 0;
    it = reiniciarIterador(it);
    while(estaDefinidaActual(it)){
        res++;
        it = avanzarIterador(it);
    }
    return res;
}

TIterador enAlguno(TIterador a, TIterador b){
    TIterador res = crearIterador();
    a = reiniciarIterador(a);
    b = reiniciarIterador(b);
    while(estaDefinidaActual(a) && estaDefinidaActual(b)){
        nat valorA = actualEnIterador(a);
        nat valorB = actualEnIterador(b); 
        if(valorA == valorB){
            res = agregarAIterador(valorA, res);
            a = avanzarIterador(a);
            b = avanzarIterador(b);
        }
        else if(valorA > valorB){
            res = agregarAIterador(valorB, res);
            b = avanzarIterador(b);
        }
        else{ //valorA < valorB
            res = agregarAIterador(valorA, res);
            a = avanzarIterador(a);
        }
    }
    while(estaDefinidaActual(a)){
        res = agregarAIterador(actualEnIterador(a), res);
        a = avanzarIterador(a);
    }
    while(estaDefinidaActual(b)){
        res = agregarAIterador(actualEnIterador(b), res);
        b = avanzarIterador(b);
    }
    res = reiniciarIterador(res);
    return res;
}

TIterador soloEnA(TIterador a, TIterador b){
    TIterador res = crearIterador();
    a = reiniciarIterador(a);
    b = reiniciarIterador(b);
    while(estaDefinidaActual(a) && estaDefinidaActual(b)){
        nat valorA = actualEnIterador(a);
        nat valorB = actualEnIterador(b); 
        if(valorA > valorB)
            b = avanzarIterador(b);
        else if(valorA == valorB){
            a = avanzarIterador(a);
            b = avanzarIterador(b);
        }
        else{
            res = agregarAIterador(valorA, res);
            a = avanzarIterador(a);
        }
    }
    while(estaDefinidaActual(a)){
        res = agregarAIterador(actualEnIterador(a), res);
        a = avanzarIterador(a);
    }
    res = reiniciarIterador(res);
    return res;
}

TIterador recorridaPorNiveles(TBinario b){
    TIterador res = crearIterador();
    if(b != NULL){
        TColaBinarios colaAux = crearColaBinarios();
        TPila pilaAux = crearPila(cantidadBinario(b)+alturaBinario(b)-1); //altura para preever las barras. -1 porque antes de la raíz no necesitamos una barra
        
        colaAux = encolar(b, colaAux);
        colaAux = encolar(NULL, colaAux);
        while(!estaVaciaColaBinarios(colaAux)){
            b = frente(colaAux);
            colaAux = desencolar(colaAux);
            
            if(esVacioBinario(b) && !estaVaciaColaBinarios(colaAux)){
                pilaAux = apilar(UINT_MAX, pilaAux);
                colaAux = encolar(NULL, colaAux);
            }
            else if(!esVacioBinario(b)){
                pilaAux = apilar(natInfo(raiz(b)), pilaAux);
                if(derecho(b) != NULL)
                    colaAux = encolar(derecho(b), colaAux);
                if(izquierdo(b) != NULL)
                    colaAux = encolar(izquierdo(b), colaAux);   
            }
        }
        while(!estaVaciaPila(pilaAux)){
            res = agregarAIterador(cima(pilaAux), res);
            pilaAux = desapilar(pilaAux);
        }
        liberarPila(pilaAux);
        liberarColaBinarios(colaAux);
    }
    return res;
}

TCadena nivelEnBinario(nat l, TBinario b){
    TCadena res = crearCadena();
    if(b != NULL && l > 0){
        TCadena izquierda = nivelEnBinario(l-1, izquierdo(b));
        TCadena derecha = nivelEnBinario(l-1, derecho(b));

        res = insertarSegmentoDespues(izquierda, finalCadena(res), res);
        if(l == 1) res = insertarAlFinal(copiaInfo(raiz(b)), res);
        res = insertarSegmentoDespues(derecha, finalCadena(res), res);
    }
    return res;
}

static bool esCaminoAux(TLocalizador loc, TCadena c, TBinario b){
    if(b == NULL && loc == NULL) return true;
    else if(b == NULL || loc == NULL || esVaciaCadena(c)) return false;
    else{
        if(natInfo(raiz(b)) == natInfo(infoCadena(loc, c))){
            if(derecho(b) != NULL && izquierdo(b) != NULL)
                return esCaminoAux(siguiente(loc, c), c, izquierdo(b)) || esCaminoAux(siguiente(loc, c), c, derecho(b));
            else if(derecho(b) == NULL) return esCaminoAux(siguiente(loc, c), c, izquierdo(b));
            else if(izquierdo(b) == NULL) return esCaminoAux(siguiente(loc, c), c, derecho(b));
        }
        return false;
    }
}

bool esCamino(TCadena c, TBinario b){
    return esCaminoAux(inicioCadena(c), c, b);
}

bool pertenece(nat elem, TCadena cad){
    if(esVaciaCadena(cad)) return false;
    else{
        TLocalizador aux;
        aux = siguienteClave(elem, inicioCadena(cad), cad);
        return aux != NULL;
    }
}

nat longitud(TCadena cad){
    int i = 0;
    TLocalizador aux = inicioCadena(cad);
    while(aux != NULL){
        aux = siguiente(aux, cad);
        i++;
    }
    return i;
}

bool estaOrdenadaPorNaturales(TCadena cad){
    if(esVaciaCadena(cad)) return true;
    else{
        TLocalizador aux = inicioCadena(cad);
        nat anterior = natInfo(infoCadena(aux, cad));
        bool ordenado = true;
        while(aux != NULL && ordenado){
            nat valor = natInfo(infoCadena(aux, cad));
            if(anterior > valor) ordenado = false;
            anterior = valor;
            aux = siguiente(aux, cad);
        }
        return ordenado;
    }
}

bool hayNatsRepetidos(TCadena cad){
    TCadena temp = crearCadena();
    TLocalizador aux = inicioCadena(cad);
    bool repetido = false;
    while(aux != NULL && !repetido){
        TInfo info = copiaInfo(infoCadena(aux, cad));
        if(pertenece(natInfo(info), temp)){
            repetido = true;
            liberarInfo(info);//Porque si no la insertamos, luego al hacer liberarCadena no se va liberar
        }
        else{
            insertarAlFinal(info, temp); 
            aux = siguiente(aux, cad);
        }
    }
    liberarCadena(temp);
    return repetido;
}

bool sonIgualesCadena(TCadena c1, TCadena c2){
    if(esVaciaCadena(c1) && esVaciaCadena(c2)) return true;
    else if((esVaciaCadena(c1) && !esVaciaCadena(c2)) || (!esVaciaCadena(c1) && esVaciaCadena(c2))) return false;
    else if(longitud(c1) != longitud(c2)) return false;
    else{
        bool iguales = true;
        nat i = 1;
        while(i <= longitud(c1) && iguales){
            if(!sonIgualesInfo(infoCadena(kesimo(i, c1), c1), infoCadena(kesimo(i, c2), c2)))
                iguales = false;
            i++;
        }
        return iguales;
    }
}
TCadena concatenar(TCadena c1, TCadena c2){
    TCadena copia_c1 = copiarSegmento(inicioCadena(c1), finalCadena(c1), c1);
    TCadena copia_c2 = copiarSegmento(inicioCadena(c2), finalCadena(c2), c2);
    insertarSegmentoDespues(copia_c2, finalCadena(copia_c1), copia_c1);
    return copia_c1;
}

TCadena ordenar(TCadena cad){
    if(!esVaciaCadena(cad)){
        //Ordenamiento burbuja
        nat n = longitud(cad);
        for (nat i = 1; i < n; i++){
            for (nat j = 1; j < n-i+1; j++) {
                TLocalizador a = kesimo(j,cad);
                TLocalizador b = kesimo(j+1,cad);
                if (natInfo(infoCadena(a, cad)) > natInfo(infoCadena(b, cad))) 
                    cad = intercambiar(a, b, cad); 
            }
        }
    }
    return cad;
}

TCadena cambiarTodos(nat original, nat nuevo, TCadena cad){
    TLocalizador aux = inicioCadena(cad);
    while(aux != NULL){
        TInfo infoOriginal = infoCadena(aux, cad);
        if(natInfo(infoOriginal) == original){
            TInfo infoCambio = crearInfo(nuevo, realInfo(infoOriginal));
            liberarInfo(infoOriginal);
            cambiarEnCadena(infoCambio, aux, cad);
        }
        aux = siguiente(aux, cad);
    }
    return cad;
}

TCadena subCadena(nat menor, nat mayor, TCadena cad){
    //Usando siguienteClave y anteriorClave nos aseguramos de que 5->5 sea un caso posible
    return copiarSegmento(siguienteClave(menor, inicioCadena(cad), cad), anteriorClave(mayor, finalCadena(cad), cad), cad);
}