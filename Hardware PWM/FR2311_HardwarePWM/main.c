#include <msp430.h>
int duty = 499;
int max = 999;

/**Date Created: 10/04/18
   Date Last Edited: 10/07/18
   Author: Colin Craig
   MSP430FR2311 Hardware PWM
   */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disables watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //Unlocks I/O Pins and defaults to reset conditions

    LEDSetup(); //Initializes LEDSetup function
    ButtonSetup(); //Initializes ButtonSetup function
    TimerB0Setup(); //Initializes TimerB0 function
    TimerB1Setup(); //Initializes TimerB1 function

    __bis_SR_register(GIE); //Global Interrupts Enabled
}

void LEDSetup(void)
{
    P1DIR |= BIT0; //P1.0 as an output
    P2DIR |= BIT0; //P2.0 as an output
    P2SEL0 |= BIT0; //P2.0 as a GPIO function select
}

void ButtonSetup(void)
{
    P1DIR &= ~BIT1; //P1.1 as an input
    P1REN |= BIT1; //Enables resistor for P1.1
    P1OUT |= BIT1; //Enables pull-up resistor for P1.1

    P1IE |= BIT1; //Enables P1.1 interrupts
    P1IES |= BIT1; //Enables IFG to be set on a falling edge transition
    P1IFG &= ~BIT1; //Disables pending P1.1 interrupts
}

void TimerB0Setup(void)
{
    TB0CTL = TBSSEL_2 + MC_1; //SMCLK, Up-Mode configuration on TimerB0 Control Register
    TB0CCTL0 = CCIE + CCIFG; //Capture/Compare interrupt enabled, Enables pending interrupts
    TB0CCR0 = 15000; //Capture/Compare Register value for P1.1
}

void TimerB1Setup(void)
{
    TB1CTL = TBSSEL_2 + MC_1; //SMLCK, Up-Mode configuration on TimerB1 Control Register
    TB1CCR1 = duty; //Sets Capture/Compare Register to 50% duty cycle
    TB1CCR0 = max; //Sets Capture/Compare Register to max/100% duty cycle
    //Hardware
    TB1CCTL1 = OUTMOD_7; //Sets Capture/Compare Control Register to Set/Reset Mode
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IE &= ~BIT1; //P1.1 Interrupts Disabled

    if(P1IES & BIT1) //If falling edge and button pressed
    {
            if (TB1CCR1 < max)
            {
                TB1CCR1 += 100; //Increments Duty by 100 or 10%
            }
            else if (TB1CCR1 >= max)
            {
                TB1CCR1 = 0; //Resets Duty to 0 or 0%
            }
    }
    P1OUT ^= BIT0; //Toggles P1.0 LED
    P1IES ^= BIT1; //Toggles Interrupt Edge Register
    P1IFG &= ~BIT1; //Disables P1.1 Interrupt Flags
    TB0CTL = TBSSEL_2 + MC_1; //SMCLK, Up-Mode configuration on TimerB0 Control Register
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void) {
    TB0CTL = MC_0; //TimerB0 Control register in stop mode
    P1IE |= BIT1; //Enables P1.1 Interrupts
}
