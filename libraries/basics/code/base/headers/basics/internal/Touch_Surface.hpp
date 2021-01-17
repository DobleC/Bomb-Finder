/*
 * TOUCH SURFACE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712301545
 */

#ifndef BASICS_TOUCH_SURFACE_HEADER
#define BASICS_TOUCH_SURFACE_HEADER

    #include <basics/Input_Device>

    namespace basics
    {

        class Touch_Surface : public Input_Device
        {
        public:

            struct Touch
            {
                int id;
            };

        public:

            class State
            {
            public:

                int touch_count () const;

                const Touch & get_touch (int index) const;

            };

        protected:

            Touch_Surface() = default;
            virtual ~Touch_Surface() = default;

        public:

            Id get_id () const
            {
                return FNV(touch-surface);
            }

            const char * get_name () const
            {
                return "touch surface";
            }

        };

        extern Touch_Surface * const touch_surface;

    }

#endif
