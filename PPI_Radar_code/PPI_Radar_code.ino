#include <SoftwareSerial.h>
SoftwareSerial espSerial(3, 2);

#include <Servo.h>
Servo myServo;

int switchPin = 4;

const int trigPin = 8;
const int echoPin = 7;

int speakerPin = 12;

float distance = 0;

unsigned long DistanceTime = 0;
unsigned long SignalDelay = 0;

int active = 0;

int direction = 1;

int attached = 0;


void setup() {
  myServo.attach(13);
  myServo.write(90);
  myServo.detach();
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  espSerial.begin(9600);
  Serial.begin(9600);
  delay(1000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
  espSerial.println("AT");
  delay(1000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
  espSerial.println("AT+CWMODE=1");
  delay(1000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
  espSerial.println("AT+CWJAP=\"NH2025\",\"152207skb\"");
  delay(8000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
  espSerial.println("AT+CIPSTART=\"TCP\",\"192.168.86.29\",5000");
  delay(500);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
}


void loop() {
  unsigned long currentTime = millis();
  if (digitalRead(4) == LOW) {
    distance = getDistance();
    attach();
    if (direction == 1) {
      myServo.write(0);
    }
    if (direction == -1) {
      myServo.write(180);
    }
    if (distance <= 10 && distance > 0) {
      espSerial.println("AT+CIPSEND=6");
      if (currentTime - SignalDelay >= 500) {
        SignalDelay = currentTime;
        distance = getDistance();
        espSerial.print("Motion");
        play('b', 4);
        play('a', 6);
      }
    }
  } else {
      myServo.detach();
    }
  if (currentTime - DistanceTime >= 3000) {
    DistanceTime = currentTime;
    direction = direction * -1;
  }
}


float getDistance() {
  float echoTime;
  float calculatedDistance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoTime = pulseIn(echoPin, HIGH);
  calculatedDistance = echoTime / 148.0;
  return calculatedDistance;
}


void play(char note, int beats) {
  int numNotes = 14;
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' ' };
  int frequencies[] = { 131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0 };
  int currentFrequency = 0;
  int beatLength = 150;
  for (int i = 0; i < numNotes; i++) {
    if (notes[i] == note) {
      currentFrequency = frequencies[i];
    }
  }
  tone(speakerPin, currentFrequency, beats * beatLength);
  delay(beats * beatLength);
  delay(50);
}

void attach() {
  if (attached == 0) {
    myServo.attach(13);
    int attach = 1;
  }
}