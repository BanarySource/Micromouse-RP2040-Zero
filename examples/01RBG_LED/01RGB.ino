/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de LED RGB
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código realiza la prueba del LED RGB (Neopixel) integrado
 * en la tarjeta del robot Micromouse. Cambia de color (rojo, verde
 * y azul) de forma secuencial con una pausa de 1 segundo entre
 * cada cambio.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - LED RGB tipo Neopixel conectado al pin GP16
 * - Librería utilizada: Adafruit_NeoPixel
 **************************************************************/

#include <Adafruit_NeoPixel.h>

// Configuración del LED RGB
#define LED_PIN    16         // Pin donde está conectado el LED RGB (GP16)
#define NUM_LEDS   1          // Número de LEDs en la tira (en este caso, 1)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Función de configuración
void setup() {
  pixels.begin(); // Inicializa el LED RGB
}

// Bucle principal
void loop() {
  // Encender el LED en color rojo
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(1000);

  // Encender el LED en color verde
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  delay(1000);

  // Encender el LED en color azul
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(1000);
}
