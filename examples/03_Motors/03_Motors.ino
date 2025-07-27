/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de motores DC
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código permite probar ambos motores DC conectados a través
 * de un puente H. Al presionar el botón físico (GP8), los motores
 * giran hacia adelante a velocidad fija. Al soltarlo, se detienen.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - 2 motores DC
 * - Puente H con entradas PWM y control de dirección
 * - Push button en GP8
 **************************************************************/

// Definición de pines para el Puente H y el botón
const int PWMA   = 5;  // PWM motor A
const int AIN1   = 4;  // Dirección motor A
const int AIN2   = 3;  // Dirección motor A
const int PWMB   = 0;  // PWM motor B
const int BIN1   = 1;  // Dirección motor B
const int BIN2   = 2;  // Dirección motor B
const int button = 8;  // Botón de control

// Función de configuración
void setup() {
  // Configurar pines de dirección como salida
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Configurar el botón como entrada con resistencia pull-up interna
  pinMode(button, INPUT);
}

// Bucle principal
void loop() {
  // Si el botón está presionado (LOW), avanzar con velocidad media
  if (digitalRead(button) == LOW) {
    motors(100, 100);  // Ambos motores hacia adelante
  } else {
    motors(0, 0);       // Detener ambos motores
  }
}

// Función para controlar ambos motores simultáneamente
void motors(int speedA, int speedB) {
  controlMotor(PWMA, AIN1, AIN2, speedA);
  controlMotor(PWMB, BIN1, BIN2, speedB);
}

// Función genérica para controlar un motor individual
void controlMotor(int pwmPin, int pin1, int pin2, int speed) {
  if (speed >= 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    speed = -speed;  // Convertir a valor positivo para PWM
  }

  analogWrite(pwmPin, constrain(speed, 0, 255));  // Enviar señal PWM
}
