/*
 * RECONSTRUCTED from screenshots of the original project's script.js
 * (cs_project_code PDF). Not the original file — best-effort transcription.
 *
 * UNKNOWN: the Firebase apiKey shown in the screenshot is almost certainly
 * rotated/revoked since being photographed on a slide. Replace it (and the
 * rest of firebaseConfig) with your own project's values from the Firebase
 * console before this will actually work.
 */

document.addEventListener("DOMContentLoaded", function () {
  // ✅ Firebase Configuration
  const firebaseConfig = {
    apiKey: "AIzaSyB5zZmN7A7lun8i9mVK_RskmcuBChCePnk",
    authDomain: "ambulance-gps-tracker-e7072.firebaseapp.com",
    databaseURL: "https://ambulance-gps-tracker-e7072-default-rtdb.firebaseio.com",
    projectId: "ambulance-gps-tracker-e7072",
    storageBucket: "ambulance-gps-tracker-e7072.appspot.com",
    messagingSenderId: "507573838697",
    appId: "1:507573838697:web:d40a75323aae2ae47293f8",
    measurementId: "G-KF3KKF776R"
  };

  // ✅ Initialize Firebase
  firebase.initializeApp(firebaseConfig);
  const database = firebase.database();

  // ✅ DOM Elements
  const startButton = document.getElementById("startButton");
  const locationDisplay = document.getElementById("locationDisplay");
  const mapLink = document.getElementById("mapLink");

  if (!startButton) {
    console.error("❌ Start button not found in the HTML!");
    return;
  }

  // ✅ Ambulance ID (Can be changed dynamically)
  const ambulanceId = "ambulance1";

  let map;
  let marker;

  // ✅ Initialize Google Map
  window.initMap = function () {
    console.log("🚀 initMap is running...");

    map = new google.maps.Map(document.getElementById("map"), {
      zoom: 15,
      center: { lat: 0, lng: 0 } // Neutral center initially
    });

    marker = new google.maps.Marker({
      map: map,
      position: { lat: 0, lng: 0 },
      title: "🚑 Ambulance Location"
    });

    // ✅ Listen for location updates from Firebase
    listenForLocationUpdates();
  };

  function listenForLocationUpdates() {
    database.ref(`ambulances/${ambulanceId}/currentLocation`).on("value", (snapshot) => {
      const data = snapshot.val();
      console.log("📡 Firebase update received:", data);
      if (data && data.latitude && data.longitude) {
        const latitude = parseFloat(data.latitude);
        const longitude = parseFloat(data.longitude);

        if (!isNaN(latitude) && !isNaN(longitude)) {
          updateMapMarker(latitude, longitude);
        } else {
          console.error("❌ Invalid location data received:", data);
        }
      }
    });
  }

  // ✅ Update Google Maps Marker
  function updateMapMarker(latitude, longitude) {
    const newPos = new google.maps.LatLng(latitude, longitude);

    if (!marker) {
      marker = new google.maps.Marker({
        position: newPos,
        map: map,
        title: "🚑 Ambulance Location"
      });
    } else {
      marker.setPosition(newPos);
    }
    // UNKNOWN: screenshot cuts off right after marker.setPosition(newPos).
    // The original likely also re-centers the map here, e.g.
    // map.setCenter(newPos); — left out since it wasn't actually visible.
  }

  startButton.addEventListener("click", function () {
    if (navigator.geolocation) {
      navigator.geolocation.watchPosition(
        (position) => {
          const latitude = position.coords.latitude;
          const longitude = position.coords.longitude;
          const timestamp = new Date().toISOString();

          console.log("✅ Location retrieved:", latitude, longitude);

          // ✅ Display on Webpage
          if (locationDisplay) {
            locationDisplay.innerText = `🚑 Current Location:\nLatitude: ${latitude}\nLongitude: ${longitude}`;
          }

          if (mapLink) {
            mapLink.href = `https://www.google.com/maps?q=${latitude},${longitude}`;
            mapLink.style.display = "inline-block";
          }

          // ✅ Update Firebase
          const locationRef = database.ref(`ambulances/${ambulanceId}/currentLocation`);
          locationRef.set({ latitude, longitude, timestamp });

          // ✅ Save to location history
          database.ref(`ambulances/${ambulanceId}/history`).push({ latitude, longitude, timestamp });
        },
        (error) => {
          console.error("❌ Error getting location:", error);
          switch (error.code) {
            case error.TIMEOUT:
              alert("⏱️ Location request timed out. Try again.");
              break;
            case error.PERMISSION_DENIED:
              alert("🚫 Location access denied. Please enable it in browser settings.");
              break;
            case error.POSITION_UNAVAILABLE:
              alert("📡 Location information is unavailable.");
              break;
            default:
              alert("❌ An unknown error occurred.");
          }
        },
        {
          enableHighAccuracy: true,
          timeout: 15000,
          maximumAge: 0
        }
      );
    } else {
      alert("❌ Geolocation is not supported by this browser.");
    }
  });
});
