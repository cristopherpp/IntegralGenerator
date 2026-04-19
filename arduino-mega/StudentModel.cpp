#include "StudentModel.h"

static CategoryStats stats[INTEGRAL_TYPE_COUNT];

void initStudentModel() {
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    stats[i].shown = 0;
    stats[i].correct = 0;
    stats[i].wrong = 0;

    stats[i].consecutiveCorrect = 0;
    stats[i].consecutiveWrong = 0;

    stats[i].difficulty = 1;
    stats[i].mastery = 40;

    stats[i].avgResponseTime = 0;
    stats[i].accuracy = 0;
    stats[i].trend = 0;
  }
}

void registerResult(IntegralType type, bool wasCorrect) {
  CategoryStats &s = stats[type];
  s.shown++;

  if (wasCorrect) {
    s.correct++;
    s.consecutiveCorrect++;
    s.consecutiveWrong = 0;
    s.mastery = min(100, s.mastery + 5);
  } else {
    s.wrong++;
    s.consecutiveWrong++;
    s.consecutiveCorrect = 0;
    s.mastery = max(0, s.mastery - 8);
  }

  // accuracy update
  s.accuracy = (float)s.correct / s.shown;
}

void updatePerformance(CategoryStats &s, bool correct, unsigned long rt) {
  // moving average time
  if (s.avgResponseTime == 0) {
    s.avgResponseTime = rt;
  } else {
    s.avgResponseTime = 0.8f * s.avgResponseTime + 0.2f * rt;
  }

  // trend
  if (correct && rt < s.avgResponseTime) s.trend = 1;
  else if (!correct) s.trend = -1;
  else s.trend = 0;
}

static IntegralType lastType = INTEGRAL_TYPE_COUNT;
static uint8_t repeatCount = 0;

IntegralType pickNextIntegralTypeSmart() {
  const CategoryStats* stats = getStats();

  int scores[INTEGRAL_TYPE_COUNT];
  int total = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    const CategoryStats &s = stats[i];

    int score = 10; // base → ensures ALL types appear

    // --- exploration ---
    if (s.shown == 0) score += 20;
    else if (s.shown < 3) score += 10;

    // --- weakness targeting ---
    if (s.correct < s.wrong) score += 10;
    score += (100 - s.mastery) / 5;

    // --- anti repetition ---
    if ((IntegralType)i == lastType) {
      score -= (10 + repeatCount * 5);
    }

    if (score < 1) score = 1;

    scores[i] = score;
    total += score;
  }

  int r = random(total);
  int acc = 0;

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    acc += scores[i];
    if (r < acc) {
      IntegralType selected = (IntegralType)i;

      if (selected == lastType) {
        repeatCount++;
      } else {
        lastType = selected;
        repeatCount = 0;
      }

      return selected;
    }
  }

  return POWER_SIMPLE; // fallback
}

DifficultyLevel getDifficultyForType(IntegralType type) {
  const CategoryStats &s = stats[type];

  if (s.accuracy < 0.4) return DIFF_EASY;
  if (s.accuracy < 0.75) return DIFF_MEDIUM;
  return DIFF_HARD;
}

const CategoryStats* getStats() {
  return stats;
}

uint8_t getTotalCorrect() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].correct;
  return total;
}

uint8_t getTotalWrong() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].wrong;
  return total;
}

uint8_t getTotalShown() {
  uint16_t total = 0;
  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) total += stats[i].shown;
  return total;
}