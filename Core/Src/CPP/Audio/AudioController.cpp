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

void AudioController::playKey(
    uint8_t keyNumber)
{
    if (keyNumber >= 17)
        return;

    _activeKey = keyNumber;
    _frequency = KEY_FREQUENCIES[keyNumber];

    _phase = 0.0f;
    _playing = true;
}

void AudioController::stopKey(
    uint8_t keyNumber)
{
    if (_playing &&
        keyNumber == _activeKey)
    {
        _playing = false;
    }
}


void AudioController::process()
{
    if (!_playing ||
        _audioHandle == nullptr)
    {
        return;
    }

    constexpr float SAMPLE_RATE = 44100.0f;
    constexpr float TWO_PI = 6.28318530718f;

    constexpr uint16_t FRAMES = 64;

    uint16_t buffer[FRAMES * 2];

    for (uint16_t i = 0; i < FRAMES; ++i)
    {
        float value =
            sinf(_phase) * 6000.0f;

        int16_t sample =
            static_cast<int16_t>(value);

        buffer[i * 2] =
            static_cast<uint16_t>(sample);

        buffer[i * 2 + 1] =
            static_cast<uint16_t>(sample);

        _phase +=
            TWO_PI *
            _frequency /
            SAMPLE_RATE;

        if (_phase >= TWO_PI)
        {
            _phase -= TWO_PI;
        }
    }

    sendBuffer(
        buffer,
        FRAMES * 2);
}
