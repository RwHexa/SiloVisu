// Programm SiloFüllstand auf ESP32 WROOM
const int potPin = 34;
const int outputPin = 25;
const int numReadings = 5;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(9600);
  //analogReadResolution(12);
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);
  
  // Array initialisieren
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Alten Wert abziehen
  total = total - readings[readIndex];
  
  // Neuen Wert lesen
  readings[readIndex] = analogRead(potPin);
  
  // Neuen Wert addieren
  total = total + readings[readIndex];
  
  // Index erhöhen
  readIndex = (readIndex + 1) % numReadings;
  
  // Durchschnitt berechnen
  average = total / numReadings;
  
  // Nur senden wenn sich etwas geändert hat (Hysterese)
  static int lastSent = -10;
  if (abs(average - lastSent) > 5) {  // Mindestens 5 Schritte Änderung
    Serial.println(average);
    lastSent = average;
  }
  
  // Auf Befehle prüfen
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "ON") {
      digitalWrite(outputPin, HIGH);
      Serial.println("OK:ON");
    }
    else if (command == "OFF") {
      digitalWrite(outputPin, LOW);
      Serial.println("OK:OFF");
    }
  }
  
  delay(150);
}
