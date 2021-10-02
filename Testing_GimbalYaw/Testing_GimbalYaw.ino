/*Testing Code for ANT Servo Motors
Alison Gonzales
Ana Sofia Cardenas Beltran
*/

#include <Servo.h>

//Servo servoP;
//Servo servoR;
Servo servoY;

void setup() {
  
  //servoP.attach(3);
  //servoR.attach(5);
  servoY.attach(9); 
}

void loop() {
  
   servoY.read();
   
   if (plane_pos <= 0 && plane_pos >= 45) {
    valY = 450 - plane_pos;
   }
   
   else if (plane_pos <= 315 && plane_pos >= 360) {
    valY = 360 - plane_pos;
   }

   //favor_west
   else if (plane_pos < 45 && plane_pos >= 135) {
    valY = 90 - plane_pos;   
   }

  //favor east
  else if (plane_pos <= 225 && plane_pos >= 315) {
    valY = 270 - plane_pos;
  }

  //favor south
  else if (plane_pos <= 135 && plane_pos > 225) {
    valY = 180 - plane_pos;
  }
}
