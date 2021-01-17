/*
 * CANVAS
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801161300
 */

#include <basics/Canvas>

namespace basics
{

    Id              Canvas::canvas_specialization_ids      [10];
    Canvas::Factory Canvas::canvas_specialization_factories[10];
    size_t          Canvas::canvas_specialization_count = 0;

    Canvas * Canvas::create (Id id, Graphics_Context::Accessor & context, const Options & options)
    {
        Id context_id = context->get_id ();

        for (unsigned index = 0; index < canvas_specialization_count; ++index)
        {
            if (canvas_specialization_ids[index] == context_id)
            {
                return canvas_specialization_factories[index] (id, context, options);
            }
        }

        return nullptr;
    }

    void Canvas::draw_text (const Point2f & where, const Text_Layout & text_layout, int handling)
    {
        const Text_Layout::Glyph_List & glyphs = text_layout.get_glyphs ();

        float width  = text_layout.get_width  ();
        float height = text_layout.get_height ();
        float left   = where[0];
        float top    = where[1];

        switch (handling & 0x03)
        {
            case CENTER: left -= width * 0.5f;  break;
            case RIGHT:  left -= width;         break;
            default:     break;
        }

        switch (handling & 0x0C)
        {
            case CENTER: top  += height * 0.5f; break;
            case BOTTOM: top  += height;        break;
            default:     break;
        }

        for (auto & glyph : glyphs)
        {
            fill_rectangle
            (
                { left + glyph.position[0], top + glyph.position[1] },
                glyph.size,
                glyph.slice,
                TOP | LEFT
            );
        }
    }

}
