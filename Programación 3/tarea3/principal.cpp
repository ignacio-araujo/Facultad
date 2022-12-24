
/*
  Módulo principal de la tarea 3.

Intérprete de comandos para probar los módulos.

Cada comando tiene un nombre y una lista (posiblemente vacía) de parámetros.
Se asume que los comandos están bien formados. Esto implica que:
- el nombre es uno de los descritos más abajo;
- la cantidad y tipo de los parámetros cumplen con lo requerido;
- se cumplen las precondiciones de las operaciones invocadas.

Descripción de los comandos:
leer_dias
  Lee de la entrada la cantidad de dias. El valor de dias queda en la variable "n".
leer_precios
  Lee de la entrada "n" precios. El valor de los precios queda en el arreglo "precios".
leer_disparos
  Lee de la entrada la cantidad de disparos. El valor de disparos queda en la variable "k".
k_disparos
  Ejecuta la función "k_disparos" con "n", "precios", y "k", como valores de entrada. Se 
  almacena el resultado en la variable "e".
k_disparos_stress
  Ejecuta la función "k_disparos" con precios calculados con una función predeterminada. Se 
  almacena el resultado en la variable "e".
imprimir_precios
  Imprime los precios almacenados en el arreglo "precios".
imprimir_estrategia
  Imprime los la ganancia maxima y los intervalos obtenidos en "e".
Fin
  Termina el programa.
# comentario
  Imprime el resto de la línea.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/

#include "include/k_disparos.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/* Estructura para identificar el nombre de comando */
enum enum_cmd_t {
  cmd_fin,
  cmd_leer_dias,
  cmd_leer_precios,
  cmd_leer_disparos,
  cmd_k_disparos,
  cmd_k_disparos_stress,
  cmd_imprimir_precios,
  cmd_imprimir_estrategia,
  cmd_comentario,
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
    {cmd_leer_dias, "leer_dias"},
    {cmd_leer_precios, "leer_precios"},
    {cmd_leer_disparos, "leer_disparos"},
    {cmd_k_disparos, "k_disparos"},
    {cmd_k_disparos_stress, "k_disparos_stress"},
    {cmd_imprimir_precios, "imprimir_precios"},
    {cmd_imprimir_estrategia, "imprimir_estrategia"},
};

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


nat calcular_ganancia(Estrategia* e, nat* precios)
{ 
  nat ganancia = 0;
  for (nat i = 0; i < e->cant_intervalos; i++) {
      ganancia += precios[e->intervalos[i].fin] - precios[e->intervalos[i].inicio];
  }
  return 1000*ganancia;
}

void imprimir_estrategia(Estrategia* e, nat k)
{   
  if (e != NULL) {
    printf("La ganancia maxima con %d disparo(s) es: %d \n", k, e->ganancia_max);
    printf("Los intervalos en los que se realizan los disparos son: \n");
    for (nat i = 0; i < e->cant_intervalos; i++) {
        printf("(%u, %u), ", e->intervalos[i].inicio, e->intervalos[i].fin);
    }
    printf("\n");
  } else
    printf("No hay estrategia calculada \n");
}

void liberar_estrategia(Estrategia* &e)
{
    delete [] e->intervalos;
    delete e;
    e = NULL;
}

int main() {

  const int MAX_LINEA = 100;
  char restolinea[MAX_LINEA];

  int cont_comandos = 0;
  bool salir = false;
  nat n, k;
  nat* precios = NULL;
  int cant_scanf;
  Estrategia* e = NULL;

  while (!salir) {

    mostrar_prompt(cont_comandos);
    enum_cmd_t enum_com = identificador_comando();
    // procesar el comando
    switch (enum_com) {

      case cmd_leer_disparos: {
        cant_scanf = scanf("%u", &k);
        assert(cant_scanf == 1);
        (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
        break;
      }

      case cmd_leer_dias: {
        cant_scanf = scanf("%u", &n);
        assert(cant_scanf == 1);
        (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
        break;
      }

      case cmd_leer_precios: {
        if (precios != NULL)
          delete [] precios;
        precios = new unsigned int[n + 1];
        for (nat i = 1; i <= n; i++)
          cant_scanf = scanf("%d", &precios[i]);

        (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG
        break;
      }

      case cmd_k_disparos: {
        printf("Con precios: ");
        for (nat i = 1; i <= n; i++)
          printf("%d ", precios[i]);
        printf("\n");
        if (e != NULL)
          liberar_estrategia(e);
        e = k_disparos(precios, n, k);
        nat ganancia = calcular_ganancia(e, precios);
        printf("La ganancia maxima con %d disparo(s) es: %d \n", k, ganancia);
        break;
      }

      case cmd_imprimir_precios: {
        if (precios != NULL) {
          printf("Precios: ");
          for (nat i = 1; i <= n; i++)
            printf("%d ", precios[i]);
          printf("\n");
        }
        break;
      }

      case cmd_imprimir_estrategia: {
        imprimir_estrategia(e, k);
        break;
      }

      case cmd_k_disparos_stress: {
        cant_scanf = scanf("%u", &n);
        assert(cant_scanf == 1);
        (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG

        cant_scanf = scanf("%u", &k);
        assert(cant_scanf == 1);
        (void)cant_scanf; // para evitar el warning al compilar con -NDEBUG

        precios = new nat[n + 1];

        for (nat i = 1; i <= n; i++)
          precios[i] = i % 1000;

        if (e != NULL)
          liberar_estrategia(e);
        e = k_disparos(precios, n, k);
        nat ganancia = calcular_ganancia(e, precios);
        printf("La ganancia maxima con %d disparo(s) es: %d \n", k, ganancia);
        break;
      }

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

      default:
        printf("Comando No Reconocido.\n");
        break;
    } // switch
    fgets(restolinea, MAX_LINEA, stdin);
  } // while

  if (precios != NULL)
    delete [] precios;
  if (e != NULL)
    liberar_estrategia(e);
  return 0;
} 

