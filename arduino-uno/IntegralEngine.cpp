#include "IntegralEngine.h"

static void generarPotenciaSimple(ProblemaIntegral &p, NivelDificultad d);
static void generarPotenciaCoef(ProblemaIntegral &p, NivelDificultad d);
static void generarConstante(ProblemaIntegral &p, NivelDificultad d);
static void generarSeno(ProblemaIntegral &p, NivelDificultad d);
static void generarCoseno(ProblemaIntegral &p, NivelDificultad d);
static void generarExponencial(ProblemaIntegral &p, NivelDificultad d);
static void generarLog(ProblemaIntegral &p, NivelDificultad d);
static void generarPotenciaNeg(ProblemaIntegral &p, NivelDificultad d);
static void generarRaiz(ProblemaIntegral &p, NivelDificultad d);
static void generarSumaPolinomio(ProblemaIntegral &p, NivelDificultad d);
static void generarSumaMixta(ProblemaIntegral &p, NivelDificultad d);

void generarProblemaIntegral(ProblemaIntegral &problema) {
  TipoIntegral tipo = (TipoIntegral)random(0, CANTIDAD_TIPOS_INTEGRAL);
  NivelDificultad dificultad = (NivelDificultad)random(0, 3);
  generarProblemaIntegralPorTipoYDificultad(problema, tipo, dificultad);
}

void generarProblemaIntegralPorTipo(ProblemaIntegral &problema, TipoIntegral tipo) {
  generarProblemaIntegralPorTipoYDificultad(problema, tipo, DIF_MEDIO);
}

void generarProblemaIntegralPorTipoYDificultad(
  ProblemaIntegral &problema,
  TipoIntegral tipo,
  NivelDificultad dificultad
) {
  problema.tipo = tipo;
  problema.dificultad = dificultad;

  switch (tipo) {
    case POTENCIA_SIMPLE:   generarPotenciaSimple(problema, dificultad); break;
    case POTENCIA_COEF:    generarPotenciaCoef(problema, dificultad); break;
    case TIPO_CONSTANTE:  generarConstante(problema, dificultad); break;
    case TIPO_SENO:       generarSeno(problema, dificultad); break;
    case TIPO_COSENO:       generarCoseno(problema, dificultad); break;
    case TIPO_EXPONENCIAL:       generarExponencial(problema, dificultad); break;
    case TIPO_LOG:       generarLog(problema, dificultad); break;
    case TIPO_POTENCIA_NEG: generarPotenciaNeg(problema, dificultad); break;
    case TIPO_RAIZ:      generarRaiz(problema, dificultad); break;
    case TIPO_SUMA_POL:  generarSumaPolinomio(problema, dificultad); break;
    case TIPO_SUMA_MIXTA: generarSumaMixta(problema, dificultad); break;
    default:             generarPotenciaSimple(problema, dificultad); break;
  }

  construirOpciones(problema);
}

void construirOpciones(ProblemaIntegral &problema) {
  if (random(0, 2) == 0) {
    strncpy(problema.opcionA, problema.respuestaCorrecta, sizeof(problema.opcionA));
    strncpy(problema.opcionB, problema.respuestaIncorrecta, sizeof(problema.opcionB));
    problema.opcionCorrecta = 'A';
  } else {
    strncpy(problema.opcionA, problema.respuestaIncorrecta, sizeof(problema.opcionA));
    strncpy(problema.opcionB, problema.respuestaCorrecta, sizeof(problema.opcionB));
    problema.opcionCorrecta = 'B';
  }

  problema.opcionA[sizeof(problema.opcionA) - 1] = '\0';
  problema.opcionB[sizeof(problema.opcionB) - 1] = '\0';
}

