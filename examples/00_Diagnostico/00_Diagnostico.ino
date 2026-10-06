/**************************************************************
 * Proyecto: Micromouse RP2040 Zero - Diagnóstico de periféricos
 * Autor: Eric G. Vallejo
 * Empresa: Banary Source
 *
 * Descripción:
 * Menú por el monitor serial (USB, 115200) para probar uno a uno
 * todos los periféricos del robot. Envía una letra + Enter:
 *
 *   h  Ayuda
 *   r  LED RGB (rojo, verde, azul, blanco)
 *   b  Botón GP8 durante 10 s (LED rojo = presionado)
 *   s  Sensores ADC GP26-29 durante 5 s (min / max / promedio)
 *   e  Encoders durante 10 s (gira las ruedas a mano)
 *   i  Escaneo I2C en Wire1 + lectura cruda del MPU6050
 *   m  Motores: cada motor adelante/atrás 0.5 s a PWM bajo,
 *      reporta el avance de los encoders. ¡LEVANTA LAS RUEDAS!
 *   w  Sondeo del módulo WiFi DT-06 (ESP8266) en UART0
 *   p  Puente USB <-> DT-06 (sal con una línea que sea solo "~")
 *   a  Todo excepto motores y puente
 *
 * Hardware:
 * - Puente H: PWMA 5, AIN1 4, AIN2 3, PWMB 0, BIN1 1, BIN2 2
 * - Botón GP8, LED RGB Neopixel GP16
 * - Sensores ADC: GP29, GP28, GP27, GP26
 * - Encoders: M1 A/B GP15/GP14, M2 A/B GP6/GP7
 *
 * Correspondencia verificada en el robot:
 * - Motor A = rueda IZQUIERDA, la lee el encoder M1
 * - Motor B = rueda DERECHA,   la lee el encoder M2
 * - Velocidad positiva = la rueda avanza y su encoder cuenta +,
 *   así que motors(+v, +v) hace avanzar el robot en línea recta
 * - MPU6050 en Wire1: SDA GP10, SCL GP11
 * - DT-06 en Serial1 (UART0 hardware): TX GP12 -> RX DT-06,
 *   RX GP13 <- TX DT-06
 **************************************************************/

#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// Puente H
const int PWMA = 5, AIN1 = 4, AIN2 = 3;
const int PWMB = 0, BIN1 = 1, BIN2 = 2;
// Botón y LED
const int PB = 8;
#define LED_PIN 16
Adafruit_NeoPixel pixels(1, LED_PIN, NEO_GRB + NEO_KHZ800);
// Sensores
const int sensorPins[4] = {29, 28, 27, 26};
// Encoders
const int M1_A = 15, M1_B = 14, M2_A = 6, M2_B = 7;
volatile long encM1 = 0, encM2 = 0;
// I2C y WiFi
const int SDA_PIN = 10, SCL_PIN = 11;
const int WIFI_TX = 12, WIFI_RX = 13;
const uint8_t MPU_ADDR = 0x68;

const int MOTOR_TEST_PWM = 80;

void encoderM1_ISR() { if (digitalRead(M1_B) == HIGH) encM1--; else encM1++; }
void encoderM2_ISR() { if (digitalRead(M2_B) == HIGH) encM2++; else encM2--; }

void led(uint8_t r, uint8_t g, uint8_t b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}

void controlMotor(int pwmPin, int pin1, int pin2, int speed) {
  digitalWrite(pin1, speed >= 0 ? HIGH : LOW);
  digitalWrite(pin2, speed >= 0 ? LOW : HIGH);
  analogWrite(pwmPin, constrain(abs(speed), 0, 255));
}

void motors(int a, int b) {
  controlMotor(PWMA, AIN1, AIN2, a);
  controlMotor(PWMB, BIN1, BIN2, b);
}

void help() {
  Serial.println(F("\n=== Micromouse RP2040 - diagnostico ==="));
  Serial.println(F("h ayuda | r RGB | b boton | s sensores | e encoders"));
  Serial.println(F("i I2C/MPU | m motores (ruedas al aire!) | w WiFi DT-06"));
  Serial.println(F("p puente USB<->DT-06 (salir: linea '~') | a todo menos m/p"));
  Serial.println(F("READY"));
}

