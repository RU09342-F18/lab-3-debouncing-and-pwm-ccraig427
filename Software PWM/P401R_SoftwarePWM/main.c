/*
 * File: main.c for MSP432P401R Software PWM
 * Author: Colin Craig
 * Credit to Kevin Beenders
 * Created on: 10/04/2018
 * Last Edited: 10/08/2018
 */

#include <msp.h>

int state = 0;
int duty = 500;     // duty cycle is initally 50%
int CEECEEARE = 1200

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;         // Disable the WatchDog Timer

    P1->DIR |=  BIT0;       // p1.0 set as an output
    P1->OUT &= ~BIT0;       // This initializes the LED as off


//Timer A0 pwm configuration

    TA0CCR0 = 1000;    // TimerA_0's frequency, since we are using SMCLK

    TIMER_A0->CTL= TIMER_A_CTL_TASSEL_2 |
            TIMER_A_CTL_MC_2  |
            TIMER_A_CTL_CLR  |
            TIMER_A_CTL_IE;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    __DSB(); //ensures sleeponexit takes effect immediately

    __enable_irq(); //enable global interrupt

    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);

        __sleep();

        __no_operation();                       // For debugger




    for(;;) //Infinite for loop for the PWM
    {
        if(TA0R <= duty)

        P1->OUT ^= BIT0; // toggles LED

        else if (TA0R > duty)

        P1->OUT &= ~BIT0;  // toggle LED
    }
}


void P1_N_IRQHandler(void)
{
    TIMER_A0->CTL= TIMER_A_CTL_TASSEL_2   |
                TIMER_A_CTL_MC_2 |
                TIMER_A_CTL_CLR |
                TIMER_A_CTL_IE;                         // Configure timer to SMCLK, Set it to up mode, Interrupt enable


                P1->IFG &= ~BIT1;                         // Interrupt interrupt flag
                P1->IE &= ~BIT1;                          // disables interrupts on button
}

void TA0_N_IRQHandler(void)                                 // TimerA_0 Interrupt Service Routine (ISR)
{
        switch(state)
        {
        case 0:// While LED is on and staying on
            if(duty < 1000) duty += 100;  // Duty cycle increases by 10%
            else duty = 0;                // Duty cycle is set to 0
            P1->IE |= BIT1;                 // The p1.1 interrupt is enabled
            P1->IES &= ~BIT1;               // The interrupt is set to be triggered at a rising edge
            TIMER_A1->CTL &= ~TASSEL_2;          // Disable SMCLK
            TIMER_A1->CTL |= TACLR;              // Clear Timer
            state = 1;                    // Set the state to state 1
            break;
        case 1://While the LED is off
            P1->IE |= BIT1;               // The pin1.1 interrupt is enabled
            P1->IFG &= ~BIT1;             // Pin1.1 interrupt flag is disabled
            P1->IES |= BIT1;              // Pin1.1 interrupt is set to be triggered on a falling edge
            TIMER_A1->CTL &= ~TASSEL_2;        // Disable SMCLK
            TIMER_A1->CTL |= TACLR;            // Clear timer
            state = 0;                  // Set the state to state 0
            break;
        }
}
