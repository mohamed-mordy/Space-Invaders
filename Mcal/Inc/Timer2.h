#ifndef __TIMER2INTS_H__ 
#define __TIMER2INTS_H__

void Timer2_Init(void(*task)(void), unsigned long period);
void Timer2_Enable(void);
void Timer2_Disable(void);

#endif // __TIMER2INTS_H__
