#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include "driver/rtc_io.h"
#include "RTClib.h"
#include "esp_sleep.h"

RTC_DS3231 rtc;
BluetoothSerial SerialBT;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define INTERRUPT_PIN 14  

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int hrButton = 25; 
const int minButton = 26; 
const int setButton = 27; 
const int resetButton = 33;
const int VibrationMotor = 2;
const int notifierButton = 20;
const int timerButton = 7;
int hours = 0;
int minutes = 0;
int whistle = 1;
bool timer_completed = false;

void modeSelection();
void handleButtonPresses();
void updateDisplay();
void bluetoothNotification();

void setup(){
  Serial.begin(115200);
  delay(1000);
  pinMode(notifierButton, INPUT_PULLUP);
  pinMode(timerButton, INPUT_PULLUP);
  pinMode(hrButton, INPUT_PULLUP);
  pinMode(minButton, INPUT_PULLUP);
  pinMode(setButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(VibrationMotor, OUTPUT);
  digitalWrite(VibrationMotor, LOW);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); 
  }

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Hello");
  display.display();

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    timer_completed = true;
    while(digitalRead(resetButton) != LOW){
      digitalWrite(VibrationMotor, HIGH);
      display.clearDisplay();
      display.display();
    }
    digitalWrite(VibrationMotor, LOW);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Hello");
    display.display();    
  } 
}

void loop(){
  modeSelection();
}

void handleButtonPresses() {
  if (digitalRead(hrButton) == LOW) {
    hours = (hours + 1) % 24;
    updateDisplay();
    delay(200); 
  }

  if (digitalRead(minButton) == LOW) {
    minutes = (minutes + 1) % 60;
    updateDisplay();
    delay(200);
  }

  if (digitalRead(resetButton) == LOW) {
    if(minutes == 0 && hours == 0){
      timer_completed = true;
      digitalWrite(VibrationMotor, LOW);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Hello");
      display.display();    
    } 
    
    else{
      hours = 0;
      minutes = 0;
      updateDisplay();
      delay(200); 
    }
}

  if (digitalRead(setButton) == LOW) {
    Serial.println("Set");
    display.clearDisplay();
    display.display();
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF);
   
    DateTime now = rtc.now();
    DateTime timerTime = now + TimeSpan(0, hours, minutes, 0);
    rtc.setAlarm1(timerTime, DS3231_A1_Minute);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);
    esp_deep_sleep_start();
  }
}

void updateDisplay() {
  display.clearDisplay();
  char timeString[6];
  snprintf(timeString, sizeof(timeString), "%02d:%02d", hours, minutes);
  display.setCursor(0, 0);
  display.print(timeString);
  display.display();
}

void bluetoothNotification(){
  SerialBT.begin("ESP32receiver");
  Serial.println("The device started, now you can pair it with Bluetooth!");
  while(digitalRead(resetButton) != LOW){
    while (SerialBT.available()) {
      char incomingChar = SerialBT.read();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(whistle/2);
      display.display();
      digitalWrite(VibrationMotor, HIGH);
      delay(2000);
      digitalWrite(VibrationMotor, LOW);
      display.clearDisplay();
      display.display();
      whistle += 1;
    }
  }

  digitalWrite(VibrationMotor, LOW);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Hello");
  display.display(); 
  delay(20);
}

void modeSelection(){
  if(digitalRead(timerButton) == LOW){
    timer_completed = false;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Timer");
    display.display();
    while(timer_completed != true){
      handleButtonPresses();
    }
    delay(200);
  }

  else if(digitalRead(notifierButton) == LOW){
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Notifier");
    display.display();
    bluetoothNotification();
    delay(200);
  }
}
