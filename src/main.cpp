#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Seeed_Arduino_SSCMA.h>

const char* AP_SSID = "PPE-Detection";
const char* AP_PASSWORD = "12345678"; 

SSCMA AI;
AsyncWebServer server(80);      
AsyncWebSocket ws("/ws");       

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>PPE Detection Dashboard</title>
  <style>
    body { font-family: sans-serif; background: #111; color: #eee; text-align: center; padding: 20px; }
    #status { font-size: 1.2em; margin-bottom: 20px; }
    .box { background: #222; border-radius: 10px; padding: 15px; margin: 10px auto; max-width: 400px; }
    .live { color: #4caf50; }
    .offline { color: #f44336; }
  </style>
</head>
<body>
  <h1>PPE Detection Dashboard</h1>
  <div id="status" class="offline">Connexion...</div>
  <img id="camera" style="max-width:100%; border-radius:10px; margin: 15px 0;">
  <div id="detections"></div>

  <script>
    const statusEl = document.getElementById('status');
    const detectionsEl = document.getElementById('detections');

    const ws = new WebSocket(`ws://${window.location.hostname}/ws`);

    ws.onopen = () => {
      statusEl.textContent = "Connecte - Detection en direct";
      statusEl.className = "live";
    };

    ws.onclose = () => {
      statusEl.textContent = "Deconnecte";
      statusEl.className = "offline";
    };

    ws.onmessage = (event) => {
      const data = JSON.parse(event.data);

      if (data.type === "image") {
        document.getElementById('camera').src = "data:image/jpeg;base64," + data.data;
        return;
      }

      if (data.type === "detections") {
        let html = "";
        if (data.boxes.length === 0) {
          html = "<div class='box'>Aucune detection</div>";
        } else {
          data.boxes.forEach(b => {
            html += `<div class="box">
              Classe: ${b.target} | Score: ${b.score}% <br>
              Position: x=${b.x}, y=${b.y} | Taille: ${b.w}x${b.h}
            </div>`;
          });
        }
        detectionsEl.innerHTML = html;
      }
    };
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("Point d'acces cree. IP : ");
  Serial.println(WiFi.softAPIP());  

  AI.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });
  server.addHandler(&ws);
  server.begin();

  Serial.println("Serveur web demarre.");
}

void loop() {
  if (!AI.invoke(1, false, true)) {

    JsonDocument doc;
    doc["type"] = "detections";
    JsonArray boxesArray = doc["boxes"].to<JsonArray>();

    for (int i = 0; i < AI.boxes().size(); i++) {
      JsonObject b = boxesArray.add<JsonObject>();
      b["target"] = AI.boxes()[i].target;
      b["score"]  = AI.boxes()[i].score;
      b["x"]      = AI.boxes()[i].x;
      b["y"]      = AI.boxes()[i].y;
      b["w"]      = AI.boxes()[i].w;
      b["h"]      = AI.boxes()[i].h;
    }

    String output;
    serializeJson(doc, output);
    ws.textAll(output);
    Serial.println(output);

    String img = AI.last_image();
    if (img.length() > 0) {
      String imgMessage = "{\"type\":\"image\",\"data\":\"" + img + "\"}";
      ws.textAll(imgMessage);
    }
  }

  delay(200);
}