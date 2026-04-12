#include <Wire.h>
#include <U8x8lib.h>

#include "IntegralEngine.h"
#include "QuizValidator.h"
#include "StudentModel.h"
#include "Telemetry.h"

#define BOTON_A 2
#define BOTON_B 3

#define LED_VERDE 4
#define LED_ROJO 5
#define BUZZER 6

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

IntegralProblem currentProblem;

enum AppState {
  STATE_SHOW_PROBLEM,
  STATE_SHOW_RESULT
};

AppState appState = STATE_SHOW_PROBLEM;

bool previousA = HIGH;
bool previousB = HIGH;

unsigned long resultStartTime = 0;
bool ledActive = false;
unsigned long ledOffTime = 0;

const unsigned long RESULT_SCREEN_MS = 900;
const unsigned long LED_ON_MS = 180;

void showProblem();
void showResult(bool correct, char selected);
void loadNextAdaptiveProblem();
void handleButtons();
void processAnswer(char selectedOption);
void triggerFeedback(bool correct);
void updateOutputs();
void sonidoCorrecto();
void sonidoIncorrecto();

struct ButtonState {
  uint8_t pin;
  bool lastRaw;
  bool stableState;
  unsigned long lastChangeMs;
};

const unsigned long DEBOUNCE_MS = 35;

ButtonState buttonA = {BOTON_A, HIGH, HIGH, 0};
ButtonState buttonB = {BOTON_B, HIGH, HIGH, 0};

bool updateButton(ButtonState &b) {
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = millis();
  }

  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS) {
    if (b.stableState != raw) {
      b.stableState = raw;

      // return true only on confirmed press
      if (b.stableState == LOW) {
        return true;
      }
    }
  }

  return false;
}

void setup() {
  Serial.begin(115200);
  Serial.println("BOOT: setup started");

  Wire.begin();

  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("BOOT: pins configured");

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW);

  randomSeed(analogRead(A0));
  initStudentModel();

  Serial.println("BOOT: student model initialized");

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  Serial.println("BOOT: display init ok");

  loadNextAdaptiveProblem();
  Serial.println("BOOT: first problem loaded");
}

/*void setup() {
  Wire.begin();

  // For sending it to the computers
  initTelemetry(115200);

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
  }*/

void loop() {
  handleButtons();
  updateOutputs();

  if (appState == STATE_SHOW_RESULT) {
    if (millis() - resultStartTime >= RESULT_SCREEN_MS) {
      loadNextAdaptiveProblem();
    }
  }

  previousA = digitalRead(BOTON_A);
  previousB = digitalRead(BOTON_B);
}

void handleButtons() {
  if (appState != STATE_SHOW_PROBLEM) return;

  bool pressA = updateButton(buttonA);
  bool pressB = updateButton(buttonB);

  if (pressA) {
    processAnswer('A');
  } else if (pressB) {
    processAnswer('B');
  }
}

void processAnswer(char selectedOption) {
  bool correct = validateAnswer(currentProblem, selectedOption);

  registerResult(currentProblem.type, correct);

  sendQuestionResultReport(currentProblem, selectedOption, correct);

  showResult(correct, selectedOption);
  triggerFeedback(correct);

  resultStartTime = millis();
  appState = STATE_SHOW_RESULT;

  if (getTotalShown() % 8 == 0) {
      sendSessionSummaryReport();
  }
}

void loadNextAdaptiveProblem() {
  IntegralType nextType = pickNextIntegralType();
  generateIntegralProblemByType(currentProblem, nextType);

  appState = STATE_SHOW_PROBLEM;
  showProblem();
}

void showProblem() {
  u8x8.clear();

  u8x8.drawString(0, 0, "Integral:");
  u8x8.drawString(0, 1, currentProblem.integral);

  u8x8.drawString(0, 4, "A)");
  u8x8.drawString(3, 4, currentProblem.optionA);

  u8x8.drawString(0, 6, "B)");
  u8x8.drawString(3, 6, currentProblem.optionB);
}

void showResult(bool correct, char selected) {
  u8x8.clear();

  u8x8.drawString(0,0, "Elegiste:");
  if (selected == 'A') u8x8.drawString(11, 0, "B");
  else u8x8.drawString(11, 0, "B");

  if (correct) {
    u8x8.drawString(0, 3, "CORRECTO");
  } else {
    u8x8.drawString(0, 3, "INCORRECTO");
    u8x8.drawString(0, 5, "Correcto:");
    if (currentProblem.correctOption == 'A') u8x8.drawString(9, 5, "A");
    else u8x8.drawString(9, 5, "B");
  }
}

void triggerFeedback(bool correct) {
  if (correct) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
    sonidoCorrecto();
  } else {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    sonidoIncorrecto();
  }

  ledActive = true;
  ledOffTime = millis() + LED_ON_MS;
}

void updateOutputs() {
  if (ledActive && millis() >= ledOffTime) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    ledActive = false;
  }
}

void sonidoCorrecto() {
  tone(BUZZER, 1047, 70);
  delay(80);
  tone(BUZZER, 1319, 80);
  delay(90);
  tone(BUZZER, 1568, 110);
}

void sonidoIncorrecto() {
  tone(BUZZER, 659, 80);
  delay(90);
  tone(BUZZER, 494, 100);
  delay(110);
  tone(BUZZER, 349, 140);
}
