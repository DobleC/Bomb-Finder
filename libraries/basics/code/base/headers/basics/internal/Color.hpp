/*
 * COLOR
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801221144
 */

#ifndef BASICS_COLOR_HEADER
#define BASICS_COLOR_HEADER

    #include <basics/types>

    namespace basics
    {

        template< typename COMPONENT_TYPE, unsigned NUMBER_OF_COMPONENTS >
        class Color
        {
        public:

            typedef COMPONENT_TYPE Component_Type;
            typedef Component_Type Component;

            static constexpr size_t number_of_components = NUMBER_OF_COMPONENTS;

        public:

            Component components[number_of_components];

        };

        typedef uint8_t  Rgb332;
        typedef uint16_t Rgb565;
        typedef uint8_t  Rgb888[8];

        typedef uint8_t  Bgr332;
        typedef uint16_t Bgr565;
        typedef uint8_t  Bgr888[8];

        typedef uint16_t Argb4444;
        typedef uint16_t Argb1555;
        typedef uint32_t Argb8888;

        typedef uint16_t Rgba4444;
        typedef uint16_t Rgba5551;
        typedef uint32_t Rgba8888;

        typedef uint16_t Abgr4444;
        typedef uint16_t Abgr1555;
        typedef uint32_t Abgr8888;

    }

#endif
