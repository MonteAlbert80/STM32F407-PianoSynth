/*
 * LEDController.h
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
 LED Color	LED Name	Pin Number	Defined word
Green		LD4			PD12		GPIO_PIN_12
Orange		LD3			PD13		GPIO_PIN_13
Red			LD5			PD14		GPIO_PIN_14
Blue		LD6			PD15		GPIO_PIN_15
They are all in GPIOD
 */

#ifndef SRC_CPP_APP_LEDCONTROLLER_H_
#define SRC_CPP_APP_LEDCONTROLLER_H_

#include "stm32f4xx.h"

#define RCC_AHB1ENR_GPIOD		(1U<<3)

class LED_Controller {
public:
	static LED_Controller *GetInstance();
	void init(void);
	void turnOnGreen(void);
	void turnOnOrange(void);
	void turnOnRed(void);
	void turnOnBlue(void);
	void turnOffGreen(void);
	void turnOffOrange(void);
	void turnOffRed(void);
	void turnOffBlue(void);
	void toggleGreen(void);
	void toggleOrange(void);
	void toggleRed(void);
	void toggleBlue(void);
	bool isGreenOn(void);
	bool isOrangeOn(void);
	bool isRedOn(void);
	bool isBlueOn(void);

private:
	LED_Controller();
	static LED_Controller *m_instance;

};

#endif /* SRC_CPP_APP_LEDCONTROLLER_H_ */