const char* obtenerNombreTipoIntegral(TipoIntegral tipo) {
  switch (tipo) {
    case POTENCIA_SIMPLE: return "Potencias";
    case POTENCIA_COEF: return "Pot. coef";
    case TIPO_CONSTANTE: return "Constantes";
    case TIPO_SENO: return "Seno";
    case TIPO_COSENO: return "Coseno";
    case TIPO_EXPONENCIAL: return "Exponencial";
    case TIPO_LOG: return "Logaritmica";
    case TIPO_POTENCIA_NEG: return "Pot. negativas";
    case TIPO_RAIZ: return "Raices";
    case TIPO_SUMA_POL: return "Sumas pol";
    case TIPO_SUMA_MIXTA: return "Sumas mixtas";
    default: return "Desconocida";
  }
}

const char* obtenerNombreDificultad(NivelDificultad dificultad) {
  switch (dificultad) {
    case DIF_FACIL: return "Facil";
    case DIF_MEDIO: return "Medio";
    case DIF_DIFICIL: return "Dificil";
    default: return "?";
  }
}

static void generarPotenciaSimple(ProblemaIntegral &p, NivelDificultad d) {
  int n;

  if (d == DIF_FACIL) n = random(1, 3);
  else if (d == DIF_MEDIO) n = random(3, 7);
  else n = random(7, 11);

  int nuevoExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S x^%d dx", n);
  snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "x^%d/%d+C", nuevoExp, nuevoExp);

  int tipoIncorrecto = random(0, 3);
  if (tipoIncorrecto == 0) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^%d+C", nuevoExp);
  } else if (tipoIncorrecto == 1) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%d+C", n, n - 1);
  } else {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^%d/%d+C", n, n);
  }
}

static void generarPotenciaCoef(ProblemaIntegral &p, NivelDificultad d) {
  int a, n;

  if (d == DIF_FACIL) {
    a = random(2, 5);
    n = random(1, 3);
  } else if (d == DIF_MEDIO) {
    a = random(3, 8);
    n = random(3, 6);
  } else {
    a = random(4, 10);
    n = random(6, 10);
  }

  int nuevoExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S %dx^%d dx", a, n);

  if (a % nuevoExp == 0) {
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dx^%d+C", a / nuevoExp, nuevoExp);
  } else {
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dx^%d/%d+C", a, nuevoExp, nuevoExp);
  }

  int tipoIncorrecto = random(0, 3);
  if (tipoIncorrecto == 0) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%d+C", a, nuevoExp);
  } else if (tipoIncorrecto == 1) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%d+C", a * n, n - 1);
  } else {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%d/%d+C", a, n, n);
  }
}

static void generarConstante(ProblemaIntegral &p, NivelDificultad d) {
  int k;

  if (d == DIF_FACIL) k = random(2, 6);
  else if (d == DIF_MEDIO) k = random(6, 13);
  else k = random(13, 31);

  snprintf(p.integral, sizeof(p.integral), "S %d dx", k);
  snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dx+C", k);

  if (random(0, 2) == 0) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%d+C", k);
  } else {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^2+C", k);
  }
}

static void generarSeno(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S sin(x) dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "-cos(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "cos(x)+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "sin(x)+C");
    }

  } else if (d == DIF_MEDIO) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %dsin(x)dx", a);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "-%dcos(x)+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dcos(x)+C", a);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dsin(x)+C", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int potencia = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dsin(x^%d)", a * n, potencia, n);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "-%dcos(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dcos(x^%d)+C", a, n);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "-%dx^%dcos(x^%d)", a, potencia, n);
    }
  }
}

static void generarCoseno(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S cos(x) dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "sin(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "-sin(x)+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "cos(x)+C");
    }

  } else if (d == DIF_MEDIO) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %dcos(x)dx", a);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dsin(x)+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "-%dsin(x)+C", a);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dcos(x)+C", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int potencia = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dcos(x^%d)", a * n, potencia, n);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dsin(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "-%dsin(x^%d)+C", a, n);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%dsin(x^%d)", a, potencia, n);
    }
  }
}

