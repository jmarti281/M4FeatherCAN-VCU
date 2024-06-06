#ifndef FIR_FILTER_H
#define FIR_FILTER_H
#include <stdint.h>

#define FIR_FILTER_LENGTH 8


// A class that averages pedal data
class FIR_Filter {
  private:
    float buf[FIR_FILTER_LENGTH];
    uint8_t bufIndex;

    float out;
  public:
    FIR_Filter();
    float FIRFilter_Update(float inp);
    float* FIRFilterArray();
};

#endif