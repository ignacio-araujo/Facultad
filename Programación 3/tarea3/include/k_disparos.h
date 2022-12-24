
#ifndef __K_DISPAROS__
#define __K_DISPAROS__

/*
 * Módulo de definición de `k_disparos.h'.
 *
 * Laboratorio de Programación 3.
 * InCo-FIng-UDELAR
 */

typedef unsigned int nat; // naturales
struct Intervalo { //intervalos de dias
    nat inicio;
    nat fin;
}; 

struct Estrategia { //estrategia para k disparos
    nat ganancia_max;
    nat cant_intervalos;
    Intervalo* intervalos;
};

/*
 * Precondiciones: 
 * - `n' indica la cantidad de dias con identificadores 1 a `n' (se asume `n' >= 1)
 * - `precios' es un arreglo de largo `n' + 1, que indica el precio de las acciones en cada día (se deja la primera posición sin uso).
        Por ejemplo: precios[1] es el precio de una acción el primer dia, precios[2] es el precio de una acción el segundo día, y así sucesivamente.
 * - `k' es la cantidad de disparos que se pueden realizar
 * 
 * Postcondiciones:
 *  - Se retorna una estrategia de mayor ganancia utilizando el registro Estrategia. Dicha estrategia debe cumplir con las siguientes restricciones:
 *    - `ganancia_max' debe guardar la ganancia máxima.
 *    - `cant_intervalos' <= k
 *    - `intervalos' debe ser un arreglo de largo `cant_intervalos' (o NULL si `cant_intervalos' == 0)
 *      que guarda los intervalos de compra y venta en los que se realizan los disparos. 
 *      Donde para todo intervalo i, 0 <= i < `cant_intervalos', se cumple que:
 *              - 1 <= intervalos[i].inicio <= `n'
 *              - 1 <= intervalos[i].fin <= `n'
 *              - intervalos[i].inicio < intervalos[i].fin
 *      y para todo intervalo i, 0 <= i < `cant_intervalos' - 1, se cumple que:
 *              - intervalos[i].fin < intervalos[i+1].inicio
 */
Estrategia* k_disparos(nat* precios, nat n, nat k);

#endif
