#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// WiFi credentials
const char* ssid = "BlackRockCity";
const char* password = "12345678";

// OSC
WiFiUDP Udp;
const unsigned int localPort = 8000;  // Local port to listen for OSC packets
const char* faderAddress1 = "/1/fader1";
const char* faderAddress2 = "/1/fader2";
const char* faderAddress3 = "/1/fader3";
const char* faderAddress4 = "/1/fader4";

// Relay pins
#define RELAY_PIN_1 21
#define RELAY_PIN_2 19
#define RELAY_PIN_3 18
#define RELAY_PIN_4 5
#define LED_PIN     25

float faderValue1 = 0.0;
float faderValue2 = 0.0;
float faderValue3 = 0.0;
float faderValue4 = 0.0;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;

int duration1 = 0;
int duration2 = 0;
int duration3 = 0;
int duration4 = 0;

bool relayState1 = false;
bool relayState2 = false;
bool relayState3 = false;
bool relayState4 = false;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  
  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  Udp.begin(localPort);
  Serial.print("Listening on UDP port ");
  Serial.println(localPort);
  
  // Set relay pins and LED pin as output
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);  // Ensure the relays are off initially
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(RELAY_PIN_3, LOW);
  digitalWrite(RELAY_PIN_4, LOW);
  digitalWrite(LED_PIN, LOW);      // Ensure the LED is off initially
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    Serial.print("Received packet size: ");
    Serial.println(packetSize);

    OSCMessage msg;
    while (packetSize--) {
      msg.fill(Udp.read());
    }

    if (!msg.hasError()) {
      Serial.println("OSC message received:");
      msg.route(faderAddress1, updateFaderValue1);
      msg.route(faderAddress2, updateFaderValue2);
      msg.route(faderAddress3, updateFaderValue3);
      msg.route(faderAddress4, updateFaderValue4);
    } else {
      Serial.print("OSC message error: ");
      Serial.println(msg.getError());
    }
  }
  
  // Adjust each relay based on its fader value using non-blocking timing
  controlRelay(RELAY_PIN_1, faderValue1, previousMillis1, duration1, relayState1);
  controlRelay(RELAY_PIN_2, faderValue2, previousMillis2, duration2, relayState2);
  controlRelay(RELAY_PIN_3, faderValue3, previousMillis3, duration3, relayState3);
  controlRelay(RELAY_PIN_4, faderValue4, previousMillis4, duration4, relayState4);
}

void controlRelay(int relayPin, float faderValue, unsigned long &previousMillis, int &duration, bool &relayState) {
  if (faderValue >= 0.3 && faderValue < 0.4) {
    duration = 300;
  } else if (faderValue >= 0.4 && faderValue < 0.5) {
    duration = 200;
  } else if (faderValue >= 0.5 && faderValue < 0.6) {
    duration = 100;
  } else if (faderValue >= 0.6 && faderValue < 0.7) {
    duration = 50;
  } else if (faderValue >= 0.7 && faderValue < 0.8) {
    duration = 40;
  } else if (faderValue >= 0.8) {
    duration = 10;
  } else {
    digitalWrite(relayPin, LOW); // Ensure the relay is off if the fader is below 0.3
    return;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= duration) {
    previousMillis = currentMillis;
    relayState = !relayState;
    digitalWrite(relayPin, relayState ? HIGH : LOW);
    Serial.print("Relay ");
    Serial.print(relayPin);
    Serial.print(" ON duration: ");
    Serial.println(duration);
  }
}

void updateFaderValue1(OSCMessage &msg, int addrOffset) {
  faderValue1 = msg.getFloat(0);  // Get float value from OSC message
  Serial.print("Updated fader value 1: ");
  Serial.println(faderValue1);
}

void updateFaderValue2(OSCMessage &msg, int addrOffset) {
  faderValue2 = msg.getFloat(0);  // Get float value from OSC message
  Serial.print("Updated fader value 2: ");
  Serial.println(faderValue2);
}

void updateFaderValue3(OSCMessage &msg, int addrOffset) {
  faderValue3 = msg.getFloat(0);  // Get float value from OSC message
  Serial.print("Updated fader value 3: ");
  Serial.println(faderValue3);
}

void updateFaderValue4(OSCMessage &msg, int addrOffset) {
  faderValue4 = msg.getFloat(0);  // Get float value from OSC message
  Serial.print("Updated fader value 4: ");
  Serial.println(faderValue4);
}