#define GRANICA 250 // poniżej białe, powyżej czarne
#define LED 13
//Piny mostka H
const int motorAspeed  = 5; 
const int motorA1      = 6;  
const int motorA2      = 7; 
  
const int motorB1      = 8; 
const int motorB2      = 9; 
const int motorBspeed  = 10; 

const int left_sensor_pin =A0; //Ustawienie pinu analogowego podlaczonego do lewego czujnika
int left_sensor_state; //Wartosc odczytu z lewego czujnika

const int right_sensor_pin =A1; // Analogicznie jak wyżej tylko tym razem dla prawego czujnika
int right_sensor_state;
  
void setup() {
 pinMode(motorA1, OUTPUT);
 pinMode(motorA2, OUTPUT);
 pinMode(motorB1, OUTPUT);
 pinMode(motorB2, OUTPUT);
 pinMode(LED, OUTPUT); 
 digitalWrite(LED, 0); //Wylaczenie diody
Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika 
}

void loop() {

   digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  
left_sensor_state = analogRead(left_sensor_pin);
right_sensor_state = analogRead(right_sensor_pin);
 
  /*  analogWrite (motorAspeed, 200);  //robot skręca w lewo
    analogWrite (motorBspeed, 150);
  delay(5000);
    analogWrite (motorAspeed, 150); //robot skręca w prawo
    analogWrite (motorBspeed, 200);
  delay(5000);*/
 
if((left_sensor_state < GRANICA) && (right_sensor_state < GRANICA))
  {
    analogWrite (motorAspeed, 200); //jedź prosto jeśli 'widzisz' biale
    analogWrite (motorBspeed, 200);
  }

  if((left_sensor_state > GRANICA) && (right_sensor_state > GRANICA))
  {
    analogWrite (motorAspeed, 0); //stój jeśli 'widzisz' czarne
    analogWrite (motorBspeed, 0);
  }

}
