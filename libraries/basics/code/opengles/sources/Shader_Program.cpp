/*
 * SHADER PROGRAM
 * Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/opengles/Fragment_Shader>
#include <basics/opengles/OpenGL_ES2>
#include <basics/opengles/Shader_Program>
#include <basics/opengles/Vertex_Shader>

namespace basics { namespace opengles
{

    const Shader_Program * Shader_Program::active_shader_program = nullptr;
    unsigned int           Shader_Program::instance_count        = 0;

    bool Shader_Program::initialize ()
    {
        if (!initialized)
        {
            if (source_code.size () > 0)
            {
                program_object_id = glCreateProgram ();

                assert(program_object_id != 0);

                std::vector< std::shared_ptr< Shader > > shaders(source_code.size ());

                for (unsigned i = 0; i < source_code.size (); ++i)
                {
                    switch (source_code[i].get_type ())
                    {
                        case Shader::Source_Code::VERTEX:   shaders[i].reset (new   Vertex_Shader(source_code[i])); break;
                        case Shader::Source_Code::FRAGMENT: shaders[i].reset (new Fragment_Shader(source_code[i])); break;
                    }

                    if (shaders[i]->compilation_failed ())
                    {
                        return (initialized = false);
                    }

                    glAttachShader (program_object_id, *shaders[i]);
                }

                return (initialized = link ());         // EN CASO DE FALLO HAY QUE LIBERAR EL OBJETO SHADER PROGRAM (LOS SHADERS SE LIBERAN CON SHARED_PTR)
            }
        }

        return (initialized = false);
    }

    bool Shader_Program::link ()
    {
        glLinkProgram (program_object_id);

        // Se comprueba si el linkage ha tenido éxito:

        GLint succeeded = 0;

        glGetProgramiv (program_object_id, GL_LINK_STATUS, &succeeded);

        if (!succeeded)
        {
            // Se guarda el log del error que ha ocurrido:

            GLint log_length;

            glGetShaderiv (program_object_id, GL_INFO_LOG_LENGTH, &log_length);

            if (log_length > 0)
            {
                log_string.resize  (log_length);

                glGetShaderInfoLog (program_object_id, log_length, NULL, &log_string.front ());
            }

            assert(false);
        }

        return succeeded != 0;
    }

}}
