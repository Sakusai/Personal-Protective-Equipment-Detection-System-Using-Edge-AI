#include <Arduino.h>
#include <Wire.h>
#include <Seeed_Arduino_SSCMA.h>

// Grove Vision AI V2
SSCMA AI;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  AI.begin();

  Serial.println("Grove Vision AI V2 initialisé, en attente de détections...");
}

void loop() {
  if (!AI.invoke()) {

    Serial.printf(
      "perf -> pretraitement: %dms, inference: %dms, postraitement: %dms\n",
      AI.perf().prepocess, AI.perf().inference, AI.perf().postprocess
    );

    for (int i = 0; i < AI.boxes().size(); i++) {
      Serial.printf(
        "Boite %d -> classe=%d, score=%d, x=%d, y=%d, largeur=%d, hauteur=%d\n",
        i,
        AI.boxes()[i].target,  
        AI.boxes()[i].score,   
        AI.boxes()[i].x,     
        AI.boxes()[i].y,    
        AI.boxes()[i].w,       
        AI.boxes()[i].h      
      );
    }


    for (int i = 0; i < AI.classes().size(); i++) {
      Serial.printf("Classe %d -> target=%d, score=%d\n",
        i, AI.classes()[i].target, AI.classes()[i].score);
    }
  }

  delay(200); 
}