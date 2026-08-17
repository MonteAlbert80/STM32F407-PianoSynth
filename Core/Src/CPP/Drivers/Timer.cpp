/*
 * Timer.cpp
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
 */

#include "Timer.hpp"
#include "stm32f4xx.h"

#define RCC_APB2_TIM1EN			(1U<<0)
#define RCC_APB1_TIM2EN			(1U<<0)
#define RCC_APB1_TIM3EN			(1U<<1)
#define RCC_APB1_TIM4EN			(1U<<2)
#define RCC_APB1_TIM5EN			(1U<<3)
#define RCC_APB1_TIM6EN			(1U<<4)
#define RCC_APB1_TIM7EN			(1U<<5)
#define RCC_APB2_TIM8EN			(1U<<1)
#define RCC_APB2_TIM9EN			(1U<<16)
#define RCC_APB2_TIM10EN		(1U<<17)
#define RCC_APB2_TIM11EN		(1U<<18)
#define RCC_APB1_TIM12EN		(1U<<6)
#define RCC_APB1_TIM13EN		(1U<<7)
#define RCC_APB1_TIM14EN		(1U<<8)



// Implicitly zero-initialized to nullptr by the compiler automatically.
Timer* Timer::_instance[14];

/*
 * TIM2-TIM7, TIM12-TIM14 are in APB1
 * TIM1, TIM8-TIM11 are in APB2
 * WE are giving more abstraction in the init function hence switch case in the constructor here
 */

Timer::Timer(int id)
{
    _id = id;

    if ((id==1)||((id>=8)&&(id<=11)))	//APB2
    {
        _rccEnableRegister = &RCC->APB2ENR;
        _timerClockHz = 168000000U;
    }
    else	//APB1
    {
        _rccEnableRegister = &RCC->APB1ENR;
        _timerClockHz = 84000000U;
    }
    switch (id)
    {
    case 1:
        _timer = TIM1;
        _rccEnableMask = RCC_APB2ENR_TIM1EN;
        break;

    case 2:
        _timer = TIM2;
        _rccEnableMask = RCC_APB1ENR_TIM2EN;
        break;

    case 3:
        _timer = TIM3;
        _rccEnableMask = RCC_APB1ENR_TIM3EN;
        break;

    case 4:
        _timer = TIM4;
        _rccEnableMask = RCC_APB1ENR_TIM4EN;
        break;

    case 5:
        _timer = TIM5;
        _rccEnableMask = RCC_APB1ENR_TIM5EN;
        break;

    case 6:
        _timer = TIM6;
        _rccEnableMask = RCC_APB1ENR_TIM6EN;
        break;

    case 7:
        _timer = TIM7;
        _rccEnableMask = RCC_APB1ENR_TIM7EN;
        break;

    case 8:
        _timer = TIM8;
        _rccEnableMask = RCC_APB2ENR_TIM8EN;
        break;

    case 9:
        _timer = TIM9;
        _rccEnableMask = RCC_APB2ENR_TIM9EN;
        break;

    case 10:
        _timer = TIM10;
        _rccEnableMask = RCC_APB2ENR_TIM10EN;
        break;

    case 11:
        _timer = TIM11;
        _rccEnableMask = RCC_APB2ENR_TIM11EN;
        break;

    case 12:
        _timer = TIM12;
        _rccEnableMask = RCC_APB1ENR_TIM12EN;
        break;

    case 13:
        _timer = TIM13;
        _rccEnableMask = RCC_APB1ENR_TIM13EN;
        break;

    case 14:
        _timer = TIM14;
        _rccEnableMask = RCC_APB1ENR_TIM14EN;
        break;

    default:
        _timer = nullptr;
        _rccEnableRegister = nullptr;
        _rccEnableMask = 0;
        _timerClockHz = 0;
        break;
    }
}
/*
 * @brief Since this board provides 14 timers, TIM1 to TIM14, we can get 14 instances out of this
 * @param id it should be 1 to 14
 * @return	Timer instance for TIM1 to TIM14. If ID is not in the range, it will return nullptr
 */
Timer *Timer::GetInstance(int id)
{
	if ((id<1)||(id>14))
	{
		return nullptr;
	}
	id--; //because we have zero based array index
	if (_instance[id]==nullptr)
	{
		_instance[id] = new Timer(id+1);
	}
	return _instance[id];
}
/*
 * @brief this one wont create instance if it's not there.
 * @param id it should be 1 to 14
 * @return	Timer instance for TIM1 to TIM14. If ID is not in the range, it will return nullptr
 */
Timer *Timer::GetSoftInstance(int id)
{
	if ((id<1)||(id>14))
	{
		return nullptr;
	}
	return _instance[id-1];
}

/*
 * @brief initilize the timer in milliseconds. It tells after how many milliseconds does timer need to interrupt
 * @param periodMs	milliseconds
 */
