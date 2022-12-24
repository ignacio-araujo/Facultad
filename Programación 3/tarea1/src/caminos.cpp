/*
  Módulo de implementación de `caminos'.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
 */
#include "../include/caminos.h"
#include "../include/cola.h"
#include <cstddef>

nat* CantCaminos(Grafo G, nat s)
{
  nat max = cantidad_vertices(G)+1;

  nat* res = new nat[max];
  bool* descubiertos = new bool[max];
  nat* distancia = new nat[max];

  for(nat i = 0; i < max; i++){
    res[i] = 0;
    descubiertos[i] = false;
    distancia[i] = 0;
  }
  res[s] = 1;
  descubiertos[s] = true;

  Cola queue = crear_cola();
  encolar(s, queue);

  while(!es_vacia_cola(queue)){
    nat v = frente(queue);
    Lista vecinos = adyacentes(v, G);
    
    while(!es_vacia_lista(vecinos)){
      nat u = primero(vecinos);
      if(!descubiertos[u]){
        descubiertos[u] = true;
        res[u] = res[v];
        distancia[u] = distancia[v]+1;
        encolar(u, queue);
      }
      else
        if(distancia[u] == distancia[v]+1)
          res[u] = res[u]+res[v];

      vecinos = resto(vecinos);
    }
    desencolar(queue);
  }
  destruir_cola(queue);
  delete[] descubiertos;
  delete[] distancia;
  
  return res;
}
