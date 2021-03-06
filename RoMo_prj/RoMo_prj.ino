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
int pid_motors;

//Nastawy regulatora
const float Kp = 41;  
const float Ki = 0;
const float Kd = 8; 
   
void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
 
  pid_init(&pid, Kp, Ki, Kd, 10, 1);
  pid.p_max = pid_scale(&pid, 255);
  pid.p_min = pid_scale(&pid, -255);
  pid.i_max = pid_scale(&pid, 255);
  pid.i_min = pid_scale(&pid, -255);
  pid.d_max = pid_scale(&pid, 255);
  pid.d_min = pid_scale(&pid, -255);
  pid.total_max = pid_scale(&pid, 255);
  pid.total_min = pid_scale(&pid, -255);
  
  Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika
  Serial.println("lewy prawy"); 
}

void loop() {
/*
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  Serial.println(left_sensor_state);
  Serial.print(",");             
  Serial.println(right_sensor_state);   
  */
  
  int32_t error = readSensors ();
  if(error != 100){
    goForward();
    pid_motors = pid_calc(&pid,error);
    int l_motor = vSpeed + pid_motors;
    int r_motor = vSpeed - pid_motors;
    if(l_motor > 255)
      l_motor = 255;
    else if (l_motor < 0)
      l_motor = 0;
  
    if(r_motor > 255)
      r_motor = 255;
    else if (r_motor < 0)
      r_motor = 0;
  //Serial.print(l_motor);      
  //Serial.print(",");             
  //Serial.println(r_motor);  
     analogWrite (motorAspeed,r_motor);
     analogWrite (motorBspeed,l_motor); 
  }
  else{
     Stop();
  }
   
  delay(20);
  
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

void pid_init(pid_t * pid, float p, float i, float d, uint8_t f, int32_t dt_ms) {
  uint32_t k;
  pid->power = 1;
  for (k = 0; k < f; ++k) {
    pid->power = pid->power * 2;
  }
  pid->f = f;
  pid->p = (int32_t) (p * pid->power);
  pid->i = (int32_t) (i * pid->power);
  pid->d = (int32_t) (d * pid->power);
  pid->p_val = 0;
  pid->i_val = 0;
  pid->d_val = 0;
  pid->p_max = INT32_MAX;
  pid->p_min = INT32_MIN;
  pid->i_max = INT32_MAX;
  pid->i_min = INT32_MIN;
  pid->d_max = INT32_MAX;
  pid->d_min = INT32_MIN;
  pid->e_last = 0;
  pid->sum = 0;
  pid->total_max = INT32_MAX;
  pid->total_min = INT32_MIN;
  pid->dt_ms = dt_ms;
}

int32_t pid_calc(pid_t * pid, int32_t err) {
  int32_t p, i, d, e, total;
  //--------------P-------------//
  e = err;
  p = pid->p * e;

  if (p > pid->p_max)
    p = pid->p_max;
  else if (p < pid->p_min)
    p = pid->p_min;
  pid->p_val = p >> pid->f;
 //--------------I-------------//
  i = pid->sum;
  i +=(pid->i * e);
  if (i > pid->i_max)
    i = pid->i_max;
  else if (i < pid->i_min)
    i = pid->i_min;
  pid->sum = i;
  pid->i_val = i >> pid->f;
 //--------------D-------------//
  d = (pid->d * (e - pid->e_last));
  if (d > pid->d_max)
    d = pid->d_max;
  else if (d < pid->d_min)
    d = pid->d_min;
  pid->d_val = d >> pid->f;
  //Wartosc sygnalu
  total = p + i + d;
  if (total > pid->total_max)
    total = pid->total_max;
  else if (total < pid->total_min)
    total = pid->total_min;
  
  pid->control = total >> pid->f;
  pid->e_last = e;

  return pid->control;
}

int32_t pid_scale(pid_t * pid, float v) {
  return v * pid->power;
}
