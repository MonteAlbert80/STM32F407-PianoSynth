/*
 * AudioController.cpp
 *
 *  Created on: Aug 8, 2026
 *      Author: Monte Albert 
 */

#include "AudioController.hpp"

AudioController AudioController::_Instance;

AudioController::AudioController() {
	// TODO Auto-generated constructor stub

}

void AudioController::init(I2S_HandleTypeDef* audioHandle)
{
	_audioHandle = audioHandle;
}

HAL_StatusTypeDef AudioController::sendStereo(
    int16_t left,
    int16_t right)
{
    if (_audioHandle == nullptr)
    {
        return HAL_ERROR;
    }

    uint16_t frame[2];

    frame[0] = static_cast<uint16_t>(left);
    frame[1] = static_cast<uint16_t>(right);

    return HAL_I2S_Transmit(
        _audioHandle,
        frame,
        2,
        100
    );
}

HAL_StatusTypeDef AudioController::sendBuffer(
    uint16_t* buffer,
    uint16_t size)
{
    if (_audioHandle == nullptr)
    {
        return HAL_ERROR;
    }

    return HAL_I2S_Transmit(
        _audioHandle,
        buffer,
        size,
        HAL_MAX_DELAY
    );
}
