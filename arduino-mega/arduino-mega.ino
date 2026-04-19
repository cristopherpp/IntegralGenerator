#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

#include "IntegralEngine.h"
#include "QuizValidator.h"
#include "StudentModel.h"
#include "Telemetry.h"

#define BOTON_A 22
#define BOTON_B 23

#define LED_VERDE 3
#define LED_ROJO 4
#define BUZZER 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

IntegralProblem currentProblem;

enum AppState {
  STATE_SHOW_PROBLEM,
  STATE_SHOW_RESULT
};

AppState appState = STATE_SHOW_PROBLEM;

struct ButtonState {
  uint8_t pin;
  bool lastRaw;
  bool stableState;
  unsigned long lastChangeMs;
};

const unsigned long DEBOUNCE_MS = 35;

ButtonState buttonA = {BOTON_A, HIGH, HIGH, 0};
ButtonState buttonB = {BOTON_B, HIGH, HIGH, 0};

unsigned long resultStartTime = 0;
const unsigned long RESULT_SCREEN_MS = 1000;

bool ledActive = false;
unsigned long ledOffTime = 0;
const unsigned long LED_ON_MS = 180;

const uint8_t CHAR_WIDTH = 6;
const uint8_t LINE_HEIGHT = 8;
const uint8_t FULL_WIDTH_CHARS = SCREEN_WIDTH / CHAR_WIDTH;

bool updateButton(ButtonState &b) {
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = millis();
  }

  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS) {
    if (b.stableState != raw) {
      b.stableState = raw;

      // ACTIVE LOW
      if (b.stableState == LOW) return true;
    }
  }

  return false;
}

uint8_t drawChunkedText(
  uint8_t startLine,
  const char* text,
  uint8_t maxChars,
  uint8_t maxLines,
  const char* firstPrefix,
  const char* continuationPrefix
) {
  size_t textLen = strlen(text);
  size_t index = 0;
  uint8_t rendered = 0;

  while (rendered < maxLines && (index < textLen || rendered == 0)) {
    const char* prefix = (rendered == 0) ? firstPrefix : continuationPrefix;
    uint8_t prefixLen = strlen(prefix);
    uint8_t available = maxChars;

    display.setCursor(0, (startLine + rendered) * LINE_HEIGHT);
    display.print(prefix);

    if (prefixLen < maxChars) {
      available = maxChars - prefixLen;
    } else {
      available = 0;
    }

    for (uint8_t i = 0; i < available && index < textLen; i++, index++) {
      display.write(text[index]);
    }

    rendered++;
    if (textLen == 0) break;
  }

  return rendered;
}

void drawProblemScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Integral:");

  uint8_t line = 1;

  line += drawChunkedText(line, currentProblem.integral, FULL_WIDTH_CHARS, 3, "", "  ");
  line++;

  line += drawChunkedText(line, currentProblem.optionA, FULL_WIDTH_CHARS, 2, "A:", "  ");
  line += drawChunkedText(line, currentProblem.optionB, FULL_WIDTH_CHARS, 2, "B:", "  ");

  display.display();
}

void drawResultScreen(bool correct, char selected) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 8);
  display.print("Seleccion: ");
  display.print(selected);

  display.setCursor(0, 24);
  display.print("Resultado:");

  display.setCursor(0, 36);
  if (correct) {
    display.print("CORRECTO");
  } else {
    display.print("INCORRECTO");
    display.setCursor(0, 48);
    display.print("Resp: ");
    display.print(currentProblem.correctOption);
  }

  display.display();
}

void setup() {
  initTelemetry(115200);
  Serial.println("BOOT");

  Wire.begin();

  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);

  randomSeed(analogRead(A0));
  initStudentModel();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Integral Trainer");
  display.display();

  loadNextAdaptiveProblem();
}

void loop() {
  handleButtons();
  updateOutputs();

  if (appState == STATE_SHOW_RESULT) {
    if (millis() - resultStartTime >= RESULT_SCREEN_MS) {
      loadNextAdaptiveProblem();
    }
  }
}

void handleButtons() {
  if (appState != STATE_SHOW_PROBLEM) return;

  if (updateButton(buttonA)) {
    processAnswer('A');
  }
  else if (updateButton(buttonB)) {
    processAnswer('B');
  }
}

void processAnswer(char selectedOption) {
  bool correct = validateAnswer(currentProblem, selectedOption);

  registerResult(currentProblem.type, correct);

  renderDashboard(currentProblem, selectedOption, correct, true);

  drawResultScreen(correct, selectedOption);
  triggerFeedback(correct);

  resultStartTime = millis();
  appState = STATE_SHOW_RESULT;
}

void loadNextAdaptiveProblem() {
  IntegralType nextType = pickNextIntegralTypeSmart();
  DifficultyLevel diff = getDifficultyForType(nextType);

  generateIntegralProblemByTypeAndDifficulty(currentProblem, nextType, diff);

  renderDashboard(currentProblem, '-', false, false);

  appState = STATE_SHOW_PROBLEM;
  drawProblemScreen();
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
    noTone(BUZZER);
    ledActive = false;
  }
}

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
