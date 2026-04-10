#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "IntegralEngine.h"
#include "QuizValidator.h"

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

void showProblem();
void showResult(bool correct, char selected);

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

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) {}
  }

  generateIntegralProblem(currentProblem);
  showProblem();
}

void loop() {
  bool currentA = digitalRead(BOTON_A);
  bool currentB = digitalRead(BOTON_B);

  bool pressA = (previousA == HIGH && currentA == LOW);
  bool pressB = (previousB == HIGH && currentB == LOW);

  if (pressA) {
    bool correct = validateAnswer(currentProblem, 'A');
    showResult(correct, 'A');
    delay(1200);
    generateIntegralProblem(currentProblem);
    showProblem();
  }

  if (pressB) {
    bool correct = validateAnswer(currentProblem, 'B');
    showResult(correct, 'B');
    delay(1200);
    generateIntegralProblem(currentProblem);
    showProblem();
  }

  previousA = currentA;
  previousB = currentB;
}

void showProblem() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Integral:");

  display.setCursor(0, 12);
  display.println(currentProblem.integral);

  display.setCursor(0, 32);
  display.print("A) ");
  display.println(currentProblem.optionA);

  display.setCursor(0, 48);
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

  display.setCursor(0, 18);
  if (correct) {
    display.println("CORRECTO");
    mostrarCorrecto();
  } else {
    display.println("INCORRECTO");
    mostrarIncorrecto();
    display.setCursor(0, 36);
    display.print("Era: ");
    display.println(currentProblem.correctOption);
  }

  display.display();
}

void mostrarCorrecto() { 
  digitalWrite(LED_VERDE, HIGH); 
  sonidoCorrecto();
  delay(800); 
  digitalWrite(LED_VERDE, LOW);
}

void mostrarIncorrecto() { 
  digitalWrite(LED_ROJO, HIGH);
  sonidoIncorrecto();
  delay(800); 
  digitalWrite(LED_ROJO, LOW); 
}

void sonidoCorrecto() {
  tone(BUZZER, 1047, 70);
  delay(90);
  tone(BUZZER, 1319, 90);
  delay(110);
  tone(BUZZER, 1568, 120);
  delay(140);
  tone(BUZZER, 2093, 160);
  delay(180);
  noTone(BUZZER);
}

void sonidoIncorrecto() {
  tone(BUZZER, 659, 80);
  delay(100);
  tone(BUZZER, 494, 100);
  delay(120);
  tone(BUZZER, 349, 180);
  delay(200);
  noTone(BUZZER);
}
