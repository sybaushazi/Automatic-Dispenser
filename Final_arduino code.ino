#include <U8g2lib.h>
#include <Wire.h>

// Wapas original aur safe library takay Memory ka koi issue na aaye
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ================= PINS =================
const int trigPin = 10;
const int echoPin = 11;
const int relayPin = 7;
const int waterSensorPin = A0;
const int ledPin = 4;
const int buzzerPin = 5;
const int buttonPin = 2;

// ================= SETTINGS =================
const int distanceThreshold = 10;
const int lowWaterLimit = 20;

// ================= MENU =================
int selectedOption = 0;
const char* options[] = {"100ml", "150ml", "200ml", "NO LIMIT"};

// ================= STATE =================
bool pumping = false;
bool lastButtonState = HIGH;
int screenState = -1; 
bool showFilled = false; 

// ================= TIMERS =================
unsigned long lastButtonPress = 0;
const int debounceDelay = 200;
unsigned long pumpStartTime = 0;
unsigned long pumpStopTime = 0; 
unsigned long fillEndTime = 0; 
const int cooldownTime = 1500;  

// ================= DISTANCE =================
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long d = pulseIn(echoPin, HIGH, 25000); 
  if (d <= 0) return 999;
  return d * 0.034 / 2;
}

// ================= OLED DRAW FUNCTIONS =================
void drawMenu(int ml) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(0, 12, "SELECT MODE:");
    for (int i = 0; i < 4; i++) {
      int y = 25 + (i * 10);
      if (i == selectedOption) u8g2.drawStr(0, y, ">");
      u8g2.drawStr(10, y, options[i]);
    }
    u8g2.drawLine(0, 54, 128, 54);
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(0, 63);
    u8g2.print("Vol: "); u8g2.print(ml);
    u8g2.print("ml | Mode: "); u8g2.print(options[selectedOption]);
  } while (u8g2.nextPage());
}

void drawStatus(const char* t) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(5, 40, t); 
  } while (u8g2.nextPage());
}

// ================= SETUP =================
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  digitalWrite(relayPin, HIGH); 
  u8g2.begin();
}

// ================= LOOP =================
void loop() {
  int distance = getDistance();
  int sensor = analogRead(waterSensorPin);
  int ml = map(sensor, 0, 550, 0, 200);
  if (ml < 0) ml = 0;

  unsigned long now = millis();

  // --- BUTTON LOGIC ---
  bool btn = digitalRead(buttonPin);
  if (btn == LOW && lastButtonState == HIGH) {
    if (now - lastButtonPress > debounceDelay) {
      selectedOption++;
      if (selectedOption > 3) selectedOption = 0;
      lastButtonPress = now;
      screenState = -1; 
    }
  }
  lastButtonState = btn;

  // --- LOW WATER LOGIC ---
  if (ml < lowWaterLimit) {
    pumping = false;
    showFilled = false; 
    digitalWrite(relayPin, HIGH);
    bool blink = (now / 500) % 2;
    digitalWrite(ledPin, blink);
    digitalWrite(buzzerPin, blink);
    if (screenState != 2) { drawStatus("LOW WATER!"); screenState = 2; }
    return;
  }

  // --- SHOW FILLED MESSAGE LOGIC (WITH BEEP-BEEP) ---
  if (showFilled) {
    unsigned long timePassed = now - fillEndTime;
    
    if (timePassed < 1500) { 
      digitalWrite(ledPin, HIGH);
      
      // Double Beep Effect
      if ((timePassed > 0 && timePassed < 200) || (timePassed > 400 && timePassed < 600)) {
        digitalWrite(buzzerPin, HIGH);
      } else {
        digitalWrite(buzzerPin, LOW);
      }
      
      if (screenState != 3) { drawStatus("WATER FILLED!"); screenState = 3; }
    } else { 
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      showFilled = false;
      pumpStopTime = now; 
      screenState = -1; 
    }
    return; 
  }

  // --- PUMP START LOGIC ---
  if (!pumping && distance <= distanceThreshold && distance > 0 && (now - pumpStopTime > cooldownTime)) {
    if (selectedOption != 3) {
      delay(200); 
      if (getDistance() > distanceThreshold) return; 
    }
    pumping = true;
    pumpStartTime = millis(); 
    digitalWrite(relayPin, LOW); 
    if (screenState != 1) { drawStatus("PUMPING..."); screenState = 1; }
  }

  // --- PUMP STOP LOGIC ---
  if (pumping) {
    if (selectedOption == 3) { 
      if (distance > distanceThreshold) {
        pumping = false;
        digitalWrite(relayPin, HIGH);
        showFilled = true; 
        fillEndTime = millis(); 
      }
    } 
    else { 
      unsigned long runTime = 0;
      if(selectedOption == 0) runTime = 8000;
      if(selectedOption == 1) runTime = 12000;
      if(selectedOption == 2) runTime = 16000;

      if (millis() - pumpStartTime >= runTime) {
        pumping = false;
        digitalWrite(relayPin, HIGH);
        showFilled = true; 
        fillEndTime = millis(); 
      }
    }
    return; 
  }

  // --- IDLE / READY ---
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(relayPin, HIGH); 

  if (screenState != 0) {
    drawMenu(ml);
    screenState = 0;
  }
}