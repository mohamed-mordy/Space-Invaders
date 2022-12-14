#include "Laser.h"
#include "Player.h"
#include "Nokia5110.h"

// a missile in flight
// includes one blacked out row on the top, bottom, and right of the image to prevent smearing when moved 1 pixel down, up, or left
// a laser burst in flight
// includes one blacked out row on the top and bottom of the image to prevent smearing when moved 1 pixel up or down
// width=2 x height=9
static const unsigned char Laser0[] = {
    0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80,
    0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0xBB, 0x00,
    0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
};

// blank space to cover a laser after it hits something
// width=2 x height=9
static const unsigned char Laser1[] = {
    0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80,
    0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
};

#define LASERW      ((unsigned char)Laser0[18])
#define LASERH      ((unsigned char)Laser0[22])
#define MAXLASERS 30UL

static struct Lasers {
    unsigned long head;
    unsigned long tail;
    unsigned long count;
    struct Laser {
        unsigned long x;
        unsigned long y;
    } lasers[MAXLASERS];
} Lasers;

/**
 * @brief: initializes laser moudule
 * @param: none
 * @return: none
**/
void Laser_Init(void)
{
    Lasers.head = 0;
    Lasers.tail = 0;
    Lasers.count = 0;
}

/**
 * @brief: initializes a new laser to be printed on the screen
 *         depends on glabal variable PlayerPosition
 * @param: none
 * @return: none
**/
void Laser_Fire()
{
    if (Lasers.count < 30) {
        Lasers.lasers[Lasers.head].x = PlayerPosition + PLAYERW/2;
        Lasers.lasers[Lasers.head].y = 47-PLAYERH;
        Lasers.head = (Lasers.head + 1UL) % MAXLASERS;
        Lasers.count++;
    }
}

/**
 * @breif: returns Y coordinate Highest Laser
 * @param: none
 * @return: Y coordinate of Highest Laser
 *
**/
unsigned long Laser_Highest_YPos(void)
{
    return Lasers.lasers[Lasers.tail].y + LASERH;
}

/**
 * @brief: returns X coordinate of Highest Laser
 * @param: none
 * @return: X coordinate of Highest Laser
**/
unsigned long Laser_Highest_XPos(void)
{
    return Lasers.lasers[Lasers.tail].x;
}

/**
 * @brief: returns number of active lasers on the screen
 * @param: none
 * @return: number of active lasers
**/
unsigned long Laser_Count(void)
{
    return Lasers.count;
}

/**
 * @brief: moves each laser on the screen one pixel up
 * @param: none
 * @return: none
**/
void Laser_Move(void)
{
    unsigned long i, j;
    for (j = Lasers.tail, i = 0; i < Lasers.count; j = (j + 1) % MAXLASERS, i++ ) {
        Lasers.lasers[j].y--;
    }
}

/**
 * @brief: draws lasers on the screen buffer
 * @param: none
 * @return: none
**/
void Laser_Draw(void)
{
    unsigned long i, j;
    for (j = Lasers.tail, i = 0; i < Lasers.count; j = (j + 1) % MAXLASERS, i++) {
        Nokia5110_PrintBMP(Lasers.lasers[j].x,Lasers.lasers[j].y, Laser0, 0);
    }
}

/**
 * @brief: clears the highest laser on the screen
 * @param: none
 * @return: none
**/
void Laser_Demise_Highest(void)
{
    if (Lasers.count > 0) {
        Lasers.tail = (Lasers.tail + 1) % MAXLASERS;
        Lasers.count--;
    }
}

