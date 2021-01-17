/*
 * COLOR BUFFER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801221200
 */

#ifndef BASICS_COLOR_BUFFER_HEADER
#define BASICS_COLOR_BUFFER_HEADER

    #include <vector>
    #include <basics/Color>

    namespace basics
    {

        template< class COLOR_TYPE >
        class Color_Buffer
        {
        public:

            typedef  COLOR_TYPE           Color_Type;
            typedef  Color_Type           Color;
            typedef  std::vector< Color > Buffer;

        public:

            unsigned width;
            unsigned height;
            Buffer   buffer;

        public:

            Color_Buffer()
            :
                width (0),
                height(0)
            {
            }

            Color_Buffer(unsigned width, unsigned height)
            :
                width (width ),
                height(height),
                buffer(width * height)
            {
            }

        public:

            unsigned size () const
            {
                return width * height;
            }

            unsigned get_width () const
            {
                return width;
            }

            unsigned get_height () const
            {
                return height;
            }

            void resize (unsigned new_width, unsigned new_height)
            {
                width  = new_width;
                height = new_height;

                buffer.resize (width * height);
                buffer.shrink_to_fit ();
            }

        public:

            Color & operator [] (unsigned index)
            {
                return buffer[index];
            }

            const Color & operator [] (unsigned index) const
            {
                return buffer[index];
            }

            operator byte * ()
            {
                return reinterpret_cast< byte * >(buffer.data ());
            }

        };

    }

#endif
