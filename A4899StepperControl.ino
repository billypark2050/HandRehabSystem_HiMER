// Define stepper motor connections and steps per revolution:
#define dirPinX 2
#define stepPinX 3

#define dirPinY 4   // Y축
#define stepPinY 5

#define dirPinZ 6   // Z축 Height << Stepper 인 경우
#define stepPinZ 7

#define stepsPerRevolution 200 // 스탭모터마다 다른 값을 가짐. minimum resolution = 1.8 degree
#define stepMotorSpeed 1000;

int xCount = 0;
int yCount = 0;
int zCount = 0;

String inputString = "";         
bool stringComplete = false;



void setup() {
  // Declare pins as output:
  pinMode(stepPinX, OUTPUT); pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT); pinMode(dirPinY, OUTPUT);
  pinMode(stepPinZ, OUTPUT); pinMode(dirPinZ, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // Serial Communication Asynchronization(As much as possible)
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
      zCount += dz;
    }

    inputString = "";
    stringComplete = false;
  }

  // 1. Direction Setting
  // Count > 0 => LOW, Count < 0  => HIGH (모터 결선에 따라 반대일 수 있음)
  digitalWrite(dirPinX, (xCount > 0) ? LOW : HIGH);
  digitalWrite(dirPinY, (yCount > 0) ? LOW : HIGH);
  digitalWrite(dirPinZ, (zCount > 0) ? LOW : HIGH);

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

    // --- Z축 ---
    if (zCount > 0) { 
      digitalWrite(stepPinZ, HIGH); 
      zCount--; 
      stepTaken = true; 
    } 
    else if (zCount < 0) { 
      digitalWrite(stepPinZ, HIGH); 
      zCount++; 
      stepTaken = true; 
    }
    
    // 세 축 중 하나라도 HIGH 펄스가 켜졌다면 딜레이 후 일괄 LOW 처리
    if (stepTaken) {
      delayMicroseconds(stepMotorSpeed);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinY, LOW);
      digitalWrite(stepPinZ, LOW);
      delayMicroseconds(stepMotorSpeed);
    }
  }  

} 