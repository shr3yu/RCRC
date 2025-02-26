#include "ESP32_NOW.h"
#include "WiFi.h"

#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
#include <Wire.h>
#include <math.h>

const int MPU = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int AcXcal, AcYcal, AcZcal, GyXcal, GyYcal, GyZcal, tcal;
double t, tx, tf, pitch, roll;
String direction;

/* Definitions */

#define ESPNOW_WIFI_CHANNEL 6

/* Classes */

// Creating a new class that inherits from the ESP_NOW_Peer class is required.
class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer { //inherits from ESP now peer to be a broadcast type
public:
  // Constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {} //creating a broadcast peer on channel
  // LMK: local master key: enable(16byte) or disable encryption; reciever must have same LMK to decrypt and read message
  // Station mode: 
  // Destructor of the class
  ~ESP_NOW_Broadcast_Peer() {
    remove();
  }

  // Function to properly initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

/* Global Variables */

uint32_t msg_count = 0;

// Create a broadcast peer object
ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

/* Main */

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Register the broadcast peer
  if (!broadcast_peer.begin()) {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Reebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }
  //for remote
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

  Serial.println("Setup complete. Broadcasting messages every 5 seconds.");
}

String previousstate= "STILL";

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  // Call getAngle() and getDirection() before sending direction
  getAngle(AcX, AcY, AcZ);
  getDirection();  // <-- Add this before broadcasting!
  if (previousstate == direction){
    return;
  }
  // Broadcast a message to all devices within the network
  char data[32];
  snprintf(data, sizeof(data), "%s", direction);

  Serial.printf("Broadcasting message: %s\n", data);

  if (!broadcast_peer.send_message((uint8_t *)data, sizeof(data))) {
    Serial.println("Failed to broadcast message");
  }

  previousstate = direction;

  delay(200);
}

void getAngle(int Ax, int Ay, int Az) {
 double x = Ax;
 double y = Ay;
 double z = Az;
 pitch = atan(x / sqrt((y * y) + (z * z)));
 roll = atan(y / sqrt((x * x) + (z * z)));
 pitch = pitch * (180.0 / 3.14);
 roll = roll * (180.0 / 3.14) ;
}

void getDirection(){
  if (pitch > -7 && pitch < 7 && roll < 5 && roll > -5) {
    direction = "STILL";
  } 
  else if (pitch >= 25) { // No need to check roll again, previous condition ensures roll is within range
    direction = "FRONT";
  } 
  else if (pitch <= -25) {
    direction = "BACK";
  } 
  else if (roll <= -25) {
    direction = "RIGHT";
  } 
  else if (roll >= 25) {
    direction = "LEFT";
  } 
  else {
    direction = "STILL"; // Instead of NULL, assume it's still
  }
}