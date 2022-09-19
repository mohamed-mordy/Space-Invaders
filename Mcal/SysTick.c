#include "tm4c123gh6pm.h"
#include "SysTick.h"

unsigned long semaphor = 0;
void GameEngine(void);

void SysTick_Init(void)
{
    NVIC_ST_CTRL_R = 0;           // Disable SysTick during initialization
    NVIC_ST_RELOAD_R = 2666666;   // 30 Hz
    NVIC_ST_CURRENT_R = 0;        // Any write to current to clear it
    NVIC_SYS_PRI3_R &= 0X00FFFFFF;// Priority 0
    NVIC_ST_CTRL_R = 0x7;         // Enable SysTick and interrupts
}

void SysTick_Handler(void)
{
    GameEngine();
}

