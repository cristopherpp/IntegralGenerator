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
  int n;

  if (d == DIFF_EASY) n = random(1, 3);
  else if (d == DIFF_MEDIUM) n = random(3, 7);
  else n = random(7, 11);

  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S x^%d dx", n);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^%d/%d+C", newExp, newExp);

  int wrongType = random(0, 3);
  if (wrongType == 0) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^%d+C", newExp);
  } else if (wrongType == 1) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d+C", n, n - 1);
  } else {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^%d/%d+C", n, n);
  }
}

static void generatePowerCoeff(IntegralProblem &p, DifficultyLevel d) {
  int a, n;

  if (d == DIFF_EASY) {
    a = random(2, 5);
    n = random(1, 3);
  } else if (d == DIFF_MEDIUM) {
    a = random(3, 8);
    n = random(3, 6);
  } else {
    a = random(4, 10);
    n = random(6, 10);
  }

  int newExp = n + 1;

  snprintf(p.integral, sizeof(p.integral), "S %dx^%d dx", a, n);

  if (a % newExp == 0) {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d+C", a / newExp, newExp);
  } else {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^%d/%d+C", a, newExp, newExp);
  }

  int wrongType = random(0, 3);
  if (wrongType == 0) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d+C", a, newExp);
  } else if (wrongType == 1) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d+C", a * n, n - 1);
  } else {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d/%d+C", a, n, n);
  }
}

static void generateConstant(IntegralProblem &p, DifficultyLevel d) {
  int k;

  if (d == DIFF_EASY) k = random(2, 6);
  else if (d == DIFF_MEDIUM) k = random(6, 13);
  else k = random(13, 31);

  snprintf(p.integral, sizeof(p.integral), "S %d dx", k);
  snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx+C", k);

  if (random(0, 2) == 0) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d+C", k);
  } else {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^2+C", k);
  }
}

static void generateSin(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S sin(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-cos(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "cos(x)+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "sin(x)+C");
    }

  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %dsin(x)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-%dcos(x)+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dcos(x)+C", a);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dsin(x)+C", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int power = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dsin(x^%d)", a * n, power, n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-%dcos(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dcos(x^%d)+C", a, n);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-%dx^%dcos(x^%d)", a, power, n);
    }
  }
}

static void generateCos(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S cos(x) dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "sin(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-sin(x)+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "cos(x)+C");
    }

  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %dcos(x)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsin(x)+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-%dsin(x)+C", a);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dcos(x)+C", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int power = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%dcos(x^%d)", a * n, power, n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsin(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "-%dsin(x^%d)+C", a, n);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%dsin(x^%d)", a, power, n);
    }
  }
}

static void generateExp(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S e^x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^x+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "xe^(x-1)");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "xe^x");
    }

  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %de^x dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%de^x+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "e^x+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dxe^x", a);
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int power = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%de^(x^%d)", a * n, power, n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%de^(x^%d)+C", a, n);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "e^(x^%d)+C", n);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%de^(x^%d)", a, power, n);
    }
  }
}

static void generateLog(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S 1/x dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "ln|x|+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x^2+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x+C");
    }

  } else if (d == DIFF_MEDIUM) {
    int a = random(2, 8);
    snprintf(p.integral, sizeof(p.integral), "S %d/x dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dln|x|+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d/x^2+C", a);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "ln|x|+C");
    }

  } else {
    int a = random(2, 6);
    int n = random(2, 5);
    int power = n - 1;
    snprintf(p.integral, sizeof(p.integral), "S %dx^%d/x^%d", a * n, power, n);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dln|x|+C", a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "ln|x|+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^%d+C", a, power);
    }
  }
}

static void generateNegPower(IntegralProblem &p, DifficultyLevel d) {
  int n;

  if (d == DIFF_EASY) n = 2;
  else if (d == DIFF_MEDIUM) n = random(2, 4);
  else n = random(4, 7);

  snprintf(p.integral, sizeof(p.integral), "S 1/x^%d dx", n);

  if (n == 2) {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-1/x+C");
  } else {
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "-1/(%dx^%d)+C", n - 1, n - 1);
  }

  if (random(0, 2) == 0) {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x^%d+C", n - 1);
  } else {
    snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/x^%d+C", n + 1);
  }
}

static void generateRoot(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S sqrt(x)dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "2x^(3/2)/3+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^(3/2)+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/sqrt(x)+C");
    }

  } else if (d == DIFF_MEDIUM) {
    snprintf(p.integral, sizeof(p.integral), "S 1/sqrt(x)dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "2sqrt(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "sqrt(x)+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1/(2sqrt(x))+C");
    }

  } else {
    int a = random(2, 6);
    snprintf(p.integral, sizeof(p.integral), "S %d/sqrt(x)dx", a);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dsqrt(x)+C", 2 * a);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dsqrt(x)+C", a);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%d/(2sqrt(x))+C", a);
    }
  }
}

static void generatePolySum(IntegralProblem &p, DifficultyLevel d) {
  int a = random(1, 5);
  int b = random(1, 5);

  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S (x^2+x)dx");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^3/3+x^2/2+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^3+x^2+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "2x+1+C");
    }

  } else if (d == DIFF_MEDIUM) {
    snprintf(p.integral, sizeof(p.integral), "S (%dx^2+%dx)dx", a, b);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^3/3+%dx^2/2", a, b);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^3+%dx^2", a, b);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx+%d", 2 * a, b);
    }

  } else {
    int c = random(1, 5);
    snprintf(p.integral, sizeof(p.integral), "S (%dx^3+%dx+%d)", a, b, c);
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "%dx^4/4+%dx^2/2+%dx", a, b, c);

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^4+%dx^2+%dx", a, b, c);
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "%dx^2+%d", 3 * a, b);
    }
  }
}

static void generateMixedSum(IntegralProblem &p, DifficultyLevel d) {
  if (d == DIFF_EASY) {
    snprintf(p.integral, sizeof(p.integral), "S (x+sin(x))");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^2/2-cos(x)+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^2-cos(x)+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "1-cos(x)+C");
    }

  } else if (d == DIFF_MEDIUM) {
    snprintf(p.integral, sizeof(p.integral), "S (e^x+1/x)");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "e^x+ln|x|+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "e^x+1/x+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "xe^(x-1)+ln|x|");
    }

  } else {
    snprintf(p.integral, sizeof(p.integral), "S (x^2+e^x)");
    snprintf(p.correctAnswer, sizeof(p.correctAnswer), "x^3/3+e^x+C");

    if (random(0, 2) == 0) {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "x^3+e^x+C");
    } else {
      snprintf(p.wrongAnswer, sizeof(p.wrongAnswer), "2x+e^x+C");
    }
  }
}
