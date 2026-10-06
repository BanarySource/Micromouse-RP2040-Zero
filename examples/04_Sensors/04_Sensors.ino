/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de sensores analógicos
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite probar la lectura de 4 sensores analógicos
 * conectados a los pines ADC del RP2040. Los valores se muestran
 * de forma ordenada en el monitor serial.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - 4 sensores analógicos conectados a los pines GPIO26-29
 **************************************************************/

// Definición de pines analógicos (ADC)
const int sensor1Pin = 29;  // GPIO29 → ADC3
const int sensor2Pin = 28;  // GPIO28 → ADC2
const int sensor3Pin = 27;  // GPIO27 → ADC1
const int sensor4Pin = 26;  // GPIO26 → ADC0

// Función de configuración
void setup() {
  Serial.begin(115200);  // Inicializar comunicación serial
  while (!Serial);       // Esperar a que el monitor esté disponible (en algunas placas)
}

// Bucle principal
void loop() {
  // Leer los valores analógicos de cada sensor
  int sensor1 = analogRead(sensor1Pin);
  int sensor2 = analogRead(sensor2Pin);
  int sensor3 = analogRead(sensor3Pin);
  int sensor4 = analogRead(sensor4Pin);

  // Imprimir valores de forma ordenada
  Serial.print("Sensor 1 (GPIO29): "); Serial.print(sensor1);
  Serial.print("  |  Sensor 2 (GPIO28): "); Serial.print(sensor2);
  Serial.print("  |  Sensor 3 (GPIO27): "); Serial.print(sensor3);
  Serial.print("  |  Sensor 4 (GPIO26): "); Serial.println(sensor4);

  delay(500);  // Esperar 500 ms antes de la siguiente lectura
}
