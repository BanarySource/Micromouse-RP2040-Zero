/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Lectura de sensores + distancia
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código muestra las lecturas analógicas de 4 sensores IR
 * y la distancia estimada (en cm) entre paréntesis.
 * El valor máximo se limita a 900 para reducir interferencias.
 **************************************************************/

// Pines de sensores (GPIO29 - GPIO26)
const int sensorPins[4] = {29, 28, 27, 26};  // Sensor1 a Sensor4
int sensorRaw[4] = {0, 0, 0, 0};
int sensorDist[4] = {0, 0, 0, 0};

const int maxReading = 900;  // Límite superior para evitar interferencia

// ----------------- Setup -----------------
void setup() {
  Serial.begin(115200);

  Serial.println("Micromouse IR Sensor Monitor - Valor + Distancia");
  Serial.println("---------------------------------------------------------------");
  Serial.println(" Sensor1     | Sensor2     | Sensor3     | Sensor4 ");
  Serial.println("-------------|-------------|-------------|-------------");
}

// ------------------ Loop ------------------
void loop() {
  for (int i = 0; i < 4; i++) {
    int value = analogRead(sensorPins[i]);
    value = min(value, maxReading);      // Limitar a 900
    sensorRaw[i] = value;
    sensorDist[i] = map(value, 0, 900, 0, 150);  // Mapeo estimado (lineal)
  }

  // Imprimir en formato columna: valor (distancia)
  for (int i = 0; i < 4; i++) {
    Serial.print("  ");
    Serial.print(sensorRaw[i]);
    Serial.print(" (");
    Serial.print(sensorDist[i]);
    Serial.print("mm)");
    if (i < 3) Serial.print(" |");
  }
  Serial.println();

  delay(200);
}
