
//============ Change these for each robot
const int robotnum = 1;     // Robot Number (1, 2, 3)
const int LF=12;            // Left Motor Forward
const int LR=13;            // Left Motor Reverse
const int RF=10;            // Right Motor Reverse
const int RR=11;            // Right  Motor Reverse

//============

int DesiredRobot;
int leftvalue;
int rightvalue;
int x; 
int y;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        



boolean newData = false;

void setup() {
Serial.begin(9600);
Serial1.begin(9600);

  pinMode(LF,OUTPUT);
  pinMode(LR,OUTPUT);
  pinMode(RF,OUTPUT);
  pinMode(RR,OUTPUT);


}


void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        newData = false;
    }

    if(DesiredRobot == robotnum)                // Send received values to motor
    {
      motorMapping();
      debug();
    }

    if(DesiredRobot != robotnum)                // check if this robot should not be receiving the command
    {             
      motorOff();
      debug();
    }
    
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

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

//============

void parseData() {      
  // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    DesiredRobot = atoi(strtokIndx);
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    x = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    y = atoi(strtokIndx);     // convert this part to a float

}

//============

void debug() {
    Serial.print("<");
    Serial.print(DesiredRobot);
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(leftvalue);
    Serial.print(",");
    Serial.print(rightvalue);
    Serial.println(">");
}

//============

void motorMapping(){
      if(y> 0 && x > 0)                      // quadrant 1
      {
        leftvalue = y;
        rightvalue = 255 - x;
      }

      if(y > 0 && x == 0)                       // all the way up
      {
        leftvalue = y;
        rightvalue = y;
      }

      if (y > 0 && x < 0)                       // quadrant 2
      {
        leftvalue = 255 + x;
        rightvalue = y;
      }

      if(y == 0 && x < 0)                       // all the way left
      {
        leftvalue = x;
        rightvalue = x * -1;
      }

      if(y < 0 && x < 0)                        // quadrant 3
      {
        leftvalue =  0 + x;
        rightvalue = y;
      }

      if( y < 0 && x==0)                         // all the way down
      {
        leftvalue = y;
        rightvalue = y;
      }

      if( y < 0 && x > 0)                       // quadrant 4
      {
        leftvalue = y;
        rightvalue = 255-x;
      }

      if( y == 0 && x > 0 )                      // all the way right
      {
        leftvalue = x;
        rightvalue = -1*x;
      }

      if( y == 0 && x ==0)
      {
        leftvalue = 0;
        rightvalue = 0;
      }

      if(leftvalue>0){
      analogWrite(LF,leftvalue);            // write value of 1st integer to left motor 
      analogWrite(LR,0);                    // write value of 2nd integer to right motor
      delay(10);
      }
      if(rightvalue>0){
      analogWrite(RF,rightvalue);           // write value of 2nd integer to right motor
      analogWrite(RR,0);                    // write value of 2nd integer to right motor
      delay(10);
      }
      if(leftvalue<0){
      analogWrite(LF,0);                    // write value of 1st integer to left motor 
      analogWrite(LR, (-1*leftvalue));            // write value of 2nd integer to right motor
      delay(10);
      }
      if(rightvalue<0){
      analogWrite(RF,0);                    // write value of 2nd integer to right motor
      analogWrite(RR, (-1*rightvalue));           // write value of 2nd integer to right motor
      delay(10);
      }

      if(leftvalue==0 && rightvalue==0){
        motorOff();
      }
  }

  //============

  void motorOff(){
      analogWrite(LF,0);                       // turn off left motor
      analogWrite(LR,0);                       // turn off right motor
      analogWrite(RF,0);                       // turn off left motor
      analogWrite(RR,0);                       // turn off right motor
      delay(10);
  }


