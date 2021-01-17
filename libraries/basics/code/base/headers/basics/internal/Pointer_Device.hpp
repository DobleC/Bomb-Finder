/*
 * POINTER DEVICE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712302305
 */

#ifndef BASICS_POINTER_DEVICE_HEADER
#define BASICS_POINTER_DEVICE_HEADER

    #include <basics/Input_Device>

    namespace basics
    {

        class Pointer_Device : public Input_Device
        {
        public:

            class State
            {
            public:


            };

        protected:

            Pointer_Device() = default;
            virtual ~Pointer_Device() = default;

        public:

            Id get_id () const
            {
                return FNV(pointer-device);
            }

            const char * get_name () const
            {
                return "pointer device";
            }

        };

        extern Touch_Surface * const touch_surface;

    }

#endif
