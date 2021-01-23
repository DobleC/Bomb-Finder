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

        /** Esta clase guarda una imagen y cierto número de regiones rectangulares dentro de dicha
          * imagen que se pueden identificar mediante un Id. El objetivo es poder dibujar por separado
          * dichas regiones de la imagen sin necesidad de tratar multitud de imágenes individuales.
          * Ello permite mejorar el rendimiento de la lectura, de la descompresión y del render.
          * Las regiones rectangulares se denominan slice dentro del contexto de la clase Atlas.
          */
        class Atlas
        {
        public:

            /** Representa una de las regiones rectangulares que puede haber en la imagen que guarda
              * el atlas.
              */
            struct Slice
            {
                Atlas * atlas;          ///< Atlas a quien pertenece la región
                float   left;           ///< Coordenada X local del borde izquierdo de la región
                float   right;          ///< Coordenada X local del borde derecho de la región
                float   bottom;         ///< Coordenada X local del borde inferior de la región
                float   top;            ///< Coordenada X local del borde superior de la región
                float   width;          ///< Ancho de la región
                float   height;         ///< Alto de la región
            };

        private:

            typedef std::shared_ptr< Texture_2D > Texture_Handle;
            typedef std::map< Id, Slice >         Slice_Map;
            typedef std::vector< byte >           Buffer;

        private:

            Texture_Handle texture;
            Slice_Map      slices;

        public:

            /** Crea un atlas a partir de una imagen y de un archivo con extensión .sprites generado
              * con la herramienta DarkFunction Editor. Dentro del archivo .sprites se indica la imagen
              * que se va a cargar.
              * @param path Ruta relativa del archivo que tiene extensión .sprites dentro de la carpeta
              *     de assets.
              * @param context Contexto gráfico al que se vinculará la imagen que se cargue.
              */
            Atlas(const std::string & path, Graphics_Context::Accessor & context);
            
            /** Crea un atlas usando como imagen una textura que se ha cargado anteriormente. En este
              * caso el atlas no tendrá slices, pero se pueden añadir manualmente después llamando al
              * método add_slice().
              */
            Atlas(const Texture_Handle & texture);

        public:

            /** Permite comprobar si el atlas se pudo construir correctamente. Se considera correcto
              * cuando guarda una imagen y cuando tiene algún slice.
              * @return true si el atlas es válido.
              */
            bool good () const
            {
                return texture.get () != nullptr && slices.size () > 0;
            }

            /** Permite acceder a la imagen que guarda el atlas.
              * @return Devuelve un puntero a la imagen (como textura 2D).
              */
            const Texture_Handle & get_texture () const
            {
                return texture;
            }

            /** Busca un slice guardado en el atlas indicando su Id.
              * @return Devuelve un puntero al slice guardado en el atlas cuyo Id coincide con el que
              *     se indicó o nullptr si no se encontró un slice con dicho Id.
              */
            const Slice * get_slice (Id id) const
            {
                Slice_Map::const_iterator slice = slices.find (id);

                return slice != slices.end () ? &slice->second : nullptr;
            }

            /** Añade manualmente un nuevo slice al atlas.
              * @param id Identificador del nuevo slice. No debe existir algún slice con el mismo id.
              * @param position Coordenadas del vértice inferior izquierdo del slice sobre la textura.
              * @param size Tamaño del slice dentro de la textura.
              * @return Puntero al slice si no existía otro con el mismo id o nullptr en caso contrario.
              */
            Slice * add_slice (Id id, const Point2f & position, const Size2f & size);

            explicit operator bool () const
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
