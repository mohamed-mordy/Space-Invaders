#ifndef __ENEMY_H__
#define __ENEMY_H__

#define ENEMYH 10
#define ENEMYW 16

void Enemy_Init(void);
void Enemy_Move(void);
void Enemy_Draw(unsigned long frame);
void Enemy_Kill(unsigned long xpos, unsigned long ypos);

#endif // __ENEMY_H__
