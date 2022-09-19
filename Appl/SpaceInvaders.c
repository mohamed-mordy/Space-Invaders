/*
    * ******* Required Hardware I/O connections*******************
    * Slide pot pin 1 connected to ground
    * Slide pot pin 2 connected to PE2/AIN1
    * Slide pot pin 3 connected to +3.3V 
    * fire button connected to PE0
    * special weapon fire button connected to PE1
    * 8*R resistor DAC bit 0 on PB0 (least significant bit)
    * 4*R resistor DAC bit 1 on PB1
    * 2*R resistor DAC bit 2 on PB2
    * 1*R resistor DAC bit 3 on PB3 (most significant bit)
    * LED on PB4
    * LED on PB5
    *
    * Blue Nokia 5110
    * ---------------
    * Signal        (Nokia 5110) LaunchPad pin
    * Reset         (RST, pin 1) connected to PA7
    * SSI0Fss       (CE,  pin 2) connected to PA3
    * Data/Command  (DC,  pin 3) connected to PA6
    * SSI0Tx        (Din, pin 4) connected to PA5
    * SSI0Clk       (Clk, pin 5) connected to PA2
    * 3.3V          (Vcc, pin 6) power
    * back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
    * Ground        (Gnd, pin 8) ground
*/

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "Sound.h"
#include "ADC0.h"
#include "Switch.h"
#include "LED.h"
#include "PLL.h"
#include "SysTick.h"
#include "Player.h"
#include "Enemy.h"
#include "Laser.h"
#include "Missile.h"


void DisableInterrupts(void);
void EnableInterrupts(void);


/* Globals Definitions */

unsigned long Frame = 0;
unsigned long UpdateCount = 0;
unsigned long GameOver = 0;

/**
 * @brief:
 * @param:
 * @return:
**/
void DisplayGameOver(void)
{
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Nokia5110_SetCursor(0, 1);
    Nokia5110_OutString(" Game  Over ");
    Nokia5110_SetCursor(0, 4);
    Nokia5110_OutString("press enter ");
    Nokia5110_SetCursor(0, 5);
    Nokia5110_OutString("to restart");
}

/**
 * @brief:
 * @param:
 * @return:
**/
void DisplayGameStartMessage(void)
{
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Nokia5110_SetCursor(0, 0);
    Nokia5110_OutString("Hello World.");
    Nokia5110_SetCursor(0, 2);
    Nokia5110_OutString("This is:");
    Nokia5110_SetCursor(0, 3);
    Nokia5110_OutString("  Space");
    Nokia5110_SetCursor(0, 4);
    Nokia5110_OutString("  Invaders");
    Nokia5110_SetCursor(0, 5);
    Nokia5110_OutString("Press Enter");
}

/**
 * @brief: initializes the data of the game for a fresh start
 * @param: none
 * @return: none
**/
void GameInit(void)
{
    Frame = 0;
    UpdateCount = 0;
    GameOver = 0;
    Random_Init(NVIC_ST_CURRENT_R);
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Player_Init();
    Enemy_Init();
    Laser_Init();
    Missile_Init();
}

/**
 * @brief: initializes the required hardware components and modules
 *         the order of initialization is critical
 *         if changed, the system may not function as expected
 * @param: none
 * @return: none
**/
void SystemInit(void)
{
    PLL_Init();
    ADC0_Init();
    Nokia5110_Init();
    SysTick_Init();
    Sound_Init();
    Switch_Init();
    LED_Init();
}

/**
 * @brief: implements the game logic
 *         is called by SysTick_Handler()
 *         runs at rate 30 Hz
 * @param: none
 * @return: none
**/
void GameEngine(void)
{  
    unsigned long highestLaserXPos, highestLaserYPos;
    unsigned long lowestMissileXPos, lowestMissileYPos;
    UpdateCount++;

    if (Laser_Count() && (highestLaserYPos = Laser_Highest_YPos()) <= 2*ENEMYH) {
        highestLaserXPos = Laser_Highest_XPos();
        Enemy_Kill(highestLaserXPos, highestLaserYPos);
        Laser_Demise_Highest();
    }
    if (Missile_Count() && (lowestMissileYPos = Missile_Lowest_YPos()) >= 47 - PLAYERH) {
        lowestMissileXPos = Missile_Lowest_XPos();
        Player_Kill(lowestMissileXPos, lowestMissileYPos);
        Missile_Demise_Lowest();
    }

    if (!PlayerAlive && !PlayerExplosion && PlayerLives) {
        PlayerLives--;
        Player_Revive();
    } else if (!PlayerAlive && !PlayerExplosion && !PlayerLives){
        GameOver = 1;
    }

    Player_Draw();
    Enemy_Draw(Frame);
    Laser_Draw();
    Missile_Draw();
    Missile_Move();
    Laser_Move();

    // Poll input switches
    if (Switch_Fire()) {
        Laser_Fire();
        Sound_Shoot();
        LED_SetRed();
    } else {
        LED_ClearRed();
    }
    if (Switch_Special()) {
        Sound_Highpitch();
        LED_SetBlue();
    } else {
        LED_ClearBlue();
    }

    if ( UpdateCount == 10) {
        Frame ^=  0x1;
        UpdateCount = 0;
    }
    semaphor = 1;
}

/**
 * @brief: main function
 *         in this program, it acts as the game handler
 * @param: none
 * @return: none
**/
int main(void)
{
    SystemInit();              // System Initialization
    GameInit();                // Data Initializations

    DisplayGameStartMessage(); // Welcome Message
    while (!Switch_Fire()) {}  // Poll User Input
    while(Switch_Fire()) {}    // wait until switch is released
    EnableInterrupts();        // Launch Other Threads 

    while (1) {
        if (GameOver) {
            DisableInterrupts();       // Stop the GameEngine
            DisplayGameOver();
            while (!Switch_Fire()) {}  // Poll User Input
            while(Switch_Fire()) {}    // wait until switch is released
            GameInit();                // initialize the Game Data again
            GameOver = 0;              // Clear the flag
            EnableInterrupts();        // Start The Game
        }
        // Normalize result and Set The global variable SlidePot
        SlidePot = (ADC0_In()*(65))/(4095);
        if (PlayerAlive && SlidePot < PlayerPosition && PlayerPosition > 0) {
            PlayerPosition--;
        } else if (PlayerAlive && SlidePot > PlayerPosition && PlayerPosition < 65) {
            PlayerPosition++;
        }
        Nokia5110_ClearBuffer();
        while (semaphor == 0) {}       // wait for GameEngine() to return
        Nokia5110_DisplayBuffer();     // Update on the actual screen
        semaphor = 0;                  // Acknowledge
    }
}

