/*
 * AudioController.hpp
 *
 *  Created on: Aug 8, 2026
 *      Author: Monte Albert
 *
 * Most of basic hardware pin initialization and GPIO is already taken care of in hal drivers and CubeMX.
 * So we will just create our custom playground functionality here
 */

#ifndef SRC_CPP_DRIVERS_AUDIOCONTROLLER_HPP_
#define SRC_CPP_DRIVERS_AUDIOCONTROLLER_HPP_
#include "stm32f4xx_hal.h"
#include <stdint.h>


class AudioController {
public:
	static AudioController _Instance;		//Keeping singleton pattern losely without dynamic memory allocation
	void init(I2S_HandleTypeDef* audioHandle);
	HAL_StatusTypeDef sendStereo(int16_t left, int16_t right);
	HAL_StatusTypeDef sendBuffer(uint16_t* buffer, uint16_t size);

private:
    I2S_HandleTypeDef* _audioHandle;
	AudioController();
};

#endif /* SRC_CPP_DRIVERS_AUDIOCONTROLLER_HPP_ */
