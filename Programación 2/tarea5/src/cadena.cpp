#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct nodoCadena {
  TInfo dato;
  TLocalizador anterior;
  TLocalizador siguiente;
};

struct _rep_cadena {
  TLocalizador inicio;
  TLocalizador final;
};

bool esLocalizador(TLocalizador loc) { return loc != NULL; }

TCadena crearCadena() {
  TCadena res = new _rep_cadena;
  res->inicio = res->final = NULL;
  return res;
}

void liberarCadena(TCadena cad) {
  TLocalizador borrar;
  while(cad->inicio != NULL){
    liberarInfo(cad->inicio->dato);
    borrar = cad->inicio;
    cad->inicio = cad->inicio->siguiente;
    delete borrar; //Este tambien lo deleteamos, porque cuando insertemos algo se va a ser como un new nodoCadena
  }
  delete cad; //Delete porque se creo con new en crearCadena
}

bool esVaciaCadena(TCadena cad) {
  return (cad->inicio == NULL && cad->final == NULL);
}

TLocalizador inicioCadena(TCadena cad) {
  assert(!esVaciaCadena(cad) || cad->inicio == NULL);
  return cad->inicio;
}

TLocalizador finalCadena(TCadena cad) {
  assert(!esVaciaCadena(cad) || cad->final == NULL);
  return cad->final;
}

TInfo infoCadena(TLocalizador loc, TCadena cad) {
  return loc->dato;
}

TLocalizador siguiente(TLocalizador loc, TCadena cad) {
  return loc->siguiente;
}

TLocalizador anterior(TLocalizador loc, TCadena cad) {
  return loc->anterior;
}

bool esFinalCadena(TLocalizador loc, TCadena cad) {
  if(esVaciaCadena(cad)) return false;
  else return (finalCadena(cad) == loc);
}

bool esInicioCadena(TLocalizador loc, TCadena cad) {
  if(esVaciaCadena(cad)) return false;
  else return (inicioCadena(cad) == loc);
}

TCadena insertarAlFinal(TInfo i, TCadena cad) {
  TLocalizador insertar = new nodoCadena;
  insertar->dato = i;
  insertar->siguiente = NULL;
  
  if(esVaciaCadena(cad)){
    insertar->anterior = NULL;
    cad->inicio = insertar;
    cad->final = insertar;
  }else{
    insertar->anterior = cad->final;
    cad->final->siguiente = insertar;
    cad->final = insertar;
  }
  return cad;
}

TCadena insertarAntes(TInfo i, TLocalizador loc, TCadena cad) {
  TLocalizador insertar = new nodoCadena;
  insertar->dato = i;
  
  insertar->anterior = loc->anterior;
  insertar->siguiente = loc;
  if(loc->anterior == NULL) cad->inicio = insertar;
  else loc->anterior->siguiente = insertar;
  loc->anterior = insertar;
  
  return cad;
}

TCadena removerDeCadena(TLocalizador loc, TCadena cad) {
  if(loc->siguiente == NULL)
    cad->final = loc->anterior;
  else 
    loc->siguiente->anterior = loc->anterior;
  if(loc->anterior == NULL)
    cad->inicio = loc->siguiente;
  else
    loc->anterior->siguiente = loc->siguiente;

  liberarInfo(loc->dato);
  delete loc;
  return cad;
}

void imprimirCadena(TCadena cad) {
  TLocalizador pos = cad->inicio;
  while(pos != NULL){
    ArregloChars txt = infoATexto(pos->dato);
    printf("%s", txt);
    delete [] txt;
    pos = pos->siguiente;
  }
  printf("\n");
}

TLocalizador kesimo(nat k, TCadena cad) {
  if(k == 0) return NULL;
  else{
    TLocalizador pos = cad->inicio;
    unsigned int i = 1;
    while(pos != NULL && i<k){
      pos = pos->siguiente;
      i++;
    }
    return pos;
  }
}

TCadena insertarSegmentoDespues(TCadena sgm, TLocalizador loc, TCadena cad) {
  if(!esVaciaCadena(sgm)){
    if(esVaciaCadena(cad)){
      cad->inicio = sgm->inicio;
      cad->final = sgm->final;
    }
    else{
      if(loc->siguiente == NULL) cad->final = sgm->final;
      else sgm->final->siguiente = loc->siguiente; 
      loc->siguiente = sgm->inicio;
      sgm->inicio->anterior = loc;
    }
  }
  delete sgm;
  return cad;
}

TCadena copiarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad) {
  TCadena segmento = crearCadena();
  if(!esVaciaCadena(cad)){
    TLocalizador pos = desde;
    while(pos != hasta->siguiente){
      TInfo dato_copia = copiaInfo(pos->dato);
      segmento = insertarAlFinal(dato_copia, segmento);
      pos = pos->siguiente;
    }
  }
  return segmento;
}

TCadena borrarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad) {
  if(!esVaciaCadena(cad)){
    TLocalizador tope;
    if(hasta == NULL) tope = NULL; //En caso de que se trate borrar hasta un elemento null, porque no va a tener siguiente
    else tope = hasta->siguiente;
    while(desde != tope){
      TLocalizador temp = desde->siguiente;
      cad = removerDeCadena(desde, cad);
      desde = temp;
    }
  }
  return cad;
}

TCadena cambiarEnCadena(TInfo i, TLocalizador loc, TCadena cad) {
  loc->dato = i;
  return cad;
}

TCadena intercambiar(TLocalizador loc1, TLocalizador loc2, TCadena cad) {
  TInfo temp = loc1->dato;
  loc1->dato = loc2->dato;
  loc2->dato = temp;
  return cad;
}

bool localizadorEnCadena(TLocalizador loc, TCadena cad) {
  if(esVaciaCadena(cad)) return false;
  else{
    TLocalizador pos = inicioCadena(cad);
    bool res = false;
    while(!res && pos != NULL){
      if(pos == loc) res = true;
      pos = pos->siguiente;
    }
    return res;
  }
}

bool precedeEnCadena(TLocalizador loc1, TLocalizador loc2, TCadena cad) {
  if(esVaciaCadena(cad)) return false;
  else if(loc1 == loc2) return true;
  else if(loc1 != NULL && loc2 != NULL)
    if(loc1->siguiente == loc2) return true;
  return false;
}

TLocalizador siguienteClave(nat clave, TLocalizador loc, TCadena cad) {
  TLocalizador res = NULL;
  if(!esVaciaCadena(cad)){
    TLocalizador aux = loc;
    while(aux != NULL && res == NULL){
      if(natInfo(aux->dato) == clave) res = aux;
      aux = siguiente(aux, cad);
    }
  }
  return res;
}

TLocalizador anteriorClave(nat clave, TLocalizador loc, TCadena cad) {
  TLocalizador res = NULL;
  if(!esVaciaCadena(cad)){
    TLocalizador aux = loc;
    while(aux != NULL && res == NULL){
      if(natInfo(aux->dato) == clave) res = aux;
      aux = anterior(aux, cad);
    }
  }
  return res;
}

TLocalizador menorEnCadena(TLocalizador loc, TCadena cad) {
  TLocalizador res = loc;
  nat menor = natInfo(loc->dato);

  TLocalizador aux = siguiente(loc, cad);
  while(aux != NULL){
    if(natInfo(aux->dato) < menor){
      res = aux;
      menor = natInfo(aux->dato);
    }
    aux = siguiente(aux, cad);
  }
  return res;
}