void Timer::initMilliseconds(uint32_t periodMs)
{
    if ((periodMs == 0U) ||
        (_timer == nullptr) ||
        (_rccEnableRegister == nullptr))
    {
        return;
    }

    constexpr uint32_t TIMER_TICK_HZ = 10000U;

    // 1. Enable TIMx peripheral clock
    *_rccEnableRegister |= _rccEnableMask;

    // 2. Configure timer period
    _timer->PSC =
        (_timerClockHz / TIMER_TICK_HZ) - 1U;

    _timer->ARR =
        (periodMs * (TIMER_TICK_HZ / 1000U)) - 1U;

    _timer->CNT = 0;

    // 3. Force registers to be loaded now
    _timer->EGR |= TIM_EGR_UG;

    // Clear any update flag created by UG
    _timer->SR &= ~TIM_SR_UIF;

    // 4. Enable update interrupt inside TIMx
    _timer->DIER |= TIM_DIER_UIE;

    // 5. Enable corresponding interrupt in NVIC
    switch (_id)
    {
    case 1:
        // TIM1 update interrupt shares vector with TIM10
        NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 6);
        NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
        break;

    case 2:
        NVIC_SetPriority(TIM2_IRQn, 6);
        NVIC_EnableIRQ(TIM2_IRQn);
        break;

    case 3:
        NVIC_SetPriority(TIM3_IRQn, 6);
        NVIC_EnableIRQ(TIM3_IRQn);
        break;

    case 4:
        NVIC_SetPriority(TIM4_IRQn, 6);
        NVIC_EnableIRQ(TIM4_IRQn);
        break;

    case 5:
        NVIC_SetPriority(TIM5_IRQn, 6);
        NVIC_EnableIRQ(TIM5_IRQn);
        break;

    case 6:
        NVIC_SetPriority(TIM6_DAC_IRQn, 6);
        NVIC_EnableIRQ(TIM6_DAC_IRQn);
        break;

    case 7:
        NVIC_SetPriority(TIM7_IRQn, 6);
        NVIC_EnableIRQ(TIM7_IRQn);
        break;

    case 8:
        // TIM8 update interrupt shares vector with TIM13
        NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 6);
        NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
        break;

    case 9:
        // TIM9 shares vector with TIM1 break
        NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 6);
        NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
        break;

    case 10:
        // TIM10 shares vector with TIM1 update
        NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 6);
        NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
        break;

    case 11:
        // TIM11 shares vector with TIM1 trigger/commutation
        NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 6);
        NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
        break;

    case 12:
        // TIM12 shares vector with TIM8 break
        NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 6);
        NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
        break;

    case 13:
        // TIM13 shares vector with TIM8 update
        NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 6);
        NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
        break;

    case 14:
        // TIM14 shares vector with TIM8 trigger/commutation
        NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 6);
        NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
        break;

    default:
        break;
    }
    // 6. Start timer
    _timer->CR1 |= TIM_CR1_CEN;
}
/*
 * @brief you can also initialize timer in frequency in Hz
 * @param frequencyHz is frequency in hertz
 */
void Timer::initHz(uint32_t frequencyHz)
{
    if (frequencyHz == 0U)
        return;

    initMilliseconds(1000U / frequencyHz);
}
/*
 * @brief Adds event handler for timer tick
 * @param callback is the callback function
 * @param context is the pointer to object for that context
 */
bool Timer::addEventListener(Callback callback, void* context)
{
    if (callback == nullptr)
    {
        return false;
    }

    for (int i = 0; i < MAX_LISTENERS; i++)
    {
        if (_listeners[i].callback == nullptr)
        {
            _listeners[i].callback = callback;
            _listeners[i].context = context;

            return true;
        }
    }

    return false;
}

/*
 * @brief Removes event handler for timer tick
 * @param callback is the callback function
 * @param context is the pointer to object for that context
 */
bool Timer::removeEventListener(Callback callback, void* context)
{
    for (int i = 0; i < MAX_LISTENERS; i++)
    {
        if ((_listeners[i].callback == callback) &&
            (_listeners[i].context == context))
        {
            _listeners[i].callback = nullptr;
            _listeners[i].context = nullptr;

            return true;
        }
    }

    return false;
}


void Timer::handleInterrupt()
{
	_timer->SR &= ~TIM_SR_UIF;
    for (int i = 0; i < MAX_LISTENERS; i++)
    {
        if (_listeners[i].callback != nullptr)
        {
            _listeners[i].callback(_listeners[i].context);
        }
    }
}

/*
 * Following code is hardware interrupt handlers in C language.
 * And they are mostly pre-defined interrupt handlers so I have to stick with their names
 */
extern "C" void TIM1_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(1);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM2_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(2);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM3_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(3);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM4_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(4);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM5_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(5);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM6_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(6);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM7_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(7);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM8_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(8);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM9_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(9);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM10_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(10);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM11_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(11);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM12_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(12);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM13_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(13);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

extern "C" void TIM14_IRQHandler(void)
{
    Timer *ins = Timer::GetInstance(14);
    if (ins != nullptr)
    	ins->handleInterrupt();
}

