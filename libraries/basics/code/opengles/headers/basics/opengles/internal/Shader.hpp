/*
 * SHADER
 * Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_OPENGLES_SHADER_HEADER
#define BASICS_OPENGLES_SHADER_HEADER

    #include <string>
    #include <basics/opengles/OpenGL_ES2>

    namespace basics { namespace opengles
    {

        class Shader
        {
        public:

            // ESTARÍA BIEN QUE SHADER_CODE TUVIESE UN MÉTODO COMPILE() QUE LLAMASE A UN MÉTODO-PLANTILLA
            // ESPECIALIZABLE EXTERNAMENTE QUE DEVOLVIESE UN OBJETO SHADER DEL TIPO APROPIADO.
            // PARA ELLO SERÍA NECESARIO IMPLEMENTAR EL CONSTRUCTOR DE MOVIMIENTO.
            class Source_Code
            {
            public:

                enum Type
                {
                      VERTEX,
                    FRAGMENT,
                };

            public:

                static Source_Code from_string (const std::string & source_code_string, Type type)
                {
                    return Source_Code(source_code_string, type);
                }

                static Source_Code from_asset (const std::string & file_path);

            private:

                Type        type;
                std::string string;

            private:

                Source_Code(const std::string & source_code_string, Type type)
                :
                    type  (type),
                    string(source_code_string)
                {
                }

            public:

                Type   get_type     () const { return  type;            }
                size_t size         () const { return  string.size  (); }
                bool   is_empty     () const { return  string.empty (); }
                bool   is_not_empty () const { return !string.empty (); }

            public:

                operator const std::string & () const
                {
                    return (Source_Code::string);
                }

                operator const char * () const
                {
                    return (Source_Code::string.c_str ());
                }

            };

        private:

            GLuint      shader_object_id;
            std::string log_string;

        protected:

            Shader(const Source_Code & source_code, GLenum shader_type);

            virtual ~Shader();

        private:

            Shader(const Shader & );

        public:

            bool is_compiled () const
            {
                return (shader_object_id != 0);
            }

            bool compilation_failed () const
            {
                return (shader_object_id == 0);
            }

            const std::string & log () const
            {
                return (log_string);
            }

        public:

            operator GLuint () const
            {
                return (shader_object_id);
            }

        };

    }}

#endif
