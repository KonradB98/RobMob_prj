#define GRANICA 250 // poniżej białe, powyżej czarne
int vSpeed = 120;        // predkosc obrotow silnika -- MAX 255

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
//----------------PID-----------------//

struct pid_t{
  int32_t p;
  int32_t i;
  int32_t d;
  int32_t p_val;
  int32_t i_val;
  int32_t d_val;
  int32_t p_max;
  int32_t i_max;
  int32_t d_max;
  int32_t p_min;
  int32_t i_min;
  int32_t d_min;
  uint8_t f;
  uint32_t power;
  int32_t e_last;
  int32_t sum;
  int32_t total_max;
  int32_t total_min;
  int32_t control;
  int32_t dt_ms;
};

struct pid_t pid;


//Nastawy regulatora
const float Kp = 1;  
const float Ki = 0;
const float Kd = 0; 
   
void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  
  Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika
}

void loop() {
  Serial.println(Kp);
  
}
int32_t readSensors (){
  int32_t e;
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  //Jedz prosto
  if((left_sensor_state < GRANICA) && (right_sensor_state < GRANICA))
  {
    e = 0;
  }//Jedz w prawo  
  else if((left_sensor_state > GRANICA) && (right_sensor_state < GRANICA))
  {
    e = 2;
  }//Jedz w lewo  
  else if((left_sensor_state < GRANICA) && (right_sensor_state > GRANICA))
  {
    e = -2;
  }//STOP    
  else if((left_sensor_state > GRANICA) && (right_sensor_state > GRANICA))
  {
    e = 100;
  } 
  return e;
}

void goForward(){
  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,HIGH);
}

void Stop(){
  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,LOW);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,LOW);
}
