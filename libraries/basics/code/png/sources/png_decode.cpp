/*
 * PNG DECODE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801221221
 */

#include "lodepng.h"
#include <basics/png_decode>

namespace basics
{

    bool png_decode
    (
        const std::vector< byte > & encoded_data,
        Color_Buffer < Rgba8888 > & color_buffer,
        unsigned & width,
        unsigned & height
    )
    {
        std::vector< byte > decoded_data;

        int error = lodepng::decode
        (
            decoded_data,
            width,
            height,
            encoded_data,
            LCT_RGBA,
            8
        );

        if (!error)
        {
            color_buffer.resize (width, height);

            byte * buffer = color_buffer;

            for (auto i = decoded_data.begin (); i != decoded_data.end (); ++i)
            {
                *buffer++ = *i;
            }

            return true;
        }

        return false;
    }

}
