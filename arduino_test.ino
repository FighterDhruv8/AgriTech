#include <Servo.h>

Servo myservo;

byte pump_mechanism_pin_1 = 2;
byte pump_mechanism_pin_2 = 3;
unsigned int pump_mechanism_time = 1000;

byte soil_mechanism_pin_1 = 4;
byte soil_mechanism_pin_2 = 5;
unsigned int soil_mechanism_time = 500;

void setup()
{
  myservo.attach(9);                                          // Pin 9 for servomotor
  pinMode(soil_mechanism_pin_1, OUTPUT);
  pinMode(soil_mechanism_pin_2, OUTPUT);
  pinMode(pump_mechanism_pin_1, OUTPUT);
  pinMode(pump_mechanism_pin_2, OUTPUT);
  Serial.begin(9600);
}
void loop()
{

  // Servomotor routine
  for (int pos = 0; pos <= 180; pos += 1)                     // goes from 0 degrees to 180 degrees in steps of 1 degree
  {
    myservo.write(pos);                                       // tell servo to go to position in variable 'pos'
    delay(15);                                                // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 1)                     // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);                                       // tell servo to go to position in variable 'pos'
    delay(15);
  }
  delay(2000);

  // Motor Control
  if(Serial.available() > 0)
  {
    short i = 0;
    char* buf = (char*) malloc(256);
    Serial.readString().toCharArray(buf, 256);                // Convert serial input string to char array
    while((*(buf+i)) != '\0')                                 // Iterate through serial input string as a character array
    {
      if( (*(buf+i)) == '\n')                                 // Ignore newline character
      {
        i++;
        continue;
      }
      else if( (*(buf+i)) == 'f')                             // f = Soil moisture sensor forward
      {
        Serial.println("Soil Motor Forward");
        digitalWrite(soil_mechanism_pin_1, HIGH);
        digitalWrite(soil_mechanism_pin_2, LOW);
        delay(soil_mechanism_time);
        digitalWrite(soil_mechanism_pin_1, LOW);
      }
      else if( (*(buf+i)) == 'b')                             // b = Soil moisture sensor reverse
      {
        Serial.println("Soil Motor Backward");
        digitalWrite(soil_mechanism_pin_1, LOW);
        digitalWrite(soil_mechanism_pin_2, HIGH);
        delay(soil_mechanism_time);
        digitalWrite(soil_mechanism_pin_2, LOW);
      }
      else if( (*(buf+i)) == 'x')                             // x = Pump forward
      {
        Serial.println("pump Motor Forward");
        digitalWrite(pump_mechanism_pin_1, HIGH);
        digitalWrite(pump_mechanism_pin_2, LOW);
        delay(pump_mechanism_time);
        digitalWrite(pump_mechanism_pin_1, LOW);
      }
      else if( (*(buf+i)) == 'y')                             // y = Pump reverse
      {
        Serial.println("Soil Motor Backward");
        digitalWrite(pump_mechanism_pin_1, LOW);
        digitalWrite(pump_mechanism_pin_2, HIGH);
        delay(pump_mechanism_time);
        digitalWrite(pump_mechanism_pin_2, LOW);
      }
      else if( (*(buf+i)) == 32)                              // Space = Delay
      {
        Serial.println("Delay");
        delay(4000);
      }
      i++;
    }
    free(buf);
  }
}