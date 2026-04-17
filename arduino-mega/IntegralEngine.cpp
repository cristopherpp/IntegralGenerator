#include "IntegralEngine.h"

static void generatePowerSimple(IntegralProblem &p, DifficultyLevel d);
static void generatePowerCoeff(IntegralProblem &p, DifficultyLevel d);
static void generateConstant(IntegralProblem &p, DifficultyLevel d);
static void generateSin(IntegralProblem &p, DifficultyLevel d);
static void generateCos(IntegralProblem &p, DifficultyLevel d);
static void generateExp(IntegralProblem &p, DifficultyLevel d);
static void generateLog(IntegralProblem &p, DifficultyLevel d);
static void generateNegPower(IntegralProblem &p, DifficultyLevel d);
static void generateRoot(IntegralProblem &p, DifficultyLevel d);
static void generatePolySum(IntegralProblem &p, DifficultyLevel d);
static void generateMixedSum(IntegralProblem &p, DifficultyLevel d);

static int gcdInt(int a, int b) {
  if (a < 0) a = -a;
  if (b < 0) b = -b;
  while (b != 0) {
    int t = a % b;
    a = b;
    b = t;
  }
  return a == 0 ? 1 : a;
}

static void formatScaledPowerIntegral(
  char *dest,
  size_t destSize,
  int coefficient,
  int exponent
) {
  int numerator = coefficient;
  int denominator = exponent + 1;
  int divisor = gcdInt(numerator, denominator);
  numerator /= divisor;
  denominator /= divisor;
  int newExponent = exponent + 1;

  if (denominator == 1) {
    if (numerator == 1) snprintf(dest, destSize, "x^%d+C", newExponent);
    else if (numerator == -1) snprintf(dest, destSize, "-x^%d+C", newExponent);
    else snprintf(dest, destSize, "%dx^%d+C", numerator, newExponent);
  } else if (numerator == 1) {
    snprintf(dest, destSize, "x^%d/%d+C", newExponent, denominator);
  } else if (numerator == -1) {
    snprintf(dest, destSize, "-x^%d/%d+C", newExponent, denominator);
  } else {
    snprintf(dest, destSize, "%dx^%d/%d+C", numerator, newExponent, denominator);
  }
}

static void copyWrongAnswer(IntegralProblem &p, const char *candidate) {
  strncpy(p.wrongAnswer, candidate, sizeof(p.wrongAnswer));
  p.wrongAnswer[sizeof(p.wrongAnswer) - 1] = '\0';

  if (strcmp(p.correctAnswer, p.wrongAnswer) == 0) {
    strncpy(p.wrongAnswer, "0+C", sizeof(p.wrongAnswer));
    p.wrongAnswer[sizeof(p.wrongAnswer) - 1] = '\0';
  }
}

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
    case NEG_POWER_TYPE: generateNegPower(problem, difficulty); break;
    case ROOT_TYPE:      generateRoot(problem, difficulty); break;
    case POLY_SUM_TYPE:  generatePolySum(problem, difficulty); break;
    case MIXED_SUM_TYPE: generateMixedSum(problem, difficulty); break;
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
    case NEG_POWER_TYPE: return "Pot. negativas";
    case ROOT_TYPE: return "Raices";
    case POLY_SUM_TYPE: return "Sumas pol";
    case MIXED_SUM_TYPE: return "Sumas mixtas";
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
  int n = 1;

  if (d == DIFF_EASY) n = random(1, 4);
  else if (d == DIFF_MEDIUM) n = random(4, 8);
  else n = random(8, 13);

  snprintf(p.integral, sizeof(p.integral), "S x^%d dx", n);
  formatScaledPowerIntegral(p.correctAnswer, sizeof(p.correctAnswer), 1, n);

  char wrong[48];
  switch (random(0, 4)) {
    case 0: snprintf(wrong, sizeof(wrong), "x^%d+C", n + 1); break;
    case 1: snprintf(wrong, sizeof(wrong), "x^%d/%d+C", n, n); break;
    case 2: snprintf(wrong, sizeof(wrong), "%dx^%d+C", n, n - 1); break;
    default: snprintf(wrong, sizeof(wrong), "x^%d/%d+C", n + 1, n); break;
  }
  copyWrongAnswer(p, wrong);
}

