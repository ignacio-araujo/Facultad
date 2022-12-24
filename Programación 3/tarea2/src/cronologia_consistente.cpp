/*
  Módulo de implementación de `cronologia_consistente'.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/
#include <stdio.h>

#include "../include/cronologia_consistente.h"
#include "../include/grafo.h"
#include "../include/lista.h"

static void es_dag(nat n, Grafo g, bool &existe, Cronologia &cr);

Cronologia cronologia_consistente(nat n, ListaDatos tipo1, ListaDatos tipo2)
{
  bool existe_cr_consistente = false;

  Cronologia cr = new evento[2*n];

  // Determinar si existe o no una cronología consistente con los datos, 
  // en caso de existir, construirla en cr 
  Grafo g = crear_grafo(2*n, true);
  for(nat i=1; i<=2*n; i++) //De 1..n para nacimientos, de n+1...2n para muertes
    agregar_vertice(i, g);
  for(nat i=1; i<=n; i++)
    agregar_arista(i, i+n, g);

  /*Para acceder a un nacimiento de cierta persona i (1<=i<=n) usamos el elemento en la posición i
    Para acceder a una muerte de cierta persona i (1<=i<=n) usamos el elemento en la posición n+i*/

  while(!es_vacia_lista_datos(tipo1)){
    agregar_arista(n+id1(primer_dato(tipo1)), id2(primer_dato(tipo1)), g);
    tipo1 = resto_datos(tipo1);
  }
  while(!es_vacia_lista_datos(tipo2)){
    agregar_arista(id1(primer_dato(tipo2)), n+id2(primer_dato(tipo2)), g);
    agregar_arista(id2(primer_dato(tipo2)), n+id1(primer_dato(tipo2)), g);
    tipo2 = resto_datos(tipo2);
  }

  es_dag(n, g, existe_cr_consistente, cr);
  destruir_grafo(g);

  if(existe_cr_consistente) { // si existe, retornarla en cr
    return cr;
  } else {// si no existe, liberar la memoria asociada a cr y retornar NULL   
    delete[] cr;
    return NULL;
  }
}

void es_dag(nat n, Grafo g, bool &existe, Cronologia &cr){
  nat indiceActual = 0; //Para recorrer cr
  nat* gradosIncidentes = new nat[2*n+1];
  nat total = 0; //Cantidad total de grados incidentes
  Lista sinIncidentes = crear_lista();
  
  for(nat i=1; i<=2*n; i++){
    gradosIncidentes[i] = in_grado(i, g);
    total += in_grado(i, g);
    if(in_grado(i, g) == 0) 
      insertar_al_final(i, sinIncidentes);
  }

  comienzo(sinIncidentes);
  
  while(existe_actual(sinIncidentes)){
    nat v = actual(sinIncidentes);
    //Usamos v+n<=2*n, porque si usaramos v-n al ser v y n nat nunca queda negativo, que es lo que necesitariamos
    if(v<=n){ //Es del tipo 'nace'
      cr[indiceActual].id = v;
      cr[indiceActual].tipo = nace;
    }
    else{ //Es del tipo 'muere'
      cr[indiceActual].id = v-n;
      cr[indiceActual].tipo = muere;
    }
    indiceActual++;
    
    Lista adyacentesV = adyacentes(v, g);
    comienzo(adyacentesV);
    while(existe_actual(adyacentesV)){
      nat m = siguiente(adyacentesV);
      gradosIncidentes[m]--;
      total--;
      if(gradosIncidentes[m] == 0)
        insertar_al_final(m, sinIncidentes);
    }
    v = siguiente(sinIncidentes);
  }

  destruir_lista(sinIncidentes);
  delete[] gradosIncidentes;
  
  if(total == 0) existe = true;
  else existe = false; 
}