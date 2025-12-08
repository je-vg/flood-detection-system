#define BLYNK_TEMPLATE_ID "TMPL2PaIW-LB4" 
#define BLYNK_TEMPLATE_NAME "Water Level"
#define BLYNK_AUTH_TOKEN "Eb6he8UgOOHqlF1_W03KpRY6juHHWirw" 
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <NewPing.h>

char authToken[] = BLYNK_AUTH_TOKEN; 
char wifiSSID[] = "JesusiPhone"; 
char wifiPass[] = "Pelusa2252";
BlynkTimer taskScheduler; 

const int TRIG_PIN = 19; 
const int ECHO_PIN = 21; 
const int LED_PIN = 22; 
const int BUZZ_PIN = 23; 
const int PUMP_PIN = 17; 

float distanceFromSensor; 
float tankWaterHeight; 
float waterFillPercent;  
bool drainSystemActive = false; 
bool alertToggle = false; 
#define SAMPLE_COUNT 7 
float pingSamples[SAMPLE_COUNT]; 
int validPingIndex = 0; 
float lastStableDistance = 0; 
NewPing ultrasonicSensor(TRIG_PIN, ECHO_PIN, 200);
const float tankHeight = 7.5; 
const float minTankHeight = 1.0; 

void sampleUltrasonicMedian() {
  validPingIndex = 0; 

  while (validPingIndex < SAMPLE_COUNT) { 
    float pingDistanceIn = ultrasonicSensor.ping_in();

    if (pingDistanceIn < minTankHeight || pingDistanceIn > tankHeight) { 
      continue; 
    }

    pingSamples[validPingIndex] = pingDistanceIn; 
    validPingIndex++; 
    delayMicroseconds(500); 
  }

  for (int i = 0; i < SAMPLE_COUNT - 1; i++) {
    for (int j = 0; j < SAMPLE_COUNT - i - 1; j++) { 
      if (pingSamples[j] > pingSamples[j + 1]) { 
        float temp = pingSamples[j]; 
        pingSamples[j] = pingSamples[j + 1]; 
        pingSamples[j + 1] = temp; 
      }
    }
  }

  int mid = SAMPLE_COUNT / 2;
  distanceFromSensor = pingSamples[mid]; 
  lastStableDistance = distanceFromSensor; 
}

void convertWaterLevel() {
  tankWaterHeight = tankHeight - distanceFromSensor; 

  if (tankWaterHeight < 0) { 
    tankWaterHeight = 0; 
  }

  if (tankWaterHeight > tankHeight) { 
    tankWaterHeight = tankHeight; 
  }

  waterFillPercent = (tankWaterHeight / tankHeight) * 100.0;

  Blynk.virtualWrite(V0, tankWaterHeight);
  Blynk.virtualWrite(V3, waterFillPercent); 
}

void decideDrainState() {
  static const float ON_THRESHOLD = 5.0; 
  static const float OFF_THRESHOLD = 2.5; 
  static bool lastState = false;

  if (tankWaterHeight <= OFF_THRESHOLD && lastState == true) { 
    drainSystemActive = false; 
    lastState = false;
    Blynk.virtualWrite(V2, "OFF"); 
    Serial.println("System OFF");
    return;
  }

  if (tankWaterHeight >= ON_THRESHOLD && lastState == false) { 
    drainSystemActive = true; 
    lastState = true;
    Blynk.virtualWrite(V2, "ON"); 
    Serial.println("System ON");
  }
}

void systemState() {
  if (drainSystemActive == false) { 
    digitalWrite(PUMP_PIN, LOW); 
    digitalWrite(LED_PIN, LOW); 
    noTone(BUZZ_PIN);
  } else { 
    digitalWrite(PUMP_PIN, HIGH);
  }
}

void alertPattern() {
  if (!drainSystemActive) { 
    alertToggle = false;
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZ_PIN);
    return;
  }

  alertToggle = !alertToggle; 

  if (alertToggle) { 
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZ_PIN, 800);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZ_PIN);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(authToken, wifiSSID, wifiPass);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);

  distanceFromSensor = 0;
  tankWaterHeight = 0;
  waterFillPercent = 0;
  validPingIndex = 0;
  lastStableDistance = 0;

  //tasks
  taskScheduler.setInterval(250L, sampleUltrasonicMedian);
  taskScheduler.setInterval(250L, convertWaterLevel);
  taskScheduler.setInterval(250L, decideDrainState);
  taskScheduler.setInterval(250L, systemState);
  taskScheduler.setInterval(500L, alertPattern);
}

void loop() {
  Blynk.run();
  taskScheduler.run();
}
