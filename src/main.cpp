/** @file main.cpp
 *    This file contains a simple test program with which C++ for the ESP32 using 
 *    Arduino is tested along with various other tools such as Doxygen and Git.
 *
 *  @author JR Ridgely
 *  @date 2020-24-Aug Original file 
 *  @date 2020-04-Sep Improved comments while testing Doxygen on VSCode
 */

#include <Arduino.h>
#include <PrintStream.h>
#include <ESP32Encoder.h>


// This line tells setup() that there will be a function called anothertask() later in
// the file; if it weren't here, there would be a compiler error
void (anotherTask)(void*);

/// The ESP32 encoder reader object which will be used in this program
ESP32Encoder encoder;


/** @brief   Arduino setup function which runs once at program startup.
 *  @details This function sets up a serial port for communication and creates the
 *           tasks which will be run. Hardware such as an optical encoder is also
 *           set up in this function.
 */
void setup (void) 
{
    Serial.begin (115200);
    delay (1000);

    // This line uses the PrintStream library to send text conveniently
    Serial << "ESP32, blah blah blah" << endl;

    /* we create a new task here */
    xTaskCreate (
        anotherTask,        /* Task function. */
        "another Task",     /* name of task. */
        10000,              /* Stack size of task */
        NULL,               /* parameter of the task */
        1,                  /* priority of the task */
        NULL);              // Task handle

    // // Set up an encoder reader... setup() is given the ISR callback
    // rotaryEncoder1.begin ();
	  // rotaryEncoder1.setup ([]{rotaryEncoder1.readEncoder_ISR();});

    // ESP32Encoder::         // useInternalWeakPullResistors = UP;
    encoder.attachFullQuad (22, 23);
    encoder.setCount (0);
}


/** @brief   A fairly useless demonstration task function.
 *  @details This function is run by the RTOS scheduler at a higher priority
 *           than @c loop(), so it can interrupt @c loop() when it needs to.
 *           It just prints an asterisk once every two seconds.
 *  @param   p_params A pointer to any parameters passed to this task; there
 *           aren't any, so this value is ignored
 */
void anotherTask (void* p_params)
{
    // The setup part of a task function; this one doesn't really do anything
    (void)p_params;

    // Every task function should contain an infinite loop
    for(;;)
    {
        Serial.print (" * ");
        delay (2000);
    }
}


/** @brief   Arduino's low-priority loop function.
 *  @details A non-RTOS Arduino program runs all of its continuously running
 *           code in this function after @c setup() has finished. When using
 *           FreeRTOS, @c loop() implements a low priority task on most
 *           microcontrollers, and crashes on some others. This demonstration
 *           function reads the optical encoder and prints its position. 
 */
void loop (void)
{
    delay (1000);

    // There is no need for an infinite loop inside this function because it
    // will be called repeatedly by the Arduino system
    Serial.printf ("<%ld>", (int32_t)encoder.getCount ());
}

