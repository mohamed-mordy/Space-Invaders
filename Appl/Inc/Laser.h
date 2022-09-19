#ifndef __LASER_H__
#define __LASER_H__


void Laser_Init(void);
void Laser_Fire(void);
unsigned long Laser_Count(void);
unsigned long Laser_Highest_XPos(void);
unsigned long Laser_Highest_YPos(void);
void Laser_Move(void);
void Laser_Draw(void);
void Laser_Demise_Highest(void);


#endif  // __LASER_H__
