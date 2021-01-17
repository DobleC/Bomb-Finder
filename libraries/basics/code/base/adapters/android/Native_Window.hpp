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

#ifndef BASICS_NATIVE_WINDOW_HEADER
#define BASICS_NATIVE_WINDOW_HEADER

    #include <basics/Window>
    #include <android/native_window.h>

    namespace basics { namespace internal
    {

        class Native_Window final : public Window
        {
        public:

            class Accessor : public Window::Accessor
            {
            public:

                Native_Window * get ()
                {
                    return static_cast< Native_Window * >(window.get ());
                }

            };

        private:

            ANativeWindow * native_window;

        public:

            Native_Window(Id id, ANativeWindow * native_window) : Window(id)
            {
                reset_window_resource (native_window);
            }

           ~Native_Window ()
            {
                reset_window_resource ();
            }

        public:

            ANativeWindow * get_native_window ()
            {
                return native_window;
            }

            Graphics_Context * get_graphics_context ()
            {
                return graphics.context.get ();
            }

            Size2u get_size () override
            {
                Size2u size{ 0, 0 };

                if (native_window)
                {
                    size.width  = unsigned(ANativeWindow_getWidth  (native_window));
                    size.height = unsigned(ANativeWindow_getHeight (native_window));
                }

                return size;
            };

            unsigned get_width () override
            {
                return native_window ? unsigned(ANativeWindow_getWidth  (native_window)) : 0;
            }

            unsigned get_height () override
            {
                return native_window ? unsigned(ANativeWindow_getHeight (native_window)) : 0;
            }

        public:

            void set_focused (bool state)
            {
                focused = state;
            }

        public:

            void reset_window_resource (ANativeWindow * new_native_window = nullptr);

        };

    }}

#endif
