/*
 * OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712200139
 */

#ifndef BASICS_OPENGLES_CONTEXT_HEADER
#define BASICS_OPENGLES_CONTEXT_HEADER

    #include <memory>
    #include <basics/Window>
    #include <basics/Graphics_Context>

    namespace basics { namespace opengles
    {

        class Context : public basics::Graphics_Context
        {
        public:

            enum Version
            {
                VERSION_1_0 = 10,
                VERSION_1_1 = 11,
                VERSION_2_0 = 20,
                VERSION_3_0 = 30,
                VERSION_3_1 = 31,
                VERSION_3_2 = 32,
            };

        public:

            // Este método debe recibir los atributos deseados para el contexto...
            static bool create (basics::Window::Accessor & window, Graphics_Resource_Cache * cache);

        protected:

            Version version;

        protected:

            Context(Window & window, Graphics_Resource_Cache * cache) : Graphics_Context(window, cache)
            {
            }

            virtual ~Context() = default;

        public:

            Id get_id () const override
            {
                switch (version)
                {
                    case VERSION_1_0:
                    case VERSION_1_1:
                    {
                        return ID(opengles1);
                    }

                    case VERSION_2_0:
                    {
                        return ID(opengles2);
                    }

                    case VERSION_3_0:
                    case VERSION_3_1:
                    case VERSION_3_2:
                    {
                        return ID(opengles3);
                    }
                }
            }

            Version get_version () const
            {
                return version;
            }

            Renderer * get_renderer ();

        };

    }}

#endif
