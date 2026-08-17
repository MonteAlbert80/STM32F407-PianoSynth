/*
 * UARTController.cpp
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

#include "UARTController.hpp"

UARTController::UARTController() {
	// TODO Auto-generated constructor stub

}
UARTController *UARTController::_instance = nullptr;

UARTController *UARTController::getInstance() {
	if (_instance == nullptr)
    {
		static UARTController instance; // Instantiated on FIRST call, safely inside main()!
		_instance = &instance;
    }
    return _instance;
}

void UARTController::init(
    USART_TypeDef* uart,
    uint32_t peripheralClock,
    uint32_t baudRate)
{
    _uart = uart;

    // GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 = alternate function
    GPIOA->MODER &= ~(3U << (2 * 2));
    GPIOA->MODER |=  (2U << (2 * 2));

    // PA3 = alternate function
    GPIOA->MODER &= ~(3U << (3 * 2));
    GPIOA->MODER |=  (2U << (3 * 2));

    // PA2 AF7 USART2
    GPIOA->AFR[0] &= ~(0xFU << (2 * 4));
    GPIOA->AFR[0] |=  (7U   << (2 * 4));

    // PA3 AF7 USART2
    GPIOA->AFR[0] &= ~(0xFU << (3 * 4));
    GPIOA->AFR[0] |=  (7U   << (3 * 4));

    // Baud rate
    _uart->BRR =
        (peripheralClock + baudRate / 2U) / baudRate;

    // Enable RX + TX
    _uart->CR1 = USART_CR1_TE | USART_CR1_RE;

    // Enable USART
    _uart->CR1 |= USART_CR1_UE;
}

void UARTController::write(char ch)
{
    if (_uart == nullptr)
        return;

    while ((_uart->SR & USART_SR_TXE) == 0U)
    {
    }

    _uart->DR = static_cast<uint8_t>(ch);
}

char UARTController::read()
{
    if (_uart == nullptr)
        return '\0';

    while ((_uart->SR & USART_SR_RXNE) == 0U)
    {
    }

    return static_cast<char>(_uart->DR & 0xFFU);
}

void UARTController::write(const char* text)
{
    if (text == nullptr)
        return;

    while (*text != '\0')
    {
        write(*text++);
    }
}

extern "C" int __io_putchar(int ch)
{
    UARTController::getInstance()->write(
        static_cast<char>(ch)
    );

    return ch;
}


