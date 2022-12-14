#ifndef __ADC0_H__
#define __ADC0_H__

extern unsigned long SlidePot;

// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void);



//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void);

#endif  // __ADC0_H__

