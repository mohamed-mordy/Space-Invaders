#ifndef __PLAYER_H__
#define __PLAYER_H__

#define BUNKERW  18
#define BUNKERH  5
#define PLAYERW  18
#define PLAYERH  8

extern unsigned long PlayerPosition;
extern unsigned long PlayerAlive;
extern unsigned long PlayerExplosion;
extern unsigned long PlayerLives;


void Player_Init(void);
void Player_Draw(void);
void Player_Kill(unsigned long xpos, unsigned long ypos);
void Player_Revive(void);
void Player_Draw_Bunker(void);

#endif  // __PLAYER_H__
