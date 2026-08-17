/*
 * KeyboardController.h
 *
 *  Created on: Aug 14, 2026
 *      Author: Monte Albert 
 *
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
 *
 */

#ifndef SRC_CPP_DRIVERS_KEYBOARDCONTROLLER_H_
#define SRC_CPP_DRIVERS_KEYBOARDCONTROLLER_H_
#include <stdio.h>
#include "stm32f4xx_hal.h"

struct KeyState
{
    GPIO_TypeDef* port;
    uint16_t pinMask;
    uint8_t keyNumber;

    bool stablePressed;
    bool candidatePressed;

    uint32_t lastEdgeTime;
};

class KeyboardController {

public:
    using KeyEventHandler = void (*)(const KeyState& key, void* context);
    static KeyboardController* getInstance();
	void initKeyboard(void);
	void process();

    bool addEventListener(KeyEventHandler handler, void* context);
private:
	KeyboardController();
	static KeyboardController* _instance;
	KeyState _keys[17];

    struct Listener
    {
        KeyEventHandler handler = nullptr;
        void* context = nullptr;
    };

    static constexpr uint8_t MAX_LISTENERS = 4;
    Listener _listeners[MAX_LISTENERS];

    void notifyListeners(const KeyState& key);

};

#endif /* SRC_CPP_DRIVERS_KEYBOARDCONTROLLER_H_ */
