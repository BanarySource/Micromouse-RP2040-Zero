"""
Consola de telemetría del Micromouse RP2040 (ejemplo 09).

Muestra en tabla las líneas CSV que envía el robot:
encoderM1,encoderM2,accX,accY,accZ,gyroX,gyroY,gyroZ

Funciona con los dos módulos inalámbricos:
  Bluetooth HC-06 (puerto COM):   python bluetooth_test_console.py --port COM5
  WiFi DT-06 (conéctate antes a su red Doit_WiFi_XXXXXX):
                                  python bluetooth_test_console.py --tcp 192.168.4.1:9000

Requiere pyserial solo para --port:  pip install pyserial
"""
import argparse
import socket

HEADER = "EncoderM1 | EncoderM2 | AccX | AccY | AccZ | GyroX | GyroY | GyroZ"


def lines_from_serial(port, baud):
    import serial
    with serial.Serial(port, baud, timeout=1) as ser:
        print(f"Conectado a {port} @ {baud} baud")
        while True:
            line = ser.readline().decode("utf-8", "replace").strip()
            if line:
                yield line


def lines_from_tcp(address):
    host, port = address.rsplit(":", 1)
    with socket.create_connection((host, int(port)), timeout=5) as sock:
        sock.settimeout(None)
        print(f"Conectado a {host}:{port} por TCP")
        buffer = b""
        while True:
            data = sock.recv(1024)
            if not data:
                print("El módulo cerró la conexión.")
                return
            buffer += data
            while b"\n" in buffer:
                line, buffer = buffer.split(b"\n", 1)
                line = line.decode("utf-8", "replace").strip()
                if line:
                    yield line


def main():
    parser = argparse.ArgumentParser(description="Consola de telemetría del Micromouse RP2040")
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--port", help="puerto serie del Bluetooth, p. ej. COM5 o /dev/rfcomm0")
    source.add_argument("--tcp", help="IP:puerto del módulo WiFi DT-06, p. ej. 192.168.4.1:9000")
    parser.add_argument("--baud", type=int, default=9600, help="solo con --port (por defecto 9600)")
    args = parser.parse_args()

    lines = lines_from_serial(args.port, args.baud) if args.port else lines_from_tcp(args.tcp)
    print("-" * 70)
    print(HEADER)
    try:
        for line in lines:
            data = line.split(",")
            if len(data) == 8:
                print("{:>9} | {:>9} | {:>5} | {:>5} | {:>5} | {:>6} | {:>6} | {:>6}".format(*data))
            else:
                print(line)  # mensajes del robot, p. ej. "MPU6050 listo."
    except KeyboardInterrupt:
        print("\nFin.")


if __name__ == "__main__":
    main()
