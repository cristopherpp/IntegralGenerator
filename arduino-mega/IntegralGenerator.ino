#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

#include "IntegralEngine.h"
#include "QuizValidator.h"
#include "StudentModel.h"
#include "Telemetry.h"

#define BOTON_A 22
#define BOTON_B 23

#define LED_VERDE 2
#define LED_ROJO 3
#define BUZZER 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

IntegralProblem currentProblem;

enum AppState {
  STATE_SHOW_PROBLEM,
  STATE_SHOW_RESULT
};

// ===== BUTTON SYSTEM =====
struct ButtonState {
  uint8_t pin;
  bool lastRaw;
  bool stableState;
  unsigned long lastChangeMs;
};

AppState appState = STATE_SHOW_PROBLEM;

unsigned long resultStartTime = 0;
bool ledActive = false;
unsigned long ledOffTime = 0;

const unsigned long RESULT_SCREEN_MS = 900;
const unsigned long LED_ON_MS = 180;

// LCD switching
bool showOptionA = true;
unsigned long lastSwitchMs = 0;
const unsigned long SWITCH_INTERVAL = 1500;

// ===== SAFE PRINT =====
void print16Offset(const char* text, uint8_t offset) {
  char buffer[17];
  strncpy(buffer, text, 16 - offset);
  buffer[16 - offset] = '\0';
  lcd.print(buffer);
}

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
      if (b.stableState == LOW) return true;
    }
  }

  return false;
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  Serial.println("BOOT: setup started");

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

  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.println("BOOT: display init ok");

  loadNextAdaptiveProblem();
  Serial.println("BOOT: first problem loaded");
}

// ===== LOOP =====
void loop() {
  handleButtons();
  updateOutputs();
  updateOptionDisplay();

  if (appState == STATE_SHOW_RESULT) {
    if (millis() - resultStartTime >= RESULT_SCREEN_MS) {
      loadNextAdaptiveProblem();
    }
  }
}

// ===== BUTTON HANDLING =====
void handleButtons() {
  if (appState != STATE_SHOW_PROBLEM) return;

  if (updateButton(buttonA)) {
    processAnswer('A');
  } else if (updateButton(buttonB)) {
    processAnswer('B');
  }
}

// ===== PROCESS ANSWER =====
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

// ===== LOAD NEXT PROBLEM =====
void loadNextAdaptiveProblem() {
  IntegralType nextType = pickNextIntegralType();
  generateIntegralProblemByType(currentProblem, nextType);

  appState = STATE_SHOW_PROBLEM;
  showProblem();
}

// ===== DISPLAY PROBLEM =====
void showProblem() {
  lcd.clear();

  lcd.setCursor(0, 0);
  print16Offset(currentProblem.integral, 0);

  lcd.setCursor(0, 1);
  lcd.print("A:");
  print16Offset(currentProblem.optionA, 2);

  showOptionA = true;
  lastSwitchMs = millis();
}

// ===== SWITCH OPTIONS =====
void updateOptionDisplay() {
  if (appState != STATE_SHOW_PROBLEM) return;

  if (millis() - lastSwitchMs >= SWITCH_INTERVAL) {
    lastSwitchMs = millis();
    showOptionA = !showOptionA;

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);

    if (showOptionA) {
      lcd.print("A:");
      print16Offset(currentProblem.optionA, 2);
    } else {
      lcd.print("B:");
      print16Offset(currentProblem.optionB, 2);
    }
  }
}

// ===== DISPLAY RESULT =====
void showResult(bool correct, char selected) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Sel:");
  lcd.print(selected);

  lcd.setCursor(0, 1);

  if (correct) {
    lcd.print("CORRECTO");
  } else {
    lcd.print("INC R:");
    lcd.print(currentProblem.correctOption);
  }
}

// ===== FEEDBACK =====
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

// ===== LED CONTROL =====
void updateOutputs() {
  if (ledActive && millis() >= ledOffTime) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    ledActive = false;
  }
}

// ===== SOUND =====
void sonidoCorrecto() {
  tone(BUZZER, 1047, 70);
  delay(80);
  tone(BUZZER, 1319, 80);
  delay(90);
  tone(BUZZER, 1568, 110);
  delay(120);
  noTone(BUZZER);
}

void sonidoIncorrecto() {
  tone(BUZZER, 659, 80);
  delay(90);
  tone(BUZZER, 494, 100);
  delay(110);
  tone(BUZZER, 349, 140);
  delay(150);
  noTone(BUZZER);
}
