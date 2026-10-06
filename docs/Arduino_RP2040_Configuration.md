# Configuración del entorno de desarrollo para RP2040 Zero con Arduino IDE

Este documento explica paso a paso cómo configurar el Arduino IDE para programar la tarjeta RP2040 Zero, utilizando el soporte oficial de Raspberry Pi (desarrollado por Earle Philhower).

## 1. Abre el Arduino IDE
Asegúrate de tener instalada la versión más reciente del Arduino IDE (se recomienda la versión 2.x o superior).

## 2. Agrega el gestor de placas de Raspberry Pi Pico
- Dirígete a: `Archivo` > `Preferencias`
- En el campo "Gestor de URLs Adicionales de Tarjetas", pega la siguiente URL:

> https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

> ⚠️ Si ya tienes otra URL en ese campo, sepárala con una coma.

## 3. Instala el soporte para RP2040
Ve a: `Herramientas` > `Placa` > `Gestor de tarjetas`
Escribe "rp2040" o "Raspberry Pi Pico / RP2040"
Instala el paquete desarrollado por Earle Philhower

## 4. Selecciona la tarjeta adecuada
Ve a: `Herramientas` > `Placa`
Selecciona: `Raspberry Pi RP2040 Boards` > `Waveshare RP2040 Zero`
> ✅ Si no la encuentras, `Raspberry Pi Pico` también funciona con todos los ejemplos.

## 5. Conecta tu tarjeta RP2040 por primera vez
- Presiona el botón BOOTSEL en la tarjeta
- Mientras lo mantienes presionado, conecta el cable USB
- La tarjeta se montará como una unidad de almacenamiento masivo (tipo pendrive)
- Luego de subir el primer sketch, ya no es necesario usar el botón BOOTSEL para futuras cargas.

## 6. Selecciona el puerto correcto
Ve a: `Herramientas` > `Puerto`
Selecciona el puerto **COM** correspondiente a la RP2040 Zero (por ejemplo, COM3, COM4, etc.)

## 7. Verifica la carga
Carga [`examples/00_Diagnostico`](../examples/00_Diagnostico/00_Diagnostico.ino) y abre el monitor serial a **115200**: debe aparecer el menú de pruebas.
> 🌟 Listo
Tu entorno está configurado y listo para compilar y cargar programas en la RP2040 Zero.


Autor: Eric G. Vallejo - Banary Source
