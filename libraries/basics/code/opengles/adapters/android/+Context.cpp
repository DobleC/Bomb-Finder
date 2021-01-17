/*
 * OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712202250
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Android_OpenGL_ES_Context.hpp"
    #include "../../../base/adapters/android/Native_Window.hpp"

    namespace basics { namespace opengles
    {

        bool Context::create (basics::Window::Accessor & window, Graphics_Resource_Cache * cache)
        {
            if (window && window->is_available () && !window->has_graphics_context ())
            {
                std::shared_ptr< Graphics_Context > context
                (
                    new basics::opengles::internal::Android_OpenGL_ES_Context
                    (
                        *static_cast< basics::internal::Native_Window::Accessor & >(window).get (),
                         cache
                    )
                );

                if (context->is_available () && window->set_graphics_context (context))
                {
                    return context->make_current ();
                }
            }

            return false;
        }

    }}

#endif
