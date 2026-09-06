# SeedSense – Smart Non-Destructive Seed Quality Assessment System

SeedSense is an embedded computer-vision based system designed as a foundation for **non-destructive seed quality assessment**. The system captures seed images using an ESP32 camera and transfers the captured image to a computer through a serial communication interface for further image processing and machine-learning-based analysis.

The current implementation focuses on reliable **image acquisition, serial transmission, RGB565 image reconstruction, and image storage**. The captured images can subsequently be used with seed datasets and machine-learning models for quality assessment.

---

## Overview

Traditional seed quality assessment can involve manual inspection or destructive testing methods. SeedSense aims to provide a non-destructive and automated alternative by using image-based analysis.

The system follows this basic pipeline:

```text
        Seed
          │
          ▼
    ┌──────────────┐
    │ ESP32 Camera │
    └──────┬───────┘
           │
           │ RGB565 Image
           │
           ▼
    ┌──────────────┐
    │ Serial Link  │
    │ 115200 Baud  │
    └──────┬───────┘
           │
           ▼
    ┌──────────────┐
    │ Python Host  │
    │   capture.py │
    └──────┬───────┘
           │
           ▼
    RGB565 → RGB888
           │
           ▼
      seed.png
           │
           ▼
   Further Image/ML Analysis

   Key Features
ESP32 camera-based image acquisition
Non-destructive image capture
Serial communication between ESP32 and computer
Custom image packet framing using SS01 and EE99
RGB565 image transmission
Python-based image reconstruction
RGB565 to RGB888 conversion
Automatic PNG image generation
Compatible with further computer-vision and machine-learning pipelines
Dataset-based development using seed image data
System Architecture

SeedSense consists of two primary components:

1. Embedded Camera Unit

The ESP32 camera is responsible for:

Initializing the camera sensor
Capturing seed images
Receiving capture commands from the computer
Transmitting image data over the serial interface

The current camera configuration uses:

Resolution: 160 × 120
Pixel format: RGB565
Serial baud rate: 115200
Frame buffer count: 1
2. Python Host Application

The Python application communicates with the ESP32 through a serial port.

The application:

Opens the configured serial port.
Waits for the ESP32 READY message.
Sends the capture command c.
Searches for the SS01 image header.
Reads the image size.
Receives the image bytes.
Validates the EE99 footer.
Converts RGB565 data into RGB888.
Reshapes the image into 160 × 120.
Saves the reconstructed image as seed.png.
Serial Communication Protocol

SeedSense uses a simple custom serial framing protocol for transferring captured images.

Communication sequence
Computer                         ESP32 Camera
   │                                  │
   │        Serial Connection         │
   │─────────────────────────────────>│
   │                                  │
   │             READY                │
   │<─────────────────────────────────│
   │                                  │
   │              'c'                 │
   │─────────────────────────────────>│
   │                                  │
   │             SS01                 │
   │<─────────────────────────────────│
   │                                  │
   │          Image Size\n             │
   │<─────────────────────────────────│
   │                                  │
   │          Image Bytes             │
   │<─────────────────────────────────│
   │                                  │
   │             EE99                 │
   │<─────────────────────────────────│
   │                                  │
Packet structure
SS01
<Image Size>\n
<Image Data>
EE99

Where:

SS01 = start-of-image marker
<Image Size> = number of image bytes
<Image Data> = raw RGB565 image data
EE99 = end-of-image marker

This framing allows the Python application to identify the beginning and end of an image during serial transmission.

Hardware Requirements

The current implementation requires:

ESP32 camera module
OV2640-compatible camera sensor
USB/serial connection to a computer
Computer for image reception and processing
Seed sample for imaging

The exact ESP32 camera board configuration should match the GPIO definitions used in SeedSense.ino.

Software Requirements
Embedded
Arduino IDE
ESP32 board support package
esp_camera library
Python

Python 3.x is recommended.

Install the required Python packages:

pip install pyserial numpy pillow
Project Structure
SeedSense/
│
├── SeedSense.ino       # ESP32 camera firmware
├── camera.cpp           # Camera initialization and frame capture functions
├── camera.h             # Camera function declarations
├── capture.py           # Python serial image receiver
├── LICENSE              # MIT License
├── .gitignore           # Files excluded from Git
└── README.md            # Project documentation
Dataset

SeedSense development uses the DoubleBeanDataset for seed-image-based analysis.

The dataset is maintained separately from this GitHub repository because of its large size.

Local dataset location used during development:

C:\Users\turag\OneDrive\Desktop\DoubleBeanDataset

The dataset contains image and NumPy data files used for experimentation and model-development workflows.

Dataset policy

The dataset is intentionally not included in this repository.

This keeps the source-code repository lightweight and avoids committing large binary datasets to Git.

If the dataset is redistributed in the future, its original source, license, and usage conditions should be documented here.

Setup
1. Clone the repository
git clone https://github.com/turagakrishnakartheek/SeedSense.git
cd SeedSense
2. Install Python dependencies
pip install pyserial numpy pillow
3. Upload the ESP32 firmware

Open:

SeedSense.ino

in Arduino IDE.

Select the appropriate ESP32 camera board and upload the firmware.

4. Connect the ESP32

Connect the ESP32 camera to the computer through the required serial interface.

Identify the COM port assigned by Windows.

For example:

COM3
5. Configure the Python application

Open:

capture.py

and update:

PORT = "COM3"

to match the serial port assigned to your ESP32.

The default baud rate is:

BAUD = 115200
6. Run image capture

Execute:

python capture.py

The program waits for the ESP32 to send:

READY

It then sends:

c

to request an image.

After successful reception, the reconstructed image is saved as:

seed.png
Image Processing Pipeline

The ESP32 transmits images in RGB565 format.

The Python application reconstructs the image by separating the red, green, and blue components.

The conversion pipeline is:

RGB565
  │
  ├── Red   → 5-bit → 8-bit
  ├── Green → 6-bit → 8-bit
  └── Blue  → 5-bit → 8-bit
             │
             ▼
          RGB888
             │
             ▼
        160 × 120
             │
             ▼
          PNG Image

The final image is stored as:

seed.png
Current Implementation

The current version provides the following functionality:

ESP32 camera initialization
Camera frame acquisition
Serial communication
Capture command handling
Image-size transmission
Image-data transmission
Start/end packet markers
Python-side image reception
RGB565 decoding
PNG image generation

The current repository represents the image acquisition and data-transfer stage of the complete SeedSense seed-quality assessment pipeline.

Future Development

Future versions can extend the current acquisition pipeline with:

Computer Vision
Seed segmentation
Background removal
Shape analysis
Color analysis
Texture analysis
Size and dimensional measurements
Machine Learning
Seed quality classification
Defect detection
Variety identification
Quality scoring
Feature extraction
Dataset-based model training
Embedded/IoT Improvements
Wireless image transmission
Wi-Fi-based communication
Web dashboard
Edge-based inference
Real-time quality assessment
Automated image capture
Cloud/IoT integration
Limitations of Current Version

The current implementation is primarily an image acquisition prototype.

It currently does not perform:

Automated seed-quality classification
On-device machine-learning inference
Defect classification
Cloud deployment
Automated dataset training

These components can be integrated in subsequent development stages.

License

This project is released under the MIT License.

See the LICENSE file for details.

Author

Turagakrishnakartheek

GitHub:

https://github.com/turagakrishnakartheek
