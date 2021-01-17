/*
 * ANDROID SENSOR MANAGER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_ANDROID_SENSOR_MANAGER_HEADER
#define BASICS_ANDROID_SENSOR_MANAGER_HEADER

    #include <basics/macros>

    #if defined(BASICS_ANDROID_OS)

        #include <android/sensor.h>

        namespace basics { namespace internal
        {

            class Android_Sensor_Manager final
            {
            public:

                enum Sensor
                {
                    ACCELEROMETER,
                    GYROSCOPE,
                };

            private:

                static int types_for_sensor[];

            public:

                static Android_Sensor_Manager & get_instance ()
                {
                    static Android_Sensor_Manager android_sensor_manager;
                    return android_sensor_manager;
                }

            private:

                ASensorManager    * manager;
                ASensorEventQueue * event_queue;
                bool                listening;

                const ASensor     * accelerometer_sensor;
                const ASensor     * gyroscope_sensor;

            public:

                Android_Sensor_Manager()
                {
                    manager     = nullptr;
                    event_queue = nullptr;
                    listening   = false;

                    accelerometer_sensor = nullptr;
                        gyroscope_sensor = nullptr;
                }

                void wake_up   ();
                void shut_down ();

                ASensorEventQueue * create_event_queue (ALooper * looper);

                bool is_available (Sensor sensor);
                bool switch_on    (Sensor sensor);
                void switch_off   (Sensor sensor);

            };

            extern Android_Sensor_Manager & android_sensor_manager;

        }}

    #endif

#endif
