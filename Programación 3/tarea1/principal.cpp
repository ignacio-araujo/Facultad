
/*
  Módulo principal de la tarea 1.

Intérprete de comandos para probar los módulos.

Está definido un grafo `g'.

Cada comando tiene un nombre y una lista (posiblemente vacía) de parámetros.
Se asume que los comandos están bien formados. Esto implica que:
- el nombre es uno de los descritos más abajo;
- la cantidad y tipo de los parámetros cumplen con lo requerido;
- se cumplen las precondiciones de las operaciones invocadas.

Descripción de los comandos:

Fin
   Termina el programa
# comentario
   Imprime el resto de la línea.
leer_grafo <grafo en formato leer_grafo>
   Lee `g' siguiendo el formato de `leer_grafo' del módulo `grafo'.
cant_caminos <i>
   Ejecuta la función `CantCaminos' del módulo `caminos' en `g', tomando el vértice `i' como el inicial.
test_stress <t>
  realiza el test de stress invocando `CantCaminos' con un grafo de `t' vértices (y para el vértice 1).
destruir_estructuras
   Destruye las estructuras (liberando la memoria asignada) y las vuelve a
crear.
imprimir_grafo
  Imprime el grafo cargado

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/

#include "include/utils.h"
#include "include/grafo.h"
#include "include/caminos.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/* Estructura para identificar el nombre de comando */
enum enum_cmd_t {
  cmd_fin,
  cmd_comentario,
  cmd_leer_grafo,
  cmd_test_stress,
  cmd_cant_caminos,
  cmd_destruir_estructuras,
  cmd_imprimir_grafo,
  no_reconocido
};
const int cant_com = no_reconocido;

struct enum_nombre_cmd {
  const enum_cmd_t enum_cmd;
  const char *nombre_cmd;
};

/*
  Asociación entre nombre de comando y enumerado.
 */
const enum_nombre_cmd cmds[] = {
    {cmd_fin, "Fin"},
    {cmd_comentario, "#"},
    {cmd_leer_grafo, "leer_grafo"},
    {cmd_test_stress, "test_stress"},
    {cmd_cant_caminos, "cant_caminos"},
    {cmd_destruir_estructuras, "destruir_estructuras"},
    {cmd_imprimir_grafo, "imprimir_grafo"}};

/*
  Devuelve el identificador de comando leido desde la entrada estándar.
 */
enum_cmd_t identificador_comando() {
  enum_cmd_t res = no_reconocido;

  const int MAX_NOMBRE = 50;
  char nom_com[MAX_NOMBRE];
  int cant_scanf = scanf("%s", nom_com);
  assert(cant_scanf == 1);
  (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
  bool encontrado = false;
  int i = cmd_fin; // = 0
  while ((!encontrado) && (i < cant_com)) {
    if (0 == strcmp(nom_com, cmds[i].nombre_cmd)) {
      res = cmds[i].enum_cmd;
      encontrado = true;
    } else {
      i++;
    }
  }
  return res;
} // identificador_comando

/*
  Incrementa el contador de comandos y muestra el prompt.
 */
void mostrar_prompt(int &cont_comandos) {
  cont_comandos++;
  printf("%u>", cont_comandos);
} // mostrar_prompt


/* Devuelve en `str_numero' la conversión de ´nat' a  string. */
void nat_a_string(char *str_numero, nat nat) {
  if (nat == UINT_MAX)
    strcpy(str_numero, "NO");
  else
    sprintf(str_numero, "%u", nat);
} // nat_a_string


int main() {
  Grafo g = crear_grafo(1, false);

  const int MAX_LINEA = 100;
  char restolinea[MAX_LINEA];

  int cont_comandos = 0;
  bool salir = false;
  while (!salir) {

    mostrar_prompt(cont_comandos);
    enum_cmd_t enum_com = identificador_comando();

    // procesar el comando
    switch (enum_com) {
    case cmd_fin: {
      salir = true;
      printf("Fin.\n");
      break;
    }

    case cmd_comentario: {
      fgets(restolinea, MAX_LINEA, stdin);
      printf("%s", restolinea);
      ungetc('\n', stdin);
      break;
    }

    case cmd_leer_grafo: {
      destruir_grafo(g);
      g = leer_grafo();
      break;
    }

    case cmd_test_stress: {
      destruir_grafo(g);
      nat tamano;
      int cant_scanf = scanf("%u", &tamano);
      assert(cant_scanf == 1);
      (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
      
      // se crea el grafo con `tamano' vértices
      g = crear_grafo(tamano, false);

      for (nat i = 1; i <= tamano; i++) {
        agregar_vertice(i, g);
      }
      for (nat i = 1; i <= tamano - 1; i++) {
        agregar_arista(i, i+1, g);
        if (i * 2 <= tamano) {
          agregar_arista(i, 2*i, g);
        }
      }

      nat* sol = CantCaminos(g, 1);
      printf("Se imprimen los primeros 100: \n");
      if (sol != NULL) {
        for(nat v=1; v <= 100; v++) {
          printf("%i ", sol[v]);
        }
        delete [] sol; 
      }     
      printf("\n");

      break;
    }  

    case cmd_cant_caminos: {
      nat vertice;
      int cant_scanf = scanf("%u", &vertice);
      assert(cant_scanf == 1);
      (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
      nat* sol = CantCaminos(g, vertice);

      if (sol != NULL) {
        for(nat v=1; v <= cantidad_vertices(g); v++) {
          printf("%i ", sol[v]);
        }
        delete [] sol;
      }
      printf("\n");

      break;
    }

    case cmd_destruir_estructuras: {
      destruir_grafo(g);
      g = crear_grafo(1, false);
      break;
    }

    case cmd_imprimir_grafo: {
      imprimir_grafo(g);
      break;
    }

    default:
      printf("Comando No Reconocido.\n");
      break;
    } // switch
    fgets(restolinea, MAX_LINEA, stdin);
  } // while

  destruir_grafo(g);
  return 0;
}
