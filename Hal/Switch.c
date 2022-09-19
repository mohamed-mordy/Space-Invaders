#include "..\tm4c123gh6pm.h"
#include "switch.h"


void Switch_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x1 << 4;  // Enable clock for Port E
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTE_AFSEL_R &= ~0x3;
    GPIO_PORTE_AMSEL_R &= ~0x3;
    GPIO_PORTE_PCTL_R &= ~0x3;
    GPIO_PORTE_DIR_R &= ~0x3;    // Set Direction to Input
    GPIO_PORTE_PDR_R |= 0x3;     // Enable Pull-down Resistors
    GPIO_PORTE_DEN_R |= 0x3;     // Enable Digital function on PE0 and PE1
}

unsigned long fire_prev = 0;
unsigned long special_prev = 0;

unsigned long Switch_Fire(void)  // On Rising Edge 1, Otherwise 0
{
    unsigned long fire_now = GPIO_PORTE_DATA_R & 0x1;
    unsigned long return_val =  (!fire_prev && fire_now)? 1:0;
    fire_prev = fire_now;
    return return_val;
}

unsigned long Switch_Special(void)  // On Rising Edge 1, Otherwise 0
{
    unsigned long special_now = GPIO_PORTE_DATA_R & 0x2;
    unsigned long return_val =  (!special_prev && special_now)? 1:0;
    special_prev = special_now;
    return return_val;
}

