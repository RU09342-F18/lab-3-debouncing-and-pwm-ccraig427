/*
 * File: main.c for MSP430F5529 Software PWM
 * Author: Colin Craig
 * Created on: 10/04/2018
 * Last Edited: 10/09/2018
 * Credit to Kevin Beenders
 */

#include <msp430.h>

int state = 0;
int duty = 500;  // duty cycle set at 50%

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Disable the WatchDog Timer

    P1DIR |=  BIT0;       // p1.0 set as an output
    P1OUT &= ~BIT0;       // This initializes the LED as off


//Timer A0 pwm configuration

    TA0CCR0 = 1000;    // TimerA_0's frequency, since we are using SMCLK
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // Configure timer to SMCLK, Set it to up mode, Interrupt enable

//TimerA_1 is configured for Debouncing

    TA1CCTL0 = CCIE;         // Interrupt is enabled
    TA1CCR0 = 12000;         // CCR set for 2 ms


    P1DIR &= ~BIT1;          // p1.1 set as an interrupt
    P1REN |= BIT1;           // The resistor at p1.1 is enabled
    P1OUT |= BIT1;           // The enabled register at p1.1 is set to be a pull up resistor
    P1IE |= BIT1;            // p1.1 Interrupt enabled
    P1IES |= BIT1;           // The interrupt is set to be triggered at a falling edge
    P1IFG &= ~BIT1;          // Interrupt flag cleared

    __bis_SR_register(GIE);     // General purpose interrupt was


    for(;;) //Infinite for loop for the PWM
    {
        if(TA0R <= duty)

        P1OUT ^= BIT0; // toggles LED

        else if (TA0R > duty)

        P1OUT &= ~BIT0;  // toggle LED
    }
}


#pragma vector=PORT1_VECTOR  // Interrupt Service Routine(ISR
__interrupt void PORT_1(void)
{
                TA1CTL = TASSEL_2 + MC_1 + TACLR;       // Configure timer to SMCLK, Set it to up mode, Interrupt enable
                P1IFG &= ~BIT1;                         // Interrupt interrupt flag
                P1IE &= ~BIT1;                          // disables interrupts on button
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A0 (void) // TimerA_0 Interrupt Service Routine (ISR)
{
        switch(state)
        {
        case 0:// While LED is on and staying on
            if(duty < 1000) duty += 100;  // Duty cycle increases by 10%
            else duty = 0;                // Duty cycle is set to 0
            P1IE |= BIT1;                 // The p1.1 interrupt is enabled
            P1IES &= ~BIT1;               // The interrupt is set to be triggered at a rising edge
            TA1CTL &= ~TASSEL_2;          // Disable SMCLK
            TA1CTL |= TACLR;              // Clear Timer
            state = 1;                    // Set the state to state 1
            break;
        case 1://While the LED is off
            P1IE |= BIT1;               // The pin1.1 interrupt is enabled
            P1IFG &= ~BIT1;             // Pin1.1 interrupt flag is disabled
            P1IES |= BIT1;              // Pin1.1 interrupt is set to be triggered on a falling edge
            TA1CTL &= ~TASSEL_2;        // Disable SMCLK
            TA1CTL |= TACLR;            // Clear timer
            state = 0;                  // Set the state to state 0
            break;
        }
}
