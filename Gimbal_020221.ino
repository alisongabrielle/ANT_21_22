#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>

ros::NodeHandle nh;

Servo servoP; //Pitch
Servo servoR; //Roll
Servo servoY; //Yaw

//Servo Setup 
void servo_Pitch (const std_msgs::UInt16& angleP) {
    servoP.write(angleP.data);
    digitalWrite(13, HIGH-digitalRead(13));
}

void servo_Roll (const std_msgs::UInt16& angleR) {
    servoR.write(angleR.data);
    digitalWrite(11, HIGH-digitalRead(11));
}

void servo_Yaw (const std_msgs::UInt16& angleY) {
    servoY.write(angleY.data);
    digitalWrite(10, HIGH-digitalRead(10));
}

ros::Subscriber<std_msgs::UInt16> sub("servoP", servo_Pitch);
ros::Subscriber<std_msgs::UInt16> sub_("servoR", servo_Roll);
ros::Subscriber<std_msgs::UInt16> sub__("servoY", servo_Yaw);

//Topics
float valP;
float valR;
float valY;
float updatedAngleP; //connected to ServoP  
float updatedAngleR; //connected to ServoR
float updatedAngleY; //connected to ServoY

double plane_pos; // dummy variable to current plane position

String readString;

void setup(){
    pinMode(13, output);
    pinMode(12, output);
    pinMode(11, output);

    nh.initNode();
    nh.subcribe(sub);
    nh.subcribe(sub_);
    nh.subscribe(sub__);

    servoP.attach(3);
    servoP.attach(4);
    servoP.attach(5);
    
    nh.spinOnce();
    delay(15);
    
}

void callback(const std_msgs::StringConstPtr& str){

    while (ros::ok()) { //keep loop until user terminated
    
        void readPitch(){ // input = pixhawk4 pitch telem ;output = updated angle valP(0-180deg)
            
            while (Serial.available()) { //if new data arrives...
                delay(1);               //finish buffer entry
                char c = Serial.read(); //temp byte reader
                if (c == ',') {         //allows for "," to separate data entries per string
                break;
            }
                readString += c;        //compiles bytes to string
            }
                
            if (readString.length() >0) { //if an updated telemetry entry exists...
                char buf[sizeof(readString)]; //creates array for new entry
                readString.toCharArray(buf,sizeof(readString)); // converts: string-->CharArray
                valP = atoi(buf);                               // converts:          CharArray-->integer
                readString="";                                  // clears "ReadPitch" to show we are fully updated
                
                //`debug features below: turn off if needed 
            Serial.print("pitch telem data = : ");
            Serial.println(valP);
        
            valP = map(valP, 0, 1023, 0, 180); // input = IMU dataPitch, output= angle of servo 
                                              //; converts pitch telemetry data to pitch angle
            }
        }

        ros::spinOnce();
            loop_rate.sleep();
            ++count;
    }
}

ros::Subscriber sub = nh.subscribe("my_topic", 1, callback);

void readRoll(){// input = pixhawk4 roll telem ; output = updated angle valR(0-180)
  while (Serial.available()) { //if new data arrives...
    delay(1);               //finish buffer entry
    char c = Serial.read(); //temp byte reader
    if (c == ',') {         //allows for "," to separate data entries per string
      break;
    }
    readString += c;        //compiles bytes to string
  }
     
  if (readString.length() >0) { //if an updated telemetry entry exists...
    char buf[sizeof(readString)]; //creates array for new entry
    readString.toCharArray(buf,sizeof(readString)); // converts: string-->CharArray
    valR = atoi(buf);                               // converts:          CharArray-->integer
    readString = "";                                // clears "ReadPitch" to show we are fully updated
    
    //`debug features below: turn off if needed 
    Serial.print("Roll telem data = : ");
    Serial.println(valR);

    valR = map(valR, 0, 1023, 0, 180); // input = IMU dataPitch, output= angle of servo ; converts roll telemetry data to roll angle
  }
}

void gimbalMode(){

while(right_position == false){ //leveler mode = always point down
    /*
     * read the valR/P to tell invert their value to negative and replace the valR/P
     */
      valY = valY * (-1); //updated: inverse valY
      valR = valR * (-1); //updated: inverse valR
      valP = valP * (-1); //updated: inverse valP
// `exit program via calling GPS
  }

while(right_position == true){//off axis mode = point with fixed angle to ground
   //read offset incase of update
   valR = valR * (-1) + offsetR; //updated: inverse valR and adds offset
   valP = valP * (-1);     //updated: inverse valP
}

//Gimbal Yaw Code
//checks the position of the yaw of Gimbal
//repositions the yaw if misplaced
void read_yaw() {
   //favor north
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

//void update position() {}

