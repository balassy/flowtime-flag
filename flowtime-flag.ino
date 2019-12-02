// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <ESP8266WiFi.h> // Required dependency for the NTPtimeESP library.

// Third-party libraries.
#include <WiFiManager.h>  // To manage network configuration and connection.
#include "NTPtimeESP.h"   // To get the current time through NTP.

// My classes.
#include "speed-servo.h"

// Read configuration and secrets.
#include "config.h"

WiFiManager wifiManager;
NTPtime ntpClient;
SpeedServo servo;

int flowTimeBeginMinutesOfDay;
int flowTimeEndMinutesOfDay;
bool isNowFlowTimeLast;

void setup() {
  initSerial();

  initServo();
  servo.moveSlowTo(FLOWTIME_END_POSITION + 30);

  initNetwork();
  servo.moveSlowTo(FLOWTIME_END_POSITION + 60);

  initTimeServerConnection();
  servo.moveSlowTo(FLOWTIME_END_POSITION + 90);

  flowTimeBeginMinutesOfDay = getMinutesOfDay(FLOWTIME_BEGIN_HOUR, FLOWTIME_BEGIN_MINUTE);
  flowTimeEndMinutesOfDay = getMinutesOfDay(FLOWTIME_END_HOUR, FLOWTIME_END_MINUTE);
  isNowFlowTimeLast = false;

  Serial.printf("Configured flow time: %d:%d (%d) - %d:%d (%d).\n", FLOWTIME_BEGIN_HOUR, FLOWTIME_BEGIN_MINUTE, flowTimeBeginMinutesOfDay, FLOWTIME_END_HOUR, FLOWTIME_END_MINUTE, flowTimeEndMinutesOfDay);
  Serial.println(F("Setup completed."));

  wave();
  delay(500);
  servo.moveSlowTo(FLOWTIME_END_POSITION);
}

void initSerial() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Initializing serial connection DONE."));
}

void initServo() {
  Serial.print(F("Initializing the servo..."));
  servo.attach(PIN_SERVO);
  servo.moveSlowTo(FLOWTIME_END_POSITION);
  Serial.println(F("DONE."));
}

void initNetwork() {
  Serial.printf("Initializing connection to the network with MAC address %s using WiFiManager (SSID: %s)...\n", WiFi.macAddress().c_str(), WIFI_AP_SSID);
  wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("WiFi connected. IP address: %s, MAC address: %s\n", WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
}

void initTimeServerConnection() {
  Serial.printf("Initializing time server connection to %s...\n", NTP_SERVER_HOST);
  ntpClient.init(NTP_SERVER_HOST);
}

void wave() {
  servo.moveSlowTo(WAVE_START_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_END_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_START_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_END_POSITION);
}

void loop() {
  strDateTime currentTime = getCurrentTime();
  bool isNowFlowTime = isFlowTime(currentTime);
  if (isNowFlowTime != isNowFlowTimeLast) {
    if (isNowFlowTime) {
      beginFlowTime();
    }
    else {
      endFlowTime();
    }
  }
  isNowFlowTimeLast = isNowFlowTime;

  delay(60000);
}

void beginFlowTime() {
  Serial.println(F("Starting flow time..."));
  wave();
  servo.moveSlowTo(FLOWTIME_START_POSITION);
  delay(500);
}

void endFlowTime() {
  Serial.println(F("Ending flow time..."));
  servo.moveSlowTo(FLOWTIME_END_POSITION);
}

strDateTime getCurrentTime() {
  Serial.print(F("Getting current time..."));

  ntpClient.setRecvTimeout(59);
  strDateTime currentTime = ntpClient.getNTPtime(TIME_ZONE, DST_ADJUSTMENT);

  if(currentTime.valid){
    ntpClient.printDateTime(currentTime);
    return currentTime;
  } else {
    Serial.println(F("Getting current time failed, waiting and trying again..."));
    delay(100);  // Adding this delay make the function work 100% reliable on the subsequent try.
    return getCurrentTime();
  }
}

bool isFlowTime(strDateTime currentTime) {
  byte currentHour = currentTime.hour;
  byte currentMinute = currentTime.minute;
  int currentMinutesOfDay = getMinutesOfDay(currentHour, currentMinute);

  Serial.printf("Current time: %d:%d (%d)\n", currentHour, currentMinute, currentMinutesOfDay);
  return flowTimeBeginMinutesOfDay <= currentMinutesOfDay && currentMinutesOfDay <= flowTimeEndMinutesOfDay;
}

int getMinutesOfDay(byte hours, byte minutes) {
  return 60 * hours + minutes;
}
