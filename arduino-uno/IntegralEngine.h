#ifndef INTEGRAL_ENGINE_H
#define INTEGRAL_ENGINE_H

#include <Arduino.h>

enum TipoIntegral {
  POTENCIA_SIMPLE,
  POTENCIA_COEF,
  TIPO_CONSTANTE,
  TIPO_SENO,
  TIPO_COSENO,
  TIPO_EXPONENCIAL,
  TIPO_LOG,
  TIPO_POTENCIA_NEG,
  TIPO_RAIZ,
  TIPO_SUMA_POL,
  TIPO_SUMA_MIXTA,
  CANTIDAD_TIPOS_INTEGRAL
};

enum NivelDificultad {
  DIF_FACIL,
  DIF_MEDIO,
  DIF_DIFICIL
};

struct ProblemaIntegral {
  TipoIntegral tipo;
  NivelDificultad dificultad;

  char integral[24];
  char respuestaCorrecta[24];
  char respuestaIncorrecta[24];
  char opcionA[24];
  char opcionB[24];
  char opcionCorrecta;
};

void generarProblemaIntegral(ProblemaIntegral &problema);
void generarProblemaIntegralPorTipo(ProblemaIntegral &problema, TipoIntegral tipo);
void generarProblemaIntegralPorTipoYDificultad(
  ProblemaIntegral &problema,
  TipoIntegral tipo,
  NivelDificultad dificultad
);

void construirOpciones(ProblemaIntegral &problema);
const char* obtenerNombreTipoIntegral(TipoIntegral tipo);
const char* obtenerNombreDificultad(NivelDificultad dificultad);

#endif
