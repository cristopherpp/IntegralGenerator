#include "StudentModel.h"

static CategoryStats stats[INTEGRAL_TYPE_COUNT];

static const uint8_t BASE_WEIGHT = 10;
static const uint8_t MIN_WEIGHT = 6;
static const uint8_t MAX_WEIGHT = 30;

static uint8_t clampU8(int value, uint8_t minVal, uint8_t maxVal) {
  if (value < minVal) return minVal;
  if (value > maxVal) return maxVal;
  return (uint8_t)value;
}

void initStudentModel() {
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    stats[i].shown = 0;
    stats[i].correct = 0;
    stats[i].wrong = 0;
    stats[i].consecutiveCorrect = 0;
    stats[i].consecutiveWrong = 0;
    stats[i].weight = BASE_WEIGHT;
    stats[i].difficulty = 1; // starting at medium
  }
}

void registerResult(IntegralType type, bool wasCorrect) {
  CategoryStats &s = stats[type];
  s.shown++;

  if (wasCorrect) {
    s.correct++;
    s.consecutiveCorrect++;
    s.consecutiveWrong = 0;

    if (s.weight > BASE_WEIGHT) {
      s.weight = clampU8((int)s.weight - 2, MIN_WEIGHT, MAX_WEIGHT);
    } else if (s.weight < BASE_WEIGHT) {
      s.weight = clampU8((int)s.weight + 1, MIN_WEIGHT, MAX_WEIGHT);
    }

    if (s.consecutiveCorrect >= 2 && s.difficulty < 2) {
      s.difficulty++;
      s.consecutiveCorrect = 0;
    }
  } else {
    s.wrong++;
    s.consecutiveWrong++;
    s.consecutiveCorrect = 0;

    s.weight = clampU8((int)s.weight + 4, MIN_WEIGHT, MAX_WEIGHT);

    if (s.consecutiveWrong >= 2 && s.difficulty > 0) {
      s.difficulty--;
      s.consecutiveWrong = 0;
    }
  }
}

IntegralType pickNextIntegralType() {
  int totalWeight = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    totalWeight += stats[i].weight;
  }

  if (totalWeight <= 0) return POWER_SIMPLE;

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

DifficultyLevel getDifficultyForType(IntegralType type) {
  uint8_t d = stats[type].difficulty;
  if (d == 0) return DIFF_EASY;
  if (d == 1) return DIFF_MEDIUM;
  return DIFF_HARD;
}

const CategoryStats* getStats() {
  return stats;
}

uint8_t getTotalCorrect() {
  uint8_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].correct;
  return total;
}

uint8_t getTotalWrong() {
  uint8_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].wrong;
  return total;
}

uint8_t getTotalShown() {
  uint8_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].shown;
  return total;
}
