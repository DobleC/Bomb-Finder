/*
 * NATIVE WINDOW
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712171712
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Native_Window.hpp"

    namespace basics { namespace internal
    {

        void Native_Window::reset_window_resource (ANativeWindow * new_native_window)
        {
            if (new_native_window != nullptr)
            {
                if (native_window != new_native_window)
                {
                    native_window  = new_native_window;

                    if (graphics.context)
                    {
                        reset_graphics_context ();

                        event_queue.push (Event(LOST_GRAPHICS_CONTEXT));
                    }
                }
                else
                if (graphics.context)
                {
                    if (!graphics.context->resume ())
                    {
                        reset_graphics_context ();

                        event_queue.push (Event(LOST_GRAPHICS_CONTEXT));
                    }
                }

                available = true;
            }
            else
            {
                available = false;

                if (graphics.context)
                {
                    graphics.context->suspend ();
                }
            }
        }

    }}

#endif
