/*
 * SHADER PROGRAM
 * Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version 1.0
 * See the LICENSE file or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_OPENGLES_SHADER_PROGRAM_HEADER
#define BASICS_OPENGLES_SHADER_PROGRAM_HEADER

    #include <map>
    #include <vector>
    #include <string>
    #include <cassert>
    #include <basics/Graphics_Resource>
    #include <basics/Matrix>
    #include <basics/Point>
    #include <basics/Vector>
    #include <basics/opengles/Shader>

    namespace basics { namespace opengles
    {

        class Shader_Program : public Graphics_Resource
        {
        private:

            typedef std::map< std::string, GLint > Uniform_Map;

        private:

            static const Shader_Program * active_shader_program;
            static       unsigned         instance_count;

        public:

            static const Shader_Program * get_active_shader_program ()
            {
                return active_shader_program;
            }

            static void disable ()
            {
                glUseProgram (0);
            }

        private:

            std::vector< Shader::Source_Code > source_code;

            unsigned    instance_id;
            GLuint      program_object_id;
            std::string log_string;

        public:

            Shader_Program()
            {
                instance_id = instance_count++;
            }

            Shader_Program(const Shader_Program & ) = delete;

           ~Shader_Program()
            {
                finalize ();
            }

        public:

            bool initialize () override;

            void finalize () override
            {
                if (initialized)
                {
                    glDeleteProgram (program_object_id);
                }
            }

        public:

            unsigned id () const
            {
                return instance_id;
            }

            bool is_usable () const
            {
                return initialized;
            }

            void add (const Shader::Source_Code & code)
            {
                source_code.push_back (code);
            }

        private:

            bool link ();

        public:

            void use () const
            {
                assert(is_usable ());

                if (this != active_shader_program)
                {
                    glUseProgram (program_object_id);

                    active_shader_program = this;
                }
            }

        public:

            int get_uniform_id (const char * identifier) const
            {
                assert (is_usable ());

                GLint   uniform_id  = glGetUniformLocation (program_object_id, identifier);

                assert (uniform_id != -1);

                return (uniform_id);
            }

            void set_uniform_value (GLint uniform_id, const GLint     & value     ) const { glUniform1i  (uniform_id, value); }
            void set_uniform_value (GLint uniform_id, const float     & value     ) const { glUniform1f  (uniform_id, value); }
            void set_uniform_value (GLint uniform_id, const float    (& vector)[2]) const { glUniform2f  (uniform_id, vector[0], vector[1]); }
            void set_uniform_value (GLint uniform_id, const float    (& vector)[3]) const { glUniform3f  (uniform_id, vector[0], vector[1], vector[2]); }
            void set_uniform_value (GLint uniform_id, const float    (& vector)[4]) const { glUniform4f  (uniform_id, vector[0], vector[1], vector[2], vector[3]); }
            void set_uniform_value (GLint uniform_id, const Point2f   & point     ) const { glUniform2f  (uniform_id,  point[0],  point[1]); }
            void set_uniform_value (GLint uniform_id, const Point3f   & point     ) const { glUniform3f  (uniform_id,  point[0],  point[1],  point[2]); }
            void set_uniform_value (GLint uniform_id, const Point4f   & point     ) const { glUniform4f  (uniform_id,  point[0],  point[1],  point[2],  point[3]); }
            void set_uniform_value (GLint uniform_id, const Vector2f  & vector    ) const { glUniform2f  (uniform_id, vector[0], vector[1]); }
            void set_uniform_value (GLint uniform_id, const Vector3f  & vector    ) const { glUniform3f  (uniform_id, vector[0], vector[1], vector[2]); }
            void set_uniform_value (GLint uniform_id, const Vector4f  & vector    ) const { glUniform4f  (uniform_id, vector[0], vector[1], vector[2], vector[3]); }
            void set_uniform_value (GLint uniform_id, const Matrix22f & matrix    ) const { glUniformMatrix2fv (uniform_id, 1, GL_FALSE, matrix.values); }
            void set_uniform_value (GLint uniform_id, const Matrix33f & matrix    ) const { glUniformMatrix3fv (uniform_id, 1, GL_FALSE, matrix.values); }
            void set_uniform_value (GLint uniform_id, const Matrix44f & matrix    ) const { glUniformMatrix4fv (uniform_id, 1, GL_FALSE, matrix.values); }

        public:

            GLuint get_vertex_attribute_id (const char * identifier) const
            {
                assert (is_usable ());

                GLuint  attribute_id  =  glGetAttribLocation (program_object_id, identifier);

                assert (attribute_id != -1);

                return (attribute_id);
            }

            void set_vertex_attribute_id (const char * identifier, GLint attribute_id)
            {
                assert (is_usable ());

                glBindAttribLocation (program_object_id, attribute_id, identifier);
            }

            void set_vertex_attribute (GLint attribute_id, const float   & value ) { glVertexAttrib1f  (attribute_id,             value  ); }
            /*void set_vertex_attribute (GLint attribute_id, const Vector2 & vector) { glVertexAttrib2fv (attribute_id, get_values (vector)); }
            void set_vertex_attribute (GLint attribute_id, const Vector3 & vector) { glVertexAttrib3fv (attribute_id, get_values (vector)); }
            void set_vertex_attribute (GLint attribute_id, const Vector4 & vector) { glVertexAttrib4fv (attribute_id, get_values (vector)); }*/

        public:

            const std::string & log () const
            {
                return (log_string);
            }

        };

    }}

#endif
