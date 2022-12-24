#include "../include/usoTads.h"
#include "../include/cadena.h"

#include <stdlib.h>
#include <stdio.h>

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