void testRGB() {
  Serial.println(F("[RGB] rojo, verde, azul, blanco, apagado"));
  led(255, 0, 0); delay(600);
  led(0, 255, 0); delay(600);
  led(0, 0, 255); delay(600);
  led(255, 255, 255); delay(600);
  led(0, 0, 0);
  Serial.println(F("[RGB] OK? confirma visualmente"));
}

void testButton() {
  Serial.println(F("[BOTON] 10 s: presiona el boton de GP8"));
  int presses = 0, last = digitalRead(PB);
  Serial.print(F("[BOTON] estado inicial: ")); Serial.println(last == LOW ? "PRESIONADO" : "suelto");
  unsigned long t0 = millis();
  while (millis() - t0 < 10000) {
    int now = digitalRead(PB);
    if (now != last) {
      delay(20);  // antirrebote
      now = digitalRead(PB);
      if (now != last) {
        if (now == LOW) presses++;
        Serial.println(now == LOW ? F("[BOTON] presionado") : F("[BOTON] suelto"));
        last = now;
      }
    }
    led(now == LOW ? 255 : 0, now == LOW ? 0 : 40, 0);
  }
  led(0, 0, 0);
  Serial.print(F("[BOTON] pulsaciones: ")); Serial.println(presses);
}

// Un pin al aire sigue a la resistencia interna: ~0 con pull-down y ~1023
// con pull-up. Un sensor conectado sostiene su propio voltaje.
bool sensorConnected(int pin) {
  analogRead(pin);  // la primera lectura configura el pin como ADC
  gpio_set_pulls(pin, false, true);
  delay(5);
  int down = analogRead(pin);
  gpio_set_pulls(pin, true, false);
  delay(5);
  int up = analogRead(pin);
  gpio_disable_pulls(pin);
  delay(5);
  return !(down < 50 && up > 950);
}

void testSensors() {
  bool connected[4], any = false;
  for (int i = 0; i < 4; i++) {
    connected[i] = sensorConnected(sensorPins[i]);
    any |= connected[i];
    Serial.print(F("[SENSORES] GP")); Serial.print(sensorPins[i]);
    Serial.println(connected[i] ? F(" conectado") : F(" SIN CONECTAR (pin al aire)"));
  }
  if (!any) return;
  Serial.println(F("[SENSORES] 5 s, acerca y aleja la mano de cada sensor"));
  int mn[4], mx[4]; long sum[4]; int n = 0;
  for (int i = 0; i < 4; i++) { mn[i] = 4095; mx[i] = 0; sum[i] = 0; }
  unsigned long t0 = millis(), lastPrint = 0;
  while (millis() - t0 < 5000) {
    int v[4];
    for (int i = 0; i < 4; i++) {
      v[i] = analogRead(sensorPins[i]);
      mn[i] = min(mn[i], v[i]); mx[i] = max(mx[i], v[i]); sum[i] += v[i];
    }
    n++;
    if (millis() - lastPrint >= 250) {
      lastPrint = millis();
      Serial.print(F("[SENSORES] "));
      for (int i = 0; i < 4; i++) {
        Serial.print("GP"); Serial.print(sensorPins[i]); Serial.print('=');
        Serial.print(v[i]); Serial.print(i < 3 ? "  " : "\n");
      }
    }
    delay(5);
  }
  for (int i = 0; i < 4; i++) {
    if (!connected[i]) continue;
    Serial.print(F("[SENSORES] GP")); Serial.print(sensorPins[i]);
    Serial.print(F(" min=")); Serial.print(mn[i]);
    Serial.print(F(" max=")); Serial.print(mx[i]);
    Serial.print(F(" prom=")); Serial.print(sum[i] / n);
    Serial.println(mx[i] - mn[i] < 10 ? F("  <- sin variacion") : F(""));
  }
}

