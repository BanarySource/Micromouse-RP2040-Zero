/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Control RGB vía Bluetooth
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite controlar el color del LED RGB integrado
 * en la placa a través de comandos recibidos por un módulo
 * inalámbrico serie (Bluetooth HC-06 o WiFi DT-06) conectado al
 * UART0 por hardware (Serial1): GP12 (TX) y GP13 (RX), 9600 baud.
 * 
 * Los comandos recibidos son caracteres que indican una acción:
 * 'F' (Forward) verde, 'B' (Backward) rojo, 'R' (Right) azul,
 * 'L' (Left) amarillo, 'S' (Stop) blanco.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - Módulo HC-06 (Bluetooth) o DT-06 (WiFi) en GP12 (TX) y GP13 (RX)
 * - LED RGB Neopixel conectado a GP16
 * - Librería utilizada: Adafruit_NeoPixel
 **************************************************************/

#include <Adafruit_NeoPixel.h>

// Configuración del LED RGB
#define LED_PIN    16
#define NUM_LEDS   1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Módulo inalámbrico en el UART0 por hardware (Serial1)
#define BT_RX 13  // GP13: RX del RP2040 (a TX del módulo)
#define BT_TX 12  // GP12: TX del RP2040 (a RX del módulo)

SerialUART &BTSerial = Serial1;  // UART0 por hardware

void setup() {
  // Inicializar comunicación serial con módulo Bluetooth
  BTSerial.setRX(BT_RX);
  BTSerial.setTX(BT_TX);
  BTSerial.begin(9600);

  // Inicializar LED RGB
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Apagar LED inicialmente
  pixels.show();
}

void loop() {
  if (BTSerial.available()) {
    char comando = BTSerial.read();

    switch (comando) {
      case 'F':
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Verde (Forward)
        break;
      case 'B':
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Rojo (Backward)
        break;
      case 'R':
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));  // Azul (Right)
        break;
      case 'L':
        pixels.setPixelColor(0, pixels.Color(255, 255, 0));  // Amarillo (Left)
        break;
      case 'S':
        pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // Blanco (Stop)
        break;
      default:
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // Apagar LED si comando desconocido
        break;
    }

    pixels.show();  // Actualizar color del LED
  }
}
