#include "state_machine.hpp"
#include "Parameters.hpp"

float convert_to_psi(uint32_t x) {
  float brake_voltage = float(x)*5.f*0.0002442f;
  if (brake_voltage > brake_max) brake_voltage = brake_max;
  else if (brake_voltage < brake_min) brake_voltage = brake_min;
  return (brake_voltage - brake_min) / (brake_max - brake_min) * 2000.f; 
}

float convert_to_travel_APPS0(uint32_t x) {
  float pedal_voltage = float(x)*5.f*0.00097752f;
  if (pedal_voltage > APPS0_max) pedal_voltage = APPS0_max;
  else if (pedal_voltage < APPS0_min) pedal_voltage = APPS0_min;
  return (pedal_voltage - APPS0_min) / (APPS0_max - APPS0_min); 
}

float convert_to_travel_APPS1(uint32_t x) {
  float pedal_voltage = float(x)*5.f*0.00097752f;
  if (pedal_voltage > APPS1_max) pedal_voltage = APPS1_max;
  else if (pedal_voltage < APPS1_min) pedal_voltage = APPS1_min;
  return (pedal_voltage - APPS1_min) / (APPS1_max - APPS1_min); 
}

uint32_t convert_to_uint32(float x, float y) {
  float avg = (x+y)*0.5f;
  float avg_to_bit = avg*4095.f;
  uint32_t avg_32 = uint32_t(avg_to_bit);
  return avg_32;
}
