/*
 * UARTController.hpp
 *
 *  Created on: Aug 14, 2026
 *      Author: mudab
 *  UART or USART is basically a periferral We put characters
 *  in it in a stream using specified frequency
 *  Basically it goes back to the USB to computer
 *  where you can read it using realterm if you tune
 *  it to right freequency
 *  basically URT in this analogy a radio transmitter
 *  and RealTerm software is a radio reciever
 *  set on specific frequency
 */

#ifndef SRC_CPP_DRIVERS_UARTCONTROLLER_HPP_
#define SRC_CPP_DRIVERS_UARTCONTROLLER_HPP_
#include "stm32f4xx_hal.h"

/*
 * 	It's not the instructor one for lower STM board which was
 * 	115200. Nor is it the maximum for this board 921600.
 * 	Just right in the middle. No chance of packet drop
 *
#define MY_CHOSEN_BAUD_RATE		460800
*
 * sticking with standard baudrate. we were getting over excited with above
 */
#define MY_CHOSEN_BAUD_RATE		115200
#define RELATED_PERIPH_CLOCK	42000000	// 42 MHz

class UARTController {
public:
    static UARTController* getInstance();

    void init(
    	    USART_TypeDef* uart=USART2,
    	    uint32_t peripheralClock=RELATED_PERIPH_CLOCK,
    	    uint32_t baudRate=MY_CHOSEN_BAUD_RATE);

    void write(char ch);
    void write(const char* text);

    char read();

private:
    UARTController();
    static UARTController* _instance;
    USART_TypeDef* _uart;
};
#endif /* SRC_CPP_DRIVERS_UARTCONTROLLER_HPP_ */
