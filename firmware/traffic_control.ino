/*
 * Smart Traffic Control Management System
 * ESP8266 (NodeMCU) firmware
 *
 * RECONSTRUCTED from photos/screenshots of the original project's Arduino IDE
 * (see cs_project_code PDF / PPT). This is NOT the original file — it is a
 * best-effort transcription. Sections that were not visible in the source
 * material are marked with "UNKNOWN" / "INFERRED" comments below. Do not
 * present this as the original author's exact file.
 *
 * Confirmed from screenshots:
 *  - LiquidCrystal_I2C 16x2 LCD on address 0x27, SDA=D2, SCL=D1
 *  - FirebaseESP8266 library, polling /ambulances/ambulance1/currentLocation
 *  - Haversine distance check against a fixed junction lat/lon
 *  - IR sensors on D0, D4, D6 -> variable green-light duration per lane
 *  - LEDs: red1=D5, green1=D7, red2=D8, green2=D3
 */

// INFERRED: not visible in any screenshot, but required for WiFi.begin() to
// compile on ESP8266. The screenshot only showed "#include <dummy.h>" which
// is an Arduino IDE placeholder header, not a real include.
#include <ESP8266WiFi.h>

#include <dummy.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup (address 0x27, adjust if 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Firebase Configuration
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "https://ambulance-gps-tracker-e7072-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "your_firebase_database_secret" // Get from Firebase Console

// Junction Coordinates
#define JUNCTION_LAT 12.9552
#define JUNCTION_LON 77.5744

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool ambulanceDetected = false;
int ambulanceLane = 1;

// LED Pins
const int red1 = D5;   // Lane 1 Red
const int green1 = D7; // Lane 1 Green
const int red2 = D8;   // Lane 2 Red (10kΩ pull-down to GND)
const int green2 = D3; // Lane 2 Green

// IR Sensor Pins
const int ir1 = D0; // Lane 1
const int ir2 = D4; // Lane 2 Sensor 1
const int ir3 = D6; // Lane 2 Sensor 2

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  Wire.begin(D2, D1); // SDA=D2, SCL=D1
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Traffic Control"));
  delay(2000);
  lcd.clear();

  pinMode(red1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  lcd.setCursor(0, 0);
  lcd.print(F("WiFi Connected"));
  delay(1000);
  lcd.clear();

  // Firebase setup
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
}

void loop() {
  checkAmbulanceDistance();

  if (ambulanceDetected) {
    lcd.setCursor(0, 0);
    lcd.print(F("Emerg Lane "));
    lcd.print(ambulanceLane);
    runGreen("Lane " + String(ambulanceLane),
              ambulanceLane == 1 ? green1 : green2,
              ambulanceLane == 1 ? red1 : red2,
              ambulanceLane == 1 ? red2 : red1,
              ambulanceLane == 1 ? green2 : green1, 10);
    ambulanceDetected = false;
    lcd.clear();
    return;
  }

  // Read IR sensors (LOW = vehicle detected, per original wiring)
  bool ir1State = digitalRead(ir1) == LOW;
  bool ir2State = digitalRead(ir2) == LOW;
  bool ir3State = digitalRead(ir3) == LOW;

  // Case 1: IR1 HIGH(detected), IR2 LOW, IR3 LOW -> Lane 1 for 5s
  if (ir1State && !ir2State && !ir3State) {
    runGreen("Lane 1", green1, red1, red2, green2, 5);
  }
  // Case 2: IR1 + IR2 detected, IR3 not -> Lane 1 for 5s, Lane 2 for 5s
  else if (ir1State && ir2State && !ir3State) {
    runGreen("Lane 1", green1, red1, red2, green2, 5);
    runGreen("Lane 2", green2, red2, red1, green1, 5);
  }
  // Case 3: all three detected -> Lane 1 for 5s, Lane 2 for 10s (higher density)
  else if (ir1State && ir2State && ir3State) {
    runGreen("Lane 1", green1, red1, red2, green2, 5);
    runGreen("Lane 2", green2, red2, red1, green1, 10);
  }
  // Case 4: IR1 not detected, IR2 + IR3 detected -> Lane 2 for 10s
  else if (!ir1State && ir2State && ir3State) {
    runGreen("Lane 2", green2, red2, red1, green1, 10);
  }
  // Case 5: nothing detected -> default split, 5s / 5s
  else if (!ir1State && !ir2State && !ir3State) {
    runGreen("Lane 1", green1, red1, red2, green2, 5);
    runGreen("Lane 2", green2, red2, red1, green1, 5);
  }
  // Case 6: only IR2 detected -> Lane 2 for 5s
  else if (!ir1State && ir2State && !ir3State) {
    runGreen("Lane 2", green2, red2, red1, green1, 5);
  }
  // UNKNOWN: the source screenshot for this if/else-if chain was cut off
  // after this point. Remaining permutations of ir1/ir2/ir3 (e.g. ir1 &&
  // !ir2 && ir3, or !ir1 && !ir2 && ir3) were not visible and are not
  // reconstructed here. Add an else{} fallback if you recall the intended
  // default behavior.
}

// Runs the green phase for one lane, holding the opposite lane red, then
// restores both to red at the end of the duration.
void runGreen(String laneName, int greenPin, int redPin, int redOpp, int greenOpp, int duration) {
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  digitalWrite(redOpp, HIGH);
  digitalWrite(greenOpp, LOW);

  for (int i = duration; i > 0; i--) {
    lcd.setCursor(0, 0);
    lcd.print(laneName + ": Green   ");
    lcd.setCursor(0, 1);
    lcd.print(F("Time left: "));
    lcd.print(i);
    lcd.print(F("s   "));
    delay(1000);
  }

  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);
  digitalWrite(redOpp, HIGH);
  digitalWrite(greenOpp, LOW);
  lcd.clear();
}

// Polls Firebase for the ambulance's latest lat/lon, computes distance to
// the fixed junction coordinates via haversine, and flags an override when
// within 200 m.
void checkAmbulanceDistance() {
  if (Firebase.getDouble(fbdo, "/ambulances/ambulance1/currentLocation/latitude")) {
    float lat = fbdo.doubleData();
    if (Firebase.getDouble(fbdo, "/ambulances/ambulance1/currentLocation/longitude")) {
      float lon = fbdo.doubleData();
      float distance = haversine(lat, lon, JUNCTION_LAT, JUNCTION_LON);
      Serial.printf("Distance: %.2f km\n", distance);
      if (distance < 0.2) { // 200 meters
        ambulanceDetected = true;
        String direction = getDirection(lat, lon, JUNCTION_LAT, JUNCTION_LON);
        Serial.println("Direction: " + direction);
        ambulanceLane = (direction == "North" || direction == "East") ? 1 : 2;
      }
    } else {
      Serial.println("Failed to read longitude: " + fbdo.errorReason());
    }
  } else {
    Serial.println("Failed to read latitude: " + fbdo.errorReason());
  }
}

// Coarse compass-direction heuristic between ambulance and junction.
String getDirection(float ambLat, float ambLon, float junLat, float junLon) {
  float dLat = ambLat - junLat;
  float dLon = ambLon - junLon;
  if (abs(dLat) > abs(dLon)) {
    return dLat > 0 ? "North" : "South";
  }
  return dLon > 0 ? "East" : "West";
}

// Great-circle distance between two lat/lon points, in kilometers.
float haversine(float lat1, float lon1, float lat2, float lon2) {
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return 6371 * c; // Distance in km
}
