# Software Debouncing
## Author: Colin Craig 
### Embedded Systems Section 1

My task was to use the TIMER modules to implement a debounced switch to control the state of an LED. To do this, I had to use a capture and compare register value for the TIMER to count up to in Up mode. This interrupt is initialized once the button is pressed. Once this TIMER reached that value, it reset back to zero and went into stop mode. An interrupt is then set on the falling edge of the TIMER that allows the LED to toggle. The time for the TIMER to reach the CCR value is the delay time for any unnecessary interrupts from button debouncing. Once the button is pressed again, the TIMER continues incrementing and the process repeats. If you wanted to change the time for the LED to toggle after the button is pressed, you can increase the value of the CCR or set the integer divide to either 2 or 3 bits. This would be useful if more time needed to be allotted for button debouncing. 
### G2553 
For this microcontroller, TIMER A was used. The configurations for the LED, button, and timer setup are all encoded within each others functions. This makes my code easier to debug and also translate between microcontroller. ACLK was used for TIMER A. The CCR value was set at 2000. To extend the time for the TIMER to reach that value, the register TA0CTL was also integer divided by 1 bit. 
### F5529 
For this microcontroller, TIMER B was used. The configurations and code were all the same compared to the G2553. 
