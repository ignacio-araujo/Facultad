

#ifndef __CAMINOS__
#define __CAMINOS__

/*
  Módulo de definición de `caminos.h'.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
 */

#include "lista.h"
#include "grafo.h"


/**
 * Devuelve la cantidad de caminos de largo mínimo desde un vértice dado a todos los demás vértices.
 *
 * @param G     Grafo no dirigido, puede no ser conexo, compuesto de 'n' vértices y 'm' aristas.
 *              Los vértices se numeran del 1 al n y no hay aristas entre un vértice y si mismo,
 *              ni tampoco más de una arista entre dos vértices.
 *              El grafo NO DEBE SER MODIFICADO por la función.
 *               
 *
 * @param s     Vértice origen desde el cual se calculan la cantidad de caminos de largo mínimo.
 *              Se cumple que 1 <= s <= n.
 *
 * @return      Devuelve un arreglo de largo n+1, donde cada índice v, 1 <= v <= n, representa un vértice del
 *              grafo 'G', y su valor la cantidad de caminos de largo mínimo desde el origen 's' al vértice 'v'.  
 *              El índice 0 no se utiliza y su valor puede ser cualquiera.
 *              La cantidad de caminos de largo mínimo desde el origen 's' al vértice 's' es 1.
 *              El algoritmo debe ejecutar en orden O(n + m)
 */

nat* CantCaminos(Grafo G, nat s);

#endif
