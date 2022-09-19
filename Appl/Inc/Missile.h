#ifndef __MISSILE_H__
#define __MISSILE_H__


void Missile_Init(void);

// Only one usage per frame, otherwise, the behavior is undefined
void Missile_Fire(unsigned long xpos, unsigned long ypos);
unsigned long Missile_Count(void);
void Missile_Move(void);
void Missile_Draw(void);
unsigned long Missile_Lowest_YPos(void);
unsigned long Missile_Lowest_XPos(void);
void Missile_Demise_Lowest(void);

#endif  // __MISSILE_H__
