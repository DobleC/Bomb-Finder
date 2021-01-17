/*
 * ATLAS
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802012015
 */

#ifndef BASICS_ATLAS_HEADER
#define BASICS_ATLAS_HEADER

    #include <map>
    #include <memory>
    #include <string>
    #include <vector>
    #include <rapidxml.hpp>
    #include <basics/Id>
    #include <basics/Point>
    #include <basics/Size>
    #include <basics/Texture_2D>
    #include <basics/Graphics_Context>

    namespace basics
    {

        class Atlas
        {
        public:

            struct Slice
            {
                Atlas * atlas;
                float   left;
                float   right;
                float   bottom;
                float   top;
                float   width;
                float   height;
            };

        private:

            typedef std::shared_ptr< Texture_2D > Texture_Handle;
            typedef std::map< Id, Slice >         Slice_Map;
            typedef std::vector< byte >           Buffer;

        private:

            Texture_Handle texture;
            Slice_Map      slices;

        public:

            Atlas(const std::string    & path, Graphics_Context::Accessor & context);
            Atlas(const Texture_Handle & texture);

        public:

            bool good () const
            {
                return texture.get () != nullptr && slices.size () > 0;
            }

            const Texture_Handle & get_texture () const
            {
                return texture;
            }

            const Slice * get_slice (Id id) const
            {
                Slice_Map::const_iterator slice = slices.find (id);

                return slice != slices.end () ? &slice->second : nullptr;
            }

            /**
             * Añade un nuevo slice al atlas.
             * @param id Identificador del nuevo slice. No debe existir algún slice con el mismo id.
             * @param position Coordenadas del vértice inferior izquierdo del slice sobre la textura.
             * @param size Tamaño del slice dentro de la textura.
             * @return Puntero al slice si no existía otro con el mismo id o nullptr en caso contrario.
             */
            Slice * add_slice (Id id, const Point2f & position, const Size2f & size);

            operator bool () const
            {
                return this->good ();
            }

        private:

            void parse     (Buffer           & slices_data, const std::string & path, Graphics_Context::Accessor & context);
            void parse_img (rapidxml::xml_node<> * img_tag, const std::string & path, Graphics_Context::Accessor & context);
            void parse_dir (rapidxml::xml_node<> * dir_tag, const std::string & prefix = std::string());
            void parse_spr (rapidxml::xml_node<> * spr_tag, const std::string & id);

        };

    }

#endif
