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

#include <basics/png_decode>
#include <basics/Texture_2D>

namespace basics
{

    Id                  Texture_2D::texture_2d_specialization_ids      [10];
    Texture_2D::Factory Texture_2D::texture_2d_specialization_factories[10];
    size_t              Texture_2D::texture_2d_specialization_count;

    std::shared_ptr< Texture_2D > Texture_2D::create (Id id, Graphics_Context::Accessor & context, Color_Buffer< Rgba8888 > & color_buffer, const Options & options)
    {
        Id context_id = context->get_id ();

        for (unsigned index = 0; index < texture_2d_specialization_count; ++index)
        {
            if (texture_2d_specialization_ids[index] == context_id)
            {
                return texture_2d_specialization_factories[index] (id, color_buffer, options);
            }
        }

        return std::shared_ptr< Texture_2D >();
    }

    std::shared_ptr< Texture_2D > Texture_2D::create (Id id, Graphics_Context::Accessor & context, const std::string & asset_path, const Options & options)
    {
        std::shared_ptr< Asset > asset = Asset::open (asset_path);

        if (asset)
        {
            std::vector< byte >  data;

            if (asset->read_all (data))
            {
                Color_Buffer< Rgba8888 > color_buffer;
                Texture_2D::Options      options;

                if (png_decode (data, color_buffer, options.width, options.height))
                {
                    return Texture_2D::create (id, context, color_buffer, options);
                }
            }
        }

        return std::shared_ptr< Texture_2D >();
    }

}
