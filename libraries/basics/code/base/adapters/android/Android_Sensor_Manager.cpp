/*
 * ANDROID SENSOR MANAGER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Native_Activity.hpp"
    #include "Android_Sensor_Manager.hpp"

#include <basics/Log>

    namespace basics { namespace internal
    {

        int Android_Sensor_Manager::types_for_sensor[] =
        {
            ASENSOR_TYPE_ACCELEROMETER,
            ASENSOR_TYPE_GYROSCOPE
        };

        Android_Sensor_Manager & android_sensor_manager = Android_Sensor_Manager::get_instance ();

        // -----------------------------------------------------------------------------------------

        void Android_Sensor_Manager::wake_up ()
        {
            manager = ASensorManager_getInstance ();

            ////////////////////////////////////////////////////////////////////////////////////////
            if (manager)
            {
                ASensorList sensor_list;
                int total = ASensorManager_getSensorList (manager, &sensor_list);
                log.d ("SENSORS FOUND:");
                for (int i = 0; i < total; ++i)
                {
                    log.d (std::string("    ") + ASensor_getName (sensor_list[i]));
                }
            }
            ////////////////////////////////////////////////////////////////////////////////////////
        }

        // -----------------------------------------------------------------------------------------

        void Android_Sensor_Manager::shut_down ()
        {
            if (manager)
            {
                if (event_queue)
                {
                    ASensorManager_destroyEventQueue (manager, event_queue);
                }
            }
        }

        // -----------------------------------------------------------------------------------------

        ASensorEventQueue * Android_Sensor_Manager::create_event_queue (ALooper * looper)
        {
            if (manager)
            {
                if (event_queue)
                {
                    ASensorManager_destroyEventQueue (manager, event_queue);
                }

                return event_queue = ASensorManager_createEventQueue (manager, looper, 1, nullptr, nullptr);
            }

            return nullptr;
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Sensor_Manager::is_available (Sensor sensor)
        {
            return
                manager != nullptr &&
                ASensorManager_getDefaultSensor (manager, types_for_sensor[sensor]) != nullptr;
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Sensor_Manager::switch_on (Sensor sensor)
        {
            if (manager)
            {
                if (!listening)
                {
                    // Cuando se cree el hilo, se creará el looper y con él se llamará al método
                    // create_event_queue() de esta clase:

                    native_activity->start_sensor_thread ();
                }

                if (event_queue)        // Se ha creado el hilo, el looper y la event_queue?
                {
                    accelerometer_sensor = ASensorManager_getDefaultSensor (manager, types_for_sensor[sensor]);

                    if (accelerometer_sensor)
                    {
                        if (ASensorEventQueue_enableSensor (event_queue, accelerometer_sensor) == 0)
                        {
                            ASensorEventQueue_setEventRate (event_queue, accelerometer_sensor, (1000L / 50) * 1000);

                            return true;
                        }
                    }
                }
            }

            return false;
        }

        // -----------------------------------------------------------------------------------------

        void Android_Sensor_Manager::switch_off (Sensor sensor)
        {
            if (manager && event_queue)
            {
                const ASensor * sensor_handler = nullptr;

                switch (sensor)
                {
                    case ACCELEROMETER: sensor_handler = accelerometer_sensor; break;
                    case GYROSCOPE:     sensor_handler =     gyroscope_sensor; break;
                }

                if (sensor_handler)
                {
                    ASensorEventQueue_disableSensor (event_queue, sensor_handler);
                }
            }
        }

    }}

#endif
