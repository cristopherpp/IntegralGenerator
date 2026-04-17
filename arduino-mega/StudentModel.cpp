#include "StudentModel.h"

static CategoryStats stats[INTEGRAL_TYPE_COUNT];

static const uint8_t BASE_WEIGHT = 10;
static const uint8_t MIN_WEIGHT = 5;
static const uint8_t MAX_WEIGHT = 34;
static IntegralType lastType = INTEGRAL_TYPE_COUNT;
static uint8_t repeatCount = 0;

static uint8_t clampU8(int value, uint8_t minVal, uint8_t maxVal) {
  if (value < minVal) return minVal;
  if (value > maxVal) return maxVal;
  return (uint8_t)value;
}

static uint8_t computeGlobalAccuracy() {
  uint16_t correct = 0;
  uint16_t shown = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    correct += stats[i].correct;
    shown += stats[i].shown;
  }

  if (shown == 0) return 0;
  return (uint8_t)((correct * 100U) / shown);
}

void initStudentModel() {
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    stats[i].shown = 0;
    stats[i].correct = 0;
    stats[i].wrong = 0;
    stats[i].consecutiveCorrect = 0;
    stats[i].consecutiveWrong = 0;
    stats[i].weight = BASE_WEIGHT;
    stats[i].difficulty = 1;
    stats[i].mastery = 40;
  }

  lastType = INTEGRAL_TYPE_COUNT;
  repeatCount = 0;
}

void registerResult(IntegralType type, bool wasCorrect) {
  CategoryStats &s = stats[type];
  s.shown++;

  if (wasCorrect) {
    s.correct++;
    s.consecutiveCorrect = clampU8((int)s.consecutiveCorrect + 1, 0, 12);
    s.consecutiveWrong = 0;
    s.mastery = clampU8((int)s.mastery + 8, 0, 100);

    if (s.weight > BASE_WEIGHT) s.weight = clampU8((int)s.weight - 3, MIN_WEIGHT, MAX_WEIGHT);
    else s.weight = clampU8((int)s.weight - 1, MIN_WEIGHT, MAX_WEIGHT);

    if ((s.consecutiveCorrect >= 2 && s.mastery >= 55) || (s.consecutiveCorrect >= 3)) {
      s.difficulty = clampU8((int)s.difficulty + 1, 0, 2);
      s.consecutiveCorrect = 0;
    }
  } else {
    s.wrong++;
    s.consecutiveWrong = clampU8((int)s.consecutiveWrong + 1, 0, 12);
    s.consecutiveCorrect = 0;
    s.mastery = clampU8((int)s.mastery - 12, 0, 100);
    s.weight = clampU8((int)s.weight + 5 + s.consecutiveWrong, MIN_WEIGHT, MAX_WEIGHT);

    if (s.consecutiveWrong >= 2 || s.mastery <= 20) {
      s.difficulty = clampU8((int)s.difficulty - 1, 0, 2);
      s.consecutiveWrong = 0;
    }
  }
}

IntegralType pickNextIntegralType() {
  int totalScore = 0;
  int scores[INTEGRAL_TYPE_COUNT];

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    const CategoryStats &s = stats[i];
    int score = s.weight;

    if (s.shown == 0) score += 12;
    else if (s.shown < 3) score += 5;

    if (s.wrong > s.correct) score += 6;
    if (s.consecutiveWrong > 0) score += s.consecutiveWrong * 4;
    if (s.mastery < 50) score += (50 - s.mastery) / 5;

    if ((IntegralType)i == lastType) {
      score -= 8 + repeatCount * 3;
    }

    if (score < 1) score = 1;
    scores[i] = score;
    totalScore += score;
  }

  if (totalScore <= 0) return POWER_SIMPLE;

  int r = random(0, totalScore);
  int accum = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    accum += scores[i];
    if (r < accum) {
      IntegralType selected = (IntegralType)i;
      if (selected == lastType) repeatCount = clampU8((int)repeatCount + 1, 0, 6);
      else {
        lastType = selected;
        repeatCount = 0;
      }
      return selected;
    }
  }

  return POWER_SIMPLE;
}

 DifficultyLevel getDifficultyForType(IntegralType type) {
  uint8_t base = stats[type].difficulty;
  uint8_t accuracy = computeGlobalAccuracy();
  uint8_t shown = getTotalShown();

  if (shown >= 8 && accuracy >= 70 && base < 2) base++;
  if (shown >= 18 && accuracy >= 82) base = 2;
  if (accuracy <= 45 && shown >= 6 && base > 0) base--;

  if (base == 0) return DIFF_EASY;
  if (base == 1) return DIFF_MEDIUM;
  return DIFF_HARD;
}

const CategoryStats* getStats() {
  return stats;
}

uint8_t getTotalCorrect() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].correct;
  return (uint8_t)total;
}

uint8_t getTotalWrong() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].wrong;
  return (uint8_t)total;
}

uint8_t getTotalShown() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].shown;
  return (uint8_t)total;
}
