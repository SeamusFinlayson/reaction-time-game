// Filename: Seamus Finlayson uart.c
// Description: UART module source file
// Author(s): Seamus Finlayson
// Date: Oct 28, 2020

#include "msp.h"                    //device specific include file
#include "uart.h"                    //include UART module

//uart module functions

////////////////////////////////////////////////////////////
// uartInit - initialize the eUSCI_A0 UART
// Arguments: none
// Return Value: none
////////////////////////////////////////////////////////////
void uartInit(void) {

    //enable P1.2 and P1.3 to be controlled as RXD/TXD by eUSCI block
    P1->SEL0 |= (RXD | TXD);
    P1->SEL1 &= ~(RXD | TXD);

    //put eUSCI_A0 in software reset
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;

    //enable eUSCI with 8 data bits, 1 stop bit, no parity (default settings)

    //SMCLK as the clock source
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;

    //set the divisor to 13 0/16
    EUSCI_A0->BRW = BAUD_RATE_DIVISOR;
    EUSCI_A0->MCTLW = EUSCI_A_MCTLW_OS16 | (BAUD_RATE_DIVISOR_FRAC << EUSCI_A_MCTLW_BRF_OFS);

    //end eUSCI_A0 software reset
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}

////////////////////////////////////////////////////////////
// uartPutC - send data to PC
// Arguments: data - data for serial transmission
// Return Value: none
////////////////////////////////////////////////////////////
void uartPutC(char data) {

    //wait until the transmit buffer is empty
    while((EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG) == 0) {}

    //store the data byte into the transmit buffer for serial transmission
    EUSCI_A0->TXBUF = data;
}

////////////////////////////////////////////////////////////
// uartGetC - get data from PC
// Arguments: none
// Return Value: the data byte will be returned, otherwise will be NO_DATA
////////////////////////////////////////////////////////////
int uartGetC(void) {

    //If there is data, the data byte will be returned
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) {
        return EUSCI_A0->RXBUF;
    }

    //otherwise the NO_DATA value of -1 will be returned
    else {
        return NO_DATA;
    }
}
