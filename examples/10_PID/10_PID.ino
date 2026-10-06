/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Seguidor de pared con PID
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 *
 * Descripción:
 * El robot avanza siguiendo una pared lateral con un control PID
 * sobre la lectura del sensor IR de ese lado (regla de la mano
 * derecha o izquierda). Si pierde la pared, gira hacia ese lado;
 * si tiene una pared enfrente, gira hacia el lado contrario.
 * Los giros de 90° se hacen por tiempo (ajusta los delay a tu robot).
 *
 * Uso:
 * - Al encender, MANTÉN el botón para elegir mano derecha (LED
 *   azul); si no lo presionas, usa mano izquierda (LED rojo).
 * - Presiona el botón para arrancar.
 * - Ajusta KP, KD, KI, velBase y setpoint a tu robot y tu pista.
 *
 * Hardware:
 * - Motor A (izquierdo): PWMA GP5, AIN1 GP4, AIN2 GP3
 * - Motor B (derecho):   PWMB GP0, BIN1 GP1, BIN2 GP2
 * - Sensores IR en GP26-GP29, botón GP8, LED RGB GP16
 **************************************************************/

#include <Adafruit_NeoPixel.h>

// Definición de pines para el Puente H y el botón
#define PWMA        5  // PWM motor A
#define AIN1        4  // Dirección motor A
#define AIN2        3  // Dirección motor A
#define PWMB        0  // PWM motor B
#define BIN1        1  // Dirección motor B
#define BIN2        2  // Dirección motor B
#define button      8  // Botón de control
#define LED_PIN     16 // Pin donde está conectado el LED RGB (GP16)
#define NUM_LEDS    1  // Número de LEDs en la tira (en este caso, 1)

// Modificar la velocidad del robot con un valor entre 0 a 255
int velBase = 100;     // velocidad mínima base
int velMax  = 150;    // límite superior absoluto de cada rueda
int setpoint = 130;

// -----------------------------------------------------------------------------
// Instancia del RGB Neopixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// -----------------------------------------------------------------------------
// Pines de sensores (GPIO29 - GPIO26)
const int sensorPins[4] = {28, 27, 26, 29};  // Sensor1 a Sensor4
int sensorRaw[4] = {0, 0, 0, 0};
int sensorDist[4] = {0, 0, 0, 0};

const int maxReading = 900;  // Límite superior para evitar interferencia
int rightHand = 1;

//Position
long int sump, sum, pos, posLast, position;

//PID
float KP = 10;
float KD = 0.5;
float KI = 0.001;

//Integral data error
int error1 = 0;
int error2 = 0;
int error3 = 0;
int error4 = 0;
int error5 = 0;
int error6 = 0;

//PID data
int error = 0;
int proportional = 0;
int integral = 0;
int derivative = 0;
int control = 0;
int lastError;

// -----------------------------------------------------------------------------
// Configuración inicial del sistema
void setup() {
  Serial.begin(115200);
  pixels.begin(); // Inicializa el LED RGB
  // Encender el LED en color rojo
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  // Configurar pines de dirección como salida
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Configurar el botón como entrada con resistencia pull-up interna
  pinMode(button, INPUT_PULLUP);
  if(digitalRead(button) == 0){
    rightHand = 1;
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    pixels.show();
  }
  else{
    rightHand = 0;
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();
  }
  delay(3000);
  while(digitalRead(button));
  // Encender el LED en color rojo
  
}

// -----------------------------------------------------------------------------
void loop() {
  readSensors();
  if(rightHand == 1){
    if (sensorDist[2] >= 150) {
      motors(60, 60);
      delay(50);
      turnRight90();
    } 
    else if (sensorDist[0] < 100) {
      turnLeft90();
    } 
    else {
      //brake();
      PID(sensorDist[2]); 
    }
  }
  else{
    if (sensorDist[1] >= 150) {
      motors(60, 60);
      delay(50);
      turnLeft90();
    } 
    else if (sensorDist[0] < 100) {
      turnRight90();
    } 
    else {
      //brake();
      PID(sensorDist[1]); 
    }
  }
    
  //printData();
}
// -----------------------------------------------------------------------------
// Función para leer los valores analógicos de cada sensor
void readSensors(){
  for (int i = 0; i < 4; i++) {
    int value = analogRead(sensorPins[i]);
    value = min(value, maxReading);      // Limitar a 900
    sensorRaw[i] = value;
    sensorDist[i] = map(value, 0, 900, 0, 150);  // Mapeo estimado (lineal)
  }
}

// -----------------------------------------------------------------------------
// Función para controlar los motores
void motors(int left, int right){
  if(left >= 0){
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    left = -left;
  }
  analogWrite(PWMA, constrain(left, 0, 255));

  if(right >= 0){
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    right = -right;
  }
  analogWrite(PWMB, constrain(right, 0, 255));
}

// -----------------------------------------------------------------------------
// Función PID
void PID(int posRead){
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  error = posRead - setpoint;
  proportional = error;
  derivative = error - lastError;
  integral = error1 + error2 + error3 + error4 + error5 + error6;
  lastError = error;
  error6 = error5;
  error5 = error4;
  error4 = error3;
  error3 = error2;
  error2 = error1;
  error1 = error;

  control = (proportional * KP) + (derivative * KD) + (integral * KI);

  // Limitar la acción de control
  control = constrain(control, -100, 100); // evita giros bruscos

  // Calcular velocidades individuales
  int leftSpeed  = velBase + control;
  int rightSpeed = velBase - control;

  // Limitar velocidades máximas
  leftSpeed  = constrain(leftSpeed, 0, velMax);
  rightSpeed = constrain(rightSpeed, 0, velMax);

  motors(leftSpeed, rightSpeed);
}

void turnLeft90() {
  pixels.setPixelColor(0, pixels.Color(255, 0, 255));
  pixels.show();
  // Motor izquierdo atrás, motor derecho adelante
  motors(-100, 100);
  delay(30);  // ajustar este tiempo a tu robot real
  motors(0, 0);
}

void turnRight90() {
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  // Motor izquierdo adelante, motor derecho atrás
  motors(100, -100);
  delay(30);  // ajustar este tiempo a tu robot real
  motors(0, 0);
}

void brake() {
  // frena ambos motores
  motors(-150, -150);
  delay(40);
  motors(0, 0);
  delay(20);
}

void printData(){
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
