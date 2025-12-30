int buzzer = 12;

const int ardpins = 10;
int led[ardpins] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

int puckPos = 4;
int direction = 1;
int yPin = A4;
int yVal = 530;          // Joystick-1 center ≈770
int yp2Pin = A6;
int y2val = 659;

const int btnPin = A3;
int onswt = 1;

bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastMoveTime = 0;
unsigned long lastJoystickRead = 0;
unsigned long lastJoystickRead2 = 0;

const int joystickDelay = 10;
int count1 = 0;
int count2 = 0;

// Anti-Cheat Flags
bool ready1 = false;
bool ready2 = false;

// Game timer
unsigned long gameStartTime = 0;

// Display Pins
const int dataPin = A0;
const int clockPin = A2;
const int latchPin = A1;

// Corrected digits arrays (0–9) for each display
const byte digits1[10] = {
  B1110111,  // 0
  B1000001,  // 1
  B1101110,  // 2
  B1101011,  // 3
  B1011001,  // 4
  B0111011,  // 5
  B0111111,  // 6
  B1100001,  // 7
  B1111111,  // 8
  B1111011   // 9
};

const byte digits2[10] = {
  B1101111,  // 0
  B0000011,  // 1
  B1011101,  // 2
  B1010111,  // 3
  B0110011,  // 4
  B1110110,  // 5
  B1111110,  // 6
  B1000011,  // 7
  B1111111,  // 8
  B1110111   // 9
};

void setup() {
  for (int i = 0; i < ardpins; i++) {
    pinMode(led[i], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(yPin, INPUT);
  pinMode(btnPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Joystick1\tJoystick2\tPuckPos\tDirection");

  displayScores(count2, count1);
}

void loop() {
  unsigned long currentTime = millis();

  // Toggle Button
  int reading = digitalRead(btnPin);
  if (reading != lastButtonState) {
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      onswt++;
      Serial.print("Toggled game. onswt = ");
      Serial.println(onswt);

      if (onswt % 2 == 1) {
        puckPos = 4;
        direction = 1;
        count1 = 0;
        count2 = 0;
        ready1 = false;
        ready2 = false;
        gameStartTime = millis();
        displayScores(count2, count1);
      }
    }
    buttonState = reading;
  }
  lastButtonState = reading;

  if (onswt % 2 == 0) {
    for (int i = 0; i < ardpins; i++) {
      digitalWrite(led[i], LOW);
    }

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);

    return;
  }

  // --- Joystick reads with live output ---
  if (currentTime - lastJoystickRead >= joystickDelay) {
    lastJoystickRead = currentTime;
    yVal = analogRead(yPin);
    Serial.print("P1: ");
    Serial.print(yVal);
    Serial.print("\t");
  }

  if (currentTime - lastJoystickRead2 >= joystickDelay) {
    lastJoystickRead2 = currentTime;
    y2val = analogRead(yp2Pin);
    Serial.print("P2: ");
    Serial.println(y2val);
  }
  // ---------------------------------------

  // Anti-cheat readiness (Player 1 centered ~770)
  if (yVal < 650) ready1 = true;       // pull back to arm
  if (y2val > 500) ready2 = true;

  // Speed curve based on time
  unsigned long elapsed = currentTime - gameStartTime;
  int dynamicDelay = 50;
  if (elapsed > 5000) dynamicDelay = 40;
  if (elapsed > 10000) dynamicDelay = 30;
  if (elapsed > 20000) dynamicDelay = 20;
  if (elapsed > 25000) dynamicDelay = 18;

  if (currentTime - lastMoveTime >= dynamicDelay) {
    lastMoveTime = currentTime;

    for (int i = 0; i < ardpins; i++) {
      digitalWrite(led[i], LOW);
    }

    digitalWrite(led[puckPos], HIGH);

    // Player 1 hit check using new center
    if (puckPos == 1 && direction == -1 && yVal > 600 && ready1) {
      direction = 1;
      ready1 = false;
    }
    else if (puckPos == 8 && direction == 1 && y2val < 300 && ready2) {
      direction = -1;
      ready2 = false;
    }
    else if (puckPos == ardpins - 1) {
      direction = -1;
    }
    else if (puckPos == 0) {
      direction = 1;
    }

    if (puckPos == 9 && count1 < 10) {       // allow 0–9 display
      count1++;
      playScoreSound();
      displayScores(count2, count1);
    }

    if (puckPos == 0 && count2 < 10) {
      count2++;
      playScoreSound();
      displayScores(count2, count1);
    }

    puckPos += direction;
    if (puckPos < 0) puckPos = 0;
    if (puckPos >= ardpins) puckPos = ardpins - 1;
  }
}

void displayScores(int p2, int p1) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digits2[p2]);
  shiftOut(dataPin, clockPin, MSBFIRST, digits1[p1]);
  digitalWrite(latchPin, HIGH);
}

void playScoreSound() {
  tone(buzzer, 800, 100); delay(120);
  tone(buzzer, 1000, 100); delay(120);
  tone(buzzer, 1200, 100); delay(200);
  noTone(buzzer);
}