static void generatePowerCoeff(IntegralProblem &p, DifficultyLevel d) {
  int a = 2;
  int n = 1;

  if (d == DIFF_EASY) {
    a = random(2, 6);
    n = random(1, 4);
  } else if (d == DIFF_MEDIUM) {
    a = random(3, 10);
    n = random(3, 7);
  } else {
    a = random(6, 15);
    n = random(6, 11);
  }

  snprintf(p.integral, sizeof(p.integral), "S %dx^%d dx", a, n);
  formatScaledPowerIntegral(p.correctAnswer, sizeof(p.correctAnswer), a, n);

  char wrong[48];
  switch (random(0, 4)) {
    case 0: snprintf(wrong, sizeof(wrong), "%dx^%d+C", a, n + 1); break;
    case 1: snprintf(wrong, sizeof(wrong), "%dx^%d/%d+C", a, n, n); break;
    case 2: snprintf(wrong, sizeof(wrong), "%dx^%d+C", a * n, n - 1); break;
    default: snprintf(wrong, sizeof(wrong), "%dx^%d/%d+C", a, n + 1, n); break;
  }
  copyWrongAnswer(p, wrong);
}

static void generateConstant(IntegralProblem &p, DifficultyLevel d) {
  int k = 1;

  if (d == DIFF_EASY) k = random(2, 7);
  else if (d == DIFF_MEDIUM) k = random(-12, 13);
  else {
    do {
      k = random(-30, 31);
    } while (k == 0);
  }

  snprintf(p.integral, sizeof(p.integral), "S %d dx", k);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx+C", k);

  char wrong[48];
  if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%d+C", k);
  else snprintf(wrong, sizeof(wrong), "%dx^2+C", k);
  copyWrongAnswer(p, wrong);
}

static void generateSin(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S sin(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-cos(x)+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "cos(x)+C");
    else snprintf(wrong, sizeof(wrong), "sin(x)+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 6);
    int b = random(1, 7);
    snprintf(p.integral, sizeof(p.integral), "S sin(%dx+%d)dx", a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-cos(%dx+%d)/%d+C", a, b, a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "cos(%dx+%d)/%d+C", a, b, a);
    else snprintf(wrong, sizeof(wrong), "-cos(%dx+%d)+C", a, b);
  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int coeff = a * (n + 1);
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dsin(x^%d)dx", coeff, n, n + 1);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-%dcos(x^%d)+C", a, n + 1);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dcos(x^%d)+C", a, n + 1);
    else snprintf(wrong, sizeof(wrong), "-%dx^%dcos(x^%d)", a, n, n + 1);
  }

  copyWrongAnswer(p, wrong);
}

static void generateCos(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S cos(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "sin(x)+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "-sin(x)+C");
    else snprintf(wrong, sizeof(wrong), "cos(x)+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 6);
    int b = random(1, 7);
    snprintf(p.integral, sizeof(p.integral), "S cos(%dx+%d)dx", a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "sin(%dx+%d)/%d+C", a, b, a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "-sin(%dx+%d)/%d+C", a, b, a);
    else snprintf(wrong, sizeof(wrong), "sin(%dx+%d)+C", a, b);
  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int coeff = a * (n + 1);
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dcos(x^%d)dx", coeff, n, n + 1);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsin(x^%d)+C", a, n + 1);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "-%dsin(x^%d)+C", a, n + 1);
    else snprintf(wrong, sizeof(wrong), "%dx^%dsin(x^%d)", a, n, n + 1);
  }

  copyWrongAnswer(p, wrong);
}

static void generateExp(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S e^x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^x+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "xe^x");
    else snprintf(wrong, sizeof(wrong), "e^(x-1)");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 6);
    int b = random(1, 6);
    snprintf(p.integral, sizeof(p.integral), "S e^(%dx+%d)dx", a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^(%dx+%d)/%d+C", a, b, a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "e^(%dx+%d)+C", a, b);
    else snprintf(wrong, sizeof(wrong), "xe^(%dx+%d)", a, b);
  } else {
    int a = random(2, 5);
    int n = random(2, 4);
    int coeff = a * (n + 1);
    snprintf(p.integral, sizeof(p.integral), "S %dx^%de^(x^%d)dx", coeff, n, n + 1);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%de^(x^%d)+C", a, n + 1);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "e^(x^%d)+C", n + 1);
    else snprintf(wrong, sizeof(wrong), "%dx^%de^(x^%d)", a, n, n + 1);
  }

  copyWrongAnswer(p, wrong);
}

static void generateLog(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S 1/x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "ln|x|+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "1/x+C");
    else snprintf(wrong, sizeof(wrong), "1/x^2+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 7);
    snprintf(p.integral, sizeof(p.integral), "S %d/x dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dln|x|+C", a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "ln|x|+C");
    else snprintf(wrong, sizeof(wrong), "%d/x^2+C", a);
  } else {
    int a = random(2, 6);
    int b = random(1, 9);
    int k = random(2, 7);
    snprintf(p.integral, sizeof(p.integral), "S %d/(%dx+%d)dx", k * a, a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dln|%dx+%d|+C", k, a, b);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dln|%dx+%d|/%d+C", k * a, a, b, a);
    else snprintf(wrong, sizeof(wrong), "%d/(%dx+%d)+C", k * a, a, b);
  }

  copyWrongAnswer(p, wrong);
}

