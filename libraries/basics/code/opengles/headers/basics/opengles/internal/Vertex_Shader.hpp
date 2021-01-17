/*
 * VERTEX SHADER
 * Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_OPENGLES_VERTEX_SHADER_HEADER
#define BASICS_OPENGLES_VERTEX_SHADER_HEADER

    #include <basics/opengles/Shader>

    namespace basics { namespace opengles
    {

        class Vertex_Shader : public Shader
        {
        public:

            Vertex_Shader(const Source_Code & source_code)
            :
                Shader(source_code, GL_VERTEX_SHADER)
            {
            }

        };

    }}

#endif
