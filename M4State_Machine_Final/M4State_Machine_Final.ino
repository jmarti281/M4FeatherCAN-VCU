#include "FirFilter.hpp"
#include "pinout.hpp"
#include "Parameters.hpp"
#include "state_machine.hpp"

// 4 states
enum {IDLE, READY_TO_START, READY_TO_DRIVE, MOTOR_RESTART};
int next_state, current_state = IDLE;

// On-Board LED color will correspond to each state
Adafruit_NeoPixel rgbled(1, RGB_CONTROL, NEO_GRB + NEO_KHZ800);

// Analog Inputs Rolling Average
FIR_Filter APPS0_RA;
FIR_Filter APPS1_RA;
FIR_Filter BrakePressure_RA;

// variables for input and output conversion
float brake_pressure_psi, brake_pressure_avg;
float APPS0_pedal_travel, APPS0_travel_avg;
float APPS1_pedal_travel, APPS1_travel_avg;
uint32_t throttle_avg;

void setup() {
  // put your setup code here, to run once:
  // Inputs
  //    Analog
  pinMode(APPS0, INPUT);
  pinMode(APPS1, INPUT);
  pinMode(BrakePressure, INPUT);
  pinMode(ExtraAnalog, INPUT);

  //    Digital
  pinMode(SteeringStart, INPUT);

  // Outputs
  //    Analog
  pinMode(Throttle_Out, OUTPUT);
  //    Digital
  // STG will occur if output is high
  // STB will occur if output is low
  pinMode(MC_Power, OUTPUT); // STG
  pinMode(BrakeLight, OUTPUT); // STG
  pinMode(RTD, OUTPUT); // STB
  pinMode(FWD_EN, OUTPUT); // STG
  pinMode(Ignition, OUTPUT); // STB
  pinMode(BrakeSwitch, OUTPUT); // STG
  pinMode(Start_Out, OUTPUT); // STB

  // Initial Output Level
  // Want to send power to Motor Controller
  digitalWrite(MC_Power, LOW);
  // Have brakelight on initially
  digitalWrite(BrakeLight, HIGH);
  // Don't sound buzzer
  digitalWrite(RTD, HIGH);
  // Don't set FWD_EN until startup sequence
  digitalWrite(FWD_EN, LOW);
  // Don't set ignition until startup sequence
  digitalWrite(Ignition, HIGH);
  // Have brakelight on
  digitalWrite(BrakeSwitch, HIGH);
  // Don't send start pulse until startup sequence
  digitalWrite(Start_Out, HIGH);

  // Begin LED
  pinMode(RGB_CONTROL, OUTPUT);
  rgbled.begin();

  Serial.begin(9600);
  delay(5000);
}


void loop() {
  // put your main code here, to run repeatedly:

  // Poll brakes every cycle
  brake_pressure_psi = convert_to_psi(analogRead(BrakePressure));
  brake_pressure_avg = BrakePressure_RA.FIRFilter_Update(brake_pressure_psi);
  if (brake_pressure_avg > brake_threshold) {
    digitalWrite(BrakeLight, HIGH);//turn on brake light
    digitalWrite(BrakeSwitch, HIGH);
  }
  else {
    digitalWrite(BrakeLight, LOW); //turn off brake light
    digitalWrite(BrakeSwitch, LOW);
  }

  switch (current_state){

    // Before car is turned on, loop until ignition sequence is started
	  case IDLE:
      setled(cyan);

		  if ((brake_pressure_avg > brake_threshold) && digitalRead(SteeringStart)) next_state = READY_TO_START;
		  else next_state = IDLE; 
		  break;

    // Begin startup sequence to activate motor controller power to motor
	  case READY_TO_START:

      setled(blue);
		  // do all your stuff
      digitalWrite(MC_Power, HIGH);
      delay(200);
      digitalWrite(Ignition, LOW);
      digitalWrite(FWD_EN, HIGH);
      digitalWrite(BrakeLight, HIGH);//turn on brake light
      digitalWrite(BrakeSwitch, HIGH);
      digitalWrite(Start_Out, LOW);
      delay(50);
      digitalWrite(Start_Out, HIGH);
      // Siren for 2.5 seconds
      digitalWrite(RTD, LOW);
      wait_function(500);
      digitalWrite(RTD, HIGH);
      wait_function(500);
      digitalWrite(RTD, LOW);
      wait_function(500);
      digitalWrite(RTD, HIGH);
      wait_function(500);
      digitalWrite(RTD, LOW);
      wait_function(500);
      digitalWrite(RTD, HIGH);

		  next_state = READY_TO_DRIVE; 
		  break;

    // Poll APPS to average and send to motor controller throttle
    // If APPS exceed 25% travel while braking, power to motor will stop
	  case READY_TO_DRIVE:
      setled(green);
		  // do all your stuff
      APPS0_pedal_travel = convert_to_travel_APPS0(analogRead(APPS0));
      APPS0_travel_avg = APPS0_RA.FIRFilter_Update(APPS0_pedal_travel);
      APPS1_pedal_travel = convert_to_travel_APPS1(analogRead(APPS1));
      APPS1_travel_avg = APPS1_RA.FIRFilter_Update(APPS1_pedal_travel);

		  if ((APPS0_travel_avg > pedal_threshold || APPS1_travel_avg > pedal_threshold) && brake_pressure_avg > brake_threshold) next_state = MOTOR_RESTART;
		  else next_state = READY_TO_DRIVE;

      throttle_avg = convert_to_uint32(APPS0_travel_avg, APPS1_travel_avg);
      analogWrite(Throttle_Out, throttle_avg);
		  break;

    // Returns power to motor once pedal travel falls below 5%
	  case MOTOR_RESTART:
      setled(red);

      APPS0_pedal_travel = convert_to_travel_APPS0(analogRead(APPS0));
      APPS0_travel_avg = APPS0_RA.FIRFilter_Update(APPS0_pedal_travel);
      APPS1_pedal_travel = convert_to_travel_APPS1(analogRead(APPS1));
      APPS1_travel_avg = APPS1_RA.FIRFilter_Update(APPS1_pedal_travel);
		  
      if (APPS0_travel_avg < restart_threshold && APPS1_travel_avg < restart_threshold) next_state = IDLE;
		  else next_state = MOTOR_RESTART;
		  break;
	  
    default: next_state = IDLE; break;
  }

  current_state = next_state;
  delay(5);
}

// Polls brakes during the long siren sequence
void wait_function (int x) {
  int count = x;
  while(count) {
      brake_pressure_psi = convert_to_psi(analogRead(BrakePressure));
      brake_pressure_avg = BrakePressure_RA.FIRFilter_Update(brake_pressure_psi);

      if (brake_pressure_avg > brake_threshold) {
        digitalWrite(BrakeLight, HIGH);//turn on brake light
        digitalWrite(BrakeSwitch, HIGH);
      }
      else {
        digitalWrite(BrakeLight, LOW); //turn off brake light
        digitalWrite(BrakeSwitch, LOW);
      }

      delay(10);
      count -= 10;
  }
}
