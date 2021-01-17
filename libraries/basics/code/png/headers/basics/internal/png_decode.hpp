/*
 *  TGA DECODE
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801221222
 */

#ifndef BASICS_PNG_DECODE_HEADER
#define BASICS_PNG_DECODE_HEADER

    #include <basics/Color_Buffer>

    namespace basics
    {

        bool png_decode (const std::vector< byte > & encoded_data, Color_Buffer< Rgba8888 > & color_buffer, unsigned & width, unsigned & height);

    }

#endif
