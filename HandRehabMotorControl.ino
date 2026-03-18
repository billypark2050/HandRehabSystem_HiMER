#include <TimerOne.h>

#define dirPinX 2
#define stepPinX 3

#define dirPinY 4   // Y축
#define stepPinY 5

#define stepsPerRevolution 200 // Minimum angular resolution = 1.8 degree
#define stepMotorSpeed 1000
// Measurement for Stepper motor: (149.6, 53.6) => Comment (-500,-500) => (49.7,153.7)
// unit dx = 1 => 1.8 degree, 0.2 mm resolution for NEMA 17

// Defining the Linear Motor Control Pins
const int linearMotorPin1 = 8; // Connects to IN1 and IN3 for L298N
const int linearMotorPin2 = 9; // Connects to IN2 and IN4

// for stepper motor control
int xCount = 0;
int yCount = 0;

volatile int zCount = 0; // interrupt variables 

String inputString = "";         
bool stringComplete = false;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); 

  // put your setup code here, to run once:
  pinMode(linearMotorPin1, OUTPUT);
  pinMode(linearMotorPin2, OUTPUT);

  // Declare pins for stepper as output:
  pinMode(stepPinX, OUTPUT); pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT); pinMode(dirPinY, OUTPUT);
  

  // --- 타이머 인터럽트 설정 ---
  Timer1.initialize(1000); // 1000us = 1ms 마다 인터럽트 발생
  Timer1.attachInterrupt(dcMotorISR); // 1ms마다 dcMotorISR 함수 실행

}
// -------------------------------------------------------------
// [백그라운드 제어] Z축 DC 모터 타이머 인터럽트 (1ms 주기)
// 메인 루프가 뭘 하든 상관없이 1ms마다 정확하게 zCount를 소모하며 모터를 돌립니다.
// -------------------------------------------------------------
void dcMotorISR() {
  if (zCount > 0) {
    // Forward
    digitalWrite(linearMotorPin1, HIGH);
    digitalWrite(linearMotorPin2, LOW);
    zCount--; // 1ms 소모
  } 
  else if (zCount < 0) {
    // Backward
    digitalWrite(linearMotorPin1, LOW);
    digitalWrite(linearMotorPin2, HIGH);
    zCount++; // 1ms 소모 (0으로 다가감)
  } 
  else {
    // zCount == 0 이면 Stop
    digitalWrite(linearMotorPin1, LOW);
    digitalWrite(linearMotorPin2, LOW);
  }
}

void loop() {


  // Serial Communication Receive; Asynchronization(As much as possible)
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();  
    
    if (inChar == '\n') {
      stringComplete = true;
      break;
    }     
    else {
      inputString += inChar;
    }
  }

  if (stringComplete) {
    Serial.println(inputString);
    int firstComma = inputString.indexOf(',');
    int secondComma = inputString.indexOf(',', firstComma + 1);

    if (firstComma > 0 && secondComma > 0) {
      int dx = inputString.substring(0, firstComma).toInt();
      int dy = inputString.substring(firstComma + 1, secondComma).toInt();
      int dz = inputString.substring(secondComma + 1).toInt();

      xCount += dx;
      yCount += dy;

      // [안전 장치] zCount는 인터럽트에서도 건드리는 변수이므로, 
      // 값을 더할 때 잠깐 인터럽트를 꺼서 충돌(Race Condition)을 방지합니다.
      noInterrupts(); 
      zCount += dz; 
      interrupts();
    }

    inputString = "";
    stringComplete = false;
  }

  
  // 1. Direction Setting
  // Count > 0 => LOW, Count < 0  => HIGH (모터 결선에 따라 반대일 수 있음)
  digitalWrite(dirPinX, (xCount > 0) ? LOW : HIGH);
  digitalWrite(dirPinY, (yCount > 0) ? LOW : HIGH);

  // 2. Generating Pulse
  while (xCount != 0 || yCount != 0 || zCount != 0) {
    bool stepTaken = false;

    // --- X축 ---
    if (xCount > 0) { 
      digitalWrite(stepPinX, HIGH); 
      xCount--; // 양수면 빼서 0으로
      stepTaken = true; 
    } 
    else if (xCount < 0) { 
      digitalWrite(stepPinX, HIGH); 
      xCount++; // 음수면 더해서 0으로
      stepTaken = true; 
    }

    // --- Y축 ---
    if (yCount > 0) { 
      digitalWrite(stepPinY, HIGH); 
      yCount--; 
      stepTaken = true; 
    } 
    else if (yCount < 0) { 
      digitalWrite(stepPinY, HIGH); 
      yCount++; 
      stepTaken = true; 
    }
    
    // 세 축 중 하나라도 HIGH 펄스가 켜졌다면 딜레이 후 일괄 LOW 처리
    if (stepTaken) {
      delayMicroseconds(stepMotorSpeed);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinY, LOW);      
      delayMicroseconds(stepMotorSpeed);
    }
  }  

  
}