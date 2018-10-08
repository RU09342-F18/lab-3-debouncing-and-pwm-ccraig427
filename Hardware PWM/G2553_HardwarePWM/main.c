#include <msp430.h>
int duty = 499;
int max = 999;

/**Date Created: 10/04/18
   Date Last Edited: 10/07/18
   Author: Colin Craig
   MSP430G2553 Hardware PWM
   */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disables watchdog timer

    LEDSetup(); //Initializes LEDSetup function
    ButtonSetup(); //Initializes ButtonSetup function
    TimerA0Setup(); //Initializes TimerA0 function
//    TimerA1Setup();

    __enable_interrupt();
    __bis_SR_register(GIE); //Global Interrupts Enabled
}

void LEDSetup(void)
{
    P1DIR |= (BIT0 + BIT6); //P1.0 as an output
    P1SEL |= BIT6; //P1.6 as a GPIO function select
}

void ButtonSetup(void)
{
    P1DIR &= ~BIT3; //P1.1 as an input
    P1REN |= BIT3; //Enables resistor for P1.3
    P1OUT |= BIT3; //Enables pull-up resistor for P1.3

    P1IE |= BIT3; //Enables P1.3 interrupts
    P1IES |= ~BIT3; //Enables IFG to be set on a falling edge transition
    P1IFG &= ~BIT3; //Disables pending P1.3 interrupts
}

void TimerA0Setup(void)
{
    TA0CCR0 = max; //Sets Capture/Compare Register to the period/max/100% duty cycle
    TA0CCR1 = duty; //Sets Capture/Compare Register to 50% duty cycle
    TA0CCTL1 = OUTMOD_7; //Sets Capture/Compare Control Register to Set/Reset Mode
    TA0CTL = TASSEL_2 + MC_1; //SMLCK, Up-Mode configuration on TimerA0 Control Register
}

// void TimerA1Setup(void)
//{
//    TA1CTL = TASSEL_2 + MC_1; //SMCLK, Up-Mode configuration on TimerB0 Control Register
//    TA1CCTL0 = CCIE + CCIFG; //Capture/Compare interrupt enabled, Enables pending interrupts
//    TA1CCR0 = 15000; //Capture/Compare Register value for P1.1
//}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IES & BIT3) //If falling edge and button pressed
    {
            if (TA0CCR1 < max)
            {
                TA0CCR1 += 100; //Increments Duty by 100 or 10%
            }
            else if (TA0CCR1 >= max)
            {
                TA0CCR1 = 0; //Resets Duty to 0 or 0%
            }
    }
    P1OUT ^= BIT0; //Toggles P1.0 LED
    P1IES ^= BIT3; //Toggles Interrupt Edge Register
    P1IFG &= ~BIT3; //Disables P1.3 Interrupt Flags
//    TA1CTL = TASSEL_2 + MC_1; //SMCLK, Up-Mode configuration on TimerB0 Control Register
}

//#pragma vector = TIMER0_A1_VECTOR
//__interrupt void Timer_A (void) {
//    TA1CTL = MC_0; //TimerB0 Control register in stop mode
//    P1IE |= BIT3; //Enables P1.1 Interrupts
//}
