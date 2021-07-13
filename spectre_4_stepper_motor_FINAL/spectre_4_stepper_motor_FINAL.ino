#define dirPin1a 2 // Direction pin for Motor 1a
#define stepPin1a 3 // Step enable pin for Motor 1a
#define dirPin1b 4 // Direction pin for Motor 1b
#define stepPin1b 5 // Step enable pin for Motor 1b
#define dirPin2a 6 // Direction pin for Motor 2a
#define stepPin2a 7 // Step enable pin for Motor 2a
#define dirPin2b 8 // Direction pin for Motor 2b
#define stepPin2b 9 // Step enable pin for Motor 2b

char motor1apower; // Sets stepPin1 for Motor 1 HIGH or LOW (1=HIGH, 0=LOW)
char motor1bpower; // Sets stepPin1 for Motor 1 HIGH or LOW (1=HIGH, 0=LOW)
char motor2apower; // Sets stepPin2 for Motor 2 HIGH or LOW (1=HIGH, 0=LOW)
char motor2bpower; // Sets stepPin2 for Motor 2 HIGH or LOW (1=HIGH, 0=LOW)

char motor1adir; //Direction of Motor 1 (1=Clockwise, 2= Counterclockwise)
char motor1bdir; //Direction of Motor 1 (1=Clockwise, 2= Counterclockwise)
char motor2adir; //Direction of Motor 2 (1=Clockwise, 2= Counterclockwise)
char motor2bdir; //Direction of Motor 2 (1=Clockwise, 2= Counterclockwise)

char m1Amovementstenthou;  //m1a movements raw characters
char m1Amovementsthou;
char m1Amovementshundreds;
char m1Amovementstens;
char m1Amovementsones;

char m1Bmovementstenthou; //m1b movements raw characters
char m1Bmovementsthou;
char m1Bmovementshundreds;
char m1Bmovementstens;
char m1Bmovementsones;

char m2Amovementstenthou; //m2a movements raw characters
char m2Amovementsthou;
char m2Amovementshundreds;
char m2Amovementstens;
char m2Amovementsones;

char m2Bmovementstenthou; //m1a movements raw characters
char m2Bmovementsthou;
char m2Bmovementshundreds;
char m2Bmovementstens;
char m2Bmovementsones;

char motor1Aspeedhundreds; //motor 1A speed raw characters
char motor1Aspeedtens;
char motor1Aspeedones;

char motor1Bspeedhundreds; //motor1B speed raw characters
char motor1Bspeedtens;
char motor1Bspeedones;

char motor2Aspeedhundreds; //motor2A speed raw characters
char motor2Aspeedtens;
char motor2Aspeedones;

char motor2Bspeedhundreds; //motor2B speed raw characters
char motor2Bspeedtens;
char motor2Bspeedones;

char emergencystop; //emergency stop indicator

int m1Amovementstenthoumultiply; //conversion to number for m1a movements
int m1Amovementsthoumultiply;
int m1Amovementshundredsmultiply;
int m1Amovementstensmultiply;
int m1Amovementsonesmultiply;

int m1Bmovementstenthoumultiply; //conversion to number for m1b movements
int m1Bmovementsthoumultiply;
int m1Bmovementshundredsmultiply;
int m1Bmovementstensmultiply;
int m1Bmovementsonesmultiply;

int m2Amovementstenthoumultiply; //conversion to number for m2a movements
int m2Amovementsthoumultiply;
int m2Amovementshundredsmultiply;
int m2Amovementstensmultiply;
int m2Amovementsonesmultiply;

int m2Bmovementstenthoumultiply; //conversion to number for m2b movements
int m2Bmovementsthoumultiply;
int m2Bmovementshundredsmultiply;
int m2Bmovementstensmultiply;
int m2Bmovementsonesmultiply;

int numberofmovements1A; //sum of m1a movement multiples
int numberofmovements1B; //sum of m1b movement multiples
int numberofmovements2A; //sum of m2a movement multiples
int numberofmovements2B; //sum of m2b movement multiples

int motor1Aspeedhundredsmultiple; //conversion to number for motor1A speed
int motor1Aspeedtensmultiple;
int motor1Aspeedonesmultiple;

int motor1Bspeedhundredsmultiple; //conversion to number for motor1B speed
int motor1Bspeedtensmultiple;
int motor1Bspeedonesmultiple;

int motor2Aspeedhundredsmultiple; //conversion to number for motor2A speed
int motor2Aspeedtensmultiple;
int motor2Aspeedonesmultiple;

