#include "..//tm4c123gh6pm.h"
#include "DAC.h"

/**
 * @brief: Initialize 4-bit DAC
 * @param: none
 * @return: none
**/
void DAC_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x02;         // Enable clock for PORT B
    delay = SYSCTL_RCGC2_R;         // Some delay for the clock to stabilize
    GPIO_PORTB_PCTL_R &= ~(0xf);    // Clear PCTL bits to enable regular digital function
    GPIO_PORTB_AFSEL_R &= ~(0xf);   // Disable Analog function
    GPIO_PORTB_AMSEL_R &= ~(0xf);   // Disable Analog Mode
    GPIO_PORTB_DEN_R |= 0X0f;       // Enable digital function on PORT B
    GPIO_PORTB_DIR_R |= 0x0f;       // Make PORT B bins 0-3 digital output
    GPIO_PORTB_DR8R_R |= 0x0f;      // Output pins could drive up to 8 mA of current
}

/**
 * @brief: output to DAC
 * @param: 4-bit data, 0 to 15
 * @return: none
**/
void DAC_Out(unsigned long data){
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & ~0xf ) | (data & 0xf);
}

