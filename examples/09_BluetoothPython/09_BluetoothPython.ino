/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Envío de datos por Bluetooth
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código unifica la lectura de encoders y del MPU6050
 * y envía los datos por el módulo inalámbrico (HC-06 Bluetooth o
 * DT-06 WiFi) conectado al UART0 por hardware (TX: GP12, RX: GP13).
 *
 * Formato de salida (CSV): 
 * encoder1,encoder2,accX,accY,accZ,gyroX,gyroY,gyroZ
 *
 * Hardware:
 * - RP2040 Zero
 * - Módulo HC-06 (Bluetooth) o DT-06 (WiFi) en GP12 y GP13
 * - Módulo MPU6050 (I2C en GP10 y GP11)
 * - 2 encoders con canales A y B
 **************************************************************/

#include <Wire.h>
#include <MPU6050_light.h>

// ------------------------ Encoders -------------------------
const int M1_A = 15;  // Canal A Motor 1 - GP15
const int M1_B = 14;  // Canal B Motor 1 - GP14
const int M2_A = 6;   // Canal A Motor 2 - GP6
const int M2_B = 7;   // Canal B Motor 2 - GP7

volatile long encoderCountM1 = 0;
volatile long encoderCountM2 = 0;

// ------------------------ MPU6050 --------------------------
MPU6050 mpu(Wire1);

// --------------------- Bluetooth Serial ---------------------
#define BT_RX 13  // GP13: RX del RP2040 (a TX del módulo)
#define BT_TX 12  // GP12: TX del RP2040 (a RX del módulo)
SerialUART &BTSerial = Serial1;  // UART0 por hardware

// ------------------------ Setup ---------------------------
void setup() {
  // Iniciar Bluetooth
  BTSerial.setRX(BT_RX);
  BTSerial.setTX(BT_TX);
  BTSerial.begin(9600);

  // Configurar pines de encoder
  pinMode(M1_A, INPUT_PULLUP);
  pinMode(M1_B, INPUT_PULLUP);
  pinMode(M2_A, INPUT_PULLUP);
  pinMode(M2_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(M1_A), encoderM1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_A), encoderM2_ISR, RISING);

  // Configurar I2C remapeado
  Wire1.setSDA(10);  // SDA -> GP10
  Wire1.setSCL(11);  // SCL -> GP11
  Wire1.begin();
  delay(100);

  // Iniciar MPU6050
  BTSerial.println("Iniciando MPU6050...");
  byte status = mpu.begin();
  if (status != 0) {
    BTSerial.print("Error MPU6050. Código: ");
    BTSerial.println(status);
    while (1);
  }

  // Calibración inicial
  delay(1000);
  mpu.calcOffsets();
  BTSerial.println("MPU6050 listo.");
}

// ------------------------ Loop -----------------------------
void loop() {
  mpu.update();  // Actualizar datos del MPU

  // Lecturas del MPU6050
  float accX = mpu.getAccX();
  float accY = mpu.getAccY();
  float accZ = mpu.getAccZ();
  float gyroX = mpu.getGyroX();
  float gyroY = mpu.getGyroY();
  float gyroZ = mpu.getGyroZ();

  // Enviar por Bluetooth en formato CSV
  BTSerial.print(encoderCountM1); BTSerial.print(",");
  BTSerial.print(encoderCountM2); BTSerial.print(",");
  BTSerial.print(accX); BTSerial.print(",");
  BTSerial.print(accY); BTSerial.print(",");
  BTSerial.print(accZ); BTSerial.print(",");
  BTSerial.print(gyroX); BTSerial.print(",");
  BTSerial.print(gyroY); BTSerial.print(",");
  BTSerial.println(gyroZ);

  delay(100);  // Frecuencia de envío
}

// --------------------- ISRs Encoders -----------------------
void encoderM1_ISR() {
  if (digitalRead(M1_B)) encoderCountM1--;
  else encoderCountM1++;
}

void encoderM2_ISR() {
  if (digitalRead(M2_B)) encoderCountM2++;
  else encoderCountM2--;
}