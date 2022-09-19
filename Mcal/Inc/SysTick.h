#ifndef __SYSTICK_H__
#define __SYSTICK_H__

extern unsigned long semaphor;

void SysTick_Init(void);
void SysTick_Handler(void);

#endif  // __SYSTICK_H__
