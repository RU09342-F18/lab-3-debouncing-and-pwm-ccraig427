#include <msp430.h>

/**Date Created: 10/1/18
   Date Last Edited: 10/5/2018
   Author: Colin Craig
   MSP430G2553 Software Debouncing
   */
void LEDSetup()
{
    P1DIR |= BIT0; //Sets direction of P1.0 as an output
}

void ButtonSetup()
{
    P1DIR &= ~BIT3; //Sets P1.3 as an input
    P1REN |= BIT3; //Adds a pull-up resistor to P1.3
    P1IE |= BIT3; //Enable button as an interrupt
    P1IES |= BIT3; //Sets an interrupt for a falling edge
    P1IFG &= ~BIT3; //Clears P1.3 Interrupt flags
}

void TimerASetup()
{
    TA0CTL = TASSEL_1 + ID_1 + MC_0; //TimerB, ACLK, Integer Divide of 1 bit, timer stop
    TA0CCTL0 = CCIE; //Capture and Compare Interrupt
    TA0CCR0 = 2000; //Sets Value of Capture and Compare Register
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disables watchdog timer
    LEDSetup();
    ButtonSetup();
    TimerASetup();

    _BIS_SR(LPM0_bits + GIE);   //Enables global interrupts
    return 0;
}
#pragma vector =TIMER0_A0_VECTOR
__interrupt void TIMER0_A1(void)
{
    P1OUT ^= BIT0; //Toggles P1.0/LED
    TA0CTL = TASSEL_2 + ID_1 + MC_0;
    TACTL |= TACLR; //Clears TimerA
    P1IE |= BIT3; //Re-enables interrupts on P1.3
    P1IFG &= ~BIT3; //Clears Interrupt Flag
}


#pragma vector=PORT1_VECTOR
__interrupt void Button_down(void)   //Button Interrupt
{
    TA0CTL = TASSEL_2 + ID_1 + MC_1; //Selects SMCLK, and in Up Mode
    P1IE &= ~BIT3; //Disables any interrupts from P1.3
    P1IFG &= ~BIT3; //Clears interrupt flags
}


