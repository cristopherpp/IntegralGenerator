#include "IntegralEngine.h"

static void generatePowerSimple(IntegralProblem &p, DifficultyLevel d);
static void generatePowerCoeff(IntegralProblem &p, DifficultyLevel d);
static void generateConstant(IntegralProblem &p, DifficultyLevel d);
static void generateSin(IntegralProblem &p, DifficultyLevel d);
static void generateCos(IntegralProblem &p, DifficultyLevel d);
static void generateExp(IntegralProblem &p, DifficultyLevel d);
static void generateLog(IntegralProblem &p, DifficultyLevel d);

void generateIntegralProblem(IntegralProblem &problem) {
  IntegralType type = (IntegralType)random(0, INTEGRAL_TYPE_COUNT);
  DifficultyLevel difficulty = (DifficultyLevel)random(0, 3);
  generateIntegralProblemByTypeAndDifficulty(problem, type, difficulty);
}

void generateIntegralProblemByType(IntegralProblem &problem, IntegralType type) {
  generateIntegralProblemByTypeAndDifficulty(problem, type, DIFF_MEDIUM);
}

void generateIntegralProblemByTypeAndDifficulty(
  IntegralProblem &problem,
  IntegralType type,
  DifficultyLevel difficulty
) {
  problem.type = type;
  problem.difficulty = difficulty;

  switch (type) {
    case POWER_SIMPLE:   generatePowerSimple(problem, difficulty); break;
    case POWER_COEFF:    generatePowerCoeff(problem, difficulty); break;
    case CONSTANT_TYPE:  generateConstant(problem, difficulty); break;
    case SIN_TYPE:       generateSin(problem, difficulty); break;
    case COS_TYPE:       generateCos(problem, difficulty); break;
    case EXP_TYPE:       generateExp(problem, difficulty); break;
    case LOG_TYPE:       generateLog(problem, difficulty); break;
    default:             generatePowerSimple(problem, difficulty); break;
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

const char* getIntegralTypeName(IntegralType type) {
  switch (type) {
    case POWER_SIMPLE: return "Potencias";
    case POWER_COEFF: return "Pot. coef";
    case CONSTANT_TYPE: return "Constantes";
    case SIN_TYPE: return "Seno";
    case COS_TYPE: return "Coseno";
    case EXP_TYPE: return "Exponencial";
    case LOG_TYPE: return "Logaritmica";
    default: return "Desconocida";
  }
}

const char* getDifficultyName(DifficultyLevel difficulty) {
  switch (difficulty) {
    case DIFF_EASY: return "Easy";
    case DIFF_MEDIUM: return "Medium";
    case DIFF_HARD: return "Hard";
    default: return "?";
  }
}

static void generatePowerSimple(IntegralProblem &p, DifficultyLevel d) {
  int n;

  if (d == DIFF_EASY) n = random(1, 3);
  else if (d == DIFF_MEDIUM) n = random(3, 6);
  else n = random(6, 9);

  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S x^%d dx", n);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^%d/%d + C", newExp, newExp);
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^%d + C", newExp);
}

static void generatePowerCoeff(IntegralProblem &p, DifficultyLevel d) {
  int a, n;

  if (d == DIFF_EASY) {
    a = random(2, 5);
    n = random(1, 3);
  } else if (d == DIFF_MEDIUM) {
    a = random(2, 8);
    n = random(2, 5);
  } else {
    a = random(3, 10);
    n = random(4, 7);
  }

  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S %dx^%d dx", a, n);

  if (a % newExp == 0) {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d + C", a / newExp, newExp);
  } else {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d/%d+C", a, newExp, newExp);
  }

  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d + C", a, newExp);
}

static void generateConstant(IntegralProblem &p, DifficultyLevel d) {
  int k;

  if (d == DIFF_EASY) k = random(2, 6);
  else if (d == DIFF_MEDIUM) k = random(6, 13);
  else k = random(13, 21);

  snprintf(p.integral, sizeof(p.integral), "S %d dx", k);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx + C", k);
  snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d + C", k);
}

static void generateSin(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S sin(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-cos(x) + C");
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "cos(x) + C");
  } else {
    int a = (d == DIFF_MEDIUM) ? random(2, 6) : random(5, 10);
    snprintf(p.integral, sizeof(p.integral), "S %dsin(x)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-%dcos(x)+C", a);
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dcos(x)+C", a);
  }
}

static void generateCos(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S cos(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "sin(x) + C");
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-sin(x) + C");
  } else {
    int a = (d == DIFF_MEDIUM) ? random(2, 6) : random(5, 10);
    snprintf(p.integral, sizeof(p.integral), "S %dcos(x)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsin(x)+C", a);
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-%dsin(x)+C", a);
  }
}

static void generateExp(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S e^x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^x + C");
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x*e^(x-1)");
  } else {
    int a = (d == DIFF_MEDIUM) ? random(2, 6) : random(5, 10);
    snprintf(p.integral, sizeof(p.integral), "S %de^x dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%de^x + C", a);
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "e^x + C");
  }
}

static void generateLog(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S 1/x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "ln|x| + C");
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x^2 + C");
  } else {
    int a = (d == DIFF_MEDIUM) ? random(2, 6) : random(5, 10);
    snprintf(p.integral, sizeof(p.integral), "S %d/x dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dln|x|+C", a);
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d/x^2 + C", a);
  }
}
