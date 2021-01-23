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
    #include "Native_Activity.hpp"

    namespace basics { namespace internal
    {

        Native_Window::Native_Window(Id id, ANativeWindow * native_window) : Window(id)
        {
            reset_window_resource (native_window);

            /* EL FLAG AWINDOW_FLAG_LAYOUT_INSET_DECOR NO PARECE RESPONDER COMO CABRÍA ESPERAR...
            ANativeActivity_setWindowFlags
            (
               &native_activity->get_activity (),
                AWINDOW_FLAG_LAYOUT_INSET_DECOR | AWINDOW_FLAG_LAYOUT_IN_SCREEN | AWINDOW_FLAG_FORCE_NOT_FULLSCREEN,
                AWINDOW_FLAG_FULLSCREEN
            );*/
        }

        bool Native_Window::set_full_screen (bool status)
        {
            if (native_activity)
            {
                ANativeActivity_setWindowFlags
                (
                   &native_activity->get_activity (),
                    status == true  ? AWINDOW_FLAG_FULLSCREEN : 0,
                    status == false ? AWINDOW_FLAG_FULLSCREEN : 0
                );

                return true;
            }

            return false;
        }

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
