/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de MPU6050
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite probar el acelerómetro y giroscopio 
 * MPU6050 utilizando la librería ligera `MPU6050_light`.
 * La comunicación se realiza mediante I2C remapeado a 
 * GP10 (SDA) y GP11 (SCL) utilizando Wire1.
 *
 * El programa muestra las lecturas del acelerómetro en g y
 * del giroscopio en grados por segundo (°/s), además de 
 * realizar una calibración inicial.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - Módulo MPU6050 conectado por I2C (SDA: GP10, SCL: GP11)
 **************************************************************/

#include <Wire.h>
#include <MPU6050_light.h>

// Instancia del sensor utilizando Wire1 (I2C alternativo)
MPU6050 mpu(Wire1);

// Función de configuración
void setup() {
  Serial.begin(115200);

  // Configuración de pines I2C remapeados
  Wire1.setSDA(10);  // SDA en GP10
  Wire1.setSCL(11);  // SCL en GP11
  Wire1.begin();
  delay(100);  // Pequeña espera tras iniciar el bus

  // Inicializar el sensor MPU6050
  Serial.println("Inicializando MPU6050...");
  byte status = mpu.begin();
  if (status != 0) {
    Serial.print("Error al iniciar MPU. Código: ");
    Serial.println(status);
    while (1);  // Detener el programa si hay error
  }

  // Calibración automática
  Serial.println("Calibrando...");
  delay(1000);
  mpu.calcOffsets();  // Determinar los offsets del sensor
  Serial.println("Calibración lista.");
}

// Bucle principal
void loop() {
  mpu.update();  // Actualizar datos del sensor

  // Mostrar aceleración en g
  Serial.print("Acc (g) => X: ");
  Serial.print(mpu.getAccX(), 3);
  Serial.print(" | Y: ");
  Serial.print(mpu.getAccY(), 3);
  Serial.print(" | Z: ");
  Serial.println(mpu.getAccZ(), 3);

  // Mostrar giroscopio en grados/segundo
  Serial.print("Gyro (°/s) => X: ");
  Serial.print(mpu.getGyroX(), 2);
  Serial.print(" | Y: ");
  Serial.print(mpu.getGyroY(), 2);
  Serial.print(" | Z: ");
  Serial.println(mpu.getGyroZ(), 2);

  Serial.println("------------------------------");
  delay(300);  // Esperar antes de la siguiente lectura
}
