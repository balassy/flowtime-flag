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

int flowTime1BeginMinutesOfDay;
int flowTime1EndMinutesOfDay;
int flowTime2BeginMinutesOfDay;
int flowTime2EndMinutesOfDay;
bool isNowFlowTimeLast;

void setup() {
  initSerial();

  initServo();
  servo.moveSlowTo(FLOWTIME_END_POSITION + 30);

  initNetwork();
  initTimeServerConnection();

  flowTime1BeginMinutesOfDay = getMinutesOfDay(FLOWTIME1_BEGIN_HOUR, FLOWTIME1_BEGIN_MINUTE);
  flowTime1EndMinutesOfDay = getMinutesOfDay(FLOWTIME1_END_HOUR, FLOWTIME1_END_MINUTE);
  flowTime2BeginMinutesOfDay = getMinutesOfDay(FLOWTIME2_BEGIN_HOUR, FLOWTIME2_BEGIN_MINUTE);
  flowTime2EndMinutesOfDay = getMinutesOfDay(FLOWTIME2_END_HOUR, FLOWTIME2_END_MINUTE);
  isNowFlowTimeLast = false;

  Serial.printf("Configured flow times: %d:%d (%d) - %d:%d (%d) and %d:%d (%d) - %d:%d (%d).\n", FLOWTIME1_BEGIN_HOUR, FLOWTIME1_BEGIN_MINUTE, flowTime1BeginMinutesOfDay, FLOWTIME1_END_HOUR, FLOWTIME1_END_MINUTE, flowTime1EndMinutesOfDay, FLOWTIME2_BEGIN_HOUR, FLOWTIME2_BEGIN_MINUTE, flowTime2BeginMinutesOfDay, FLOWTIME2_END_HOUR, FLOWTIME2_END_MINUTE, flowTime2EndMinutesOfDay);
  Serial.println(F("Setup completed."));

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
  servo.moveSlowTo(WAVE_START_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_END_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_START_POSITION);
  delay(500);
  servo.moveSlowTo(WAVE_END_POSITION);
  delay(500);
  servo.moveSlowTo(FLOWTIME_START_POSITION);
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
    delay(200);  // Adding this delay make the function work 100% reliable on the subsequent try.
    return getCurrentTime();
  }
}

bool isFlowTime(strDateTime currentTime) {
  byte currentHour = currentTime.hour;
  byte currentMinute = currentTime.minute;
  int currentMinutesOfDay = getMinutesOfDay(currentHour, currentMinute);

  Serial.printf("Current time: %d:%d (%d)\n", currentHour, currentMinute, currentMinutesOfDay);

  bool isFlowTime1 = flowTime1BeginMinutesOfDay <= currentMinutesOfDay && currentMinutesOfDay <= flowTime1EndMinutesOfDay;
  bool isFlowTime2 = flowTime2BeginMinutesOfDay <= currentMinutesOfDay && currentMinutesOfDay <= flowTime2EndMinutesOfDay;

  return isFlowTime1 || isFlowTime2;
}

int getMinutesOfDay(byte hours, byte minutes) {
  return 60 * hours + minutes;
}
