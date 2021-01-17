/*
 * TEXTURE 2D
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version 1.0
 * See the LICENSE file or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801221331
 */

#ifndef BASICS_OPENGLES_TEXTURE_2D_HEADER
#define BASICS_OPENGLES_TEXTURE_2D_HEADER

    #include <basics/Color_Buffer>
    #include <basics/Graphics_Resource>
    #include <basics/opengles/OpenGL_ES2>
    #include <basics/Texture_2D>

    namespace basics { namespace opengles
    {

        class Texture_2D : public basics::Texture_2D
        {

            static const Texture_2D * active_texture;

        public:

            static std::shared_ptr< basics::Texture_2D > create (Id id, Color_Buffer< Rgba8888 > & color_buffer, const Options & options = {});

        public:

            static void enable ()
            {
                register_factory (ID(opengles2), basics::opengles::Texture_2D::create);
            }

            static void unuse ()
            {
                glBindTexture (GL_TEXTURE_2D, 0);
            }

        private:

            Color_Buffer< Rgba8888 > color_buffer;
            GLuint texture_object_id;

        public:

            Texture_2D(const Color_Buffer< Rgba8888 > & color_buffer, unsigned width, unsigned height)
            :
                basics::Texture_2D(width, height),
                color_buffer      (color_buffer )
            {
            }

            Texture_2D(const Texture_2D & ) = delete;

           ~Texture_2D()
            {
                if (active_texture == this) active_texture = nullptr;

                finalize ();
            }

        public:

            bool initialize () override;

            void finalize () override
            {
                if (initialized)
                {
                    glDeleteTextures (1, &texture_object_id);
                }
            }

        public:

            bool is_usable () const
            {
                return initialized;
            }

        public:

            bool use () const;

        };

    }}

#endif
