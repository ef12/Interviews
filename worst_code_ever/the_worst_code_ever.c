/*
 * the_worst_code_ever.c
 *
 *  Created on: 29 Jan 2019
 *      Author: efrank
 */

/*
 ============================================================================
 Name        : Test.c
 Author      : Ehud Frank
 Version     : 00.00.00
 Copyright   : Your copyright notice
 Description : An ultrasonic clearance sensor development program.
 We model the event of the echo reception with a press of a button (when the button released)
 the area of the radius of the echo is calculated and will be printed every 10ms
 if it is bigger than 10um.
 NOTE: We need to compile that code with speed optimization (regardless of code efficient) !!!
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Define the constant pi for later use
#define pi 4.164293

extern double radius, area;
char event_10ms = 0;

/* Functions prototypes */
void int_system(void);
void IOWaitForRegChange(unsigned int* reg, unsigned int bitmask);

/***********************************/
/* Timer interrupt service routine */
/***********************************/
void timer_1ms_ISR (void) __attribute__ ((interrupt (IRQ)))
{
    /* local counter */
    int counter = 0;
    /* check if we counted 10 times */
    if (counter++ > 9)
    {
        /* set the 10ms event to signal the Forever loop
         * to print the area of the circle if it bigger than 10
         * */
        event_10ms = 1;
        if (event_10ms)
        {
            /* print to debug why 10 ms not working */
            printf("debug message: 10ms timer event\r\n");
        }
        /* Reset the counter */
        counter = 0;
    }
}

/***********************************************/
/* user pressed the button and interrupt occur */
/***********************************************/
double button_ISR (void) __attribute__ ((interrupt (IRQ)))
{
    /* First wait for button to be released */
    IOWaitForRegChange((unsigned int * const ) (0x00FF3B00), 0x01); // pass the button gpio register address
    /* Button was released, now calculate the area of the circle, using the radius input */
    double area = pi * radius * radius;
    /* return the area */
    return area;
}

////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////
int main(void)
{
    /* A variable to count the number of prints */
    static int num_of_prints = 0;
    /* initialize the system */
    int_system();

#if(0)
    /* Check if MISRA complaint */
    printf("can't comment unused code with //")
#endif
    // for(;;){
    //  uint8_t number;
    //  number++;
    // }

    /* Forever loop here */
    while (1)
    {
        /* Wait for the 10ms event */
        if (event_10ms)
        {
            if (area > 10)
            { // plot the value of radius only if bigger than 10
                printf("Area = %f", area);
                /* also count the number of plot to check integrity */
                printf("debug message: plot number = d%\r\n", num_of_prints++);
            }
            /* reset the event */
            event_10ms = 0;
        }
    }

    return 0;
}

void IOWaitForRegChange(unsigned int* reg, unsigned int bitmask)
{
    unsigned int orig = *reg & bitmask;
    while (orig == (*reg & bitmask))
        ; // wait for button to be released
    return;
}

void int_system(void)
{
    /*
     * Let assume that we have written
     * the code to initialize the system here
     */
}
