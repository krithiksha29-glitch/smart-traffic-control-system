# 🚦 Smart Traffic Control Management System

A two-lane smart traffic control system that manages traffic signals based on vehicle density and provides priority to ambulances using smartphone GPS and Firebase.

---

##  Project Details

- **Project Type:** Academic Project
- **Semester:** 4th Semester
- **Academic Year:** 2024–2025
  
---

##  Project Overview

The Smart Traffic Control Management System is designed to improve traffic signal management at a two-lane junction.

The system uses IR sensors to detect vehicle presence and adjust traffic signal timing based on traffic density. It also provides priority to ambulances by using smartphone GPS location, Firebase Realtime Database, and an ESP8266 controller.

> **Note:** This repository is a reconstruction of the project's source code based on the original project report, presentation, code screenshots, and project documentation. The original source code was not available to all team members. Parts that could not be confirmed from the available material are marked as `UNKNOWN`.

---

##  Main Features

###  Traffic Density-Based Signal Control

- Uses IR sensors to detect vehicles in two lanes.
- The ESP8266 reads the sensor inputs.
- Traffic density is evaluated based on the sensor states.
- The lane with higher detected traffic is given a longer green-light duration.
- Traffic signal status and countdown are displayed on a 16x2 I2C LCD.

###  Ambulance Prioritization

- A smartphone is used to share the ambulance's live GPS location.
- The location is sent to Firebase Realtime Database.
- The ESP8266 retrieves the ambulance location from Firebase.
- The system calculates the distance between the ambulance and the junction.
- When the ambulance is within the defined proximity, priority is given to the appropriate lane.
- The selected lane is given a green signal to allow the ambulance to pass.

---

##  System Working

### Part A — Traffic Density Control

IR sensors detect the presence of vehicles in the lanes.

The ESP8266 processes the sensor inputs and determines the traffic condition. Based on the detected traffic density, the corresponding traffic signal timing is controlled.

**Basic Flow:**

Vehicle Detection → IR Sensors → ESP8266 → Density Evaluation → Traffic Signal Control → LCD Display

### Part B — Ambulance Priority

The ambulance-side web page obtains the smartphone's GPS location using the browser's geolocation feature.

The location is sent to Firebase Realtime Database. The ESP8266 retrieves the ambulance location and checks its distance from the junction.

If the ambulance is within the defined range, the system provides priority to the appropriate lane.

**Basic Flow:**

Smartphone GPS → Web Page → Firebase → ESP8266 → Location/Distance Check → Ambulance Priority → Green Signal

---

##  Hardware Used

| Component | Purpose |
|---|---|
| ESP8266 (NodeMCU) | Main controller |
| IR Sensors | Vehicle detection |
| Red LEDs | Traffic signal indication |
| Green LEDs | Traffic signal indication |
| 16x2 I2C LCD | Traffic status and countdown display |
| Smartphone | Ambulance GPS location source |

### Pin Configuration

| Component | ESP8266 Pin |
|---|---|
| Lane 1 IR Sensor | D0 |
| Lane 2 IR Sensor 1 | D4 |
| Lane 2 IR Sensor 2 | D6 |
| Red LED 1 | D5 |
| Green LED 1 | D7 |
| Red LED 2 | D8 |
| Green LED 2 | D3 |
| LCD SDA | D2 |
| LCD SCL | D1 |
| LCD I2C Address | 0x27 |

---

##  Software and Technologies

- Arduino IDE
- ESP8266 / NodeMCU
- C/C++ (Arduino)
- HTML
- JavaScript
- Firebase Realtime Database
- Google Maps JavaScript API
- Browser Geolocation API

---

## 📁 Project Structure

```text
smart-traffic-control-system/
│
├── firmware/
│   └── traffic_control.ino
│
├── web/
│   ├── index.html
│   ├── script.js
│   └── style.css
│
└── README.md
```

---

## 📂 File Description

### `firmware/traffic_control.ino`

ESP8266 firmware responsible for:

- Reading IR sensor inputs
- Controlling traffic signal LEDs
- Displaying information on the LCD
- Connecting to Wi-Fi
- Communicating with Firebase
- Checking ambulance location
- Handling ambulance priority

### `web/index.html`

Web page used for the ambulance-side interface and GPS location sharing.

### `web/script.js`

JavaScript responsible for:

- Obtaining smartphone GPS coordinates
- Sending location data to Firebase
- Updating the ambulance location on the map

### `web/style.css`

A stylesheet file included in the project structure. Its original contents could not be confirmed from the available project material.

---

##  Setup

### ESP8266 Firmware

1. Open `firmware/traffic_control.ino` in Arduino IDE.
2. Select the ESP8266 / NodeMCU board.
3. Install the required libraries used by the project.
4. Enter your own Wi-Fi credentials.
5. Configure your Firebase connection details.
6. Set the junction coordinates used by the project.
7. Upload the firmware to the ESP8266.

### Web Application

1. Open the files inside the `web` folder.
2. Configure your own Firebase web configuration.
3. Configure your own Google Maps API key if required.
4. Host the web page using a suitable web-hosting service.
5. Open the page on the smartphone used for ambulance location sharing.
6. Allow location access when requested.

> **Security:** Do not upload Wi-Fi passwords, Firebase database secrets, or other private credentials to a public GitHub repository. Use placeholders or private configuration for sensitive information.

---

##  Working Principle

The system works using two main control mechanisms.

**Traffic Density Control:**

IR sensors detect vehicles and provide inputs to the ESP8266. Based on the sensor states, the controller determines the traffic condition and controls the traffic signal LEDs accordingly.

**Ambulance Priority:**

The smartphone provides the ambulance's GPS coordinates. Firebase is used to store and retrieve the location data. The ESP8266 checks the ambulance's location relative to the junction and provides signal priority when the ambulance approaches the junction.

---

##  Project Workflow

1. IR sensors detect vehicles.
2. ESP8266 receives sensor inputs.
3. Traffic density is evaluated.
4. Traffic signal timing is controlled.
5. LCD displays the current signal information.
6. Ambulance smartphone shares GPS location.
7. GPS data is stored in Firebase.
8. ESP8266 retrieves the ambulance location.
9. Ambulance proximity to the junction is checked.
10. Priority is provided to the required lane.

---

##  Known Limitations

- The original source code was not available and therefore this repository is a reconstruction.
- Some portions of the original firmware were not completely visible in the available code screenshots.
- The exact contents of `style.css` could not be recovered from the available project material.
- Original credentials and passwords are not included in this repository.
- The exact implementation of portions that were not visible in the screenshots cannot be guaranteed.

---

##  Future Enhancements

- Improve traffic density detection.
- Increase the accuracy of ambulance location detection.
- Improve signal timing optimization.
- Add more traffic lanes and junctions.
- Add additional traffic monitoring features.
- Improve the web-based monitoring interface.

---

##  Team

- KRITHIKSHA S
- Ambuj Mishra
- Jeevan R
- Manasa S K

**Department:** Electronics & Telecommunication Engineering  
**Institution:** Bangalore Institute of Technology

---

##  Documentation

The project was developed and documented using the team's project report, presentation, code documentation, and physical prototype.

---

## Project Status

**Academic Project | Completed**

**Electronics & Telecommunication Engineering**
