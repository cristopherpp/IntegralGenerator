#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "IntegralEngine.h"

#define BOTON_A 4
#define BOTON_B 3

#define LED_VERDE 5
#define LED_ROJO 6
#define BUZZER 2

LiquidCrystal_I2C lcd(0x27, 16, 2);
ProblemaIntegral problemaActual;

unsigned long tiempoCambioOpcion = 0;
bool mostrandoOpcionA = true;

const unsigned long MS_CAMBIO_OPCION = 1500;

void mostrarProblema() {
  mostrandoOpcionA = true;
  tiempoCambioOpcion = millis();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(problemaActual.integral);

  lcd.setCursor(0, 1);
  lcd.print("A) ");
  lcd.print(problemaActual.opcionA);
}

void cargarSiguienteProblema() {
  generarProblemaIntegral(problemaActual);
  mostrarProblema();
}

void setup() {
  Wire.begin();
  randomSeed(analogRead(A0));

  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);

  lcd.init();
  lcd.backlight();

  cargarSiguienteProblema();
}

void sonidoCorrecto() {
  tone(BUZZER, 1047, 80); // C6
  delay(100);
  tone(BUZZER, 1319, 80); // E6
  delay(100);
  tone(BUZZER, 1568, 80); // G6
  delay(100);
  tone(BUZZER, 2093, 150); // C7
}

void sonidoIncorrecto() {
  tone(BUZZER, 392, 120); // G4
  delay(150);
  tone(BUZZER, 370, 120); // Gb4
  delay(150);
  tone(BUZZER, 349, 120); // F4
  delay(150);
  tone(BUZZER, 330, 250); // E4
}

void mostrarResultado(bool esCorrecto) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (esCorrecto) {
    lcd.print("CORRECTO");
    digitalWrite(LED_VERDE, HIGH);
    sonidoCorrecto();
  } else {
    lcd.print("Error! Era: ");
    lcd.print(problemaActual.opcionCorrecta);
    digitalWrite(LED_ROJO, HIGH);
    sonidoIncorrecto();
  }
  
  delay(1500); // Esperar 1.5 segundos para leer el resultado
  
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
}

void loop() {
  // Alternar continuamente las opciones A y B
  if (millis() - tiempoCambioOpcion >= MS_CAMBIO_OPCION) {
    tiempoCambioOpcion = millis();
    mostrandoOpcionA = !mostrandoOpcionA;

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    if (mostrandoOpcionA) {
      lcd.print("A) ");
      lcd.print(problemaActual.opcionA);
    } else {
      lcd.print("B) ");
      lcd.print(problemaActual.opcionB);
    }
  }

  // Comprobar los botones
  bool presionoA = (digitalRead(BOTON_A) == LOW);
  bool presionoB = (digitalRead(BOTON_B) == LOW);

  if (presionoA || presionoB) {
    delay(50); // Anti-rebote simple
    presionoA = (digitalRead(BOTON_A) == LOW);
    presionoB = (digitalRead(BOTON_B) == LOW);

    if (presionoA || presionoB) {
      char opcionSeleccionada = presionoA ? 'A' : 'B';
      mostrarResultado(opcionSeleccionada == problemaActual.opcionCorrecta);

      // Esperar a que se suelte el boton
      while (digitalRead(BOTON_A) == LOW || digitalRead(BOTON_B) == LOW) {
        delay(10);
      }

      cargarSiguienteProblema();
    }
  }
}
