
int ls1; 

void setup() {
 Serial.begin(9600);

}

void loop() {

  ls1 = RCTime(6); // replace this with the pin you attatched the WHITE wire of the LEFT qti sensor too
 Serial.println(ls1);

 
}

long RCTime(int sensorIn){
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