int motor2Bspeedhundredsmultiple; //conversion to number for motor2B speed
int motor2Bspeedtensmultiple;
int motor2Bspeedonesmultiple;

int m1Astepcounter; //keepts track of the number of steps that each step pin has been set HIGH
int m1Bstepcounter;
int m2Astepcounter;
int m2Bstepcounter;

int speedM1A; //sum of multiples for motor 1A
int speedM1B; //sum of multiples for motor 1B
int speedM2A; //sum of multiples for motor 2A
int speedM2B; //sum of multiples for motor 2B

unsigned long currentMillis = 0; //sets the clock time for all timing functions

unsigned long motor1Aontime = 0; //marks the time since each motors step pin was set HIGH
unsigned long motor1Bontime = 0;
unsigned long motor2Aontime = 0;
unsigned long motor2Bontime = 0;

byte motor1Astate = LOW; //step pin state for each motor
byte motor1Bstate = LOW;
byte motor2Astate = LOW;
byte motor2Bstate = LOW;

const byte numChars = 64;
char receivedChars[numChars];

boolean newData = false;
boolean loopComplete = true;
boolean motor1aFinished = false;
boolean motor1bFinished = false;
boolean motor2aFinished = false;
boolean motor2bFinished = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  recvWithStartEndMarkers();
  useNewData();
  initializeDirection();
  runMotorsTest();

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
    motor1apower = receivedChars[0];
    motor1bpower = receivedChars[1];
    motor2apower = receivedChars[2];
    motor2bpower = receivedChars[3];
    motor1adir = receivedChars[4];
    motor1bdir = receivedChars[5];
    motor2adir = receivedChars[6];
    motor2bdir = receivedChars[7];
    motor1Aspeedhundreds = receivedChars[8];
    motor1Aspeedtens = receivedChars[9];
    motor1Aspeedones = receivedChars[10];
    motor1Bspeedhundreds = receivedChars[11];
    motor1Bspeedtens = receivedChars[12];
    motor1Bspeedones = receivedChars[13];
    motor2Aspeedhundreds = receivedChars[14];
    motor2Aspeedtens = receivedChars[15];
    motor2Aspeedones = receivedChars[16];
    motor2Bspeedhundreds = receivedChars[17];
    motor2Bspeedtens = receivedChars[18];
    motor2Bspeedones = receivedChars[19];
    m1Amovementstenthou = receivedChars[20];
    m1Amovementsthou = receivedChars[21];
    m1Amovementshundreds = receivedChars[22];
    m1Amovementstens = receivedChars[23];
    m1Amovementsones = receivedChars[24];
    m1Bmovementstenthou = receivedChars[25];
    m1Bmovementsthou = receivedChars[26];
    m1Bmovementshundreds = receivedChars[27];
    m1Bmovementstens = receivedChars[28];
    m1Bmovementsones = receivedChars[29];
    m2Amovementstenthou = receivedChars[30];
    m2Amovementsthou = receivedChars[31];
    m2Amovementshundreds = receivedChars[32];
    m2Amovementstens = receivedChars[33];
    m2Amovementsones = receivedChars[34];
    m2Bmovementstenthou = receivedChars[35];
    m2Bmovementsthou = receivedChars[36];
    m2Bmovementshundreds = receivedChars[37];
    m2Bmovementstens = receivedChars[38];
    m2Bmovementsones = receivedChars[39];
    emergencystop = receivedChars[40];

    m1Amovementstenthoumultiply = 10000 * (m1Amovementstenthou - 48);
    m1Amovementsthoumultiply = 1000 * (m1Amovementsthou - 48);
    m1Amovementshundredsmultiply = 100 * (m1Amovementshundreds - 48);
    m1Amovementstensmultiply = 10 * (m1Amovementstens - 48);
    m1Amovementsonesmultiply = 1 * (m1Amovementsones - 48);

    m1Bmovementstenthoumultiply = 10000 * (m1Bmovementstenthou - 48);
    m1Bmovementsthoumultiply = 1000 * (m1Bmovementsthou - 48);
    m1Bmovementshundredsmultiply = 100 * (m1Bmovementshundreds - 48);
    m1Bmovementstensmultiply = 10 * (m1Bmovementstens - 48);
    m1Bmovementsonesmultiply = 1 * (m1Bmovementsones - 48);

    m2Amovementstenthoumultiply = 10000 * (m2Amovementstenthou - 48);
    m2Amovementsthoumultiply = 1000 * (m2Amovementsthou - 48);
    m2Amovementshundredsmultiply = 100 * (m2Amovementshundreds - 48);
    m2Amovementstensmultiply = 10 * (m2Amovementstens - 48);
    m2Amovementsonesmultiply = 1 * (m2Amovementsones - 48);

    m2Bmovementstenthoumultiply = 10000 * (m2Bmovementstenthou - 48);
    m2Bmovementsthoumultiply = 1000 * (m2Bmovementsthou - 48);
    m2Bmovementshundredsmultiply = 100 * (m2Bmovementshundreds - 48);
    m2Bmovementstensmultiply = 10 * (m2Bmovementstens - 48);
    m2Bmovementsonesmultiply = 1 * (m2Bmovementsones - 48);

    motor1Aspeedhundredsmultiple = 100 * (motor1Aspeedhundreds - 48);
    motor1Aspeedtensmultiple = 10 * (motor1Aspeedtens - 48);
    motor1Aspeedonesmultiple =  1 * (motor1Aspeedones - 48);

    motor1Bspeedhundredsmultiple = 100 * (motor1Bspeedhundreds - 48);
    motor1Bspeedtensmultiple = 10 * (motor1Bspeedtens - 48);
    motor1Bspeedonesmultiple =  1 * (motor1Bspeedones - 48);

    motor2Aspeedhundredsmultiple = 100 * (motor2Aspeedhundreds - 48);
    motor2Aspeedtensmultiple = 10 * (motor2Aspeedtens - 48);
    motor2Aspeedonesmultiple =  1 * (motor2Aspeedones - 48);

    motor2Bspeedhundredsmultiple = 100 * (motor2Bspeedhundreds - 48);
    motor2Bspeedtensmultiple = 10 * (motor2Bspeedtens - 48);
    motor2Bspeedonesmultiple =  1 * (motor2Bspeedones - 48);

    numberofmovements1A = (m1Amovementstenthoumultiply + m1Amovementsthoumultiply +
                           m1Amovementshundredsmultiply + m1Amovementstensmultiply +
                           m1Amovementsonesmultiply);

    numberofmovements1B = (m1Bmovementstenthoumultiply + m1Bmovementsthoumultiply +
                           m1Bmovementshundredsmultiply + m1Bmovementstensmultiply +
                           m1Bmovementsonesmultiply);

    numberofmovements2A = (m2Amovementstenthoumultiply + m2Amovementsthoumultiply +
                           m2Amovementshundredsmultiply + m2Amovementstensmultiply +
                           m2Amovementsonesmultiply);

    numberofmovements2B = (m2Bmovementstenthoumultiply + m2Bmovementsthoumultiply +
                           m2Bmovementshundredsmultiply + m2Bmovementstensmultiply +
                           m1Amovementsonesmultiply);

    speedM1A = (motor1Aspeedhundredsmultiple + motor1Aspeedtensmultiple + motor1Aspeedonesmultiple);
    speedM1B = (motor1Bspeedhundredsmultiple + motor1Bspeedtensmultiple + motor1Bspeedonesmultiple);
    speedM2A = (motor2Aspeedhundredsmultiple + motor2Aspeedtensmultiple + motor2Aspeedonesmultiple);
    speedM2B = (motor2Bspeedhundredsmultiple + motor2Bspeedtensmultiple + motor2Bspeedonesmultiple);

    newData = false;
    loopComplete = false;
    motor1aFinished = false;
    motor1bFinished = false;
    motor2aFinished = false;
    motor2bFinished = false;
  }
}

