#include "IntegralEngine.h"

static void generatePowerSimple(IntegralProblem &p);
static void generatePowerCoeff(IntegralProblem &p);
static void generateConstant(IntegralProblem &p);
static void generateSin(IntegralProblem &p);
static void generateCos(IntegralProblem &p);
static void generateExp(IntegralProblem &p);
static void generateLog(IntegralProblem &p);

void generateIntegralProblem(IntegralProblem &problem) {
  int type = random(0, 7);

  switch (type) {
    case POWER_SIMPLE:
      generatePowerSimple(problem);
      break;
    case POWER_COEFF:
      generatePowerCoeff(problem);
      break;
    case CONSTANT_TYPE:
      generateConstant(problem);
      break;
    case SIN_TYPE:
      generateSin(problem);
      break;
    case COS_TYPE:
      generateCos(problem);
      break;
    case EXP_TYPE:
      generateExp(problem);
      break;
    case LOG_TYPE:
      generateLog(problem);
      break;
  }

  buildOptions(problem);
}

void buildOptions(IntegralProblem &problem) {
  if (random(0, 2) == 0) {
    strncpy(problem.optionA, problem.correctAnswer, sizeof(problem.optionA));
    strncpy(problem.optionB, problem.wrongAnswer, sizeof(problem.optionB));
    problem.correctOption = 'A';
  } else {
    strncpy(problem.optionA, problem.wrongAnswer, sizeof(problem.optionA));
    strncpy(problem.optionB, problem.correctAnswer, sizeof(problem.optionB));
    problem.correctOption = 'B';
  }

  problem.optionA[sizeof(problem.optionA) - 1] = '\0';
  problem.optionB[sizeof(problem.optionB) - 1] = '\0';
}

static void generatePowerSimple(IntegralProblem &p) {
  int n = random(1, 6);
  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S x^%d dx", n);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^%d/%d + C", newExp, newExp);
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^%d + C", newExp);
}

static void generatePowerCoeff(IntegralProblem &p) {
  int a = random(2, 10);
  int n = random(1, 5);
  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S %dx^%d dx", a, n);

  if (a % newExp == 0) {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d + C", a / newExp, newExp);
  } else {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d/%d + C", a, newExp, newExp);
  }

  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d + C", a, newExp);
}

static void generateConstant(IntegralProblem &p) {
  int k = random(2, 10);

  snprintf(p.integral, sizeof(p.integral), "S %d dx", k);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx + C", k);
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d + C", k);
}

static void generateSin(IntegralProblem &p) {
  snprintf(p.integral, sizeof(p.integral), "S sin(x) dx");
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-cos(x) + C");
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "cos(x) + C");
}

static void generateCos(IntegralProblem &p) {
  snprintf(p.integral, sizeof(p.integral), "S cos(x) dx");
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "sin(x) + C");
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-sin(x) + C");
}

static void generateExp(IntegralProblem &p) {
  snprintf(p.integral, sizeof(p.integral), "S e^x dx");
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^x + C");
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x*e^(x-1)+C");
}

static void generateLog(IntegralProblem &p) {
  snprintf(p.integral, sizeof(p.integral), "S 1/x dx");
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "ln|x| + C");
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x^2 + C");
}
