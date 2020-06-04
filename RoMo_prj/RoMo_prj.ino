#define GRANICA 250 // poniżej białe, powyżej czarne
int vSpeed = 150;        // predkosc obrotow silnika -- MAX 255
//Prawy silnik
const int motorAspeed  = 5; 
const int motorA1      = 6;  
const int motorA2      = 7; 
//Lewy silnik  
const int motorB1      = 8; 
const int motorB2      = 9; 
const int motorBspeed  = 10; 

const int left_sensor_pin =A0; //Ustawienie pinu analogowego podlaczonego do lewego czujnika
int left_sensor_state; //Wartosc odczytu z lewego czujnika

const int right_sensor_pin =A1; // Analogicznie jak wyżej tylko tym razem dla prawego czujnika
int right_sensor_state;

float e,last_e,e_sum,e_d,Pout,Iout,Dout,tau;
unsigned long currentTime, previousTime;
//Nastawy regulatora
const float Kp = 25;  
const float Ki = 0;
const float Kd = 4;  
  
void setup() {
 pinMode(motorA1, OUTPUT);
 pinMode(motorA2, OUTPUT);
 pinMode(motorB1, OUTPUT);
 pinMode(motorB2, OUTPUT);
 e = last_e = e_sum = e_d = Pout = Iout = Dout = 0;
 Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika 
}

void loop() {
 
/* Serial.print("Lewy czujnik: ");
Serial.println(left_sensor_state);
delay(500);
Serial.print("Prawy czujnik: ");
Serial.println(right_sensor_state);
delay(250); */

  //Ustawienie kierunku jazdy robota (Do przodu)
 // goForward();
 e = readSensors();
 if(e != 100)
 {
    regMotors(e);
 }
 else{
    Serial.println("STOP");
    //delay(1000);
 }
 
}

void goForward(){
  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,HIGH);
}

float readSensors (){
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  goForward();
  //Jedz prosto
  if((left_sensor_state < GRANICA) && (right_sensor_state < GRANICA))
  {
    e = 0;
      analogWrite (motorAspeed, vSpeed);
      analogWrite (motorBspeed, vSpeed);
  }//Jedz w prawo  
  else if((left_sensor_state > GRANICA) && (right_sensor_state < GRANICA))
  {
    e = 2;
      analogWrite (motorAspeed, 0);
      analogWrite (motorBspeed, vSpeed);
  }//Jedz w lewo  
  else if((left_sensor_state < GRANICA) && (right_sensor_state > GRANICA))
  {
    e = -2;
      analogWrite (motorAspeed, vSpeed);
      analogWrite (motorBspeed, 0);
  }//STOP    
  else if((left_sensor_state > GRANICA) && (right_sensor_state > GRANICA))
  {
    e = 100;
      analogWrite (motorAspeed, 0);
      analogWrite (motorBspeed, 0);
  } 
  return e;
}

float computePID(float er){
  
  Pout = Kp*er;
  
  e_sum += (er);
  Iout = Ki*e_sum;

  e_d = (er - last_e);
  Dout = Kd*e_d;
   
  float out = (Pout + Iout + Dout);
  
  last_e = er;
  previousTime = currentTime;
  return out;
}

void regMotors(float er){
  float PID = computePID(er);

  int l_motor = vSpeed + (int)PID;
  int r_motor = vSpeed - (int)PID;
  
  l_motor = constrain(l_motor,0,250);
  r_motor = constrain(r_motor,0,250);

   analogWrite (motorAspeed,r_motor);
   analogWrite (motorBspeed,l_motor);
  
  //Serial.print("Predkosc silnika lewego  = ");
  //Serial.println(l_motor);
  //Serial.print(" ");
  //Serial.print("Predkosc silnika prawego  = ");
  //Serial.println(r_motor);
  //delay(100);
  
}
