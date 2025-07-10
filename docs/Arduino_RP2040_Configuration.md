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
Selecciona: `Raspberry Pi RP2040 Boards` > `Raspberry Pi Pico`
> ✅ Puedes seleccionar cualquier placa compatible RP2040. La RP2040 Zero es funcionalmente equivalente.

## 5. Conecta tu tarjeta RP2040 por primera vez
1.- Presiona el botón BOOTSEL en la tarjeta
2.- Mientras lo mantienes presionado, conecta el cable USB
3.- La tarjeta se montará como una unidad de almacenamiento masivo (tipo pendrive)
4.- Luego de subir el primer sketch, ya no es necesario usar el botón BOOTSEL para futuras cargas.

## 6. Selecciona el puerto correcto
Ve a: `Herramientas` > `Puerto`
Selecciona el puerto **COM** correspondiente a la RP2040 Zero (por ejemplo, COM3, COM4, etc.)

## 7. Verifica la carga
Ya puedes cargar cualquier sketch desde el Arduino IDE. Usa la consola serial para verificar comunicación con Serial.begin().
> 🌟 Listo
Tu entorno está configurado y listo para compilar y cargar programas en la RP2040 Zero.

Autor: Eric G. Vallejo
Empresa: Banary Source