void testEncoders() {
  Serial.println(F("[ENCODERS] 10 s: gira cada rueda a mano en ambos sentidos"));
  noInterrupts(); encM1 = 0; encM2 = 0; interrupts();
  unsigned long t0 = millis();
  while (millis() - t0 < 10000) {
    Serial.print(F("[ENCODERS] M1=")); Serial.print(encM1);
    Serial.print(F("  M2=")); Serial.println(encM2);
    delay(250);
  }
}

uint8_t mpuRead(uint8_t reg) {
  Wire1.beginTransmission(MPU_ADDR);
  Wire1.write(reg);
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU_ADDR, (uint8_t)1);
  return Wire1.available() ? Wire1.read() : 0xFF;
}

void testI2C() {
  Serial.println(F("[I2C] escaneando Wire1 (SDA GP10, SCL GP11)"));
  int found = 0;
  bool mpuFound = false;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire1.beginTransmission(addr);
    if (Wire1.endTransmission() == 0) {
      Serial.print(F("[I2C] dispositivo en 0x"));
      if (addr < 16) Serial.print('0');
      Serial.println(addr, HEX);
      found++;
      if (addr == MPU_ADDR) mpuFound = true;
    }
  }
  Serial.print(F("[I2C] total: ")); Serial.println(found);
  if (!mpuFound) { Serial.println(F("[MPU] no responde en 0x68")); return; }

  uint8_t who = mpuRead(0x75);
  Serial.print(F("[MPU] WHO_AM_I=0x")); Serial.print(who, HEX);
  Serial.println(who == 0x68 ? F(" (MPU6050 original)") : F(" (clon/variante: la lib de Adafruit puede rechazarlo)"));

  // Despertar (PWR_MGMT_1 = 0) y leer 14 bytes desde ACCEL_XOUT_H
  Wire1.beginTransmission(MPU_ADDR); Wire1.write(0x6B); Wire1.write(0); Wire1.endTransmission();
  delay(100);
  for (int k = 0; k < 5; k++) {
    Wire1.beginTransmission(MPU_ADDR); Wire1.write(0x3B); Wire1.endTransmission(false);
    Wire1.requestFrom(MPU_ADDR, (uint8_t)14);
    int16_t d[7];
    for (int i = 0; i < 7; i++) d[i] = (Wire1.read() << 8) | Wire1.read();
    // Rangos por defecto: +-2 g (16384 LSB/g), +-250 °/s (131 LSB/°/s)
    Serial.print(F("[MPU] acc[g] "));
    Serial.print(d[0] / 16384.0, 2); Serial.print(' ');
    Serial.print(d[1] / 16384.0, 2); Serial.print(' ');
    Serial.print(d[2] / 16384.0, 2);
    Serial.print(F("  gyro[dps] "));
    Serial.print(d[4] / 131.0, 1); Serial.print(' ');
    Serial.print(d[5] / 131.0, 1); Serial.print(' ');
    Serial.print(d[6] / 131.0, 1);
    Serial.print(F("  temp[C] ")); Serial.println(d[3] / 340.0 + 36.53, 1);
    delay(200);
  }
}

void testMotor(const char *name, int a, int b) {
  noInterrupts(); long e1 = encM1, e2 = encM2; interrupts();
  motors(a, b);
  delay(500);
  motors(0, 0);
  delay(300);
  noInterrupts(); long d1 = encM1 - e1, d2 = encM2 - e2; interrupts();
  Serial.print(F("[MOTORES] ")); Serial.print(name);
  Serial.print(F("  dM1=")); Serial.print(d1);
  Serial.print(F("  dM2=")); Serial.println(d2);
}

void testMotors() {
  Serial.println(F("[MOTORES] PWM 80, 0.5 s cada paso. Ruedas al aire."));
  led(255, 80, 0);
  testMotor("A adelante", MOTOR_TEST_PWM, 0);
  testMotor("A atras   ", -MOTOR_TEST_PWM, 0);
  testMotor("B adelante", 0, MOTOR_TEST_PWM);
  testMotor("B atras   ", 0, -MOTOR_TEST_PWM);
  led(0, 0, 0);
  Serial.println(F("[MOTORES] fin"));
}

