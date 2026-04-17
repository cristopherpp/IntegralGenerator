#include "Telemetry.h"

static const char* outcomeText(bool correct) {
  return correct ? "Correct" : "Incorrect";
}

static const char* optionLabel(char option) {
  switch (option) {
    case 'A': return "A";
    case 'B': return "B";
    default: return "?";
  }
}

void initTelemetry(unsigned long baudRate) {
  Serial.begin(baudRate);
}

void sendQuestionResultReport(
  const IntegralProblem &problem,
  char selectedOption,
  bool correct
) {
  const CategoryStats* stats = getStats();
  const CategoryStats &s = stats[problem.type];

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("[QUESTION RESULT]"));

  Serial.print(F("Category: "));
  Serial.println(getIntegralTypeName(problem.type));

  Serial.print(F("Difficulty: "));
  Serial.println(getDifficultyName(problem.difficulty));

  Serial.print(F("Integral shown: "));
  Serial.println(problem.integral);

  Serial.print(F("Option A: "));
  Serial.println(problem.optionA);

  Serial.print(F("Option B: "));
  Serial.println(problem.optionB);

  Serial.print(F("Selected option: "));
  Serial.println(optionLabel(selectedOption));

  Serial.print(F("Correct option: "));
  Serial.println(optionLabel(problem.correctOption));

  Serial.print(F("Outcome: "));
  Serial.println(outcomeText(correct));

  Serial.println(F("Progress in this category:"));
  Serial.print(F("  Asked: "));
  Serial.println(s.shown);

  Serial.print(F("  Correct: "));
  Serial.println(s.correct);

  Serial.print(F("  Wrong: "));
  Serial.println(s.wrong);

  Serial.print(F("  Practice priority: "));
  Serial.println(s.weight);

  Serial.print(F("  Current challenge level: "));
  if (s.difficulty == 0) Serial.println(F("Easy"));
  else if (s.difficulty == 1) Serial.println(F("Medium"));
  else Serial.println(F("Hard"));

  Serial.println(F("========================================"));
}

void sendSessionSummaryReport() {
  const CategoryStats* stats = getStats();

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("[SESSION SUMMARY]"));

  Serial.print(F("Total answered: "));
  Serial.println(getTotalShown());

  Serial.print(F("Total correct: "));
  Serial.println(getTotalCorrect());

  Serial.print(F("Total wrong: "));
  Serial.println(getTotalWrong());

  Serial.println(F(""));
  Serial.println(F("Category breakdown:"));

  for (int i = 0; i < INTEGRAL_TYPE_COUNT; i++) {
    Serial.print(F("- "));
    Serial.print(getIntegralTypeName((IntegralType)i));
    Serial.print(F(": "));

    Serial.print(stats[i].correct);
    Serial.print(F(" correct / "));
    Serial.print(stats[i].wrong);
    Serial.print(F(" wrong"));

    Serial.print(F(" | priority="));
    Serial.print(stats[i].weight);

    Serial.print(F(" | level="));
    if (stats[i].difficulty == 0) Serial.println(F("Easy"));
    else if (stats[i].difficulty == 1) Serial.println(F("Medium"));
    else Serial.println(F("Hard"));
  }

  Serial.println(F("========================================"));
}
