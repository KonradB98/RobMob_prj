#define GRANICA 250 // poniżej białe, powyżej czarne

const int left_sensor_pin =A0; //Ustawienie pinu analogowego podlaczonego do lewego czujnika
int left_sensor_state; //Wartosc odczytu z lewego czujnika

const int right_sensor_pin =A1; // Analogicznie jak wyżej tylko tym razem dla prawego czujnika
int right_sensor_state;
  
void setup() {
Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika 
}

void loop() {
 left_sensor_state = analogRead(left_sensor_pin);//Przypisanie wartosci sczytanej z lewego czujnika
 right_sensor_state = analogRead(right_sensor_pin);//Przypisanie wartosci sczytanej z prawego czujnika
 
 //Wypisanie komunikatu i wartosci odczytu do konsoli
  Serial.print("Lewy czujnik: ");
  Serial.println(left_sensor_state);
  delay(1000);
  Serial.print("Prawy czujnik: ");
  Serial.println(right_sensor_state);
  delay(1000);
}
