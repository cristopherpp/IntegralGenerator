#line 1 "/home/cristopher/Desktop/projects/IntegralGenerator/StudentModel.cpp"
#include "StudentModel.h"

static CategoryStats stats[INTEGRAL_TYPE_COUNT];

void initStudentModel() {
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    stats[i].shown = 0;
    stats[i].correct = 0;
    stats[i].wrong = 0;
    stats[i].totalResponseTimeMs = 0;
    stats[i].weight = 10; // peso inicial equilibrado
  }
}

void registerResult(IntegralType type, bool wasCorrect, uint16_t responseTimeMs) {
  stats[type].shown++;
  stats[type].totalResponseTimeMs += responseTimeMs;

  if (wasCorrect) {
    stats[type].correct++;

    // si acertó, baja ligeramente el peso
    if (stats[type].weight > 2) {
      stats[type].weight -= 1;
    }

    // si acertó pero tardó mucho, reforzar un poco
    if (responseTimeMs > 4000 && stats[type].weight < 50) {
      stats[type].weight += 1;
    }
  } else {
    stats[type].wrong++;

    // si falló, sube más el peso
    if (stats[type].weight < 47) {
      stats[type].weight += 3;
    } else {
      stats[type].weight = 50;
    }
  }
}

IntegralType pickNextIntegralType() {
  int totalWeight = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    totalWeight += stats[i].weight;
  }

  if (totalWeight <= 0) {
    return POWER_SIMPLE;
  }

  int r = random(0, totalWeight);
  int accum = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    accum += stats[i].weight;
    if (r < accum) {
      return (IntegralType)i;
    }
  }

  return POWER_SIMPLE;
}

const CategoryStats* getStats() {
  return stats;
}

uint8_t getWeakestCategory() {
  uint8_t weakest = 0;
  int worstScore = -1;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    int score = stats[i].wrong * 3 - stats[i].correct;
    if (score > worstScore) {
      worstScore = score;
      weakest = i;
    }
  }

  return weakest;
}

uint8_t getStrongestCategory() {
  uint8_t strongest = 0;
  int bestScore = -32000;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    int score = stats[i].correct * 3 - stats[i].wrong;
    if (score > bestScore) {
      bestScore = score;
      strongest = i;
    }
  }

  return strongest;
}

uint16_t getTotalCorrect() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    total += stats[i].correct;
  }
  return total;
}

uint16_t getTotalWrong() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    total += stats[i].wrong;
  }
  return total;
}

uint16_t getTotalShown() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    total += stats[i].shown;
  }
  return total;
}
