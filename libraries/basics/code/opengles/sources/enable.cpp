/*
 * ENABLE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version 1.0
 * See the LICENSE file or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801231242
 */

#include <basics/enable>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/OpenGL_ES2>
#include <basics/opengles/Texture_2D>

namespace basics
{

    template< >
    bool enable< OpenGL_ES2 > ()
    {
        opengles::Canvas_ES2::enable ();
        opengles::Texture_2D::enable ();

        return true;
    }

}
