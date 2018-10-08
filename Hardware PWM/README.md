# Hardware PWM
## Author: Colin Craig
### Embedded Systems: Section 1
Unlike software PWM, our task was to use the capabilities of the Timer Modules, specifically the Capture/Control Control Register (TAxCCTL1 and TBxCCTL1), to directly control the GPIO pins. To do this, the register has to be equal to OUTMOD_7, which sets the register to set/reset mode. For this part of the lab, I used the G2553 and FR2311 development boards. Both development boards use Up-down mode and SMCLK. It works by pressing the button to increment the duty cycle of one of the LEDS by 10%. It starts at 50% duty cycle and increments to 100% and then resetting back to zero. This condition is tested after everytime the button is pressed, which sends an interrupt on the falling edge of the button press. Also, PxSEL must be set for the PWM LED to a GPIO functionality. 
### G2553 
For this development board, TimerA0 was used. I tried replicating my code from the FR2311 to the G2553, but the debounce interrupt was not working as planned. Within this board, P1.0 and P1.6 are ports used to toggle the LEDS. Specifically, P1.6 is the PWM LED and P1.0 is the button press toggle. The button is set as an input on P1.3. 
### FR2311 
For this development board, TimerB0 and TimerB1 were used. TimerB0 is used as a debounce interrupt on the button press which is P1.1. The LEDS used are controlled by P1.0 and P2.0. TimerB1 is used to increment and reset the duty cycle of the PWM LED. 


