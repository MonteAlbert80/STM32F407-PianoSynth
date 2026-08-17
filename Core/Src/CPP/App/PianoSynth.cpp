/*
 * PianoSynth.cpp
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
 */

#include <cstdlib>
#include <ctime>
#include "PianoSynth.hpp"
#include "../Drivers/UARTController.hpp"
#include "../Drivers/KeyboardController.hpp"

/*
 * @brief Initial entry point after basic initiation happend in main.c
 * @param audioHandle	This provide I2S style communication to send audio
 * @param i2cHandle		For any I2C communication
 * @param spiHandle		For any SPI communication
 */
bool PianoSynth::init(
    I2S_HandleTypeDef* audioHandle,
    I2C_HandleTypeDef* i2cHandle,
    SPI_HandleTypeDef* spiHandle)
{
    if ((audioHandle == nullptr) ||
        (i2cHandle == nullptr) ||
        (spiHandle == nullptr))
    {
        return false;
    }

    audioHandle_ = audioHandle;
    i2cHandle_ = i2cHandle;
    spiHandle_ = spiHandle;


    UARTController::getInstance()->init();
    KeyboardController::getInstance()->initKeyboard();
    initialized_ = true;

    playLEDs();

    return true;
}

/*
 * Gets called all the time from main thread to basically keep
 * application alive. Most application logic is event driven elsewhere.
 * Update: ignore what I said earlier. interrupt events are only used to change
 * state. most code that require things done on hardware
 * such as printf statement, that sends data to uart channels
 * or anything to do with sending audio or to the LCD screen should be handled
 * inside the process
 */
void PianoSynth::process()
{
    if (!initialized_)
    {
        return;
    }

    /*
     * Nonblocking application work:
     * - scan keys
     * - process note events
     * - update audio state
     * - update UI when needed
     */
    KeyboardController::getInstance()->process();
}

/*
 * initializes timer. For LED Synth project, we are setting up
 * one timer at fixed value so we can keep turning LEDs on and off
 * we send event handler in it.
 */
void PianoSynth::initTimer(void)
{
	_tim4 = Timer::GetInstance(4);
	_tim4->initMilliseconds(500);
	_tim4->addEventListener(timerTick, this);
}

/*
 * entry point for playing LEDs. It initializes timer
 * and random number generator to keep LEDs turning on
 * based on random values
 */
void PianoSynth::playLEDs(void)
{
	_ledController = LED_Controller::GetInstance();
	initTimer();
	//seed the random number generator
	srand(HAL_GetTick());
}

/*
 * Event handler for timer event. It's static function
 * So the context gets the PianoSynth object back
 */
void PianoSynth::timerTick(void *context)
{
	static_cast<PianoSynth *>(context)->onTimerTick();
}

/*
 * more object oriented entry point of the event handler
 * This one has logic of turning on and off 4 LEDs based
 *  on the random generated number
 */
void PianoSynth::onTimerTick()
{
	bool green = _ledController->isGreenOn();
	bool orange = _ledController->isOrangeOn();
	bool red = _ledController->isRedOn();
	bool blue = _ledController->isBlueOn();
	int rnd;

	rnd = rand()%4;
	// Now check if the random number generated light was already on, we need
	// to generate new number;
	while(((rnd==0)&&green)||((rnd==1)&&orange)||((rnd==2)&&red)||((rnd==3)&&blue))
	{
		rnd = rand()%4;
	}
	if (green)
	{
		_ledController->turnOffGreen();
	}
	if (orange)
	{
		_ledController->turnOffOrange();
	}
	if (red)
	{
		_ledController->turnOffRed();
	}
	if (blue)
	{
		_ledController->turnOffBlue();
	}
	switch(rnd)
	{
	case 0:
		_ledController->turnOnGreen();
		break;
	case 1:
		_ledController->turnOnOrange();
		break;
	case 2:
		_ledController->turnOnRed();
		break;
	case 3:
		_ledController->turnOnBlue();
		break;
	}
}
