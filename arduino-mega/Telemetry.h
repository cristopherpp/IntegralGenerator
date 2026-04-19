#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "IntegralEngine.h"
#include "StudentModel.h"

void initTelemetry(unsigned long baudRate = 115200);

void renderDashboard(
  const IntegralProblem &problem,
  char selectedOption,
  bool correct,
  bool hasAnswer
);

#endif
