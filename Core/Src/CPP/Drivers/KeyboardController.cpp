/*
 * KeyboardController.cpp
 *
 *  Created on: Aug 14, 2026
 *      Author: Monte Albert 
 * This is a custom Piano Keyboard with 17 keys.
 * Why 17 keys? It's just the amount of space I had in an amazon box that I felt
 * was perfect.
 * I marked keys from K0 to K16.
 * Those keys are Mapped like this:
 *
 * Header Block A: Pins PE7–PE15 (K0 to K8)

These sit in a direct, uninterrupted line along the outer edge of Header P1:
Key	Pin Name	Physical Header Pin	Notes
K0	PE7	P1 - Pin 25	Continuous block on Header P1
K1	PE8	P1 - Pin 26	Adjacent
K2	PE9	P1 - Pin 27	Adjacent
K3	PE10	P1 - Pin 28	Adjacent
K4	PE11	P1 - Pin 29	Adjacent
K5	PE12	P1 - Pin 30	Adjacent
K6	PE13	P1 - Pin 31	Adjacent
K7	PE14	P1 - Pin 32	Adjacent
K8	PE15	P1 - Pin 33	End of PE group
Header Block B: Pins PB10–PB14 & PB4–PB5 (K9 to K16)
These sit directly in the adjacent row/segment of Header P1:
Key	Pin Name	Physical Header Pin	Notes
K9	PB10	P1 - Pin 34	Continuing directly from PE15
K10	PB11	P1 - Pin 35	Adjacent
K11	PB12	P1 - Pin 36	Adjacent
K12	PB13	P1 - Pin 37	Adjacent
K13	PB14	P1 - Pin 38	Adjacent (Skipping PB15 used by I2S)
K14	PB4	P1 - Pin 19	Start of PB lower group
K15	PB5	P1 - Pin 20	Adjacent
K16	PB8	P1 - Pin 23	Adjacent
 */

#include "KeyboardController.hpp"

KeyboardController* KeyboardController::_instance = nullptr;

KeyboardController::KeyboardController() {
	// TODO Auto-generated constructor stub

}

KeyboardController *KeyboardController::getInstance() {
	if (_instance == nullptr)
    {
		static KeyboardController instance; // Instantiated on FIRST call, safely inside main()!
		_instance = &instance;
    }
    return _instance;
}

void KeyboardController::initKeyboard(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Enable Clocks for GPIOB and GPIOE */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* Configure GPIOE pins: PE7 to PE15 (K0 - K8) */
	GPIO_InitStruct.Pin = GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  |
	                      GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
	                      GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* Configure GPIOB pins: PB4, PB5, PB8, PB10 to PB14 (K9 - K16) */
	GPIO_InitStruct.Pin = GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_8  |
	                      GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
	                      GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	//initializing KeyState Array
	_keys[0].pinMask = GPIO_PIN_7;
	_keys[1].pinMask = GPIO_PIN_8;
	_keys[2].pinMask = GPIO_PIN_9;
	_keys[3].pinMask = GPIO_PIN_10;
	_keys[4].pinMask = GPIO_PIN_11;
	_keys[5].pinMask = GPIO_PIN_12;
	_keys[6].pinMask = GPIO_PIN_13;
	_keys[7].pinMask = GPIO_PIN_14;
	_keys[8].pinMask = GPIO_PIN_15;
	_keys[9].pinMask = GPIO_PIN_10;
	_keys[10].pinMask = GPIO_PIN_11;
	_keys[11].pinMask = GPIO_PIN_12;
	_keys[12].pinMask = GPIO_PIN_13;
	_keys[13].pinMask = GPIO_PIN_14;
	_keys[14].pinMask = GPIO_PIN_4;
	_keys[15].pinMask = GPIO_PIN_5;
	_keys[16].pinMask = GPIO_PIN_8;

	_keys[0].port = GPIOE;
	_keys[1].port = GPIOE;
	_keys[2].port = GPIOE;
	_keys[3].port = GPIOE;
	_keys[4].port = GPIOE;
	_keys[5].port = GPIOE;
	_keys[6].port = GPIOE;
	_keys[7].port = GPIOE;
	_keys[8].port = GPIOE;
	_keys[9].port = GPIOB;
	_keys[10].port = GPIOB;
	_keys[11].port = GPIOB;
	_keys[12].port = GPIOB;
	_keys[13].port = GPIOB;
	_keys[14].port = GPIOB;
	_keys[15].port = GPIOB;
	_keys[16].port = GPIOB;

	_keys[0].keyNumber = 0;
	_keys[1].keyNumber = 1;
	_keys[2].keyNumber = 2;
	_keys[3].keyNumber = 3;
	_keys[4].keyNumber = 4;
	_keys[5].keyNumber = 5;
	_keys[6].keyNumber = 6;
	_keys[7].keyNumber = 7;
	_keys[8].keyNumber = 8;
	_keys[9].keyNumber = 9;
	_keys[10].keyNumber = 10;
	_keys[11].keyNumber = 11;
	_keys[12].keyNumber = 12;
	_keys[13].keyNumber = 13;
	_keys[14].keyNumber = 14;
	_keys[15].keyNumber = 15;
	_keys[16].keyNumber = 16;

	for (auto& key : _keys)
	{
	    bool pressed =
	        (HAL_GPIO_ReadPin(key.port, key.pinMask) == GPIO_PIN_RESET);

	    key.stablePressed = pressed;
	    key.candidatePressed = pressed;
	    key.lastEdgeTime = HAL_GetTick();
	}

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	/* No longer using NVIC. Will check status registers in process.
	 * Removing that code */
}

void KeyboardController::process()
{
    uint32_t now = HAL_GetTick();

    constexpr uint32_t DEBOUNCE_MS = 10U;

    for (auto& key : _keys)
    {
        bool currentPressed =
            (HAL_GPIO_ReadPin(key.port, key.pinMask) == GPIO_PIN_RESET);


        // Raw electrical state changed
        if (currentPressed != key.candidatePressed)
        {
            key.candidatePressed = currentPressed;
            key.lastEdgeTime = now;
        }

        //Checking if it was stable enough key state
        if ((now - key.lastEdgeTime) < DEBOUNCE_MS)
            continue;

        //if already accepted new debounced state, then continue
        if (key.candidatePressed == key.stablePressed)
            continue;

        //otherwise accept the new debounced state
        key.stablePressed = key.candidatePressed;

        notifyListeners(key);
        //Previous code of printf have been moved to the listener
    }
}

bool KeyboardController::addEventListener(
    KeyEventHandler handler,
    void* context)
{
    if (handler == nullptr)
        return false;

    for (auto& listener : _listeners)
    {
        if (listener.handler == nullptr)
        {
            listener.handler = handler;
            listener.context = context;
            return true;
        }
    }

    return false;
}

void KeyboardController::notifyListeners(
    const KeyState& key)
{
    for (auto& listener : _listeners)
    {
        if (listener.handler != nullptr)
        {
            listener.handler(
                key,
                listener.context);
        }
    }
}
