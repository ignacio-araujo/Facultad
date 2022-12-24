/*
  Módulo de implementación de `Grafo'.

  `Grafo' representa a los grafos dirigidos o no dirigidos.

  Los vértices se identifican con enteros entre 1 y un cierto número `max'
  establecido al crearlo. No todos los números entre 1 y `max' tienen que ser
  vértices del grafo.

  En las funciones que tienen un parámetro `v' que es un identificador de
  vértice se asume la precondición implícita `1 <= v <= max'.
  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
 */

#include "../include/utils.h"
#include "../include/lista.h"
#include "../include/grafo.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct vertice_t {
  Lista adyacentes;
  nat ingrado;  // aristas entrantes
  nat outgrado; // aristas salientes
  // otros datos
} * Vertice;
static Vertice crear_vertice() {
  Vertice res = new vertice_t;
  res->adyacentes = crear_lista();
  res->ingrado = 0;
  res->outgrado = 0;
  return res;
}
static Vertice copia_vertice(Vertice v) {
  Vertice res = new vertice_t;
  res->adyacentes = copia_lista(v->adyacentes);
  res->ingrado = v->ingrado;
  res->outgrado = v->outgrado;
  return res;
}
static void destruir_vertice(Vertice &v) {
  destruir_lista(v->adyacentes);
  delete v;
}

struct celda_t {
  bool existe;
  Vertice vertice;
};
static void celda_vacia(celda_t &c) {
  c.existe = false;
  c.vertice = NULL;
}
static void iniciar_celda(celda_t &c) {
  c.existe = true;
  c.vertice = crear_vertice();
}

struct rep_grafo {
  celda_t *celdas; // array de celdas
  nat max;         // identificador de vértice más alto
  nat n;           // cantidad de vértices
  nat m;           // cantidad de aristas
  bool dirigido;
};

Grafo crear_grafo(nat max, bool dirigido) {
  Grafo g = new rep_grafo;
  g->max = max;
  g->n = 0;
  g->m = 0;
  g->dirigido = dirigido;
  g->celdas = new celda_t[max + 1]; // se va a ignorar la celda 0
  for (nat i = 1; i <= max; i++)
    celda_vacia(g->celdas[i]);
  return g;
}

nat max_vertice(Grafo g) { return g->max; }

bool es_dirigido(Grafo g) { return g->dirigido; }

Grafo copia_grafo(Grafo g) {
  Grafo copia = crear_grafo(g->max, g->dirigido);
  copia->n = g->n;
  copia->m = g->m;
  copia->dirigido = g->dirigido;
  for (nat i = 1; i <= g->max; i++)
    if (existe_vertice(i, g)) {
      copia->celdas[i].existe = true;
      copia->celdas[i].vertice = copia_vertice(g->celdas[i].vertice);
    }

  return copia;
}

void agregar_vertice(nat v, Grafo &g) {
  assert((1 <= v) && (v <= g->max));
  if (!g->celdas[v].existe) {
    g->n++;
    iniciar_celda(g->celdas[v]);
  }
}

void remover_vertice(nat v, Grafo &g) {
  assert((1 <= v) && (v <= g->max));
  if (g->celdas[v].existe) {
    assert(g->celdas[v].vertice != NULL);
    g->n--;
    g->m -= (g->celdas[v].vertice->outgrado - g->celdas[v].vertice->ingrado);
    Lista lst = g->celdas[v].vertice->adyacentes;
    while (!es_vacia_lista(lst)) {
      g->celdas[primero(lst)].vertice->ingrado--;
      lst = resto(lst);
    }
    destruir_lista(g->celdas[v].vertice->adyacentes);
    celda_vacia(g->celdas[v]);

    // remover aristas entrantes en v
    for (nat i = 1; i <= g->max; i++)
      if (g->celdas[i].existe)
        remover(v, g->celdas[i].vertice->adyacentes);
  }
}

bool existe_vertice(nat v, Grafo g) {
  assert((1 <= v) && (v <= g->max));
  return g->celdas[v].existe;
}

