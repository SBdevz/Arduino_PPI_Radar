#include <Servo.h>
Servo myServo;

int switchPin = 4;

const int trigPin = 8;
const int echoPin = 7;

int speakerPin = 12;

float distance = 0;


void setup() {
  myServo.attach(13);
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(9600); 
  pinMode (speakerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


void loop() {
  delay(100);
  if (digitalRead(4) == LOW) {
    distance = getDistance();
    myServo.write(0);
    if (distance <= 10) {
        play('b', 4);       
        play('a', 6);  
    }     
  } else {
    myServo.write(90);
  }
}


float getDistance()
{
  float echoTime;                   
  float calculatedDistance;         
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoTime = pulseIn(echoPin, HIGH);                                          
  calculatedDistance = echoTime / 148.0;  
  return calculatedDistance;
}


void play( char note, int beats)
{
  int numNotes = 14;  
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' '};
  int frequencies[] = {131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0};
  int currentFrequency = 0;    
  int beatLength = 150;   
  for (int i = 0; i < numNotes; i++)  
  {
    if (notes[i] == note)            
    {
      currentFrequency = frequencies[i];   
    }
  }
  tone(speakerPin, currentFrequency, beats * beatLength);
  delay(beats * beatLength);  
  delay(50);                 
}  