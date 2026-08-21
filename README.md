# Smart Traffic Control Management System

Control Systems (BEC403) project — Bangalore Institute of Technology.
Two-lane, IR-sensor-based traffic signal control with ambulance
prioritization via smartphone GPS + Firebase, on an ESP8266 (NodeMCU).

> **This repo is a reconstruction, not the original source.**
> The original code lived with a teammate and wasn't available. Everything
> here was transcribed from screenshots in the team's own project PDFs/PPT
> (report, code screenshots, and photos of the physical build). Where the
> screenshots didn't show something, it's marked `UNKNOWN` in the code and
> summarized below — nothing was invented to "fill gaps" beyond what was
> strictly required to compile (see Known Gaps).

## What it does

- **Part A — Density-based signal control:** Three IR sensors (Lane 1: 1
  sensor, Lane 2: 2 sensors) report vehicle presence to the ESP8266, which
  extends the green-light duration for whichever lane looks busier (5s vs
  10s), shown live on a 16x2 I2C LCD.
- **Part B — Ambulance prioritization:** An ambulance-side web page
  (`web/index.html` + `script.js`) reads the phone's GPS via
  `navigator.geolocation` and pushes lat/lon to Firebase Realtime Database.
  The ESP8266 polls Firebase, computes haversine distance to a fixed
  junction coordinate, and if the ambulance is within ~200m, forces that
  lane's light green for 10s regardless of IR sensor state.

## Hardware (as documented in the project report)

| Component | Role |
|---|---|
| ESP8266 (NodeMCU) | Main controller — WiFi, Firebase, LCD, LED, IR sensor I/O |
| 3x IR obstacle sensors | Lane 1: D0. Lane 2: D4 and D6 |
| 4x LEDs (2 per lane: red/green) | red1=D5, green1=D7, red2=D8, green2=D3 |
| 16x2 LCD, I2C backpack (addr 0x27) | SDA=D2, SCL=D1 — shows active lane + countdown |
| Smartphone | Simulates ambulance GPS via a browser page |

## Repo layout

```
firmware/
  traffic_control.ino   # ESP8266 sketch — IR logic, LED/LCD control, Firebase polling, haversine
web/
  index.html             # Ambulance-side "Start Sharing Location" page
  script.js               # Geolocation -> Firebase, Firebase -> Google Map marker
  style.css                # Empty — never appeared in any source screenshot (see Known Gaps)
```

## Setup

1. **Firmware:** Open `firmware/traffic_control.ino` in Arduino IDE with the
   ESP8266 board package installed. Install libraries: `FirebaseESP8266`,
   `LiquidCrystal_I2C`. `FIREBASE_HOST` is already set to your real database
   URL. `FIREBASE_AUTH` still needs your **Firebase Realtime Database
   secret** — this is a different value from the "Web API Key" shown in
   Project Settings; find it under Firebase Console → Project Settings →
   Service Accounts → Database Secrets (legacy). Fill in your own
   `WIFI_SSID` / `WIFI_PASSWORD` too.
2. **Web page:** `web/script.js` and `web/index.html` now contain the
   project's real `firebaseConfig` and Google Maps API key (confirmed
   directly from the Firebase console / Google Cloud console, not just
   guessed from a slide screenshot). Host statically (Firebase Hosting,
   GitHub Pages, anything) or open locally for testing (geolocation may
   require HTTPS on real devices).
   ⚠️ **Before pushing to a public GitHub repo:** the Google Maps key is
   currently *unrestricted* (per the Google Cloud console screenshot) —
   anyone who finds it in your repo can rack up usage on your billing
   account. Go to Google Cloud Console → APIs & Services → Credentials →
   your key → restrict it to `HTTP referrers` (your domain) and to the
   Maps JavaScript API only, before making the repo public. Same caution
   applies to the Firebase Web API key, though Firebase's own database
   rules matter more there than the key itself — make sure your Realtime
   Database rules aren't wide open (`".read": true, ".write": true`) for a
   public repo.
3. Set `JUNCTION_LAT` / `JUNCTION_LON` in the firmware to your actual
   intersection's coordinates.

## Known gaps (things the screenshots didn't show)

- **`web/style.css`** — `index.html` links to it, but its actual contents
  were never captured in any screenshot; only an inline `<style>` block was
  visible (and that's already in `index.html`). File is left empty here.
- **`favicon.ico`** — referenced but never shown; not recoverable from a
  screenshot.
- **Rest of the IR sensor `if`/`else if` chain** — the loop() logic
  screenshot is cut off after 6 of the possible sensor-state combinations.
  Marked in the `.ino` file with a comment at the point it was cut off.
- **`#include <ESP8266WiFi.h>`** — never literally visible (only the
  placeholder `<dummy.h>` was shown), but added because `WiFi.begin()`
  won't compile without it. Flagged inline as inferred, not photographed.
- **`map.setCenter(...)` in `updateMapMarker()`** — script.js screenshot
  cuts off right after `marker.setPosition(newPos)`; whether the map also
  recentered isn't confirmed, so nothing was added there.
- **All API keys / secrets** in the screenshots (Firebase apiKey, Google
  Maps key, Firebase database secret) are treated as compromised/rotated
  since they were shown on a presentation slide, and replaced with
  placeholders here on purpose — don't reuse the ones visible in your PDFs.

## Attribution

Team: Ambuj Mishra, Krithiksha S, Jeevan R, Manasa S K — BEC403 Control
Systems, Dept. of ETE, BIT, under Dr. Girish Kumar N G. Reconstructed from
the team's own submitted report/PPT/code-screenshots after the original
source file became unavailable.
