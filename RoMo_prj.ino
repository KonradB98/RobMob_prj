const int left_sensor_pin =A0; //Ustawienie pinu analogowego
int left_sensor_state; //Wartosc odczytu z lewego czujnika
  
void setup() {
Serial.begin(9600);//Otworzenie portu szeregowego do monitorowania danych z czujnika 
}

void loop() {
 left_sensor_state = analogRead(left_sensor_pin);//Przypisanie wartosci sczytanej z lewego czujnika
 //Wypisanie komunikatu i wartosci odczytu do konsoli
  Serial.print("Lewy czujnik: ");
  Serial.println(left_sensor_state);
  delay(250);
}
