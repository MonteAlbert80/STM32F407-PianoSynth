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
#include <cmath>

static constexpr float KEY_FREQUENCIES[17] =
{
	261.63f, // K0  C4
	277.18f, // K1  C#4
	293.66f, // K2  D4
	311.13f, // K3  D#4
	329.63f, // K4  E4
	349.23f, // K5  F4
	369.99f, // K6  F#4
	392.00f, // K7  G4
	415.30f, // K8  G#4
	440.00f, // K9  A4
	466.16f, // K10 A#4
	493.88f, // K11 B4
	523.25f, // K12 C5
	554.37f, // K13 C#5
	587.33f, // K14 D5
	622.25f, // K15 D#5
	659.25f  // K16 E5
};

class AudioController {
public:
	static AudioController _Instance;		//Keeping singleton pattern losely without dynamic memory allocation
	void init(I2S_HandleTypeDef* audioHandle);
	HAL_StatusTypeDef sendStereo(int16_t left, int16_t right);
	HAL_StatusTypeDef sendBuffer(uint16_t* buffer, uint16_t size);
	void playKey(uint8_t keyNumber);
	void stopKey(uint8_t keyNumber);
	void process();
private:
    I2S_HandleTypeDef* _audioHandle;
	AudioController();
	bool _playing = false;
	uint8_t _activeKey = 0;

	float _phase = 0.0f;
	float _frequency = 440.0f;
};

#endif /* SRC_CPP_DRIVERS_AUDIOCONTROLLER_HPP_ */
