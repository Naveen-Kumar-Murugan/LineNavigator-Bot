// LineNavigator-Bot Arduino Code

// Define pin numbers
const int leftIRPin = A0;     // Analog pin for left IR sensor
const int rightIRPin = A1;    // Analog pin for right IR sensor
const int sonarTrigPin = 9;   // Digital pin for SONAR trigger
const int sonarEchoPin = 10;  // Digital pin for SONAR echo
const int leftMotorPin = 3;   // PWM pin for left motor
const int rightMotorPin = 5;  // PWM pin for right motor

// Define constants
const int thresholdDistance = 20; // Threshold distance for obstacle avoidance (in cm)

// Function to initialize components
void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);

    // Initialize IR sensor pins
    pinMode(leftIRPin, INPUT);
    pinMode(rightIRPin, INPUT);

    // Initialize SONAR sensor pins
    pinMode(sonarTrigPin, OUTPUT);
    pinMode(sonarEchoPin, INPUT);

    // Initialize motor pins
    pinMode(leftMotorPin, OUTPUT);
    pinMode(rightMotorPin, OUTPUT);
}

// Function to read the distance from the SONAR sensor
long readSonarDistance() {
    // Send a pulse to trigger the SONAR sensor
    digitalWrite(sonarTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(sonarTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sonarTrigPin, LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(sonarEchoPin, HIGH);

    // Calculate the distance in centimeters
    long distance = (duration / 2) / 29.1;

    return distance;
}

// Function to follow the line based on IR sensor input
void followLine() {
    int leftIRValue = analogRead(leftIRPin);
    int rightIRValue = analogRead(rightIRPin);

    if (leftIRValue < 500) { // Assume line is detected if IR value is below a threshold
        // Turn slightly left
        analogWrite(leftMotorPin, 100);
        analogWrite(rightMotorPin, 255);
    } else if (rightIRValue < 500) {
        // Turn slightly right
        analogWrite(leftMotorPin, 255);
        analogWrite(rightMotorPin, 100);
    } else {
        // Move forward
        analogWrite(leftMotorPin, 255);
        analogWrite(rightMotorPin, 255);
    }
}

// Function to avoid obstacles using the SONAR sensor
void avoidObstacle() {
    long distance = readSonarDistance();

    if (distance < thresholdDistance) {
        // Stop the bot
        analogWrite(leftMotorPin, 0);
        analogWrite(rightMotorPin, 0);

        delay(500); // Wait for a short duration

        // Avoid obstacle by turning right
        analogWrite(leftMotorPin, 255);
        analogWrite(rightMotorPin, 0);
        delay(1000); // Turn for a short duration

        // Move forward after avoiding obstacle
        analogWrite(leftMotorPin, 255);
        analogWrite(rightMotorPin, 255);
    }
}

// Main loop
void loop() {
    followLine();    // Follow the line based on IR sensor input
    avoidObstacle(); // Check and avoid obstacles using the SONAR sensor
}
