# SeedSense – Smart Non-Destructive Seed Quality Assessment System

SeedSense is an embedded computer-vision system designed as a foundation for **non-destructive seed quality assessment**. The system uses an ESP32 camera to capture seed images and transfers the image data to a computer through a serial communication interface for further image processing and machine-learning analysis.

The current implementation focuses on the **image acquisition and transmission pipeline**, including ESP32 camera initialization, image capture, serial packet transmission, RGB565 image reconstruction, and PNG image generation.

---

## Overview

Conventional seed quality assessment may involve manual inspection or destructive laboratory procedures. SeedSense explores an image-based approach in which seed samples can be captured without physically damaging them.

The current system consists of two major stages:

### 1. Embedded Image Acquisition

- ESP32 camera captures the seed image.
- Image data is obtained in RGB565 format.
- The ESP32 waits for a capture command from the host computer.
- The image is transmitted through a serial interface.

### 2. Host-Side Image Reconstruction

- A Python application communicates with the ESP32.
- The application receives the image packet.
- RGB565 data is converted into RGB888.
- The reconstructed image is saved as a PNG file.
- The resulting image can be used for subsequent computer-vision and machine-learning analysis.

---

## System Architecture

```text
┌─────────────┐
│ Seed Sample │
└──────┬──────┘
       │
       ▼
┌────────────────┐
│  ESP32 Camera  │
└───────┬────────┘
        │
        │ Image Capture
        ▼
┌────────────────┐
│  RGB565 Image  │
│      Data      │
└───────┬────────┘
        │
        │ Serial Communication
        │ 115200 Baud
        ▼
┌────────────────────┐
│ Python Host        │
│ Application        │
│ capture.py         │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│ Receive Image      │
│ Packet              │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│ Validate SS01/EE99 │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│ RGB565 → RGB888    │
│ Conversion          │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│ 160 × 120 RGB      │
│ Image              │
└─────────┬──────────┘
          │
          ▼
┌────────────────────┐
│     seed.png       │
└─────────┬──────────┘
          │
          ▼
┌─────────────────────────────┐
│ Further Computer Vision /   │
│ Machine Learning Analysis   │
└─────────────────────────────┘
Key Features
ESP32 camera-based image acquisition
Non-destructive seed imaging
Serial communication between ESP32 and computer
115200 baud communication
Custom image packet framing
SS01 start-of-image marker
EE99 end-of-image marker
Image-size transmission
RGB565 image reception
RGB565 to RGB888 conversion
160 × 120 image reconstruction
PNG image generation
Python-based host-side processing
Dataset-based development for future seed-quality analysis
Hardware Requirements

The current implementation requires:

ESP32 camera board
OV2640-compatible camera sensor
USB/serial connection
Computer running Python
Seed samples for image acquisition
Camera Configuration

The current firmware uses the following configuration:
| Parameter             | Configuration |
| --------------------- | ------------- |
| Frame Size            | 160 × 120     |
| Pixel Format          | RGB565        |
| XCLK Frequency        | 10 MHz        |
| Frame Buffer Count    | 1             |
| Frame Buffer Location | DRAM          |
| JPEG Quality Setting  | 15            |
| Serial Baud Rate      | 115200        |
The ESP32 camera GPIO configuration is defined directly in SeedSense.ino. The selected camera board must match this configuration.

Software Requirements
Embedded Software
Arduino IDE
ESP32 board support package
ESP32 Camera library (esp_camera)
Python

Python 3.x is recommended.

Install the required Python packages:
pip install pyserial numpy pillow
Python Libraries
Library	Purpose
PySerial	Serial communication with ESP32
NumPy	RGB565 image-data processing
Pillow	Image reconstruction and PNG generation
Project Structure
SeedSense/
│
├── SeedSense.ino
│   └── Main ESP32 camera firmware
│
├── camera.cpp
│   └── Camera initialization and frame capture functions
│
├── camera.h
│   └── Camera function declarations
│
├── capture.py
│   └── Python application for receiving and reconstructing images
│
├── LICENSE
│   └── MIT License
│
├── .gitignore
│   └── Files excluded from version control
│
└── README.md
    └── Project documentation
Working Principle

The SeedSense image acquisition process follows these steps.

Step 1 – Camera Initialization

When the ESP32 starts, the firmware initializes the camera using the configured GPIO pins and camera parameters.

The ESP32 reports:

SeedSense USB Mode
Camera Ready
READY

The Python application waits for the READY message before requesting an image.

Step 2 – Capture Command

The Python application sends the character:

c

to the ESP32.

The ESP32 detects the command and captures a frame using:

esp_camera_fb_get();
Step 3 – Image Packet Transmission

After successfully capturing a frame, the ESP32 transmits:

SS01

as the start marker.

It then sends the image size followed by a newline.

The image data is then transmitted.

Finally, the ESP32 sends:

EE99

as the end marker.

Step 4 – Image Reception

The Python application searches the incoming serial stream until it detects:

SS01

It then reads the image-size value and receives exactly that number of bytes.

Step 5 – Packet Validation

After receiving the image data, Python reads the four-byte footer.

The expected footer is:

EE99

If the footer does not match, the application reports:

Footer Error

and terminates the capture process.

Step 6 – RGB565 Conversion

The ESP32 transmits the image using RGB565 pixel representation.

RGB565 contains:

Red   = 5 bits
Green = 6 bits
Blue  = 5 bits

The Python application expands these components into 8-bit RGB values:

RGB565
  │
  ├── Red   : 5-bit → 8-bit
  ├── Green : 6-bit → 8-bit
  └── Blue  : 5-bit → 8-bit
              │
              ▼
           RGB888
Step 7 – Image Reconstruction

The RGB values are reshaped into:

Width    = 160 pixels
Height   = 120 pixels
Channels = 3

The resulting image is converted into a Pillow image and saved as:

seed.png
Serial Communication Protocol

SeedSense uses a simple custom serial framing protocol.

Communication Sequence
Computer                              ESP32
   │                                    │
   │        Serial Connection           │
   │───────────────────────────────────>│
   │                                    │
   │              READY                 │
   │<───────────────────────────────────│
   │                                    │
   │                c                   │
   │───────────────────────────────────>│
   │                                    │
   │               SS01                 │
   │<───────────────────────────────────│
   │                                    │
   │           Image Size               │
   │<───────────────────────────────────│
   │                                    │
   │           Image Data               │
   │<───────────────────────────────────│
   │                                    │
   │               EE99                 │
   │<───────────────────────────────────│
   │                                    │
Packet Format
| Field          | Description                |
| -------------- | -------------------------- |
| `SS01`         | Start-of-image marker      |
| `<Image Size>` | Number of image-data bytes |
| `<Image Data>` | Raw RGB565 image data      |
| `EE99`         | End-of-image marker        |

This framing mechanism allows the Python application to identify the beginning and end of each transmitted image.

ESP32 Firmware

The main firmware is contained in:

SeedSense.ino

The firmware performs:

Serial initialization
Camera configuration
Camera initialization
READY status transmission
Serial command monitoring
Image capture
Image-size transmission
Image-data transmission
End-of-image marker transmission
Frame-buffer release

The camera is initialized using:

esp_camera_init(&config);

An image frame is captured using:

esp_camera_fb_get();

After transmission, the frame buffer is returned using:

esp_camera_fb_return(fb);
Python Capture Application

The host-side acquisition program is:

capture.py

The application performs:

Open Serial Port
       │
       ▼
Wait for READY
       │
       ▼
Send 'c'
       │
       ▼
Search for SS01
       │
       ▼
Read Image Size
       │
       ▼
Receive Image Bytes
       │
       ▼
Validate EE99
       │
       ▼
RGB565 → RGB888
       │
       ▼
Reshape to 160 × 120
       │
       ▼
Save seed.png
Installation and Setup
1. Clone the Repository
git clone https://github.com/turagakrishnakartheek/SeedSense.git

Enter the project directory:

cd SeedSense
2. Install Python Dependencies
pip install pyserial numpy pillow
3. Configure the ESP32

Open:

SeedSense.ino

in Arduino IDE.

Install the ESP32 board support package and select the appropriate ESP32 camera board configuration.

Upload the firmware to the ESP32.

4. Connect the ESP32

Connect the ESP32 to the computer using the required USB/serial interface.

On Windows, determine the COM port assigned to the device.

For example:

COM3
5. Configure the Python Serial Port

Open:

capture.py

Locate:

PORT = "COM3"

Change COM3 to the actual serial port assigned to your ESP32.

The default communication speed is:

BAUD = 115200
Running the System

Once the ESP32 firmware has been uploaded and the correct COM port has been configured, run:

python capture.py

The program waits for the ESP32 to report:

READY

The capture command is then automatically sent.

A successful run produces output similar to:

Opening serial port...
Waiting for ESP32 READY...
ESP32 > SeedSense USB Mode
ESP32 > Camera Ready
ESP32 > READY
ESP32 Ready!
Capture command sent
Header Found
Image Size : ...
Received : ...
Footer : b'EE99'
Converting...
seed.png saved successfully

The resulting image is saved as:

seed.png
Dataset

SeedSense development uses a seed-image dataset referred to as:

DoubleBeanDataset

The dataset is maintained separately from this GitHub repository because of its large size.

The dataset contains image files and NumPy data files used during development and experimentation.

Dataset Storage

The dataset is intentionally not included in this repository.

This repository contains the SeedSense source code and documentation, while the dataset is maintained separately.

SeedSense Repository
        │
        ├── Source Code
        ├── Configuration
        └── Documentation

DoubleBeanDataset
        │
        ├── Image Data
        └── NumPy Data

If the dataset is redistributed in the future, its original source, license, citation requirements, and access instructions should be documented here.

Current Implementation

The current version implements the image acquisition and transmission stage of SeedSense.

Implemented
 ESP32 camera initialization
 Seed image capture
 Serial communication
 115200 baud communication
 Capture command handling
 Image-size transmission
 Custom image packet framing
 SS01 start marker
 EE99 end marker
 Python image reception
 Image-size based byte reception
 Footer validation
 RGB565 decoding
 RGB888 conversion
 160 × 120 image reconstruction
 PNG image generation
Machine Learning and Computer Vision

The current repository establishes the image-acquisition pipeline required for subsequent computer-vision and machine-learning stages.

The captured image:

seed.png

can be used as input for future processing such as:

Seed Image
    │
    ▼
Preprocessing
    │
    ▼
Segmentation
    │
    ▼
Feature Extraction
    │
    ├── Color Features
    ├── Shape Features
    ├── Size Features
    └── Texture Features
    │
    ▼
Machine Learning Model
    │
    ▼
Seed Quality Assessment

Note: Machine-learning inference is not currently implemented in the code contained in this repository.

Future Development
Computer Vision
Seed segmentation
Background removal
Shape analysis
Color analysis
Texture analysis
Size and dimensional measurements
Image-quality assessment
Machine Learning
Seed quality classification
Defect detection
Variety identification
Quality scoring
Feature extraction
Dataset-based model training
Model evaluation
Edge AI
On-device inference
Lightweight neural-network models
Real-time seed classification
Embedded feature extraction
IoT Integration
Wi-Fi-based image transmission
Web dashboard
Remote monitoring
Cloud integration
Database storage
Real-time analytics
Automated Acquisition
Automated seed positioning
Multiple-seed imaging
Batch image capture
Automatic image naming
Controlled illumination
Consistent imaging conditions
Limitations

The current implementation has several limitations:

The image acquisition system currently uses a fixed image resolution of 160 × 120.
The serial port is configured manually in capture.py.
The system currently requires a computer for image reconstruction.
The current repository does not perform machine-learning inference.
Automated seed segmentation is not currently implemented.
Automated seed-quality classification is not currently implemented.
The camera GPIO configuration is specific to the configured ESP32 camera hardware.
Dataset files are maintained separately from the source-code repository.

These limitations define the scope of the current prototype and provide directions for future development.

Error Handling

The Python application includes basic communication checks.

READY Timeout

The Python program waits for the ESP32 READY message for up to 15 seconds.

If the message is not received:

READY timeout

is reported and the serial connection is closed.

Camera Capture Failure

If the ESP32 fails to obtain a camera frame, it sends:

FAIL
Footer Validation

After receiving the image, Python checks for:

EE99

If the expected footer is not received, the program reports:

Footer Error

This provides basic protection against incomplete or incorrectly framed image transfers.

Reproducibility

To reproduce the current image-acquisition pipeline:

Obtain compatible ESP32 camera hardware.
Configure the camera GPIO pins according to SeedSense.ino.
Install the ESP32 board support package in Arduino IDE.
Upload SeedSense.ino.
Connect the ESP32 to the host computer.
Identify the assigned serial port.
Update the PORT variable in capture.py.
Install the Python dependencies.
Run capture.py.
Verify that seed.png is generated.
Security and Privacy

The current project does not require cloud credentials or API keys.

Do not commit the following to the repository:

API keys
Passwords
Authentication tokens
Private configuration files
Personal datasets
Large generated files

The included .gitignore helps prevent common generated and data files from being accidentally committed.

Repository Guidelines

When contributing to SeedSense:

Keep source code organized.
Do not commit large datasets.
Do not commit generated images.
Do not commit credentials or secrets.
Document hardware-specific changes.
Test serial communication changes before committing.
Use meaningful commit messages.

Example:

git add .
git commit -m "Improve image capture handling"
git push
License

This project is released under the MIT License.

See the LICENSE file for the complete license text.

Author

Turagakrishnakartheek

GitHub:

https://github.com/turagakrishnakartheek

Project Status

Status: Active Prototype

Current Stage
ESP32 Camera
     ↓
Image Capture
     ↓
Serial Transmission
     ↓
Python Reception
     ↓
RGB565 Reconstruction
     ↓
PNG Image
Planned Stage
PNG Image
     ↓
Computer Vision
     ↓
Feature Extraction
     ↓
Machine Learning
     ↓
Seed Quality Assessment
Summary

SeedSense provides a hardware-software foundation for non-destructive seed image acquisition. The current implementation demonstrates communication between an ESP32 camera and a Python host application, including custom packet framing and RGB565 image reconstruction.

The system is designed to serve as the acquisition layer for future computer-vision and machine-learning based seed quality assessment.