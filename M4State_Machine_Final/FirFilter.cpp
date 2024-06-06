#include "FIRFilter.hpp"

// Weights for each sample. For a rolling average, all weights should be proportional according to length
// Must change FIR_FILTER_LENGTH in the .hpp file according to how many samples you're averaging
static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f};

// Initial constructor for class
FIR_Filter::FIR_Filter() {
      bufIndex = 0.0f;
      out = 0.0f;
      for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++) {
    
        buf[n] = 0.0f;
      } 
    }

// Method to store newest sample and return average
float FIR_Filter::FIRFilter_Update(float inp) {

  // Store latest sample in buffer
  buf[bufIndex] = inp;

  // Increment buffer index and wrap around if necessary
  bufIndex++;

  if (bufIndex == FIR_FILTER_LENGTH) {

    bufIndex = 0;

  }

  // Compute new output sample (via convolution)
  out = 0.0f;

  uint8_t sumIndex = bufIndex;

  for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++) {

    // Decrement index and wrap if necessary
    if (sumIndex > 0) {

      sumIndex--;
    } else { 

      sumIndex = FIR_FILTER_LENGTH - 1;

    }

    out += FIR_IMPULSE_RESPONSE[n] * buf[sumIndex];

  }

  // Return filtered output
  return out;
}

float* FIR_Filter::FIRFilterArray() {
  return buf;
}