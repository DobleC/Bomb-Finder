/*
 * RASTER FONT
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802030114
 */

#include <cstring>
#include <rapidxml.hpp>
#include <basics/Raster_Font>

using namespace std;
using namespace rapidxml;

namespace basics
{

    Raster_Font::Raster_Font(const string & path, Graphics_Context::Accessor & context)
    {
        shared_ptr< Asset > font_file = Asset::open (path);

        if (font_file->good ())
        {
            Buffer font_data;

            if (font_file->read_all (font_data))
            {
                ready = parse (font_data, path, context);
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse
    (
        Buffer                     & font_data,
        const std::string          & path,
        Graphics_Context::Accessor & context
    )
    {
        // Se pone un caracter nulo al final para que el parseador de rapidxml sepa dónde está el
        // final de los datos:

        font_data.push_back (0);

        // Se parsea el xml de datos de la fuente:

        xml_document<> xml;

        xml.parse< 0 > (reinterpret_cast< char * >(font_data.data ()));

        // Se comprueba si se ha podido parsear el xml y, si se ha podido, se empieza a analizar el tag raíz:

        xml_node<> * font_tag = xml.first_node ("font");

        if (font_tag)
        {
            return parse_font (font_tag, path, context);
        }

        return false;
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_font
    (
        rapidxml::xml_node<>       * font_tag,
        const std::string          & path,
        Graphics_Context::Accessor & context
    )
    {
        xml_node<> *   info_tag = font_tag->first_node ("info"  );
        xml_node<> * common_tag = font_tag->first_node ("common");
        xml_node<> *  chars_tag = font_tag->first_node ("chars" );
        xml_node<> *  pages_tag = font_tag->first_node ("pages" );

        return
              info_tag &&
            common_tag &&
             pages_tag &&
             chars_tag &&
             parse_pages  ( pages_tag, path, context) &&
             parse_info   (  info_tag) &&
             parse_common (common_tag) &&
             parse_chars  ( chars_tag);
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_pages
    (
        rapidxml::xml_node<>       * pages_tag,
        const std::string          & path,
        Graphics_Context::Accessor & context
    )
    {
        xml_node<> * page = pages_tag->first_node ("page");

        if (page)
        {
            xml_attribute<> * file_attritube = page->first_attribute ("file");

            if (file_attritube)
            {
                // Se determina la ruta de la textura:

                size_t slash     = path.find_last_of ('/' );
                size_t backslash = path.find_last_of ('\\');
                string texture_path;

                if (slash != string::npos && backslash != string::npos)
                {
                    texture_path = path.substr (0, std::max (slash, backslash + 1));
                }
                else
                if (slash != string::npos)
                {
                    texture_path = path.substr (0, slash + 1);
                }
                else
                if (backslash != string::npos)
                {
                    texture_path = path.substr (0, backslash + 1);
                }

                // Se intenta cargar la textura:

                auto texture = Texture_2D::create (0, context, texture_path + file_attritube->value ());

                assert(texture);

                if (texture)
                {
                    context->add (texture);

                    atlas.reset (new Atlas(texture));

                    return true;
                }
            }
        }

        return false;
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_info (rapidxml::xml_node<> * info_tag)
    {
        xml_attribute<> * face_attribute = info_tag->first_attribute ("face");

        if (face_attribute)
        {
            name = face_attribute->value ();

            return true;
        }

        return false;
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_common (rapidxml::xml_node<> * common_tag)
    {
        xml_attribute<> *  pages_attribute = common_tag->first_attribute ("pages");
        xml_attribute<> * height_attribute = common_tag->first_attribute ("lineHeight");
        xml_attribute<> *   base_attribute = common_tag->first_attribute ("base");

        if (pages_attribute)
        {
            if (std::atoi (pages_attribute->value ()) != 1) return false;
        }

        if (height_attribute)
        {
            metrics.line_height = std::atoi (height_attribute->value ());

            if (base_attribute)
            {
                metrics.base_height = metrics.line_height - std::atoi (base_attribute->value ());

                return metrics.line_height > 0 && metrics.base_height < metrics.line_height;
            }
        }

        return false;
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_chars (rapidxml::xml_node<> * chars_tag)
    {
        xml_attribute<> * count_attribute = chars_tag->first_attribute ("count");

        int count = count_attribute ? std::atoi (count_attribute->value ()) : 0;
        int total = 0;

        for
        (
            xml_node<> * char_tag = chars_tag->first_node ("char");
            char_tag;
            char_tag = char_tag->next_sibling ("char")
        )
        {
            if (!parse_char (char_tag)) return false;

            total++;
        }

        return total > 0 && (total == count || count == 0);
    }

    // ---------------------------------------------------------------------------------------------

    bool Raster_Font::parse_char (rapidxml::xml_node<> * char_tag)
    {
        xml_attribute<> *       id_attribute = char_tag->first_attribute ("id"      );
        xml_attribute<> *        x_attribute = char_tag->first_attribute ("x"       );
        xml_attribute<> *        y_attribute = char_tag->first_attribute ("y"       );
        xml_attribute<> *    width_attribute = char_tag->first_attribute ("width"   );
        xml_attribute<> *   height_attribute = char_tag->first_attribute ("height"  );
        xml_attribute<> * x_offset_attribute = char_tag->first_attribute ("xoffset" );
        xml_attribute<> * y_offset_attribute = char_tag->first_attribute ("yoffset" );
        xml_attribute<> *  advance_attribute = char_tag->first_attribute ("xadvance");

        if
        (
                   id_attribute &&
                    x_attribute &&
                    y_attribute &&
                width_attribute &&
               height_attribute &&
             x_offset_attribute &&
             y_offset_attribute &&
              advance_attribute
        )
        {
            int id       = std::atoi (      id_attribute->value ());
            int x        = std::atoi (       x_attribute->value ());
            int y        = std::atoi (       y_attribute->value ());
            int width    = std::atoi (   width_attribute->value ());
            int height   = std::atoi (  height_attribute->value ());
            int x_offset = std::atoi (x_offset_attribute->value ());
            int y_offset = std::atoi (y_offset_attribute->value ());
            int advance  = std::atoi ( advance_attribute->value ());

            if (width > 0 && height > 0 && character_map.count (uint32_t(id)) == 0)
            {
                Character & character = character_map[uint32_t(id)];

                character.slice   = atlas->add_slice (Id(id), { float(x), float(y) }, { float(width), float(height) });
                character.offset  = Vector2f{ float(x_offset), float(y_offset) };
                character.advance = float(advance);

                return true;
            };
        }

        return false;
    }

}
