//APPS sensor variables
#define APPS0_min 3.07//0.76 //APPS0 min sensor value in volts
#define APPS0_max 4.38//2.06 //APPS0 max sensor value in volts

#define APPS1_min 3.2//1.52 //APPS0 min sensor value in volts
#define APPS1_max 3.5//1.86 //APPS0 max sensor value in volts

#define k_throttle 0.75 //throttle scaling factor for motor controller
#define throttle_offset 50 //set throttle offset value for motor controller

//brake variables
#define max_brake_pressure 2000.0
#define brake_threshold 40.0 //threshold to determine if the brake pedal is pressed or not (in PSI)
#define brake_min 0.61 //brake pressure min sensor value in volts
#define brake_max 4.0 //brake pressure max sensor value in volts

//pedal variables
#define pedal_threshold 0.25 // in percent
#define restart_threshold 0.05 //in percent