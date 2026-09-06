import serial
import time
import numpy as np
from PIL import Image

# ----------------------------
# CONFIGURATION
# ----------------------------
PORT = "COM3"
BAUD = 115200

WIDTH = 160
HEIGHT = 120

print("Opening serial port...")

ser = serial.Serial(PORT, BAUD, timeout=1)

print("Waiting for ESP32 READY...")

# Wait up to 15 seconds for READY
start = time.time()

while True:

    line = ser.readline().decode(errors="ignore").strip()

    if line:
        print("ESP32 >", line)

    if line == "READY":
        break

    if time.time() - start > 15:
        print("READY timeout")
        ser.close()
        exit()

print("ESP32 Ready!")

# ----------------------------
# SEND CAPTURE COMMAND
# ----------------------------

ser.write(b'c')

print("Capture command sent")

# ----------------------------
# FIND SS01 HEADER
# ----------------------------

buffer = b""

while True:

    b1 = ser.read(1)

    if not b1:
        continue

    buffer += b1

    if len(buffer) > 4:
        buffer = buffer[-4:]

    if buffer == b"SS01":
        break

print("Header Found")

# ----------------------------
# IMAGE SIZE
# ----------------------------

size_string = ""

while True:

    c = ser.read(1).decode(errors="ignore")

    if c == "\n":
        break

    size_string += c

size = int(size_string.strip())

print("Image Size :", size)

# ----------------------------
# RECEIVE IMAGE
# ----------------------------

image_bytes = bytearray()

while len(image_bytes) < size:

    chunk = ser.read(size - len(image_bytes))

    if chunk:
        image_bytes.extend(chunk)

print("Received :", len(image_bytes))

# ----------------------------
# FOOTER
# ----------------------------

footer = ser.read(4)

print("Footer :", footer)

if footer != b"EE99":

    print("Footer Error")

    ser.close()

    exit()

ser.close()

print("Converting...")

rgb565 = np.frombuffer(image_bytes, dtype=np.uint16)

r = ((rgb565 >> 11) & 0x1F) * 255 // 31
g = ((rgb565 >> 5) & 0x3F) * 255 // 63
b = (rgb565 & 0x1F) * 255 // 31

rgb = np.zeros((HEIGHT * WIDTH, 3), dtype=np.uint8)

rgb[:, 0] = r
rgb[:, 1] = g
rgb[:, 2] = b

rgb = rgb.reshape((HEIGHT, WIDTH, 3))

img = Image.fromarray(rgb)

img.save("seed.png")

print("seed.png saved successfully")