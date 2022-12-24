/*
  Módulo de implementación de `k_disparos'.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/
#include <stdio.h>
#include "../include/k_disparos.h"

static int max(int a, int b);

Estrategia* k_disparos(nat* precios, nat n, nat k)
{
  Estrategia* ret = new Estrategia;
  ret->cant_intervalos = 0;
  ret->intervalos = NULL;
  ret->ganancia_max = 0;

  // Determinar la mejor estrategia
  nat **ganancias = new nat*[k+1];
  
  //Inicializar ganancias
  for(nat i=0; i<=k; i++){
    ganancias[i] = new nat[n];
    for(nat j=0; j<n; j++)
      ganancias[i][j] = 0;
  }

  //Armar ganancias
  int maximoK = 0; //Guarda el indice de la fila (k) donde se encontro por primera vez el máximo, porque puede darse con un número menor de transacciones
  int maximoJ = 0; //Idem para las columnas (j)
  for(nat i=1; i<=k; i++){
    int diferencia = -precios[1];
    for(nat j=2; j<=n; j++){
      ganancias[i][j-1] = max(precios[j]+diferencia, ganancias[i][j-2]);
      if(ganancias[i][j-1] > ret->ganancia_max){
        maximoK = i;
        maximoJ = j;
        ret->ganancia_max = ganancias[i][j-1];
      }
      diferencia = max(ganancias[i-1][j-2]-precios[j], diferencia);
    }
  }

  //Armar Intervalos
  ret->cant_intervalos = maximoK;
  ret->intervalos = new Intervalo[maximoK];
  
  int intervaloActual = maximoK-1;
  nat kActual = maximoK;
  nat jActual = maximoJ-1;

  while(intervaloActual >= 0){
    ret->intervalos[intervaloActual].fin = jActual+1;
    
    int diferenciaAnterior = ganancias[kActual][jActual]-precios[jActual+1];

    kActual--;
    jActual = 0;
    int diferencia = ganancias[kActual][jActual]-precios[jActual+1];
    while(diferencia != diferenciaAnterior){
      jActual++;
      diferencia = ganancias[kActual][jActual]-precios[jActual+1];
    }

    ret->intervalos[intervaloActual].inicio = jActual+1;
    
    int gananciaAnterior = ganancias[kActual][jActual];

    jActual = 0;
    int ganancia = ganancias[kActual][jActual];
    while(ganancia != gananciaAnterior){
      jActual++;
      ganancia = ganancias[kActual][jActual];
    }

    intervaloActual--;
  }

  //Liberamos ganancias
  for(nat i=0; i<=k; i++)
    delete[] ganancias[i];
  delete[] ganancias;

  return ret;
}

int max(int a, int b){
  if(a>=b) return a;
  else return b;
}