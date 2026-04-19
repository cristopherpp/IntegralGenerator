#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include <Arduino.h>
#include "IntegralEngine.h"

struct CategoryStats {
  uint16_t shown;
  uint16_t correct;
  uint16_t wrong;

  uint8_t consecutiveCorrect;
  uint8_t consecutiveWrong;

  uint8_t difficulty;
  uint8_t mastery;

  float avgResponseTime;
  float accuracy;
  int8_t trend; // -1 worse, 0 stable, +1 improving
};

void initStudentModel();
void registerResult(IntegralType type, bool wasCorrect);

void updatePerformance(CategoryStats &s, bool correct, unsigned long responseTime);
IntegralType pickNextIntegralTypeSmart();

DifficultyLevel getDifficultyForType(IntegralType type);

const CategoryStats* getStats();

uint8_t getTotalCorrect();
uint8_t getTotalWrong();
uint8_t getTotalShown();

#endif
