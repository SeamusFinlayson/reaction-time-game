// Filename: Seamus Finlayson lab 10 reaction time game main.c
// Description: reaction timer when the green light comes on press s1 to start,
//              when the red light comes on press s2 as fast as possible, your
//              reaction time is being recorded, reaction time displays in
//              the terminal
// Author(s): Seamus Finlayson
// Date: Dec. 3, 2020

#include "msp.h"                    //device specific include file
#include "uart.h"                   //include UART module
#include "stdio.h"                  //sprintf()

#define GREEN BIT4                  //green led
#define RED BIT6                    //red led
#define S1 BIT1                     //switch 1
#define S2 BIT5                     //switch 2
#define PERIOD 3000                 //generate 1000Hz signal from SMCLK
#define STRING_LENGTH_1 18          //length of "Reaction Time Is: "
#define STRING_LENGTH_2 8           //length of reaction time string
#define STRING_LENGTH_3 5           //length of " ms" new line


////////////////////////////////////////////////////////////
// msDelay - Function to generate a variable delay
// Arguments: ms - The number of milliseconds for the delay
//Return Value: none
////////////////////////////////////////////////////////////
void msDelay(int ms);

int msCounter;

void main(void)
{
    // stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //variables
        //for selecting character in strings
        int stringSelect;

        //duration for delay before red light turns on
        int delayDuration;

        //records reaction time
        int reactionTime;

        //stores timer register as seed
        int seed;

    //string for reaction time report
        //"Reaction Time Is: "
        char sendReaction[STRING_LENGTH_1];
        sprintf(sendReaction,"Reaction Time Is: ");

        //reaction time value
        char reactionTimeStr[STRING_LENGTH_2];

        //" ms" new line
        char units[STRING_LENGTH_3];
        sprintf(units," ms\r\n");

    //led setup
        //GREEN LED enable P2.4 and initialize off
        P2->DIR |= GREEN;
        P2->OUT &= ~GREEN;

        //RED LED enable P2.6 and initialize off
        P2->DIR |= RED;
        P2->OUT &= ~RED;

    //button setup
        //S1, configure P5.1 as input with pull up resistor
        P5->DIR &= ~S1;
        P5->REN |= S1;
        P5->OUT |= S1;

        //S1, configure P5.1 as input with pull up resistor
        P3->DIR &= ~S2;
        P3->REN |= S2;
        P3->OUT |= S2;

    //clock set up
        //TA0 register:set clk in up mode, divide clk by 8, use 3MHz subsystem main clk
        TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_IE;

        //set up counter period
        TIMER_A0->CCR[0] = PERIOD -1;

    //initialize uart
    uartInit();

    //global interrupt enable
    _enable_interrupts();

    //enable TA0 interrupts
    NVIC_EnableIRQ(TA0_N_IRQn);

    //main loop
    while(1) {

        //green led on
        P2->OUT |= GREEN;

        //idle until s1 pushed
        while (P5->IN & S1);

        //green led off
        P2->OUT &= ~GREEN;

        //get seed from clock
        seed = TIMER_A0->R;

        //calculate delay until red on
        //scaled from (0 to 3000) to (1000 to 5000) ms
        delayDuration = seed * 4000 / 3000 + 1000;

        //play delay
        msDelay(delayDuration);

        //red led on
        P2->OUT |= RED;

        //start timer
        msCounter = 0;

        //wait until s2 press
        while (P3->IN & S2);

        //store reaction time
        reactionTime = msCounter;

        //red led off
        P2->OUT &= ~RED;

        //send reaction time to terminal on PC
            //send "Reaction Time Is: "
            for(stringSelect = 0; stringSelect != STRING_LENGTH_1; stringSelect++) {
                uartPutC(sendReaction[stringSelect]);
            }

            //convert reaction time to string
            sprintf(reactionTimeStr, "%d", reactionTime);

            //send reaction time value
            for(stringSelect = 0; stringSelect != STRING_LENGTH_2; stringSelect++) {
                    uartPutC(reactionTimeStr[stringSelect]);
            }

            //send units (ms)
            for(stringSelect = 0; stringSelect != STRING_LENGTH_3; stringSelect++) {
                uartPutC(units[stringSelect]);
            }
    }
}

////////////////////////////////////////////////////////////
// TA0_N_IRQHandler - Periodic interrupt for recording delay in ms
////////////////////////////////////////////////////////////
void TA0_N_IRQHandler (void) {

    //clear interrupt flag
    TIMER_A0->CTL &= ~TIMER_A_CTL_IFG;

    //increment reaction counter
    msCounter++;
}

////////////////////////////////////////////////////////////
// msDelay - Function to generate a variable delay
// Arguments: ms - The number of milliseconds for the delay
//Return Value: none
////////////////////////////////////////////////////////////
void msDelay(ms) {
    while (ms > 0) {

        //count 3000 clock cycles
        __delay_cycles(3000);

        //decrement counter
        ms--;
    }
}


