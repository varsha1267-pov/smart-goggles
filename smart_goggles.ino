#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 4

#define DF_RX 16
#define DF_TX 17

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini dfPlayer;

long getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return 999;

  return (duration * 0.0343) / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dfSerial.begin(9600, SERIAL_8N1, DF_RX, DF_TX);

  if (dfPlayer.begin(dfSerial)) {
    dfPlayer.volume(25);
    Serial.println("DFPlayer Mini connected.");
  } else {
    Serial.println("DFPlayer Mini not detected.");
  }
}

void loop() {
  long distance = getDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 50) {
    digitalWrite(BUZZER_PIN, HIGH);
    dfPlayer.play(2);   // Very close
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);

  } else if (distance <= 100) {
    digitalWrite(BUZZER_PIN, HIGH);
    dfPlayer.play(1);   // Obstacle ahead
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);

  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200);
}