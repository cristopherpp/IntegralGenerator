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
  LOG_TYPE
};

struct IntegralProblem {
  IntegralType type;
  char integral[32];
  char correctAnswer[32];
  char wrongAnswer[32];
  char optionA[32];
  char optionB[32];
  char correctOption; // 'A' o 'B'
};

void generateIntegralProblem(IntegralProblem &problem);
void buildOptions(IntegralProblem &problem);

#endif
