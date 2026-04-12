#include <Arduino.h>
#line 1 "/home/cristopher/Desktop/projects/IntegralGenerator/IntegralGenerator.ino"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "IntegralEngine.h"
#include "QuizValidator.h"
#include "StudentModel.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BOTON_A 2
#define BOTON_B 3

#define LED_VERDE 4
#define LED_ROJO 5

#define BUZZER 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

IntegralProblem currentProblem;

bool previousA = HIGH;
bool previousB = HIGH;

unsigned long questionStartTime = 0;
uint16_t totalQuestionsAnswered = 0;

void showProblem();
void showResult(bool correct, char selected);
void mostrarCorrecto();
void mostrarIncorrecto();
void sonidoCorrecto();
void sonidoIncorrecto();
void loadNextAdaptiveProblem();
void showSessionSummary();

#line 40 "/home/cristopher/Desktop/projects/IntegralGenerator/IntegralGenerator.ino"
void setup();
#line 64 "/home/cristopher/Desktop/projects/IntegralGenerator/IntegralGenerator.ino"
void loop();
#line 40 "/home/cristopher/Desktop/projects/IntegralGenerator/IntegralGenerator.ino"
void setup() {
  Wire.begin();

  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW);

  randomSeed(analogRead(A0));
  initStudentModel();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) {}
  }

  loadNextAdaptiveProblem();
}

void loop() {
  bool currentA = digitalRead(BOTON_A);
  bool currentB = digitalRead(BOTON_B);

  bool pressA = (previousA == HIGH && currentA == LOW);
  bool pressB = (previousB == HIGH && currentB == LOW);

  if (pressA) {
    bool correct = validateAnswer(currentProblem, 'A');
    uint16_t responseTime = (uint16_t)(millis() - questionStartTime);

    registerResult(currentProblem.type, correct, responseTime);
    totalQuestionsAnswered++;

    showResult(correct, 'A');
    delay(1200);

    if (totalQuestionsAnswered % 10 == 0) {
      showSessionSummary();
      delay(2500);
    }

    loadNextAdaptiveProblem();
  }

  if (pressB) {
    bool correct = validateAnswer(currentProblem, 'B');
    uint16_t responseTime = (uint16_t)(millis() - questionStartTime);

    registerResult(currentProblem.type, correct, responseTime);
    totalQuestionsAnswered++;

    showResult(correct, 'B');
    delay(1200);

    if (totalQuestionsAnswered % 10 == 0) {
      showSessionSummary();
      delay(2500);
    }

    loadNextAdaptiveProblem();
  }

  previousA = currentA;
  previousB = currentB;
}

void loadNextAdaptiveProblem() {
  IntegralType nextType = pickNextIntegralType();
  generateIntegralProblemByType(currentProblem, nextType);
  questionStartTime = millis();
  showProblem();
}

void showProblem() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Integral:");

  display.setCursor(0, 12);
  display.println(currentProblem.integral);

  display.setCursor(0, 24);
  display.print("Tipo: ");
  display.println(getIntegralTypeName(currentProblem.type));

  display.setCursor(0, 40);
  display.print("A) ");
  display.println(currentProblem.optionA);

  display.setCursor(0, 54);
  display.print("B) ");
  display.println(currentProblem.optionB);

  display.display();
}

void showResult(bool correct, char selected) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Elegiste: ");
  display.println(selected);

  display.setCursor(0, 16);
  if (correct) {
    display.println("CORRECTO");
  } else {
    display.println("INCORRECTO");
    display.setCursor(0, 30);
    display.print("Era: ");
    display.println(currentProblem.correctOption);
  }

  display.setCursor(0, 46);
  display.print("Tipo: ");
  display.println(getIntegralTypeName(currentProblem.type));

  display.display();

  if (correct) {
    mostrarCorrecto();
  } else {
    mostrarIncorrecto();
  }
}

void showSessionSummary() {
  uint16_t totalCorrect = getTotalCorrect();
  uint16_t totalWrong = getTotalWrong();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Resumen:");

  display.setCursor(0, 12);
  display.print("Bien: ");
  display.println(totalCorrect);

  display.setCursor(0, 24);
  display.print("Mal: ");
  display.println(totalWrong);

  display.setCursor(0, 36);
  display.print("Debil: ");
  display.println(getIntegralTypeName((IntegralType)getWeakestCategory()));

  display.setCursor(0, 48);
  display.print("Fuerte: ");
  display.println(getIntegralTypeName((IntegralType)getStrongestCategory()));

  display.display();
}

void mostrarCorrecto() {
  digitalWrite(LED_VERDE, HIGH);
  sonidoCorrecto();
  delay(500);
  digitalWrite(LED_VERDE, LOW);
}

void mostrarIncorrecto() {
  digitalWrite(LED_ROJO, HIGH);
  sonidoIncorrecto();
  delay(500);
  digitalWrite(LED_ROJO, LOW);
}

void sonidoCorrecto() {
  tone(BUZZER, 880, 90);
  delay(110);
  tone(BUZZER, 1175, 110);
  delay(130);
  tone(BUZZER, 1568, 140);
  delay(160);
  noTone(BUZZER);
}

void sonidoIncorrecto() {
  tone(BUZZER, 700, 100);
  delay(120);
  tone(BUZZER, 523, 140);
  delay(160);
  tone(BUZZER, 392, 220);
  delay(240);
  noTone(BUZZER);
}

