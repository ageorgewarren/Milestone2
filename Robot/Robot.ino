#include <Servo.h>

Servo leftservo;                  // name left servo
Servo rightservo;                 // name right servo

const int Left = 13;
const int Right = 8;
int  leftvalue = 94;
int rightvalue = 94;
int x; 
int y;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
int integer0FromPC = 0;
int integer1FromPC = 0;
int integer2FromPC = 0;

int robotnum = 1;

boolean newData = false;

void setup() {
Serial.begin(9600);
Serial1.begin(9600);

leftservo.attach(8);                // declare pin left servo is connected to
leftservo.write(94);                // write left servo to start at midpoint
rightservo.attach(9);                // declare pin right servo is connected to
rightservo.write(94);                // write right servo to start at midpoint



pinMode(Left,OUTPUT);
pinMode(Right,OUTPUT);
Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
Serial.println();
}
void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }

    if(integer1FromPC > 0)
    {
      x = map(integer1FromPC, 0, 255, 95, 180);   // map values from motor format to servo format
    }

    if(integer1FromPC < 0)
    {
      x = map(integer1FromPC, -1, -255, 93, 0);   // map values from motor format to servo format
    }

    if(integer1FromPC == 0)
    {
      x = 94;                                     // set value of motor equal to 0 RPM
    }

    if(integer2FromPC > 0)
    {
      y = map(integer2FromPC, 0, 255, 95, 180);   // map values from motor format to servo format
    }

    if(integer2FromPC < 0)
    {
      y = map(integer2FromPC, -1, -255, 93, 0);   // map values from motor format to servo format
    }

     if(integer2FromPC == 0)
    {
      y = 94;                                     // set value of motor equal to 0 RPM
    }
    

    if(robotnum == integer0FromPC)                // check if this robot should be receiving driving intstructions
    {
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
      delay(10);
      rightservo.write(rightvalue);           // write value of 2nd integer to right motor
      delay(10);
    }

    if(integer0FromPC != robotnum)                // check if this robot should not be receiving the command
    {             
      leftservo.write(94);                        // turn off left motor
      delay(10);
      rightservo.write(94);                       // turn off right motor
      delay(10);
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


