#include <DSXLite.h>

DSX_pinMotors motor(2,3,4,5); // pin motors input (Digital pins)

void setup(){
  motor.pinMotors();

}

void loop(){
  motor.forward(); // if the motors is forward
  delay(2000);
  motor.stops(); // motor stops
  delay(500);
  motor.backward(); // if the motors is backward
  delay(2000);
  motor.stops();
  delay(500);
  motor.turnLeft(); // if the motors turns left
  delay(2000);
  motor.stops();
  delay(500);
  motor.turnRight(); // if the motors turns right
  delay(2000);
  motor.stops();
  delay(500);
  
}
