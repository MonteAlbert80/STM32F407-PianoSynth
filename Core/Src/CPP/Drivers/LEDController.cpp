/*
 * LEDController.cpp
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
LED Color	LED Name	Pin Number
Green		LD4			PD12
Orange		LD3			PD13
Red			LD5			PD14
Blue		LD6			PD15
 */

#include "LEDController.hpp"
LED_Controller *LED_Controller::m_instance = nullptr;

LED_Controller::LED_Controller() {
	// TODO Auto-generated constructor stub

}

/*
 * implemnting more object oriented singleton design pattern
 * But since it's small device, allocating memory using new operator
 * is frowned upon. But I wanted to prove that I can work with
 * bare metal with object oriented fashion. But my later implementations
 * will be more along the lines of allocating all singleton objects in
 * the global setting rather than through Object oriented
 * design patterns
 */
LED_Controller *LED_Controller::GetInstance() {
	if (m_instance == nullptr) {
		m_instance = new LED_Controller();
	}
	return m_instance;
}

/**
 * initializes RCC AHB1ENR and and GPIOD Mode register
 * to make sure we can actually use the LED.
 * For that, we need to enable clock access to GPIOD
 * And set up mode register bits related to 4 LEDs
 * which are basically pins 12 to 15 to output mode
 */
void LED_Controller::init(void)
{
	//Enable clock access to GPIOD
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOD;
	//Set GPIOD mode to output for all 4 pins
	// That means we have to set 01 for each of them
	// bit 24, 26, 28, 30 needs to be 1
	// bit 25, 27, 29, 31 needs to be 0
	GPIOD->MODER |= (1U<<24)|(1U<<26)|(1U<<28)|(1U<<30);
	GPIOD->MODER &= ~((1U<<25)|(1U<<27)|(1U<<29)|(1U<<31));
}

/*
 * Turns on green LED
 */
void LED_Controller::turnOnGreen(void)
{
	GPIOD->ODR |= GPIO_PIN_12;
}

/*
 *  Turns on Orange LED
 */
void LED_Controller::turnOnOrange(void)
{
	GPIOD->ODR |= GPIO_PIN_13;
}

/*
 * Turns on red LED
 */
void LED_Controller::turnOnRed(void)
{
	GPIOD->ODR |= GPIO_PIN_14;
}

/*
 * Turns on blue LED
 */
void LED_Controller::turnOnBlue(void)
{
	GPIOD->ODR |= GPIO_PIN_15;
}

/*
 * Turns off green LED
 */
void LED_Controller::turnOffGreen(void)
{
	GPIOD->ODR &= ~GPIO_PIN_12;
}

/*
 * Turns off orange LED
 */
void LED_Controller::turnOffOrange(void)
{
	GPIOD->ODR &= ~GPIO_PIN_13;
}

/*
 * Turns off red LED
 */
void LED_Controller::turnOffRed(void)
{
	GPIOD->ODR &= ~GPIO_PIN_14;
}

/*
 * Turns off blue LED
 */
void LED_Controller::turnOffBlue(void)
{
	GPIOD->ODR &= ~GPIO_PIN_15;
}

/*
 * Toggles state of green LED between ON and off
 */
void LED_Controller::toggleGreen(void)
{
	GPIOD->ODR ^= GPIO_PIN_12;
}

/*
 * Toggles state of orange LED between ON and off
 */
void LED_Controller::toggleOrange(void)
{
	GPIOD->ODR ^= GPIO_PIN_13;
}

/*
 * Toggles state of red LED between ON and off
 */
void LED_Controller::toggleRed(void)
{
	GPIOD->ODR ^= GPIO_PIN_14;
}

/*
 * Toggles state of blue LED between ON and off
 */
void LED_Controller::toggleBlue(void)
{
	GPIOD->ODR ^= GPIO_PIN_15;
}

/*
 * @return true if green LED is on
 */
bool LED_Controller::isGreenOn(void)
{
	return (GPIOD->ODR & GPIO_PIN_12);
}

/*
 * @return true if orange LED is on
 */
bool LED_Controller::isOrangeOn(void)
{
	return (GPIOD->ODR & GPIO_PIN_13);
}

/*
 * @return true if red LED is on
 */
bool LED_Controller::isRedOn(void)
{
	return (GPIOD->ODR & GPIO_PIN_14);
}

/*
 * @return true if blue LED is on
 */
bool LED_Controller::isBlueOn(void)
{
	return (GPIOD->ODR & GPIO_PIN_15);
}
