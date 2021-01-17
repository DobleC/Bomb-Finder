/*
 * TEXTURE 2D
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801231014
 */

#ifndef BASICS_TEXTURE_2D_HEADER
#define BASICS_TEXTURE_2D_HEADER

    #include <memory>
    #include <string>
    #include <basics/Asset>
    #include <basics/Color_Buffer>
    #include <basics/Graphics_Context>
    #include <basics/Graphics_Resource>

    namespace basics
    {

        struct Texture_2D : public Graphics_Resource
        {
        public:

            struct Options
            {
                unsigned width;
                unsigned height;
            };

        public:

            typedef std::shared_ptr< Texture_2D > (* Factory) (Id id, Color_Buffer< Rgba8888 > & color_buffer, const Options & options);

        private:

            static Id      texture_2d_specialization_ids      [10];
            static Factory texture_2d_specialization_factories[10];
            static size_t  texture_2d_specialization_count;

        public:

            static void register_factory (Id id, Factory factory)
            {
                texture_2d_specialization_ids      [texture_2d_specialization_count] = id;
                texture_2d_specialization_factories[texture_2d_specialization_count] = factory;
                texture_2d_specialization_count++;
            }

        public:

            static std::shared_ptr< Texture_2D > create (Id id, Graphics_Context::Accessor & context, Color_Buffer< Rgba8888 > & color_buffer, const Options & options = {});
            static std::shared_ptr< Texture_2D > create (Id id, Graphics_Context::Accessor & context, const std::string & asset_path, const Options & options = {});

        protected:

            float width;
            float height;

        protected:

            Texture_2D(unsigned width, unsigned height)
            :
                width (float(width )),
                height(float(height))
            {
            }

        public:

            virtual ~Texture_2D() = default;

        public:

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
