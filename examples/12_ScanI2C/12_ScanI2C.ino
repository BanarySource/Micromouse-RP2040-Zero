/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Escáner I2C con Wire1
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este programa realiza un escaneo del bus I2C utilizando el 
 * bus Wire1 remapeado a los pines GP10 (SDA) y GP11 (SCL) del 
 * microcontrolador RP2040. Su propósito es verificar que los 
 * dispositivos I2C, como el MPU6050, estén correctamente 
 * conectados y activos, mostrando su dirección en hexadecimal 
 * a través del monitor serial.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - Dispositivo I2C conectado a:
 *     SDA: GP10
 *     SCL: GP11
 **************************************************************/

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire1.setSDA(10);
  Wire1.setSCL(11);
  Wire1.begin();

  Serial.println("Buscando dispositivos I2C en Wire1...");

  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en la dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" 👍");

      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No se encontraron dispositivos I2C 😞");
  else
    Serial.println("Escaneo finalizado ✅");
}

void loop() {}

