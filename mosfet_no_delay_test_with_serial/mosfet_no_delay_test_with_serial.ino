#define signalPinPiston1 2
#define signalPinPiston2 3
#define signalPinPiston3 4
#define signalPinPiston4 5

char group1Power; // indicates whether piston 1 and 2 are on or off (1 = on, 0 = off) 
char group2Power; // indicates whether piston 3 and 4 are on or off (1 = on, 0 = off)

char group1Direction; // direction of piston 1 and 2 (1 = forward, 0 = reverse)
char group2Direction; // direction of piston 3 and 4 (1 = forward, 0 = reverse)

char hundredsrevmultiply;
char tensrevmultiply;
char onesrevmultiply;

char hundredsspeedmultiply;
char tensspeedmultiply;
char onesspeedmultiply;

int hundredsrev;
int tensrev;
int onesrev;

int numberOfMovements;

int hundredsspeed;
int tensspeed;
int onesspeed;

int switchDelay;
int switchDelayBothMotors;
int switchDelayRounded;

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;
boolean loopComplete = true;
    
void setup() {
  pinMode(signalPinPiston1, OUTPUT);
  pinMode(signalPinPiston2, OUTPUT);
  pinMode(signalPinPiston3, OUTPUT);
  pinMode(signalPinPiston4, OUTPUT);
  Serial.begin(9600);
  }


void loop() {
    recvWithStartEndMarkers();
    useNewData();
    runMotors();
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void useNewData() {
    if (newData == true) {
    group1Power = receivedChars[0];
    group2Power = receivedChars[1];
    group1Direction = receivedChars[2];
    group2Direction = receivedChars[3];
    hundredsrevmultiply = receivedChars[4];
    tensrevmultiply = receivedChars[5];
    onesrevmultiply = receivedChars[6];
    hundredsspeedmultiply = receivedChars[7];
    tensspeedmultiply = receivedChars[8];
    onesspeedmultiply = receivedChars[9];
    hundredsrev = 100*(hundredsrevmultiply-48);
    tensrev = 10*(tensrevmultiply-48);
    onesrev = 1*(onesrevmultiply-48);
    numberOfMovements = (hundredsrev + tensrev + onesrev);
    hundredsspeed = 100*(hundredsspeedmultiply-48);
    tensspeed = 10*(tensspeedmultiply-48);
    onesspeed = 1*(onesspeedmultiply-48);
    switchDelay = (hundredsspeed + tensspeed + onesspeed);
    switchDelayBothMotors = (switchDelay/2);
    switchDelayRounded = round(switchDelayBothMotors);
    Serial.println(group1Power);
    Serial.println(group2Power);
    Serial.println(group1Direction);
    Serial.println(group2Direction);
    Serial.println(hundredsrevmultiply);
    Serial.println(tensrevmultiply);
    Serial.println(onesrevmultiply);
    Serial.println(hundredsrev);
    Serial.println(tensrev);
    Serial.println(onesrev);
    Serial.println(hundredsspeed);
    Serial.println(tensspeed);
    Serial.println(onesspeed);
    Serial.println(numberOfMovements);
    Serial.println(switchDelay);
    Serial.println(switchDelayRounded);
        newData = false;
        loopComplete = false;
    }
}


void runMotors() {
  if (loopComplete == false) {
  if (group1Power == '1' && group2Power == '1'){
    for (int i = 0; i < numberOfMovements; i++){
    if (group1Direction == '1' && group2Direction =='1'){
      bothGroupForward();
      }
    else if (group1Direction == '0' && group2Direction =='0'){
      bothGroupBackward(); 
      }
    else if (group1Direction == '1' && group2Direction =='0'){
      group1ForwardGroup2Backward();
      }
    else if (group1Direction == '0' && group2Direction =='1') {
      group1BackwardGroup2Forward(); 
      }
    }
  }
  else if (group1Power == '1' && group2Power == '0'){
    for (int i = 0; i < numberOfMovements; i++){
    if (group1Direction == '1') {
      group1Forward();
      }
    else if (group1Direction == '0'){
     group1Backward(); 
      }
    }
  }
  else if (group1Power == '0' && group2Power == '1'){
    for (int i = 0; i < numberOfMovements; i++) {
    if (group2Direction == '1') {
     group2Forward(); 
      }
    else if (group2Direction == '0'){
      group2Backward();  
      }
  }
  }
}
loopComplete = true;
}

void bothGroupForward() {
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelayRounded);
    }

void bothGroupBackward() {
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelayRounded); 
    }

void group1ForwardGroup2Backward(){
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelayRounded);
    }

void group1BackwardGroup2Forward(){
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelayRounded);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelayRounded); 
    }

void group1Forward(){
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelay);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelay);
    }

void group1Backward(){
    digitalWrite (signalPinPiston2, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston1, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston2, LOW);
    delay (switchDelay);
    digitalWrite (signalPinPiston1, LOW);
    delay (switchDelay);
    }

void group2Forward(){
  digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelay);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelay);
    }

void group2Backward(){
    digitalWrite (signalPinPiston4, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston3, HIGH);
    delay (switchDelay);
    digitalWrite (signalPinPiston4, LOW);
    delay (switchDelay);
    digitalWrite (signalPinPiston3, LOW);
    delay (switchDelay); 
    }
