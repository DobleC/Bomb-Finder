/*
 * ANDROID ACCELEROMETER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_ANDROID_ACCELEROMETER_HEADER
#define BASICS_ANDROID_ACCELEROMETER_HEADER

    #include <basics/Accelerometer>
    #include "Android_Sensor_Manager.hpp"

     namespace basics { namespace internal
    {

        class Android_Accelerometer final : public Accelerometer
        {
        public:

            bool switch_on  () override
            {
                return android_sensor_manager.switch_on (Android_Sensor_Manager::ACCELEROMETER);
            }

            void switch_off () override
            {
                android_sensor_manager.switch_off (Android_Sensor_Manager::ACCELEROMETER);
            }

        };

    }}

#endif
