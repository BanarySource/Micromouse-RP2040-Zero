/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de botón con LED RGB
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código realiza la prueba del botón físico conectado al pin GP8.
 * Al presionar el botón, el LED RGB cambia a rojo; si no se presiona,
 * se muestra el color verde. Esto permite validar el estado del botón
 * y su lectura digital.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - LED RGB tipo Neopixel conectado al pin GP16
 * - Push button conectado al pin GP8
 * - Librería utilizada: Adafruit_NeoPixel
 **************************************************************/

#include <Adafruit_NeoPixel.h>

// Configuración de pines
#define LED_PIN   16  // Pin del LED RGB (GP16)
#define NUM_LEDS  1   // Número de LEDs (1)
#define PB        8   // Pin del botón (GP8)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Función de configuración
void setup() {
  pixels.begin();         // Inicializa el LED RGB
  pinMode(PB, INPUT);     // Configura el pin del botón como entrada
}

// Bucle principal
void loop() {
  // Leer el estado del botón
  if (digitalRead(PB) == LOW) {
    // Si el botón está presionado, encender el LED en rojo
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  } else {
    // Si el botón no está presionado, encender el LED en verde
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  }

  pixels.show();  // Actualiza el color del LED
}