nat out_grado(nat v, Grafo g) {
  assert(existe_vertice(v, g));
  return g->celdas[v].vertice->outgrado;
}

nat in_grado(nat v, Grafo g) {
  assert(existe_vertice(v, g));
  return g->celdas[v].vertice->ingrado;
}

Lista vertices(Grafo g) {
  Lista lst = crear_lista();
  for (nat i = g->max; i > 0; i--)
    if (existe_vertice(i, g))
      insertar(i, lst);
  return lst;
}

nat cantidad_vertices(Grafo g) { return g->n; }

void agregar_arista(nat v1, nat v2, Grafo g) {
  assert(existe_vertice(v1, g) && existe_vertice(v2, g));
  if (!pertenece_a_lista(v2, g->celdas[v1].vertice->adyacentes)) {
    g->m++;
    g->celdas[v1].vertice->outgrado++;
    g->celdas[v2].vertice->ingrado++;
    insertar(v2, g->celdas[v1].vertice->adyacentes);
    if (!g->dirigido) {
      assert(!pertenece_a_lista(v1, g->celdas[v2].vertice->adyacentes));
      g->celdas[v2].vertice->outgrado++;
      g->celdas[v1].vertice->ingrado++;
      insertar(v1, g->celdas[v2].vertice->adyacentes);
      assert(g->celdas[v1].vertice->outgrado == g->celdas[v1].vertice->ingrado);
      assert(g->celdas[v2].vertice->outgrado == g->celdas[v2].vertice->ingrado);
    }
  }
}

void remover_arista(nat v1, nat v2, Grafo g) {
  assert(existe_vertice(v1, g) && existe_vertice(v2, g));
  if (pertenece_a_lista(v2, g->celdas[v1].vertice->adyacentes)) {
    g->m--;
    g->celdas[v1].vertice->outgrado--;
    g->celdas[v2].vertice->ingrado--;
    remover(v2, g->celdas[v1].vertice->adyacentes);
    if (!g->dirigido) {
      assert(pertenece_a_lista(v1, g->celdas[v2].vertice->adyacentes));
      g->celdas[v2].vertice->outgrado--;
      g->celdas[v1].vertice->ingrado--;
      remover(v1, g->celdas[v2].vertice->adyacentes);
      assert(g->celdas[v1].vertice->outgrado == g->celdas[v1].vertice->ingrado);
      assert(g->celdas[v2].vertice->outgrado == g->celdas[v2].vertice->ingrado);
    }
  }
}

bool existe_arista(nat v1, nat v2, Grafo g) {
  assert(existe_vertice(v1, g) && existe_vertice(v2, g));
  return pertenece_a_lista(v2, g->celdas[v1].vertice->adyacentes);
}

nat cantidad_aristas(Grafo g) { return g->m; }

Lista adyacentes(nat v, Grafo g) {
  assert(existe_vertice(v, g));
  return g->celdas[v].vertice->adyacentes;
}

Grafo traspuesto(Grafo g) {
  Grafo t = crear_grafo(g->max, g->dirigido);
  // la cantidad de vértices se establece mediante los agregar_vertice
  t->m = g->m;
  for (nat i = 1; i <= g->max; i++)
    if (existe_vertice(i, g))
      agregar_vertice(i, t);
  for (nat i = 1; i <= g->max; i++)
    if (existe_vertice(i, g)) {
      t->celdas[i].vertice->ingrado = g->celdas[i].vertice->outgrado;
      t->celdas[i].vertice->outgrado = g->celdas[i].vertice->ingrado;
      Lista lst = g->celdas[i].vertice->adyacentes;
      while (!es_vacia_lista(lst)) {
        assert(existe_vertice(primero(lst), t));
        insertar(i, t->celdas[primero(lst)].vertice->adyacentes);
        lst = resto(lst);
      }
    }
  return t;
}

