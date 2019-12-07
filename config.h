#ifndef LED_BUILTIN
#define LED_BUILTIN D0
#endif

// Network configuration.
const char* WIFI_AP_SSID = "FlowTimeFlag";  // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "FlowTime!"; // The password required to connect to the wireless network used to configure the network parameters.

// Time server configuration.
const char* NTP_SERVER_HOST = "hu.pool.ntp.org";
const float TIME_ZONE = 1.0;  // Time zone in floating point (e.g. for India).
const int DST_ADJUSTMENT = 1; // 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment.

// Define this constant to log debug messages from the NTPTimeESP library.
// #define DEBUG_ON

// Time period configuration.
const byte FLOWTIME_BEGIN_HOUR = 13;
const byte FLOWTIME_BEGIN_MINUTE = 15;
const byte FLOWTIME_END_HOUR = 14;
const byte FLOWTIME_END_MINUTE = 30;

// Pin configuration.
const uint8_t PIN_SERVO = D7;

// Positions.
const int FLOWTIME_START_POSITION = 100;
const int FLOWTIME_END_POSITION = 10;
const int WAVE_START_POSITION = 130;
const int WAVE_END_POSITION = 70;

