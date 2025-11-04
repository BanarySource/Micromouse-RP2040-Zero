/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Prueba de encoders
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 * Fecha: Julio 2025
 *
 * Descripción:
 * Este código realiza la lectura de los encoders conectados a 
 * los motores del robot para determinar el número de pasos y 
 * la dirección de giro. Utiliza interrupciones sobre el canal A 
 * y lee el canal B para identificar el sentido de rotación.
 *
 * Hardware:
 * - Microcontrolador RP2040 Zero
 * - 2 encoders con canales A y B
 * - Pines con interrupciones en GP15 y GP6
 **************************************************************/

// Pines para los encoders
const int M1_A = 15; // Canal A del Motor 1 - GP15
const int M1_B = 14; // Canal B del Motor 1 - GP14
const int M2_A = 6;  // Canal A del Motor 2 - GP6
const int M2_B = 7;  // Canal B del Motor 2 - GP7

// Contadores globales para los encoders
volatile long encoderCountM1 = 0;
volatile long encoderCountM2 = 0;

// Función de configuración
void setup() {
  Serial.begin(115200);
  
  // Configurar pines como entradas con resistencia pull-up
  pinMode(M1_A, INPUT_PULLUP);
  pinMode(M1_B, INPUT_PULLUP);
  pinMode(M2_A, INPUT_PULLUP);
  pinMode(M2_B, INPUT_PULLUP);

  // Asignar interrupciones al canal A de cada encoder (flanco de subida)
  attachInterrupt(digitalPinToInterrupt(M1_A), encoderM1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_A), encoderM2_ISR, RISING);
}

// Bucle principal
void loop() {
  // Imprimir en consola el conteo de pasos de cada encoder
  Serial.print("Encoder M1: "); Serial.print(encoderCountM1);
  Serial.print("  |  Encoder M2: "); Serial.println(encoderCountM2);
  
  delay(200);  // Esperar medio segundo entre lecturas
}

// Rutina de interrupción para el encoder del motor 1
void encoderM1_ISR() {
  // Leer canal B para determinar dirección
  if (digitalRead(M1_B) == HIGH) {
    encoderCountM1--; // Sentido horario
  } else {
    encoderCountM1++; // Sentido antihorario
  }
}

// Rutina de interrupción para el encoder del motor 2
void encoderM2_ISR() {
  if (digitalRead(M2_B) == HIGH) {
    encoderCountM2++; // Sentido horario
  } else {
    encoderCountM2--; // Sentido antihorario
  }
}