static void generarExponencial(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S e^x dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "e^x+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "xe^(x-1)");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "xe^x");
    }

  } else if (d == DIF_MEDIO) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %de^x dx", a);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%de^x+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "e^x+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dxe^x", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int potencia = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%de^(x^%d)", a * n, potencia, n);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%de^(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "e^(x^%d)+C", n);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%de^(x^%d)", a, potencia, n);
    }
  }
}

static void generarLog(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S 1/x dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "ln|x|+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/x^2+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/x+C");
    }

  } else if (d == DIF_MEDIO) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %d/x dx", a);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dln|x|+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%d/x^2+C", a);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "ln|x|+C");
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int potencia = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%d/x^%d", a * n, potencia, n);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dln|x|+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "ln|x|+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^%d+C", a, potencia);
    }
  }
}

static void generarPotenciaNeg(ProblemaIntegral &p, NivelDificultad d) {
  int n;

  if (d == DIF_FACIL) n = 2;
  else if (d == DIF_MEDIO) n = random(2, 4);
  else n = random(4, 7);

  snprintf(p.integral, sizeof(p.integral), "S 1/x^%d dx", n);

  if (n == 2) {
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "-1/x+C");
  } else {
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "-1/(%dx^%d)+C", n - 1, n - 1);
  }

  if (random(0, 2) == 0) {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/x^%d+C", n - 1);
  } else {
    snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/x^%d+C", n + 1);
  }
}

static void generarRaiz(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S sqrt(x)dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "2x^(3/2)/3+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^(3/2)+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/sqrt(x)+C");
    }

  } else if (d == DIF_MEDIO) {
    snprintf(p.integral, sizeof(p.integral), "S 1/sqrt(x)dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "2sqrt(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "sqrt(x)+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1/(2sqrt(x))+C");
    }

  } else {
    int a = random(2, 6);
    snprintf(p.integral, sizeof(p.integral), "S %d/sqrt(x)dx", a);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dsqrt(x)+C", 2 * a);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dsqrt(x)+C", a);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%d/(2sqrt(x))+C", a);
    }
  }
}

static void generarSumaPolinomio(ProblemaIntegral &p, NivelDificultad d) {
  int a = random(1, 5);
  int b = random(1, 5);

  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S (x^2+x)dx");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "x^3/3+x^2/2+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^3+x^2+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "2x+1+C");
    }

  } else if (d == DIF_MEDIO) {
    snprintf(p.integral, sizeof(p.integral), "S (%dx^2+%dx)dx", a, b);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dx^3/3+%dx^2/2", a, b);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^3+%dx^2", a, b);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx+%d", 2 * a, b);
    }

  } else {
    int c = random(1, 5);
    snprintf(p.integral, sizeof(p.integral), "S (%dx^3+%dx+%d)", a, b, c);
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "%dx^4/4+%dx^2/2+%dx", a, b, c);

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^4+%dx^2+%dx", a, b, c);
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "%dx^2+%d", 3 * a, b);
    }
  }
}

static void generarSumaMixta(ProblemaIntegral &p, NivelDificultad d) {
  if (d == DIF_FACIL) {
    snprintf(p.integral, sizeof(p.integral), "S (x+sin(x))");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "x^2/2-cos(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^2-cos(x)+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "1-cos(x)+C");
    }

  } else if (d == DIF_MEDIO) {
    snprintf(p.integral, sizeof(p.integral), "S (e^x+1/x)");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "e^x+ln|x|+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "e^x+1/x+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "xe^(x-1)+ln|x|");
    }

  } else {
    snprintf(p.integral, sizeof(p.integral), "S (x^2+e^x)");
    snprintf(p.respuestaCorrecta, sizeof(p.respuestaCorrecta), "x^3/3+e^x+C");

    if (random(0, 2) == 0) {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "x^3+e^x+C");
    } else {
      snprintf(p.respuestaIncorrecta, sizeof(p.respuestaIncorrecta), "2x+e^x+C");
    }
  }
}
