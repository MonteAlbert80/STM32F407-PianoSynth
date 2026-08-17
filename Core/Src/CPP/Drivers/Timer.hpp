/*
 * Timer.h
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert
 *
 * This board provides TIM1 to TIM14
 * Advanced-control timers (TIM1 and TIM8)
 *
 * TIM2-TIM7, TIM12-TIM14 are in APB1
 * TIM1, TIM8-TIM11 are in APB2
 *
 * This provide up to 8 event listeners for each timer
 *
 */

#ifndef SRC_CPP_DRIVERS_TIMER_HPP_
#define SRC_CPP_DRIVERS_TIMER_HPP_

#include <stdint.h>
#include "stm32f4xx.h"

class Timer {
public:
    using Callback = void (*)(void* context);
	static Timer *GetInstance(int id);
	static Timer *GetSoftInstance(int id);
	void initMilliseconds(uint32_t periodMs);
	void initHz(uint32_t frequencyHz);

    bool addEventListener(Callback callback, void* context);
    bool removeEventListener(Callback callback, void* context);

    void handleInterrupt();
private:
	Timer(int id);

    static constexpr int MAX_LISTENERS = 8;

    struct Listener
    {
        Callback callback = nullptr;
        void* context = nullptr;
    };

	static Timer *_instance[14];
	Listener _listeners[MAX_LISTENERS];
	int _id;
    TIM_TypeDef *_timer;
    volatile uint32_t* _rccEnableRegister;
    uint32_t _rccEnableMask;
    uint32_t _timerClockHz;

};

#endif /* SRC_CPP_DRIVERS_TIMER_HPP_ */
