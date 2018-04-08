
int QTI1; 
int QTI2;
const int QTIsense1=5;  // Front QTI sensor
const int QTIsense2=4;  // Back QTI sensor

void setup() {
 Serial.begin(9600);

}

void loop() {

  QTI1 = QTIVal(QTIsense1); 
  QTI2 = QTIVal(QTIsense2); 
 Serial.print(QTI1);
 Serial.print(',');
 Serial.println(QTI2);
 

 
}

long QTIVal(int sensorIn){
   long duration = 0;
   pinMode(sensorIn, OUTPUT);     
   digitalWrite(sensorIn, HIGH);  
   delay(1);                      
   pinMode(sensorIn, INPUT);      
   digitalWrite(sensorIn, LOW);   
   while(digitalRead(sensorIn)){  
      duration++;
   }
   return duration;
}
