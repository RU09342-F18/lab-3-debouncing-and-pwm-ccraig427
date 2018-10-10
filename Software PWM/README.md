# Software PWM
## Author: Colin Craig 
### Embedded Systems Section 1 
I was tasked in this part of the lab to create a varying pulse width modulation on an LED based on a button press interrupt. I attempted three different boards with this task: G2553, F5529, and P401R. I was unsuccessful with the P401R. In this software PWM, two timer modules were used, a state machine, a for loop, one LED, and one button. Timer A0 was used to configure the PWM configuration. This worked by using a capture/compare register set to 1000. The reason this value was picked was due to the clock speed of SMCLK which is 1 MHZ. Picking 1000 would allow the LED to toggle at 1 kHZ. Initially, the duty cycle was set to 500. This is the value of a varying capture/compare register that controls the PWM. This register value is manipulated within the Timer A0 interrupt in a state machine with two possible states. One, the LED is on for the time the timer is incrementing to the duty cycle register value. Two, the LED is off when the value has been reached and a falling edge is detected. When the LED is off, the button interrupt flag, timer, and SMCLK are either disabled or cleared. An interrupt is then set when a rising edge is detected which then also switches the states. The other timer, Timer A1, is used for debouncing purposes of the button press. The capture/compare register is set to allow a 2 ms delay once the button is pressed, so no unwanted debouncing would occur. Lastly, the for loop is used to reset the duty cycle to zero once it has reached the max value of 1000. 
### G2553 
In the G2553, P1.3 is used for the button press/interrupt and P1.6 is used to toggle the LED. Timers A0 and A1 are both used for debouncing and button interrupts. 
### F5529 
In the F5529, P1.1 is used for the button press/interrupt and P1.0 is used to toggle the LED. Like the other board, both Timers A0 and A1 were used. Other then the difference in pin assignments, the implementation of this task was the same between the boards. 
### P401R 
Kevin Beenders and I attempted to complete this task on the P401R. We were unsuccessful in running an errorless code file. The way this board works is very different then the other three and takes more time to master and complete. 