static void generateNegPower(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S 1/x^2 dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-1/x+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "1/x+C");
    else snprintf(wrong, sizeof(wrong), "1/x^3+C");
  } else if (d == DIFF_MEDIUM) {
    int n = random(3, 6);
    snprintf(p.integral, sizeof(p.integral), "S 1/x^%d dx", n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-1/(%dx^%d)+C", n - 1, n - 1);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "1/(%dx^%d)+C", n - 1, n - 1);
    else snprintf(wrong, sizeof(wrong), "1/x^%d+C", n - 1);
  } else {
    int a = random(2, 7);
    int n = random(3, 6);
    snprintf(p.integral, sizeof(p.integral), "S %d/x^%d dx", a * (n - 1), n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-%d/x^%d+C", a, n - 1);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%d/x^%d+C", a, n - 1);
    else snprintf(wrong, sizeof(wrong), "-%d/x^%d+C", a, n);
  }

  copyWrongAnswer(p, wrong);
}

static void generateRoot(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S sqrt(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "2x^(3/2)/3+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "x^(3/2)+C");
    else snprintf(wrong, sizeof(wrong), "1/sqrt(x)+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 7);
    snprintf(p.integral, sizeof(p.integral), "S %d/sqrt(x) dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsqrt(x)+C", 2 * a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dsqrt(x)+C", a);
    else snprintf(wrong, sizeof(wrong), "%d/(2sqrt(x))+C", a);
  } else {
    int a = random(2, 5);
    snprintf(p.integral, sizeof(p.integral), "S %dxsqrt(x^2+1)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%d(x^2+1)^(3/2)/3+C", a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%d(x^2+1)^(1/2)+C", a);
    else snprintf(wrong, sizeof(wrong), "%d(x^2+1)^(3/2)+C", a);
  }

  copyWrongAnswer(p, wrong);
}

static void generatePolySum(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S (x^2+x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^3/3+x^2/2+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "x^3+x^2+C");
    else snprintf(wrong, sizeof(wrong), "2x+1+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 6);
    int b = random(2, 7);
    int c = random(1, 6);
    snprintf(p.integral, sizeof(p.integral), "S (%dx^3+%dx+%d)dx", a, b, c);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^4/4+%dx^2/2+%dx+C", a, b, c);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dx^4+%dx^2+%dx+C", a, b, c);
    else snprintf(wrong, sizeof(wrong), "%dx^3+%dx+%d+C", 3 * a, b, c);
  } else {
    int a = random(4, 9);
    int b = random(3, 8);
    int c = random(2, 7);
    snprintf(p.integral, sizeof(p.integral), "S (%dx^5+%dx^2+%d/x)dx", a, b, c);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^6/6+%dx^3/3+%dln|x|+C", a, b, c);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dx^6+%dx^3+%dln|x|+C", a, b, c);
    else snprintf(wrong, sizeof(wrong), "%dx^6/5+%dx^3/2+%dln|x|+C", a, b, c);
  }

  copyWrongAnswer(p, wrong);
}

static void generateMixedSum(IntegralProblem &p, DifficultyLevel d) {
  char wrong[48];

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S (x+sin(x)) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^2/2-cos(x)+C");
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "x^2-cos(x)+C");
    else snprintf(wrong, sizeof(wrong), "1-cos(x)+C");
  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 5);
    snprintf(p.integral, sizeof(p.integral), "S (x^2+e^(%dx))dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^3/3+e^(%dx)/%d+C", a, a);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "x^3+e^(%dx)+C", a);
    else snprintf(wrong, sizeof(wrong), "2x+e^(%dx)/%d+C", a, a);
  } else {
    int a = random(2, 5);
    int b = random(2, 6);
    snprintf(p.integral, sizeof(p.integral), "S (%dx^2+1/(%dx)+cos(x))dx", a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^3/3+ln|%dx|/%d+sin(x)+C", a, b, b);
    if (random(0, 2) == 0) snprintf(wrong, sizeof(wrong), "%dx^3+ln|%dx|/%d+sin(x)+C", a, b, b);
    else snprintf(wrong, sizeof(wrong), "%dx^3/3+1/(%dx)+sin(x)+C", a, b);
  }

  copyWrongAnswer(p, wrong);
}
