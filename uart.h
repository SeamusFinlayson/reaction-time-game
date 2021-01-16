// Filename: Seamus Finlayson uart.h
// Description: UART module header file
// Author(s): Seamus Finlayson
// Date: Oct 28, 2020

#define NO_DATA -1                  //return value for empty register

#define RXD BIT2                    // USCA0RXD function on P1.2
#define TXD BIT3                    // USCA0TXD function on P1.3

#define BAUD_RATE_DIVISOR 13        //divisor for 14400bps connection using SMCLK and oversampling
#define BAUD_RATE_DIVISOR_FRAC 0    //fractional component of divisor


//uart mudule function prototypes

////////////////////////////////////////////////////////////
// uartInit - initialize the eUSCI_A0 UART
// Arguments: none
// Return Value: none
////////////////////////////////////////////////////////////
void uartInit(void);

////////////////////////////////////////////////////////////
// uartPutC - send data to PC
// Arguments: data - data for serial transmission
// Return Value: none
////////////////////////////////////////////////////////////
void uartPutC(char data);

////////////////////////////////////////////////////////////
// uartGetC - get data from PC
// Arguments: none
// Return Value: the data byte will be returned, otherwise the NO_DATA
////////////////////////////////////////////////////////////
int uartGetC(void);

