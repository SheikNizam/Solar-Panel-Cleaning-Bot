#include <ESP32Servo.h>  // Include the Servo library

#define forwardMotor11 22
#define forwardMotor12 23
#define forwardMotor21 2
#define forwardMotor22 15
#define reverseMotor11 19
#define reverseMotor12 21
#define reverseMotor21 4
#define reverseMotor22 5

const int trigPinFront = 12;  
const int echoPinFront = 13;  
const int trigPinLeft = 14;  
const int echoPinLeft = 27;  
const int trigPinRight = 26;  
const int echoPinRight = 25;  

// Servo motor pin
const int servoPin = 33;

// Variables to store the duration of the pulse and distances
long durationFront, durationLeft, durationRight;
int distanceFront, distanceLeft, distanceRight;

// Create a servo object
Servo myServo;

void setup() {
  pinMode(forwardMotor11, OUTPUT);
  pinMode(forwardMotor12, OUTPUT);
  pinMode(forwardMotor21, OUTPUT);
  pinMode(forwardMotor22, OUTPUT);
  pinMode(reverseMotor11, OUTPUT);
  pinMode(reverseMotor12, OUTPUT);
  pinMode(reverseMotor21, OUTPUT);
  pinMode(reverseMotor22, OUTPUT);
  
  // Set up sensors
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  // Attach the servo to the designated pin
  myServo.attach(servoPin);

  // Initialize servo position to "down"
  myServo.write(0);

  Serial.begin(115200);
}

// Motor control functions
void back() {
  digitalWrite(forwardMotor11, LOW);
  digitalWrite(forwardMotor12, HIGH);
  digitalWrite(forwardMotor21, HIGH);
  digitalWrite(forwardMotor22, LOW);
  digitalWrite(reverseMotor11, HIGH);
  digitalWrite(reverseMotor12, LOW);
  digitalWrite(reverseMotor21, HIGH);
  digitalWrite(reverseMotor22, LOW);
  Serial.println("back");
}

void front() {
  digitalWrite(forwardMotor11, HIGH);
  digitalWrite(forwardMotor12, LOW);
  digitalWrite(forwardMotor21, LOW);
  digitalWrite(forwardMotor22, HIGH);
  digitalWrite(reverseMotor11, LOW);
  digitalWrite(reverseMotor12, HIGH);
  digitalWrite(reverseMotor21, LOW);
  digitalWrite(reverseMotor22, HIGH);
  Serial.println("front");
}

void stopBot() {
  digitalWrite(forwardMotor11, LOW);
  digitalWrite(forwardMotor12, LOW);
  digitalWrite(forwardMotor21, LOW);
  digitalWrite(forwardMotor22, LOW);
  digitalWrite(reverseMotor11, LOW);
  digitalWrite(reverseMotor12, LOW);
  digitalWrite(reverseMotor21, LOW);
  digitalWrite(reverseMotor22, LOW);
  Serial.println("stop");
}

void leftTurn() {
  digitalWrite(forwardMotor11, LOW);
  digitalWrite(forwardMotor12, HIGH);
  digitalWrite(forwardMotor21, LOW);
  digitalWrite(forwardMotor22, HIGH);
  digitalWrite(reverseMotor11, HIGH);
  digitalWrite(reverseMotor12, LOW);
  digitalWrite(reverseMotor21, LOW);
  digitalWrite(reverseMotor22, HIGH);
  Serial.println("leftturn");
}

void rightTurn() {
  digitalWrite(forwardMotor11, HIGH);
  digitalWrite(forwardMotor12, LOW);
  digitalWrite(forwardMotor21, HIGH);
  digitalWrite(forwardMotor22, LOW);
  digitalWrite(reverseMotor11, LOW);
  digitalWrite(reverseMotor12, HIGH);
  digitalWrite(reverseMotor21, HIGH);
  digitalWrite(reverseMotor22, LOW);
  Serial.println("rightturn");
}

// Function to measure distance
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.0344 / 2; // Convert to cm
}

void loop() {
  // Read distances from all sensors
  distanceFront = measureDistance(trigPinFront, echoPinFront);
  distanceLeft = measureDistance(trigPinLeft, echoPinLeft);
  distanceRight = measureDistance(trigPinRight, echoPinRight);

  // Debugging: Print distances
  Serial.print("Front: ");
  Serial.print(distanceFront);
  Serial.print(" cm | Left: ");
  Serial.print(distanceLeft);
  Serial.print(" cm | Right: ");
  Serial.println(distanceRight);

  // Control the servo motor based on front distance
  if (distanceFront < 10) {
    myServo.write(90);  // Servo up position
  } else {
    myServo.write(0);   // Servo down position
  }

  // Edge detection logic
  if(distanceFront<10&&distanceLeft<10){
    stopBot();
  }
  else if(distanceFront<10&&distanceRight<10){
    stopBot();
  }
  else if (distanceLeft < 10 || distanceRight < 10) {
    stopBot();
    delay(2000);
    
    if (distanceLeft < 10) {
      rightTurn();
      delay(2000);
    } else if (distanceRight < 10) {
      leftTurn();
      delay(2000);
    }

    delay(2000);
    front();  // Continue moving forward
  }
  // End of row detection
  else if (distanceFront < 10) {
    stopBot();
    delay(2000);
    rightTurn();  // Example logic for turning right at the end of a row
    delay(2000);
    front();
  } 
  else {
    front();  // Default: Move forward
  }

  delay(100);  // Small delay for stability
}
