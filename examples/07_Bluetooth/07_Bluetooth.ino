/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Control RGB vía Bluetooth
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite controlar el color del LED RGB integrado
 * en la placa a través de comandos recibidos por un módulo
 * Bluetooth HC-06 conectado por software serial a los pines
 * GP12 (TX) y GP13 (RX).
 * 
 * Los comandos recibidos son caracteres que indican una acción:
 * 'F' (Forward) verde, 'B' (Backward) rojo, 'R' (Right) azul,
 * 'L' (Left) amarillo, 'S' (Stop) blanco.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - Módulo Bluetooth HC-06 conectado a GP12 (TX) y GP13 (RX)
 * - LED RGB Neopixel conectado a GP16
 * - Librerías utilizadas: Adafruit_NeoPixel, SoftwareSerial
 **************************************************************/

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

// Configuración del LED RGB
#define LED_PIN    16
#define NUM_LEDS   1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Configuración Software Serial para el módulo Bluetooth
#define BT_RX 13  // Pin RX para recibir datos del Bluetooth
#define BT_TX 12  // Pin TX para enviar datos al Bluetooth

SoftwareSerial BTSerial(BT_RX, BT_TX);

void setup() {
  // Inicializar comunicación serial con módulo Bluetooth
  pinMode(BT_RX, INPUT);
  pinMode(BT_TX, OUTPUT);
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
