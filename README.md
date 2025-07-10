# Micromouse RP2040 Zero - Pruebas de Periféricos

Este repositorio contiene los códigos de prueba para verificar el correcto funcionamiento de los periféricos integrados en un robot tipo **Micromouse** basado en el microcontrolador **RP2040 Zero**.

## 🎯 Objetivo

Facilitar a los usuarios con el mismo prototipo la validación de hardware antes de programar rutinas de navegación, control de trayectoria o resolución de laberintos.

---

## 🧩 Componentes probados

| Componente         | Descripción                                      |
|--------------------|--------------------------------------------------|
| 🟢 RP2040 Zero     | Microcontrolador principal                      |
| 🔵 LED RGB (Neopixel) | LED direccionable en GP16                    |
| 🔘 Push Button     | Botón de inicio conectado a GP8                  |
| ⚙️ Motores DC      | 2 motores con control PWM + dirección (Puente H) |
| 🔄 Encoders        | Encoders de cuadratura con detección de dirección |
| 🎯 MPU6050         | Acelerómetro y giroscopio vía I2C               |
| 📶 Bluetooth HC-06 | Comunicación UART con comandos por BT           |

---

## ⚙️ Requisitos
Arduino IDE (o equivalente compatible)

### Librerías:
- Adafruit_NeoPixel
- MPU6050_light
- SoftwareSerial (si se usa Bluetooth con UART por software)
Cable USB-C
Módulo Bluetooth HC-06 emparejado a 9600 baudios

## 🚀 Cómo usar
1.- Conecta tu RP2040 Zero por USB al PC.
2.- Abre uno de los archivos .ino según el periférico que desees probar.
3.- Carga el sketch desde el Arduino IDE.
4.- Sigue las instrucciones del monitor serial o desde la app Bluetooth.

## 📲 Comandos vía Bluetooth
En la prueba de RGB por Bluetooth, puedes enviar desde el celular los siguientes caracteres:
| Comando | Acción    | Color del LED |
| ------- | --------- | ------------- |
| `F`     | Adelante  | Verde         |
| `B`     | Atrás     | Rojo          |
| `R`     | Derecha   | Azul          |
| `L`     | Izquierda | Amarillo      |
| `S`     | Stop      | Blanco        |

## 📌 Notas
El bus I2C fue remapeado a los pines GP10 (SDA) y GP11 (SCL).
El puerto UART0 fue utilizado para comunicación Bluetooth mediante SoftwareSerial.
Se recomienda utilizar una fuente externa si se conectan motores directamente.

## 🤝 Contribución
Si tienes un robot basado en este diseño y deseas mejorar las pruebas o agregar nuevas funciones, ¡las pull requests son bienvenidas!

## 🔖 Licencia
Este proyecto está bajo licencia MIT. Puedes usarlo, modificarlo y distribuirlo libremente.

## ✍️ Autor
Eric G. Vallejo
Docente e integrador de proyectos de robótica, automatización y sistemas embebidos.
