//mandosoft pickup winder
#include <AccelStepper.h>

#define INTERFACE_TYPE 1

// Data pin connections
const int sig_int   = 2;
const int wind_pin  = 3;
const int dir_pin   = 4;
const int step_pin  = 5;
const int wire_pin  = 6;
const int ir_pin    = 7;
const int start_pin = 8; 
    int turn_count  = 0;
    int sensor      = 1;
    int btn_state   = 0;  
 
AccelStepper NemaStepper(INTERFACE_TYPE, step_pin, dir_pin);

void setup() 
{   
    Serial.begin(9600);
    pinMode(start_pin, INPUT); 
    pinMode(wind_pin, OUTPUT);
    pinMode(wire_pin, OUTPUT); 
    pinMode(sig_int, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(sig_int), magnet_detect, RISING);


    while(btn_state != 1)
    {
      btn_state = digitalRead(start_pin);
      Serial.println(btn_state);  
    }
   
    NemaStepper.moveTo(-200);
    NemaStepper.setMaxSpeed(500);
    NemaStepper.setAcceleration(1000);
    NemaStepper.setSpeed(400);
    NemaStepper.runToPosition(); 

    pinMode(ir_pin, INPUT);
    NemaStepper.moveTo(1500);
       
    while(sensor != 0)
    {  
       NemaStepper.run();
       sensor = digitalRead(ir_pin);
    }

    NemaStepper.setCurrentPosition(0);
    NemaStepper.moveTo(-764);
    NemaStepper.runToPosition();

    NemaStepper.setCurrentPosition(0);
    NemaStepper.moveTo(70);
    NemaStepper.setMaxSpeed(50);
    NemaStepper.setAcceleration(200);
    NemaStepper.setSpeed(50);

    btn_state = 0; 
    while(btn_state != 1)
    {
      btn_state = digitalRead(start_pin);  
    }

    analogWrite(wind_pin, 35); //29
    analogWrite(wire_pin, 45 ); // 30
    delay(4000);

}

void loop() 
{
    if (NemaStepper.distanceToGo() == 0) 
        NemaStepper.moveTo(-NemaStepper.currentPosition());
       
    // Move the motor one step
    NemaStepper.run();
  
}

int magnet_detect()
{
    if (turn_count < 4000){
      turn_count++;
    } 
    else {
      digitalWrite(wind_pin, LOW);
      digitalWrite(wire_pin, LOW);
      NemaStepper.stop();
   
      return 0;
    }
}
