/*
 * INPUT DEVICE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712301540
 */

#ifndef BASICS_INPUT_DEVICE_HEADER
#define BASICS_INPUT_DEVICE_HEADER

    #include <basics/Id>

    namespace basics
    {

        class Input_Device
        {
        public:

            virtual Id get_id () const = 0;
            virtual const char * get_name () const = 0;

        public:

            virtual bool has_state () const = 0;
            virtual bool has_queue () const = 0;
            virtual bool keep_state (bool keep) = 0;
            virtual bool keep_queue (bool keep) = 0;

        };

    }

#endif
