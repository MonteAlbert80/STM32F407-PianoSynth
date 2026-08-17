/*
 * piano_app_bridge.cpp
 *
 *  Created on: Aug 6, 2026
 *      Author: Monte Albert 
 *
 *
 * This file is app bridge between C language which most STM project
 * code is in, with C++ where my project files and classes are.
 * Reason I chose C++ is to use more object oriented architecture
 * for most of my future play projects
 */

#include "piano_app.h"
#include "PianoSynth.hpp"

#include "main.h"

static PianoSynth& GetPianoSynth()
{
    static PianoSynth pianoSynth;
    return pianoSynth;
}
/*
 * @brief Entry bridge from C to C++ initiating C++ objects
 */
extern "C" void PianoApp_Init(void)
{
    PianoSynth& pianoSynth = GetPianoSynth();
    const bool initialized =
        pianoSynth.init(&hi2s3, &hi2c1, &hspi1);

    if (!initialized)
    {
        Error_Handler();
    }
}

/*
 * @brief this gets called by infinite loop in main C function
 * to give more continuous control inside C++ layer. Currently
 * most of the functionality is event driven, so this code is mostly
 * doormat to keep application alive as long as the STM device has the
 * power
 */
extern "C" void PianoApp_Process(void)
{
	GetPianoSynth().process();
}
