// I DELAY DI ARDUINO VANNO SETTATI PER SINCRONIZZARE PYTHON

int BUTTON = 2  ;           // pin di input dove è collegato il pulsante  
int  val; 
int ledPin =  13;      // the number of the LED pin  

boolean stato = false;

void setup() {  
 
  pinMode(BUTTON, INPUT);
  //digitalWrite(BUTTON, LOW);// imposta il pin digitale come input  
  pinMode(ledPin, OUTPUT);
  boolean stato = false;
  Serial.begin(9600);
}  
  
void loop() { 
  delay(100);
  val = digitalRead(BUTTON); 
  
  Serial.println(val);
  if(val == LOW){
    stato = not(stato);
  }
  if (stato) {
      Serial.println("ON");
      // LED on:
      digitalWrite(ledPin, HIGH);
  }
  else {
    Serial.println("OFF");
    digitalWrite(ledPin, LOW);
  }
   
 }  
  
