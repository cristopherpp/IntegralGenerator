#line 1 "/home/cristopher/Desktop/projects/IntegralGenerator/IntegralEngine.h"
#ifndef INTEGRAL_ENGINE_H
#define INTEGRAL_ENGINE_H

#include <Arduino.h>

enum IntegralType {
  POWER_SIMPLE,
  POWER_COEFF,
  CONSTANT_TYPE,
  SIN_TYPE,
  COS_TYPE,
  EXP_TYPE,
  LOG_TYPE,
  INTEGRAL_TYPE_COUNT
};

struct IntegralProblem {
  IntegralType type;
  char integral[24];
  char correctAnswer[24];
  char wrongAnswer[24];
  char optionA[24];
  char optionB[24];
  char correctOption; // 'A' o 'B'
};

void generateIntegralProblem(IntegralProblem &problem);
void generateIntegralProblemByType(IntegralProblem &problem, IntegralType type);
void buildOptions(IntegralProblem &problem);
const char* getIntegralTypeName(IntegralType type);

#endif
