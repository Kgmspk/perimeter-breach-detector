// --- PIN DEFINITIONS (Exact mapping: a, b, c, d, e, g, f) ---
const int segA = 2;   // a (top)
const int segB = 3;   // b (top-right)
const int segC = 4;   // c (bottom-right)
const int segD = 5;   // d (bottom)
const int segE = 6;   // e (bottom-left)
const int segG = 7;   // g (middle)
const int segF = 12;  // f (top-left)

const int buzzerPin = 8;
const int trigPin   = 9;
const int echoPin   = 10;

const int restrictedDistance = 25; // Proximity threshold in cm

// Common Anode Truth Table: 0 = ON (LOW), 1 = OFF (HIGH)
const byte digitPatterns[10][7] = {
  {0, 0, 0, 0, 0, 1, 0}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 0, 1}, // 2
  {0, 0, 0, 0, 1, 0, 1}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}  // 9
};

void showDigit(int num) {
  if (num < 0 || num > 9) { clearDisplay(); return; }
  digitalWrite(segA, digitPatterns[num][0]);
  digitalWrite(segB, digitPatterns[num][1]);
  digitalWrite(segC, digitPatterns[num][2]);
  digitalWrite(segD, digitPatterns[num][3]);
  digitalWrite(segE, digitPatterns[num][4]);
  digitalWrite(segG, digitPatterns[num][5]);
  digitalWrite(segF, digitPatterns[num][6]);
}

void clearDisplay() {
  for (int p : {segA, segB, segC, segD, segE, segG, segF}) digitalWrite(p, HIGH);
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return 999.0;
  return (duration * 0.0343) / 2.0;
}

bool monitorWindow(int durationMs, bool buzzerOn) {
  unsigned long start = millis();
  int clearReadingsCount = 0;
  digitalWrite(buzzerPin, buzzerOn ? HIGH : LOW);

  while (millis() - start < (unsigned long)durationMs) {
    float dist = getDistance();
    if (dist > restrictedDistance) {
      clearReadingsCount++;
      if (clearReadingsCount >= 3) {
        digitalWrite(buzzerPin, LOW);
        return false; // Target confirmed cleared
      }
    } else {
      clearReadingsCount = 0;
    }
    delay(30);
  }
  return true; // Target stayed throughout window
}

void setup() {
  for (int p : {segA, segB, segC, segD, segE, segG, segF}) pinMode(p, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  clearDisplay();
}

void loop() {
  float dist = getDistance();
  if (dist > restrictedDistance) {
    digitalWrite(buzzerPin, LOW);
    clearDisplay();
    delay(50);
    return;
  }

  // Intruder detected: Execute 5 to 1 countdown
  bool intruderStayed = true;
  for (int count = 5; count >= 1; count--) {
    showDigit(count);
    if (!monitorWindow(150, true))  { intruderStayed = false; break; }
    if (!monitorWindow(600, false)) { intruderStayed = false; break; }
  }

  if (!intruderStayed) {
    digitalWrite(buzzerPin, LOW);
    delay(400); // Brief hold of interrupted digit
    clearDisplay();
    return;
  }

  // Breach triggered: Latch 0 and continuous alarm
  showDigit(0);
  int clearCount = 0;
  while (true) {
    digitalWrite(buzzerPin, HIGH); delay(100);
    digitalWrite(buzzerPin, LOW);  delay(80);
    if (getDistance() > restrictedDistance) {
      if (++clearCount >= 3) break;
    } else {
      clearCount = 0;
    }
  }
  digitalWrite(buzzerPin, LOW);
  clearDisplay();
  delay(100);
}
