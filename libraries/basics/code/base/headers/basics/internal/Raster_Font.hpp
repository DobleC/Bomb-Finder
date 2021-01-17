/*
 * RASTER FONT
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802030037
 */

#ifndef BASICS_RASTER_FONT_HEADER
#define BASICS_RASTER_FONT_HEADER

    #include <memory>
    #include <unordered_map>
    #include <vector>
    #include <basics/Atlas>
    #include <basics/Font>
    #include <basics/Vector>

    namespace basics
    {

        class Raster_Font : public Font
        {
        public:

            struct Metrics
            {
                float line_height;
                float base_height;
            };

            struct Character : public Font::Character
            {
                Atlas::Slice * slice;
                Vector2f       offset;
                float          advance;
            };

        private:

            typedef std::unordered_map< uint32_t, Character > Character_Map;
            typedef std::vector< byte >                       Buffer;
            typedef std::unique_ptr< Atlas >                  Atlas_Handle;

        private:

            Character_Map character_map;
            Atlas_Handle  atlas;
            Metrics       metrics;

        public:

            Raster_Font(const std::string & path, Graphics_Context::Accessor & context);

        public:

            const Metrics & get_metrics () const
            {
                return metrics;
            }

            const Character * get_character (uint32_t code) const
            {
                Character_Map::const_iterator item = character_map.find (code);

                return item != character_map.end () ? &item->second : nullptr;
            }

        private:

            bool parse        (Buffer & font_data, const std::string & path, Graphics_Context::Accessor & context);
            bool parse_font   (rapidxml::xml_node<> *   font_tag, const std::string & path, Graphics_Context::Accessor & context);
            bool parse_pages  (rapidxml::xml_node<> *  pages_tag, const std::string & path, Graphics_Context::Accessor & context);
            bool parse_info   (rapidxml::xml_node<> *   info_tag);
            bool parse_common (rapidxml::xml_node<> * common_tag);
            bool parse_chars  (rapidxml::xml_node<> *  chars_tag);
            bool parse_char   (rapidxml::xml_node<> *   char_tag);

        };

    }

#endif
