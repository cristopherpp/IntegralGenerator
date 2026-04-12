#line 1 "/home/cristopher/Desktop/projects/IntegralGenerator/StudentModel.h"
#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include <Arduino.h>
#include "IntegralEngine.h"

struct CategoryStats {
  uint16_t shown;
  uint16_t correct;
  uint16_t wrong;
  uint32_t totalResponseTimeMs;
  uint8_t weight;
};

void initStudentModel();
void registerResult(IntegralType type, bool wasCorrect, uint16_t responseTimeMs);
IntegralType pickNextIntegralType();

const CategoryStats* getStats();
uint8_t getWeakestCategory();
uint8_t getStrongestCategory();
uint16_t getTotalCorrect();
uint16_t getTotalWrong();
uint16_t getTotalShown();

#endif
