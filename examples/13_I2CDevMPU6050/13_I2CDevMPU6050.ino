/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de MPU6050 (Wire1)
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código prueba el sensor MPU6050 utilizando la librería
 * original MPU6050.h de Jeff Rowberg, remapeando el bus I2C 
 * a los pines GP10 (SDA) y GP11 (SCL) mediante Wire1, sin usar
 * el módulo I2Cdev.
 *
 * Se muestran los valores de aceleración (en g), giroscopio 
 * (en °/s) y temperatura (en °C).
 *
 * Hardware:
 * - RP2040 Zero
 * - MPU6050 conectado a I2C (SDA: GP10, SCL: GP11, VCC 3.3V)
 **************************************************************/

#include <Wire.h>
#include "MPU6050.h"

// Instanciar sensor en dirección 0x68
MPU6050 mpu(0x68);

void setup() {
  Serial.begin(115200);

  // Configurar pines I2C en Wire1 (SDA: GP10, SCL: GP11)
  Wire1.setSDA(10);
  Wire1.setSCL(11);
  Wire1.begin();

  // Inicializar MPU6050 usando Wire1
  mpu.initialize();
  Serial.println("Inicializando MPU");

  // Comprobar conexión
  if (mpu.testConnection()) {
    Serial.println("MPU6050 conectado correctamente.");
  } else {
    Serial.println("Error: no se detectó el MPU6050.");
    while (1); // Detener ejecución
  }
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t tempRaw;

  // Leer datos crudos del sensor
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  tempRaw = mpu.getTemperature();

  // Convertir temperatura a °C
  float tempC = tempRaw / 340.0 + 36.53;

  // Mostrar lecturas
  Serial.print("Acel [g] X: "); Serial.print(ax);
  Serial.print(" Y: "); Serial.print(ay);
  Serial.print(" Z: "); Serial.print(az);

  Serial.print(" | Giro [°/s] X: "); Serial.print(gx);
  Serial.print(" Y: "); Serial.print(gy);
  Serial.print(" Z: "); Serial.print(gz);

  Serial.print(" | Temp [°C]: "); Serial.println(tempC);

  delay(500);
}
