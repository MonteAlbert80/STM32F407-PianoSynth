/*
 * PianoSynth.hpp
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
 */

#ifndef SRC_CPP_APP_PIANOSYNTH_HPP_
#define SRC_CPP_APP_PIANOSYNTH_HPP_

#include "stm32f4xx_hal.h"

#include "../Drivers/LEDController.hpp"
#include "../Drivers/Timer.hpp"
#include "../Audio/AudioController.hpp"

class PianoSynth {
public:
	PianoSynth() = default;

    bool init(
        I2S_HandleTypeDef* audioHandle,
        I2C_HandleTypeDef* i2cHandle,
        SPI_HandleTypeDef* spiHandle);

    void process();

private:
    I2S_HandleTypeDef* audioHandle_ = nullptr;
    I2C_HandleTypeDef* i2cHandle_ = nullptr;
    SPI_HandleTypeDef* spiHandle_ = nullptr;
    bool initialized_ = false;
    void playLEDs(void);
    void initTimer(void);
    Timer *_tim4;
    static void timerTick(void *context);
    void onTimerTick();
    LED_Controller *_ledController;
};

#endif /* SRC_CPP_APP_PIANOSYNTH_HPP_ */
