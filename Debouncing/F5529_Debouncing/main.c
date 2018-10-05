#include <msp430.h> 

/*
 * MSP430F5529 Software Debouncing
 * Author: Colin Craig
 * Created: 10/1/2018
 * Last Edited:
 */

void LEDSetup()
{
    P1DIR |= BIT0; //Sets direction of P1.0 as an output
}

void ButtonSetup()
{
    P2DIR &= ~BIT1; //Sets P1.1 as an input
    P2IE |= BIT1; //Enable P1.1 as an interrupt
    P2IES |= BIT1; //Sets an interrupt for a falling edge
    P2REN |= BIT1; //Adds a pull-up resistor to P1.1
    P2IFG &= ~BIT1; //Interrupt Flag cleared for P1.1
}

void TimerBSetup()
{
    TB0CTL = TBSSEL_1 + MC_0; //TimerB, ACLK, timer stop
    TB0CCTL0 = CCIE; //Capture and Compare Interrupt
    TB0CCR0 = 2000; //Sets Value of Capture and Compare Register
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disables watchdog timer
    LEDSetup();
    ButtonSetup();
    TimerBSetup();

    _BIS_SR(LPM0_bits + GIE);   //Enables global interrupts
    return 0;
}


#pragma vector =PORT2_VECTOR //Button Interrupt
__interrupt void Button_down(void)
{
    TB0CTL = TBSSEL_1 + MC_1; //TimerB, ACLK, count up to CCR0
    P2IE &= ~BIT1; //Disables any interrupts from P2.1
    P2IFG &= ~BIT1; //Clears any interrupt flags
}

#pragma vector =TIMER0_B0_VECTOR //TimerB Interrupt
__interrupt void TIMER0_B1(void)
{

    P1OUT ^= BIT0; //Toggles P1.0/LED
    TB0CTL = MC_0; //Stop Mode
    TBCTL |= TBCLR; //Clears TimerB
    P2IFG &= ~BIT1; //Clears P2.1 Interrupt Flags
    P2IE |=  BIT1; //Re-enables interrupts on P2.1
}