void initializeDirection() {
  if (loopComplete == false) {
    if (motor1adir == '1') {
      digitalWrite(dirPin1a, HIGH);
    }
    else {
      digitalWrite(dirPin1a, LOW);
    }
    if (motor1bdir == '1') {
      digitalWrite(dirPin1b, HIGH);
    }
    else {
      digitalWrite(dirPin1b, LOW);
    }
    if (motor2adir == '1') {
      digitalWrite(dirPin2a, HIGH);
    }
    else {
      digitalWrite(dirPin2a, LOW);
    }
    if (motor2bdir == '1') {
      digitalWrite(dirPin2b, HIGH);
    }
    else {
      digitalWrite(dirPin2b, LOW);
    }
  }
}

void runMotorsTest() {
  unsigned long currentMillis = millis();
  if (loopComplete == false) {
    if (motor1apower == '1') {
      if (motor1Astate == LOW) {
        if (m1Astepcounter < numberofmovements1A) {
          if (currentMillis - motor1Aontime >= speedM1A) {
            motor1Astate = HIGH;
            motor1Aontime = currentMillis;
            m1Astepcounter++;
            emergencyStop();
          }
        }
        else {
          motor1aFinished = true;
        }
      }
      else {
        if (currentMillis - motor1Aontime >= 0) {
          motor1Astate = LOW;
          motor1Aontime = currentMillis;
          emergencyStop();
        }
      }
    }
    else {
      motor1aFinished = true;
    }

    if (motor1bpower == '1') {
      if (motor1Bstate == LOW) {
        if (m1Bstepcounter < numberofmovements1B) {
          if (currentMillis - motor1Bontime >= speedM1B) {
            motor1Bstate = HIGH;
            motor1Bontime = currentMillis;
            m1Bstepcounter++;
            emergencyStop();
          }
        }
        else {
          motor1bFinished = true;
        }
      }
      else {
        if (currentMillis - motor1Bontime >= 0) {
          motor1Bstate = LOW;
          motor1Bontime = currentMillis;
          emergencyStop();
        }
      }
    }
    else {
      motor1bFinished = true;
    }

    if (motor2apower == '1') {
      if (motor2Astate == LOW) {
        if (m2Astepcounter < numberofmovements2A) {
          if (currentMillis - motor2Aontime >= speedM2A) {
            motor2Astate = HIGH;
            motor2Aontime = currentMillis;
            m2Astepcounter++;
            emergencyStop();
          }
        }
        else {
          motor2aFinished = true;
        }
      }
      else {
        if (currentMillis - motor2Aontime >= 0) {
          motor2Astate = LOW;
          motor2Aontime = currentMillis;
          emergencyStop();
        }
      }
    }
    else {
      motor2aFinished = true;
    }

    if (motor2bpower == '1') {
      if (motor2Bstate == LOW) {
        if (m2Bstepcounter < numberofmovements2B) {
          if (currentMillis - motor2Bontime >= speedM2B) {
            motor2Bstate = HIGH;
            motor2Bontime = currentMillis;
            m2Bstepcounter++;
            emergencyStop();
          }
        }
        else {
          motor2bFinished = true;
        }
      }
      else {
        if (currentMillis - motor2Bontime >= 0) {
          motor2Bstate = LOW;
          motor2Bontime = currentMillis;
          emergencyStop();
        }
      }
    }
    else {
      motor2bFinished = true;
    }

    digitalWrite(stepPin1a, motor1Astate);
    emergencyStop();
    digitalWrite(stepPin1b, motor1Bstate);
    emergencyStop();
    digitalWrite(stepPin2a, motor2Astate);
    emergencyStop();
    digitalWrite(stepPin2b, motor2Bstate);
    emergencyStop();

    if (motor1aFinished == true && motor1bFinished == true && motor2aFinished == true && motor2bFinished == true) {
      loopComplete = true;
      m1Astepcounter = 0;
      motor1Aontime = 0;
      m1Bstepcounter = 0;
      motor1Bontime = 0;
      m2Astepcounter = 0;
      motor2Aontime = 0;
      m2Bstepcounter = 0;
      motor2Bontime = 0;
      digitalWrite(stepPin1a, LOW);
      digitalWrite(stepPin1b, LOW);
      digitalWrite(stepPin2a, LOW);
      digitalWrite(stepPin2b, LOW);
    }
  }
}

void emergencyStop() {
  recvWithStartEndMarkers();
  useNewData();
  if (emergencystop == '1') {
    loopComplete = true;
    m1Astepcounter = 0;
    motor1Aontime = 0;
    m1Bstepcounter = 0;
    motor1Bontime = 0;
    m2Astepcounter = 0;
    motor2Aontime = 0;
    m2Bstepcounter = 0;
    motor2Bontime = 0;
    digitalWrite(stepPin1a, LOW);
    digitalWrite(stepPin1b, LOW);
    digitalWrite(stepPin2a, LOW);
    digitalWrite(stepPin2b, LOW);
    digitalWrite(dirPin1a, LOW);
    digitalWrite(dirPin1b, LOW);
    digitalWrite(dirPin2a, LOW);
    digitalWrite(dirPin2b, LOW);
    motor1aFinished = false;
    motor1bFinished = false;
    motor2aFinished = false;
    motor2bFinished = false;
  }
}