Grafo inducido(Lista vertices, Grafo g) {
  Grafo res = crear_grafo(g->max, g->dirigido);
  Lista lst = vertices;
  // recorrida determinando los vértices de res.
  while (!es_vacia_lista(lst)) {
    assert(existe_vertice(primero(lst), g));
    agregar_vertice(primero(lst), res);
    lst = resto(lst);
  }
  lst = vertices;
  // recorrida estableciendo las aristas
  while (!es_vacia_lista(lst)) {
    nat v = primero(lst);
    Lista ady = g->celdas[v].vertice->adyacentes;
    while (!es_vacia_lista(ady)) {
      if (existe_vertice(primero(ady), res)) {
        res->m++;
        res->celdas[v].vertice->outgrado++;
        res->celdas[primero(ady)].vertice->ingrado++;
        insertar(primero(ady), res->celdas[v].vertice->adyacentes);
      }
      ady = resto(ady);
    }
    lst = resto(lst);
  }
  if (!res->dirigido)
    res->m /= 2;
  return res;
}

void destruir_grafo(Grafo &g) {
  for (nat i = 1; i <= g->max; i++)
    if (existe_vertice(i, g))
      destruir_vertice(g->celdas[i].vertice);
  delete[] g->celdas;
  delete g;
}

void imprimir_grafo(Grafo g) {
  char str_dirigido[15];
  if (g->dirigido)
    strcpy(str_dirigido, "dirigido");
  else
    strcpy(str_dirigido, "no-dirigido");

  printf("%u %s\n", g->max, str_dirigido);
  for (nat i = 1; i <= g->max; i++) {
    if (existe_vertice(i, g)) {
      printf("%u: ", i);
      if (g->dirigido)
        imprimir_lista(g->celdas[i].vertice->adyacentes);
      else {
        Lista aimprimir =
            copia_lista_desde(g->celdas[i].vertice->adyacentes, i + 1);
        imprimir_lista(aimprimir);
        destruir_lista(aimprimir);
      }
      printf("\n");
    }
  }
  printf("0:\n");
  printf("Cantidad de vértices: %u\n", g->n);
  printf("Cantidad de aristas: %u\n", g->m);
  for (nat i = 1; i <= g->max; i++) {
    if (existe_vertice(i, g)) {
      printf("%u: %u, %u\n", i, out_grado(i, g), in_grado(i, g));
    }
  }
}

Grafo leer_grafo() {
  nat max;
  char str_dirigido[20];
  bool dirigido = true; // de manera predeterminada es dirigido
  int cant_scanf = scanf("%u %s", &max, str_dirigido); // cota superior de los
                                                       // identificadores de
                                                       // vértice y dirigido
  assert(cant_scanf == 2);
  (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
  if (!strcmp(str_dirigido, "no-dirigido"))
    dirigido = false;

  Grafo res = crear_grafo(max, dirigido);
  nat v;
  cant_scanf = scanf("%u:", &v);
  assert(cant_scanf == 1);
  assert(v <= max);
  nat anterior = 0;
  // registrar vértices y guardar temporalmente las listas de adyacencia
  Lista *array_temporal = new Lista[max + 1];
  while (v != 0) {
    assert(v > anterior);
    anterior = v;
    agregar_vertice(v, res);
    array_temporal[v] = leer_lista();

    cant_scanf = scanf("%u:", &v);
    assert(cant_scanf == 1);
    assert(v <= max);
  }
  (void)anterior; // para evitar el warning al compilar con -NDEBUG
  // agregar las aristas una por una y verificar que todos los destinos de
  // aristas son vértices del grafo
  for (nat i = 1; i <= res->max; i++) {
    if (existe_vertice(i, res)) {
      // se recorre la lista que se había guardado de manera temporal
      Lista lst = array_temporal[i];
      Lista cursor = lst;
      while (!es_vacia_lista(cursor)) {
        assert((existe_vertice(primero(cursor), res)));
        agregar_arista(i, primero(cursor), res);
        cursor = resto(cursor);
      }
      destruir_lista(lst);
    }
  }
  delete[] array_temporal;
  return res;
}
