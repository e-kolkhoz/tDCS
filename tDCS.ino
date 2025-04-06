#include <Tiny4kOLED.h>
#include "font16x32mA.h"  // хакнутый шрифт
#include "font11x16.h"

#define SHUNT_PIN A0  
#define R_SHUNT 100.0
#define VREF 1.1     
#define CALIB_FACTOR 0.97 
#define WINDOW 32

unsigned long startTime;  // Время старта

void setup() {
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.clear();
  oled.on();


  analogReference(INTERNAL1V1);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  
  startTime = millis();  // Запоминаем момент старта
}

void loop() {
  // --- Таймер ---
  unsigned long elapsed = (millis() - startTime) / 1000;  // Прошедшее время в секундах
  int minutes = (elapsed / 60) % 100; // Ограничение 00-99
  int seconds = elapsed % 60;

  // --- Замер тока ---
  int raw = 0;
  for (int i = 0; i < WINDOW; i++) raw += analogRead(SHUNT_PIN);
  float current = ((raw / WINDOW) * (VREF / 1024.0) / R_SHUNT) * 1000.0 * CALIB_FACTOR;

  // --- Отрисовка Таймер --- 
  oled.setFont(FONT11X16);
  oled.setCursor(0, 0);
  if (minutes < 10) oled.print("0");  // Добавляем ведущий ноль
  oled.print(minutes);
  oled.print(":");
  if (seconds < 10) oled.print("0");
  oled.print(seconds);

  // --- Отрисовка Тока ---
  oled.setFont(FONT16X32MA);

  oled.setCursor(0, 4);
  oled.print(current, 2);

  oled.print(",-/"); // ' mA' на хакнутом шрифте

  delay(200);  // Обновляем каждую секунду
}