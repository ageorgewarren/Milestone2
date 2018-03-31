#include <Servo.h>

Servo leftservo;                  // name left servo
Servo rightservo;                 // name right servo
//============ Change these for each robot
const int robotnum = 1;     // Robot Number (1, 2, 3)
const int LeftServeNum=8;   // Left Servo Pin Number
const int RightServeNum=9;  // Left Servo Pin Number

//============

int leftvalue = 94;
int rightvalue = 94;
int x; 
int y;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        
int integer0FromPC = 0;
int integer1FromPC = 0;
int integer2FromPC = 0;


boolean newData = false;

void setup() {
Serial.begin(9600);
Serial1.begin(9600);

leftservo.attach(LeftServeNum);                // declare pin left servo is connected to
leftservo.write(94);                // write left servo to start at midpoint
rightservo.attach(RightServeNum);               // declare pin right servo is connected to
rightservo.write(94);               // write right servo to start at midpoint

pinMode(Left,OUTPUT);
pinMode(Right,OUTPUT);
}


void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        showParsedData();
        newData = false;
    }

    if(integer1FromPC > 0)                        // Send received values to servo
    {
      servoMapping();
    }

    if(integer0FromPC != robotnum)                // check if this robot should not be receiving the command
    {             
      servoOff();
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

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    //strcpy(integer0FromPC, strtokIndx); // copy it to messageFromPC
    integer0FromPC = atoi(strtokIndx);
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integer1FromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    integer2FromPC = atoi(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("<");
    Serial.print(integer1FromPC);
    Serial.print(",");
    Serial.print(integer2FromPC);
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

void servoMapping(){
        if(y>=95 && x >= 95)                        // quadrant 1
      {
        leftvalue = y;
        rightvalue = 94 - (x-94);
      }

      if(y >=95 && x == 94)                       // all the way up
      {
        leftvalue = y;
        rightvalue = map(y, 95, 176, 93, 0);
      }

      if (y >= 95 && x<=93)                       // quadrant 2
      {
        leftvalue = 94 + (x-94);
        rightvalue = y;
      }

      if(y == 94 && x <= 93)                       // all the way left
      {
        leftvalue = x;
        rightvalue = x;
      }

      if(y <= 93 && x<=93)                        // quadrant 3
      {
        leftvalue =  94 - x;
        //rightvalue = map(y, 93, 0, 95, 180);
        rightvalue = 94+(94-y);
      }

      if( y<=93 && x==94)                         // all the way down
      {
        leftvalue = y;
        rightvalue = 94+(94-y);
      }

      if( y<= 93 && x>= 95)                       // quadrant 4
      {
        leftvalue = y;
        rightvalue = 180 + (94-x);
      }

      if( y ==94 && x >= 95)                      // all the way right
      {
        leftvalue= x;
        rightvalue = x;
      }

      if( y==94 && x ==94)
      {
        leftvalue = 94;
        rightvalue = 94;
      }
        
      leftservo.write(leftvalue);            // write value of 1st integer to left motor 
      rightservo.write(rightvalue);           // write value of 2nd integer to right motor
      delay(10);
  }

  //============

  void servoOff(){

      leftservo.write(94);                        // turn off left motor
      rightservo.write(94);                       // turn off right motor
      delay(10);
  }


