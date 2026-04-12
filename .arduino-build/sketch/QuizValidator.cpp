#line 1 "/home/cristopher/Desktop/projects/IntegralGenerator/QuizValidator.cpp"
#include "QuizValidator.h"

bool validateAnswer(const IntegralProblem &problem, char selectedOption) {
  return selectedOption == problem.correctOption;
}
