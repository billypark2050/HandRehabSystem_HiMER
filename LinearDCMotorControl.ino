// Defining the control pins
const int motorPin1 = 2; // Connects to IN1 and IN3 for L298N
const int motorPin2 = 3; // Connects to IN2 and IN4

void setup() {

  // 시리얼 통신 시작 (보드레이트 9600)
  Serial.begin(9600);
  //Serial.println("Motor Control Ready!");

  // put your setup code here, to run once:
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  //myStepper.setSpeed(10);

}

void loop() {
  // put your main code here, to run repeatedly:
  // delay 500당 얼마나 이동하는지 측정하고 그래프 그려보기
  // delay 1000 - 4000
  moveBackward();
  delay(2000); 
  
  moveForward(); 
  delay(2000); 
  
  stopMotors();
  delay(2000); 
}


// Helper Functions
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}