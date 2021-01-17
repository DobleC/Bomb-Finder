/*
 * TEXT LAYOUT
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802030140
 */

#include <basics/Text_Layout>

namespace basics
{

    Text_Layout::Text_Layout(const Raster_Font & font, const std::wstring & text)
    :
        width (0.f),
        height(0.f)
    {
        Raster_Font::Metrics metrics = font.get_metrics ();

        glyphs.reserve (text.length ());

        float current_x  = 0;
        float current_y  = -metrics.line_height;
        float line_width = 0;

        for (auto & c : text)
        {
            if (c == L'\n')
            {
                if (current_x > width) width = current_x;

                current_x  = 0.f;
                current_y -= metrics.line_height;
            }
            else
            {
                const Raster_Font::Character * character = font.get_character (uint32_t(c));

                if (character)
                {
                    glyphs.emplace_back
                    (
                         character->slice,
                         Point2f{ current_x + character->offset[0], current_y + metrics.line_height - character->offset[1] },
                         Size2f { character->slice->width, character->slice->height }
                    );

                    if (current_x == 0.f) height += metrics.line_height;

                    current_x += character->advance;
                }
            }
        }

        if (current_x > width) width = current_x;
    }

}
