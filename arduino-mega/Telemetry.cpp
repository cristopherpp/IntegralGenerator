#include "Telemetry.h"

// ANSI control
#define CLEAR_SCREEN "\033[2J"
#define CURSOR_HOME  "\033[H"
#define GREEN        "\033[32m"
#define RED          "\033[31m"
#define CYAN         "\033[36m"
#define RESET        "\033[0m"

static const char* outcomeText(bool correct) {
  return correct ? "Correct" : "Incorrect";
}

static const char* optionLabel(char option) {
  switch (option) {
    case 'A': return "A";
    case 'B': return "B";
    default: return "-";
  }
}

void initTelemetry(unsigned long baudRate) {
  Serial.begin(baudRate);
  delay(200);
}

static void sep() {
  Serial.println(F("========================================"));
}

static void printLevel(uint8_t d) {
  if (d == 0) Serial.print(F("Easy"));
  else if (d == 1) Serial.print(F("Medium"));
  else Serial.print(F("Hard"));
}

// Move accuracy logic OUT of rendering
static uint8_t computeAccuracy(const CategoryStats &s) {
  if (s.shown == 0) return 0;
  return (s.correct * 100) / s.shown;
}

void renderDashboard(
  const IntegralProblem &problem,
  char selectedOption,
  bool correct,
  bool hasAnswer
) {
  const CategoryStats* stats = getStats();
  const CategoryStats &s = stats[problem.type];

  // Clear screen
  Serial.print(CLEAR_SCREEN);
  Serial.print(CURSOR_HOME);

  // HEADER
  sep();
  Serial.print(CYAN); Serial.println(F("INTEGRAL TRAINER")); Serial.print(RESET);
  sep();

  // PROBLEM INFO
  Serial.print(F("Category: "));
  Serial.println(getIntegralTypeName(problem.type));

  Serial.print(F("Difficulty: "));
  Serial.println(getDifficultyName(problem.difficulty));

  Serial.println();
  Serial.println(F("Integral:"));
  Serial.println(problem.integral);
  Serial.println();

  Serial.print(F("A) "));
  Serial.println(problem.optionA);
  Serial.print(F("B) "));
  Serial.println(problem.optionB);
  Serial.println();

  // ANSWER FEEDBACK
  if (hasAnswer) {
    Serial.print(F("Selected: "));
    Serial.println(optionLabel(selectedOption));

    Serial.print(F("Correct: "));
    Serial.println(optionLabel(problem.correctOption));

    Serial.print(F("Result: "));
    Serial.print(correct ? GREEN : RED);
    Serial.println(outcomeText(correct));
    Serial.print(RESET);

    Serial.println();
  }

  // CATEGORY STATS
  sep();
  Serial.println(F("CATEGORY"));

  Serial.print(F("Asked: "));    Serial.println(s.shown);
  Serial.print(F("Correct: "));  Serial.println(s.correct);
  Serial.print(F("Wrong: "));    Serial.println(s.wrong);

  Serial.print(F("Accuracy: "));
  if (s.shown == 0) Serial.println(F("N/A"));
  else {
    Serial.print(computeAccuracy(s));
    Serial.println(F("%"));
  }

  Serial.print(F("Mastery: "));
  Serial.println(s.mastery);

  Serial.print(F("Level: "));
  printLevel(s.difficulty);
  Serial.println();

  // GLOBAL STATS
  sep();
  Serial.println(F("GLOBAL"));

  uint16_t total = getTotalShown();
  uint16_t correctTotal = getTotalCorrect();

  Serial.print(F("Total Answered: "));
  Serial.println(total);

  Serial.print(F("Global Accuracy: "));
  if (total == 0) Serial.println(F("N/A"));
  else {
    uint8_t acc = (correctTotal * 100) / total;
    Serial.print(acc);
    Serial.println(F("%"));
  }

  sep();
}