#include "QuizValidator.h"

bool validateAnswer(const IntegralProblem &problem, char selectedOption) {
  return selectedOption == problem.correctOption;
}
