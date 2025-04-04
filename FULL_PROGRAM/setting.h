#include <WiFi.h>
#include <time.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "DHTesp.h"
#include <SPI.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5
#define DHT_PIN 4 
#define MQ7_PIN 35 

const char* ssid = "RoboCode_2.4G";
const char* password = "RoboCode2020";

const char* apiUrl = "https://api.alerts.in.ua/v1/iot/active_air_raid_alerts/15.json";
const char* token = "8e9ca4f81345b2386e1af7b93dd5987c31a3e115ab2203";

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

struct tm timeInfo;

DHTesp dhtSensor;

static const uint8_t PIN_MP3_TX = 26; 
static const uint8_t PIN_MP3_RX = 27; 
HardwareSerial mySerial(1);

DFRobotDFPlayerMini player;
bool isPlaying = false;
int playQueue[10];
int queueSize = 0;

bool DHTMessage = false;   

bool MQ7Message = false;    

const long gmtOffset_sec = 2 * 3600;
const int daylightOffset_sec = 3600;

int alertStatus = -1;  
bool showingAlert = false;  
unsigned long alertStartTime = 0;  

unsigned long lastMessageTime = 0;  
const unsigned long messageInterval = 120000; 
bool message = false;  
