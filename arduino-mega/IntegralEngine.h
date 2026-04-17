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
  NEG_POWER_TYPE,
  ROOT_TYPE,
  POLY_SUM_TYPE,
  MIXED_SUM_TYPE,
  INTEGRAL_TYPE_COUNT
};

enum DifficultyLevel {
  DIFF_EASY,
  DIFF_MEDIUM,
  DIFF_HARD
};

struct IntegralProblem {
  IntegralType type;
  DifficultyLevel difficulty;

  char integral[48];
  char correctAnswer[48];
  char wrongAnswer[48];
  char optionA[48];
  char optionB[48];
  char correctOption;
};

void generateIntegralProblem(IntegralProblem &problem);
void generateIntegralProblemByType(IntegralProblem &problem, IntegralType type);
void generateIntegralProblemByTypeAndDifficulty(
  IntegralProblem &problem,
  IntegralType type,
  DifficultyLevel difficulty
);

void buildOptions(IntegralProblem &problem);
const char* getIntegralTypeName(IntegralType type);
const char* getDifficultyName(DifficultyLevel difficulty);

#endif