// Envía un comando al DT-06 y devuelve lo que responda en 'ms' milisegundos
String wifiCmd(const char *cmd, unsigned long ms) {
  while (Serial1.available()) Serial1.read();
  Serial1.print(cmd);
  String resp;
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    while (Serial1.available()) {
      char c = Serial1.read();
      if (resp.length() < 400) resp += (c >= 32 && c < 127) || c == '\n' ? c : '.';
    }
  }
  resp.trim();
  return resp;
}

const long wifiBauds[] = {115200, 9600, 57600, 38400, 19200};
long wifiBaud = 9600;  // default del DT-06 (firmware Doit Ser2Sock)

void testWiFi() {
  Serial.println(F("[WIFI] sondeando DT-06 en Serial1 (TX GP12, RX GP13)"));
  for (long baud : wifiBauds) {
    Serial1.end();
    Serial1.begin(baud);
    delay(50);
    String r = wifiCmd("AT\r\n", 400);
    Serial.print(F("[WIFI] ")); Serial.print(baud); Serial.print(F(" baud -> "));
    Serial.println(r.length() ? r : String("(sin respuesta)"));
    if (r.indexOf("OK") >= 0) {
      wifiBaud = baud;
      Serial.println(F("[WIFI] responde a comandos AT"));
      const char *cmds[] = {"AT+GMR\r\n", "AT+CWMODE?\r\n", "AT+CIFSR\r\n"};
      for (const char *c : cmds) {
        Serial.print(F("[WIFI] ")); Serial.print(c);
        Serial.println(wifiCmd(c, 1000));
      }
      return;
    }
  }
  Serial1.end();
  Serial1.begin(wifiBaud);
  Serial.println(F("[WIFI] ningun baud respondio a AT. Con el firmware Doit (puente transparente)"));
  Serial.println(F("[WIFI] es normal: conectate a su red, config en http://192.168.4.1, datos"));
  Serial.println(F("[WIFI] por TCP 192.168.4.1:9000, y usa 'p' para el puente."));
}

void bridgeWiFi() {
  Serial.print(F("[PUENTE] USB <-> DT-06 a ")); Serial.print(wifiBaud);
  Serial.println(F(" baud. Escribe '~' solo en una linea para salir."));
  String line;
  while (true) {
    while (Serial1.available()) Serial.write(Serial1.read());
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        if (line == "~" || line == "~\r") { Serial.println(F("\n[PUENTE] fin")); return; }
        Serial1.print(line); Serial1.print('\n');
        line = "";
      } else {
        line += c;
      }
    }
  }
}

void setup() {
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  motors(0, 0);
  pinMode(PB, INPUT_PULLUP);

  pinMode(M1_A, INPUT_PULLUP); pinMode(M1_B, INPUT_PULLUP);
  pinMode(M2_A, INPUT_PULLUP); pinMode(M2_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(M1_A), encoderM1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_A), encoderM2_ISR, RISING);

  pixels.begin();
  led(0, 0, 20);

  Wire1.setSDA(SDA_PIN);
  Wire1.setSCL(SCL_PIN);
  Wire1.begin();

  Serial1.setTX(WIFI_TX);
  Serial1.setRX(WIFI_RX);
  Serial1.begin(wifiBaud);

  Serial.begin(115200);
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 5000);  // no se cuelga si no hay monitor
  led(0, 0, 0);
  help();
}

void loop() {
  if (!Serial.available()) return;
  char c = Serial.read();
  switch (c) {
    case 'h': help(); break;
    case 'r': testRGB(); break;
    case 'b': testButton(); break;
    case 's': testSensors(); break;
    case 'e': testEncoders(); break;
    case 'i': testI2C(); break;
    case 'm': testMotors(); break;
    case 'w': testWiFi(); break;
    case 'p': bridgeWiFi(); break;
    case 'a': testRGB(); testI2C(); testSensors(); testWiFi(); break;
    default: return;  // ignora \r, \n y basura
  }
  Serial.println(F("DONE"));
}
