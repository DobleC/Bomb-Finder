/*
 * ACCELEROMETER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_ACCELEROMETER_HEADER
#define BASICS_ACCELEROMETER_HEADER

    namespace basics
    {

        class Accelerometer
        {
        public:

            struct State
            {
                float x;
                float y;
                float z;
            };

        public:

            static bool            is_available ();
            static Accelerometer * get_instance ();

        protected:

            State state;

        public:

            const State & get_state ()
            {
                return state;
            }

            /**
             * Puede servir para simular ciertos comportamientos del acelerómetro.
             */
            void set_state (float new_x, float new_y, float new_z)
            {
                state.x = new_x;
                state.y = new_y;
                state.z = new_z;
            }

        public:

            virtual bool switch_on  () = 0;
            virtual void switch_off () = 0;

        };

        //extern Accelerometer * const accelerometer;

    }

#endif
