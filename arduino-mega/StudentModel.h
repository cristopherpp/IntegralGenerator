#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include <Arduino.h>
#include "IntegralEngine.h"

struct CategoryStats {
  uint8_t shown;
  uint8_t correct;
  uint8_t wrong;

  uint8_t consecutiveCorrect;
  uint8_t consecutiveWrong;

  uint8_t weight;
  uint8_t difficulty;
  uint8_t mastery;
};

void initStudentModel();
void registerResult(IntegralType type, bool wasCorrect);

IntegralType pickNextIntegralType();
DifficultyLevel getDifficultyForType(IntegralType type);

const CategoryStats* getStats();

uint8_t getTotalCorrect();
uint8_t getTotalWrong();
uint8_t getTotalShown();

#endif
