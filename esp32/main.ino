#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "Wokwi-GUEST";
const char* password = "";

const char* serverURL = "http://tejashwinirreddy-neurovigilix-132.flowfuse.cloud/secure-sos";
const char* pingURL   = "http://tejashwinirreddy-neurovigilix-132.flowfuse.cloud/ping";

const int heartPin  = 34;
const int trigPin   = 5;
const int echoPin   = 18;
const int buzzPin   = 19;
const int buttonPin = 4;

const String securityToken = "GHOST_SECURE_AUTH_8923";

void pingServer() {
  Serial.println("\n=== PING TEST ===");
  HTTPClient http;
  http.begin(pingURL);
  http.setTimeout(20000);
  int code = http.GET();
  if (code > 0) {
    Serial.println("SERVER IS ALIVE — Code: " + String(code));
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("SERVER UNREACHABLE — " + http.errorToString(code));
  }
  http.end();
  Serial.println("=================\n");
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin,   OUTPUT);
  pinMode(echoPin,   INPUT);
  pinMode(buzzPin,   OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 40) {
      Serial.println("\nTimeout — restarting...");
      ESP.restart();
    }
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  pingServer();
}

void sendData(int hr, int distance, bool sos) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected — skipping send");
    return;
  }

  String payload = "{";
  payload += "\"token\":\"" + securityToken + "\",";
  payload += "\"heartRate\":" + String(hr) + ",";
  payload += "\"lat\":12.9716,";
  payload += "\"dist\":" + String(distance) + ",";
  payload += "\"sos\":" + String(sos ? "true" : "false");
  payload += "}";

  Serial.println("Sending: " + payload);

  const int MAX_RETRIES = 3;
  for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
    Serial.print("Attempt " + String(attempt) + "/" + String(MAX_RETRIES) + " ... ");

    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(20000);

    int responseCode = http.POST(payload);

    if (responseCode > 0) {
      Serial.println("OK — Code: " + String(responseCode));
      Serial.println("Response: " + http.getString());
      http.end();
      return;
    } else {
      Serial.println("FAILED — " + http.errorToString(responseCode));
      http.end();
      if (attempt < MAX_RETRIES) {
        Serial.println("Retrying in 3s...");
        delay(3000);
      }
    }
  }
  Serial.println("All attempts failed.\n");
}

void loop() {
  int rawHeart  = analogRead(heartPin);
  int heartRate = map(rawHeart, 0, 4095, 60, 180);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  int dist = (duration == 0) ? 999 : (int)(duration * 0.034 / 2);

  bool manualSOS = (digitalRead(buttonPin) == LOW);
  if (manualSOS) {
    heartRate = 180;
    Serial.println(">>> MANUAL SOS BUTTON PRESSED <<<");
  }

  Serial.print("Heart Rate: "); Serial.print(heartRate);
  Serial.print(" | Dist: ");    Serial.print(dist);
  Serial.print(" | SOS: ");     Serial.println(manualSOS ? "YES" : "NO");

  sendData(heartRate, dist, manualSOS);

  if ((heartRate > 120 && dist < 15) || manualSOS) {
    Serial.println("EMERGENCY!");
    digitalWrite(buzzPin, HIGH);
  } else {
    digitalWrite(buzzPin, LOW);
  }

  delay(2000);
}
