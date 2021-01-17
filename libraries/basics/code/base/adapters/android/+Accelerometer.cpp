/*
 * ACCELEROMETER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Accelerometer>
#include "Android_Accelerometer.hpp"

namespace basics
{

    bool Accelerometer::is_available ()
    {
        return internal::android_sensor_manager.is_available (internal::Android_Sensor_Manager::ACCELEROMETER);
    }

    Accelerometer * Accelerometer::get_instance ()
    {
        static internal::Android_Accelerometer accelerometer;

        if (Accelerometer::is_available ())
        {
            return &accelerometer;
        }

        return nullptr;
    }

}
