/*
 * TEXTURE 2D
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version 1.0
 * See the LICENSE file or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801221334
 */

#include <basics/assert>
#include <basics/opengles/Texture_2D>

namespace basics { namespace opengles
{

    const Texture_2D * Texture_2D::active_texture = nullptr;

    std::shared_ptr< basics::Texture_2D > Texture_2D::create (Id id, Color_Buffer< Rgba8888 > & color_buffer, const Options & options)
    {
        return std::shared_ptr< Texture_2D >(new Texture_2D(color_buffer, options.width, options.height));
    }

    bool Texture_2D::initialize ()
    {
        if (!initialized)
        {
            if (color_buffer.size () > 0)
            {
                glEnable        (GL_TEXTURE_2D);////
                glGenTextures   (1, &texture_object_id);
                glBindTexture   (GL_TEXTURE_2D, texture_object_id);

                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D
                (
                    GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    color_buffer.get_width  (),
                    color_buffer.get_height (),
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    color_buffer
                );

                int error = glGetError ();

                assert(glGetError () == GL_NO_ERROR);
                assert(width > 0 && height > 0);

                initialized = true;
            }
        }

        return initialized;
    }

    bool Texture_2D::use () const
    {
        assert(is_usable ());

        //if (active_texture != this)
        {
            glBindTexture   (GL_TEXTURE_2D, texture_object_id);
            glActiveTexture (GL_TEXTURE0);

            active_texture  = this;

            return true;
        }

        return false;
    }

}}
