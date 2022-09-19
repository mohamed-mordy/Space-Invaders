#include "..\tm4c123gh6pm.h"
#include "LED.h"

void LED_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x1 << 1;               // Enable clock for Port B
    delay = SYSCTL_RCGC2_R;                   // Some delay for the clock to stabilize
    GPIO_PORTB_AFSEL_R &= ~(0x1<<4 | 0x1<<5); // Disable Alternate function
    GPIO_PORTB_AMSEL_R &= ~(0x1<<4 | 0x1<<5); // Disable Analog Mode
    GPIO_PORTB_PCTL_R &= ~(0x1<<4 | 0x1<<5);  // Clear PCTL bits to enable regular digital function
    GPIO_PORTB_DIR_R |= (0x1<<4 | 0x1<<5);    // Set Direction to Output
    GPIO_PORTB_DEN_R |= (0x1<<4 | 0x1<<5);    // Enable Digital function on PB4 and PB5
}

void LED_SetRed(void)
{
    GPIO_PORTB_DATA_R |= 0x1<<4;
}

void LED_ClearRed(void)
{
    GPIO_PORTB_DATA_R &= ~(0x1<<4);
}

void LED_SetBlue(void)
{
    GPIO_PORTB_DATA_R |= 0x1<<5;
}

void LED_ClearBlue(void)
{
    GPIO_PORTB_DATA_R &= ~(0x1<<5);
}

