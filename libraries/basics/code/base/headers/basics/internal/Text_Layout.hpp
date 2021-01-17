/*
 * TEXT LAYOUT
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802030123
 */

#ifndef BASICS_TEXT_LAYOUT_HEADER
#define BASICS_TEXT_LAYOUT_HEADER

    #include <string>
    #include <vector>
    #include <basics/Raster_Font>
    #include <basics/Point>
    #include <basics/Size>

    namespace basics
    {

        class Text_Layout
        {
        public:

            struct Glyph
            {
                const Atlas::Slice * slice;
                Point2f position;
                Size2f  size;

                Glyph(const Atlas::Slice * slice, const Point2f & position, const Size2f & size)
                :
                    slice(slice), position(position), size(size)
                {
                }
            };

            typedef std::vector< Glyph > Glyph_List;

        private:

            Glyph_List glyphs;
            float      width;
            float      height;

        public:

            Text_Layout(const Raster_Font & font, const std::wstring & text);

        public:

            const Glyph_List & get_glyphs () const
            {
                return glyphs;
            }

            float get_width () const
            {
                return width;
            }

            float get_height () const
            {
                return height;
            }

        };

    }

#endif
