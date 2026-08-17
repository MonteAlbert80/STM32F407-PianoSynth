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

int KeyboardController::pintranslate(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case GPIO_PIN_0:
		return 0;
		break;
	case GPIO_PIN_2:
		return 2;
		break;
	case GPIO_PIN_3:
		return 3;
		break;
	case GPIO_PIN_4:
		return 4;
		break;
	case GPIO_PIN_5:
		return 5;
		break;
	case GPIO_PIN_6:
		return 6;
		break;
	case GPIO_PIN_7:
		return 7;
		break;
	case GPIO_PIN_8:
		return 8;
		break;
	case GPIO_PIN_9:
		return 9;
		break;
	case GPIO_PIN_10:
		return 10;
		break;
	case GPIO_PIN_11:
		return 11;
		break;
	case GPIO_PIN_12:
		return 12;
		break;
	case GPIO_PIN_13:
		return 13;
		break;
	case GPIO_PIN_14:
		return 14;
		break;
	case GPIO_PIN_15:
		return 15;
		break;
	default:
		return -1;
		break;
	}
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
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* Configure GPIOB pins: PB4, PB5, PB8, PB10 to PB14 (K9 - K16) */
	GPIO_InitStruct.Pin = GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_8  |
	                      GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
	                      GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	/* Enable NVIC interrupt channels */
	HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void KeyboardController::process()
{
    uint32_t now = HAL_GetTick();

    constexpr uint32_t DEBOUNCE_MS = 10;

    for (auto& key : _keys)
    {
        if (!key.pending)
            continue;

        if ((now - key.lastEdgeTime) < DEBOUNCE_MS)
            continue;

        bool pressed =
            (HAL_GPIO_ReadPin(key.port, key.pinMask) == GPIO_PIN_RESET);

        key.pending = false;

        if (pressed == key.stablePressed)
            continue;

        key.stablePressed = pressed;

        if (pressed)
        {
            printf("K%d pressed\r\n", key.keyNumber);
            // notify noteOn(key.keyNumber);
        }
        else
        {
            printf("K%d released\r\n", key.keyNumber);
            // notify noteOff(key.keyNumber);
        }
    }
}

void KeyboardController::keyInterrupt(uint16_t GPIO_Pin)
{
	int pinNumber=pintranslate(GPIO_Pin);
    // Non-overlapping pins
    if (GPIO_Pin == GPIO_PIN_7 || GPIO_Pin == GPIO_PIN_8 || GPIO_Pin == GPIO_PIN_9 || GPIO_Pin == GPIO_PIN_15) {
        // These unique pin numbers only exist on GPIOE in your setup
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, GPIO_Pin);
        // Process PE7, PE8, PE9, PE15...
        if (state == GPIO_PIN_RESET) {
            // Signal went LOW -> FALLING EDGE -> Key Pressed
        	printf("PE%d Key pressed\r\n", pinNumber);
        } else {
            // Signal went HIGH -> RISING EDGE -> Key Released
        	printf("PE%d Key released\r\n", pinNumber);
        }
    }

    else if (GPIO_Pin == GPIO_PIN_4 || GPIO_Pin == GPIO_PIN_5) {
        // These unique pin numbers only exist on GPIOB in your setup
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin);
        // Process PB4, PB5...
        if (state == GPIO_PIN_RESET) {
            // Signal went LOW -> FALLING EDGE -> Key Pressed
        	printf("PB%d Key pressed\r\n", pinNumber);
        } else {
            // Signal went HIGH -> RISING EDGE -> Key Released
        	printf("PB%d Key released\r\n", pinNumber);
        }
    }

    // Overlapping pin numbers (10, 11, 12, 13, 14 exist on BOTH PE and PB)
    // Check PE pin state first, then PB
    else if (GPIO_Pin >= GPIO_PIN_10 && GPIO_Pin <= GPIO_PIN_14) {
        GPIO_PinState pe_state = HAL_GPIO_ReadPin(GPIOE, GPIO_Pin);
        GPIO_PinState pb_state = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin);

        // Process states for PE (K3-K7) and PB (K9-K13)
        if (pb_state == GPIO_PIN_RESET) {
            // Key 11 (PB11) is PRESSED!
        	printf("PB%d Key pressed\r\n", pinNumber);
        } else if (pb_state == GPIO_PIN_SET) {
            // Key 11 (PB11) is RELEASED!
        	printf("PB%d Key released\r\n", pinNumber);
        }

        if (pe_state == GPIO_PIN_RESET) {
            // Key 4 (PE11) is PRESSED!
        	printf("PE%d Key pressed\r\n", pinNumber);
        } else if (pe_state == GPIO_PIN_SET) {
            // Key 4 (PE11) is RELEASED!
        	printf("PE%d Key released\r\n", pinNumber);
        }
    }


}



extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	KeyboardController::getInstance()->keyInterrupt(GPIO_Pin);
}
