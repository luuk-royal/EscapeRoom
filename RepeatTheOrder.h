#pragma once
#ifndef REPEAT_THE_ORDER_H
#define REPEAT_THE_ORDER_H

#include <Arduino.h>

/*!
 * \brief LCD Keypad Shield class
 */
class RepeatTheOrder
{
public:
    RepeatTheOrder();
    void start();

    bool isCompleted();
};

#endif // REPEAT_THE_ORDER_H