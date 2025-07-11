/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de MPU6050
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite probar el acelerómetro y giroscopio 
 * MPU6050 utilizando la librería oficial de Adafruit.
 * La comunicación se realiza mediante I2C remapeado a 
 * GP10 (SDA) y GP11 (SCL) utilizando Wire1.
 *
 * El programa muestra las lecturas de aceleración (en m/s²), 
 * rotación (en rad/s) y temperatura del chip en grados Celsius.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - Módulo MPU6050 conectado por I2C (SDA: GP10, SCL: GP11)
 **************************************************************/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// -----------------------------------------------------------------------------
// Instancia del sensor MPU6050 usando la librería de Adafruit
Adafruit_MPU6050 mpu;

// -----------------------------------------------------------------------------
// Configuración inicial del sistema
void setup() {
  Serial.begin(115200);
  while (!Serial);  // Espera a que se abra el monitor serial

  // Configura pines remapeados para I2C en Wire1
  Wire1.setSDA(10);  // SDA en GP10
  Wire1.setSCL(11);  // SCL en GP11
  Wire1.begin();
  delay(100);        // Espera breve tras inicializar I2C

  Serial.println("Iniciando MPU6050...");

  // Inicialización del MPU6050 usando Wire1
  if (!mpu.begin(0x68, &Wire1, 0)) {
    Serial.println("Falló la comunicación con el módulo MPU6050");
    while (1) delay(10);  // Detener ejecución si no se detecta el sensor
  }

  // Configuración del rango del acelerómetro (±16g)
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);

  // Configuración del rango del giroscopio (±250°/s)
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  // Ancho de banda del filtro de paso bajo digital
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 listo.");
  delay(100);
}

// -----------------------------------------------------------------------------
// Bucle principal: lectura continua del sensor y visualización
void loop() {
  // Estructuras para almacenar eventos de los sensores
  sensors_event_t a, g, temp;

  // Obtener lecturas del sensor
  mpu.getEvent(&a, &g, &temp);

  // Mostrar valores de aceleración (m/s²)
  Serial.print("Aceleración [m/s²] => X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" | Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(" | Z: ");
  Serial.println(a.acceleration.z);

  // Mostrar valores del giroscopio (rad/s)
  Serial.print("Giroscopio [rad/s] => X: ");
  Serial.print(g.gyro.x);
  Serial.print(" | Y: ");
  Serial.print(g.gyro.y);
  Serial.print(" | Z: ");
  Serial.println(g.gyro.z);

  // Mostrar temperatura (°C)
  Serial.print("Temperatura: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.println("--------------------------");
  delay(500);  // Esperar medio segundo antes de la siguiente lectura
}
