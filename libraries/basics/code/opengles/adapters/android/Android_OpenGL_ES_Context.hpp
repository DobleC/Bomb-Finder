/*
 * ANDROID OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712202240
 */

#ifndef BASICS_ANDROID_OPENGL_ES_CONTEXT_HEADER
#define BASICS_ANDROID_OPENGL_ES_CONTEXT_HEADER

    #include <atomic>
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <basics/opengles/Context>

    namespace basics { namespace internal
    {
        class Native_Window;
    }}

    namespace basics { namespace opengles { namespace internal
    {

        using std::atomic;

        class Android_OpenGL_ES_Context final : public opengles::Context
        {

            ANativeWindow * native_window;
            EGLDisplay      display;
            EGLSurface      surface;
            EGLContext      context;
            EGLConfig       config;

            atomic< bool >  initialized;
            atomic< bool >  available;

			EGLint   		surface_width;
			EGLint  		surface_height;

        public:

            Android_OpenGL_ES_Context(basics::internal::Native_Window & window, Graphics_Resource_Cache * cache);

           ~Android_OpenGL_ES_Context()
            {
                finalize ();
            }

        public:

            bool is_available () const override
            {
                return available;
            }

            void invalidate () override
            {
                available = false;
            }

            void suspend () override;
            bool resume () override;
            void finalize () override;

            bool is_current () const override;
            bool make_current () override;

            bool set_sync_swap (bool activated) override;
            bool flush_and_display () override;

			unsigned get_surface_width () override
			{
				return unsigned(surface_width);
			}

			unsigned get_surface_height () override
			{
				return unsigned(surface_height);
			}

            void reset_viewport () override;

            void set_viewport (const Point2u & bottom_left, const Size2u & size) override;

        private:

            bool initialize_display ();
            bool initialize_surface ();
            bool initialize_context ();

            void finalize_display ();
            void finalize_surface ();
            void finalize_context ();

        };

    }}}

#endif
