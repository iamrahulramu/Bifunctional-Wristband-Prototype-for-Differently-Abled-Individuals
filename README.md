# Bifunctional Wristband Prototype for Differently Abled Individuals

This bifunctional wristband prototype assists differently abled individuals by providing two key functionalities: a Vibrating Timer (alerts the user through vibrations once a set duration has elapsed) and a Cooking Status Notifier (alerts the user through vibrations when a cooker whistle is detected). The system uses two ESP32 Pico Kit boards - one acting as the main controller for the wristband and the other for the sound detector device.

---

## Table of Contents
- [Bifunctional Wristband Prototype for Differently Abled Individuals](#bifunctional-wristband-prototype-for-differently-abled-individuals)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
  - [Hardware Components](#hardware-components)
  - [Software Requirements](#software-requirements)
  - [Implementation Steps](#implementation-steps)
    - [On the Wristband ESP32 Board](#on-the-wristband-esp32-board)
    - [On the Detector ESP32 Board](#on-the-detector-esp32-board)
    - [Testing the Prototype](#testing-the-prototype)
  - [Implementation Description](#implementation-description)
    - [Wristband ESP32 Implementation](#wristband-esp32-implementation)
    - [Detector Device ESP32 Implementation](#detector-device-esp32-implementation)
  - [Future Improvements](#future-improvements)
  - [Acknowledgements](#acknowledgements)
  - [License](#license)

---

## Project Overview
This project presents a bifunctional wristband prototype aimed at supporting differently abled individuals, particularly those with hearing impairments, in their daily activities. The prototype can operate in two modes:

- **Timer Mode:** The user can set a desired time duration using the pushbuttons on the wristband. Once set, the ESP32 enters deep sleep mode to conserve energy. When the timer ends, the miniature vibration motor inside the wristband activates, alerting the user through vibrations.
- **Notifier Mode:** A second ESP32 device (detector) is placed near the cooker to detect whistle sounds using a sound detection sensor module. When a whistle is detected, a Bluetooth message is sent to the wristband, which activates the vibration motor and displays the cumulative number of whistles detected on the OLED screen.

---

## Hardware Components
This section lists the necessary hardware components used to develop this prototype, along with their functionalities.

| Component                                  | Function                                                                |
|--------------------------------------------|-------------------------------------------------------------------------|
| ESP32 Pico Kit (x2)                        | Main controller for both the wristband and the detector                 |
| OLED Display Module (SSD1306, 128x32, I2C) | To display necessary information to the user                            |
| RTC Module (DS3231)                        | To maintain accurate real-time tracking for timer operations            |
| Sound Detection Sensor Module              | To detect cooker whistle sounds                                         |
| Miniature Vibration Motor                  | To alert the user through vibrations                                    |
| Pushbuttons                                | To accept user inputs for selecting mode and setting the timer duration | 
| Breadboard, Jumper Wires, Power Supply     | For circuit connections and power supply                                |

---

## Software Requirements
To set up and run this project, ensure the following software tools and libraries are installed:
- **Arduino IDE:** Version 1.8x or later (or Arduino CLI)
- **Operating System:** Windows / macOS / Linux
- **Required Board Package:** ESP32 Board Package (for programming the ESP32 Pico Kit). It can be installed by following the steps below: 
  - Open ``Arduino IDE`` → ``Preferences``.
  - Add the following URL to the _Additional Board Manager URLs_ field:
    [https://dl.espressif.com/dl/package_esp32_index.json](https://dl.espressif.com/dl/package_esp32_index.json)
  - Then, open ``Tools`` → ``Board`` → ``Boards Manager``.
  - Search for ESP32 by _Espressif Systems_ and install it.
- **Required Arduino Libraries:** The following libraries are necessary to implement this prototype:
  - Adafruit GFX Library (for OLED graphics)
  - Adafruit SSD1306 (for OLED display control)
  - RTClib (for DS3231 real-time clock functionality)
  - BluetoothSerial (for Bluetooth communication between devices)
    
    You can install these libraries directly from the ``Arduino Library Manager``
    - Open ``Arduino IDE`` → ``Sketch`` → ``Include Library`` → ``Manage Libraries``.
    - Search for each library name listed above and install them.

    or install manually using Arduino CLI:

    ``` bash
        arduino-cli lib install "Adafruit GFX Library"
        arduino-cli lib install "Adafruit SSD1306"
        arduino-cli lib install "RTClib"
    ```
    (_**NOTE:** The ``BluetoothSerial`` library comes preinstalled with the ESP32 board package.)_

---

## Implementation Steps
In order to implement the prototype and verify its functionalities, follow the steps below:

### On the Wristband ESP32 Board  
1. Connect the peripherals - OLED display module, RTC module, miniature vibration motor and pushbuttons - to the ESP32 Pico Kit (used as the wristband controller) according to the pin configuration provided below:

    | ESP32 Pin                   | Connected Component           | Component Pin     |
    | --------------------------- | ----------------------------- | ----------------- |
    | 21                          | OLED Display Module           | SDA               |
    | 22                          |                               | SCL/SCK           |
    | 3V3                         |                               | VCC               |
    | GND                         |                               | GND               |
    | 21                          | RTC Module                    | SDA               |
    | 22                          |                               | SCL/SCK           |
    | 3V3                         |                               | VCC               |
    | GND                         |                               | GND               |
    | 14                          |                               | SQW               |
    | 2                           | Miniature Vibration Motor     | + / -             |
    | GND                         |                               | - / +             |
    | 27                          | Pushbutton (Set)              | Output            |
    | 33                          | Pushbutton (Reset)            | Output            |
    | 25                          | Pushbutton (Hour)             | Output            |
    | 26                          | Pushbutton (Minute)           | Output            |
    | 20                          | Pushbutton (Cook)             | Output            |    
    | 7                           | Pushbutton (Sleep)            | Output            |
    
2. Connect the ESP32 Pico Kit to your computer and select the appropriate board and ``COM`` port from the ``Tools`` menu in the  ``Arduino IDE``. 
3. Upload the wristband source code from [`Implementation/wristband.ino`](Implementation/wristband.ino) to the ESP32 board.

### On the Detector ESP32 Board  
1. Connect the sound detection sensor module to the ESP32 Pico Kit (used for the detector device) as described below:

    | ESP32 Pin                   | Connected Component           | Component Pin     |
    | --------------------------- | ----------------------------- | ----------------- |
    | 34                          | Sound Detection Sensor Module | OUT               |
    | 5V                          |                               | VCC               |
    | GND                         |                               | GND               |

2. Connect the ESP32 Pico Kit to your computer and select the appropriate board and ``COM`` port from the ``Tools`` menu in the ``Arduino IDE``. 
3. Upload the detector device source code from [`Implementation/detector.ino`](Implementation/detector.ino) to the ESP32 board.

### Testing the Prototype
Once both ESP32 boards are programmed and powered, the prototype can be tested as follows:
- **Initialization State:** When powered on, the wristband’s OLED display will show "Hello", indicating successful initialization.
- **Timer Mode (Vibrating Timer):** 
  - Press the *Timer* button (pin 7) to enter the timer mode. 
  - Set the desired duration using the *Hour* and *Minute* buttons (pins 25 and 26 respectively).
  - Press the *Set* button (pin 27) to activate the timer.
  - When the set duration elapses, the miniature vibration motor activates to alert the user.
  - Press the *Reset* button (pin 33) to reset the timer and return the wristband to its initialization state.
- **Notifier Mode (Cooking Status Notifier):**
  - Press the *Notifier* button (pin 20) to activate the cooking status notifier mode.
  - Power on the detector device and place it near the pressure cooker.
  - Every time a cooker whistle is detected, the detector transmits a Bluetooth signal to the wristband.
  - The vibration motor activates, and the OLED display shows the cumulative count of whistles detected.
  - Press the *Reset* button to return the wristband to its initialization state.  

---

## Implementation Description
This section provides a detailed description of the source code implemented on the ESP32 boards used for the wristband and the detector device. 

### Wristband ESP32 Implementation
**Script**: [Implementation/wristband.ino](Implementation/wristband.ino)

The wristband ESP32 code integrates three core functions:
- Displaying outputs/updates using the OLED display module.
- Setting timer using the DS3231 RTC module and pushbuttons.
- Setting up Bluetooth communication for receiving alerts from the detector device.

**Display Update and Reset**
```cpp
display.clearDisplay();
char timeString[6];
snprintf(timeString, sizeof(timeString), "%02d:%02d", hours, minutes);
display.setCursor(0, 0);
display.print(timeString);
display.display();
```
The OLED display continuously updates the displayed time as the user adjusts hours and minutes. The *Reset* button allows clearing or returning to the initialization state.

**Timer and Deep Sleep Setup**
```cpp
if (digitalRead(setButton) == LOW) {
  DateTime now = rtc.now();
  DateTime timerTime = now + TimeSpan(0, hours, minutes, 0);
  rtc.setAlarm1(timerTime, DS3231_A1_Minute);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);
  esp_deep_sleep_start();
}
```
In the *Timer* mode, when the *Set* button is pressed, the RTC module schedules a timer for the set duration. The ESP32 board then enters deep sleep mode to conserve power until the timer triggers, waking up through the interrupt pin (pin 14).

**Receiving Notifications via Bluetooth**
```cpp
void bluetoothNotification(){
  SerialBT.begin("ESP32receiver");
  while (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    display.clearDisplay();
    display.print(whistle/2);
    display.display();
    digitalWrite(VibrationMotor, HIGH);
    delay(2000);
    digitalWrite(VibrationMotor, LOW);
    whistle += 1;
  }
}
```
In the *Notifier* mode, the wristband waits for a Bluetooth message from the detector device. Upon receiving a signal, it vibrates and displays the cumulative number of whistles detected on the screen.

### Detector Device ESP32 Implementation
**Script**: [Implementation/detector.ino](Implementation/detector.ino)

This script continuously monitors sound levels using a sound detection sensor module and triggers a Bluetooth message when a whistle-like sound is detected.

**Bluetooth Initialization and Discovery**
```cpp
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

if (!SerialBT.begin("ESP32test", true)) {
  Serial.println("========== serialBT failed!");
  abort();
}
SerialBT.discoverAsync([](BTAdvertisedDevice *pDevice) {
  Serial.printf(">>>>>>>>>>>Found a new device asynchronously: %s\n", pDevice->toString().c_str());
});
```
The Bluetooth module runs in master mode, scanning for and automatically connecting to the wristband ESP32 board. The ``discoverAsync()`` function performs asynchronous device scanning, listing nearby ESP32 Bluetooth addresses.

**Sound Detection and Bluetooth Alerts Transmission**
```cpp
const int soundPin = 34;
const int threshold = 3000;
int soundLevel = analogRead(soundPin);
String sendData = "1\n";

if (soundLevel >= threshold) {
  count += 1;
  if (count == 3) {
    if (!SerialBT.isClosed() && SerialBT.connected()) {
        if (SerialBT.write((const uint8_t *)sendData.c_str(), sendData.length()) != sendData.length()) {
          Serial.println("tx: error");
        } else {
          Serial.printf("tx: %s", sendData.c_str());
        }
    }
  }
}
```
If the sound level detected by the sound detection sensor module exceeds the predefined threshold, it indicates the occurrence of a cooker whistle. The program confirms consistent sound readings (three consecutive detections) before sending the Bluetooth message. This reduces false triggers due to ambient noise.

---

## Future Improvements
While the current implementation successfully performs the intended functionalities, a few enhancements can further improve its usability and efficiency:
- **Enhanced Cooker Whistle Detection Logic:** In the current design, loudness is the primary parameter used to detect cooker whistles. More sophisticated techniques, such as frequency spectrum analysis, can be implemented to improve the accuracy and reliability of whistle detection.
- **Upgraded Wireless Communication:** Since the range and data throughput of Bluetooth are limited, replacing Bluetooth with Wi-Fi would enable longer transmission distances and more stable communication between the wristband and the detector device. 

---

## Acknowledgements
This project was developed between May 2024 and July 2024 as part of my internship at the Department of Electronics and Communication Engineering, SSN College of Engineering, Chennai, Tamil Nadu.

---

## License
This project is licensed under the terms specified in the [LICENSE](LICENSE) file **(MIT License)**.
