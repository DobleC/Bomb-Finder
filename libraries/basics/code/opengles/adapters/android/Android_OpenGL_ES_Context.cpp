/*
 * ANDROID OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712200208
 */

// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglGetDisplay.xhtml
// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglInitialize.xhtml
// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglChooseConfig.xhtml
// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglCreateWindowSurface.xhtml
// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglCreateContext.xhtml
// https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglMakeCurrent.xhtml
// http://directx.com/2014/06/egl-understanding-eglchooseconfig-then-ignoring-it

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include <basics/opengles/OpenGL_ES1>
    #include "Android_OpenGL_ES_Context.hpp"
    #include "../../../base/adapters/android/Native_Window.hpp"

    #define  EGL_ATTRIBUTE(ATTRIBUTE, VALUE) ATTRIBUTE, VALUE

    namespace basics { namespace opengles { namespace internal
    {

        Android_OpenGL_ES_Context::Android_OpenGL_ES_Context(basics::internal::Native_Window & window, Graphics_Resource_Cache * cache) : basics::opengles::Context(window, cache)
        {
            native_window = window.get_native_window ();
            display       = EGL_NO_DISPLAY;
            surface       = EGL_NO_SURFACE;
            context       = EGL_NO_CONTEXT;
            config        = nullptr;
            available     = initialized = native_window && initialize_display () && initialize_surface () && initialize_context ();
            version       = VERSION_2_0;
        }

        void Android_OpenGL_ES_Context::suspend ()
        {
            if (initialized)
            {
                available = false;

                finalize_surface ();
            }
        }

        bool Android_OpenGL_ES_Context::resume ()
        {
            if (initialized)
            {
                return available = initialize_display () && initialize_surface ();
            }

            return false;
        }

        void Android_OpenGL_ES_Context::finalize ()
        {
            Graphics_Context::finalize ();

            available = false;

            if (display != EGL_NO_DISPLAY)
            {
                eglWaitClient  ();
                eglMakeCurrent (display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

                finalize_context ();
                finalize_surface ();
                finalize_display ();
            }
        }

        bool Android_OpenGL_ES_Context::is_current () const
        {
            if (available)
            {
                return eglGetCurrentContext () == context;
            }

            return false;
        }

        bool Android_OpenGL_ES_Context::set_sync_swap (bool activated)
        {
            if (available)
            {
                return eglSwapInterval (display, activated ? 1 : 0) == EGL_TRUE;
            }

            return false;
        }

        bool Android_OpenGL_ES_Context::make_current ()
        {
            if (available)
            {
                return eglMakeCurrent (display, surface, surface, context) == EGL_TRUE;
            }

            return false;
        }

        bool Android_OpenGL_ES_Context::flush_and_display ()
        {
            if (available)
            {
                //return eglSwapBuffers (display, surface) == EGL_TRUE;

                if (!eglSwapBuffers (display, surface))
                {
                    EGLint error = eglGetError ();

                    if (error == EGL_BAD_SURFACE)
                    {
                        initialize_surface ();

                        //return EGL_SUCCESS;   // Still consider glContext is valid / BUT THE OPERATION FAILED
                    } else
                    if (error == EGL_CONTEXT_LOST || error == EGL_BAD_CONTEXT)
                    {
                        finalize ();
                        //InitEGLContext();     // SURE? WHAT IF THE WINDOW WAS DESTROYED?
                    }

                    return false;
                }

                return true;
            }

            return false;
        }

        void Android_OpenGL_ES_Context::reset_viewport ()
        {
            if (available)
            {
                eglQuerySurface (display, surface, EGL_WIDTH,  &surface_width );
                eglQuerySurface (display, surface, EGL_HEIGHT, &surface_height);
                glViewport      (0, 0, surface_width, surface_height);
            }
        }

        void Android_OpenGL_ES_Context::set_viewport (const Point2u & bottom_left, const Size2u & size)
        {
            if (available)
            {
                glViewport (bottom_left[0], bottom_left[1], size.width, size.height);
            }
        }

        bool Android_OpenGL_ES_Context::initialize_display ()
        {
            display = eglGetDisplay (EGL_DEFAULT_DISPLAY);

            if (display != EGL_NO_DISPLAY)
            {
                EGLint egl_version_major = 0;
                EGLint egl_version_minor = 0;

                if (eglInitialize (display, &egl_version_major, &egl_version_minor) == EGL_TRUE)
                {
                    if (egl_version_major > 1 || (egl_version_major == 1 && egl_version_minor >= 3))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        bool Android_OpenGL_ES_Context::initialize_surface ()
        {
            const EGLint desired_attributes[] =
            {
                EGL_ATTRIBUTE( EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT ),
                EGL_ATTRIBUTE( EGL_SURFACE_TYPE,    EGL_WINDOW_BIT     ),
                EGL_ATTRIBUTE( EGL_DEPTH_SIZE,      0                  ),
                EGL_NONE
            };

            EGLint number_of_suitable_configurations = 0;

            if
            (
                eglChooseConfig (display, desired_attributes, &config, 1, &number_of_suitable_configurations) &&
                number_of_suitable_configurations > 0
            )
            {
                surface = eglCreateWindowSurface (display, config, native_window, nullptr);

                if (surface != EGL_NO_SURFACE)
                {
                    eglQuerySurface (display, surface, EGL_WIDTH,  &surface_width );
                    eglQuerySurface (display, surface, EGL_HEIGHT, &surface_height);

                    return true;
                }
            }

            return false;
        }

        bool Android_OpenGL_ES_Context::initialize_context ()
        {
            const EGLint context_attributes[] =
            {
                EGL_ATTRIBUTE( EGL_CONTEXT_CLIENT_VERSION, 2 ),
                EGL_NONE
            };

            context = eglCreateContext (display, config, EGL_NO_CONTEXT, context_attributes);

            return context != EGL_NO_CONTEXT;
        }

        void Android_OpenGL_ES_Context::finalize_display ()
        {
            if (display != EGL_NO_DISPLAY)
            {
                eglTerminate (display);

                display  = EGL_NO_DISPLAY;
            }
        }

        void Android_OpenGL_ES_Context::finalize_surface ()
        {
            if (surface != EGL_NO_SURFACE)
            {
                eglDestroySurface (display, surface);

                surface  = EGL_NO_SURFACE;
            }
        }

        void Android_OpenGL_ES_Context::finalize_context ()
        {
            if (context != EGL_NO_CONTEXT)
            {
                eglDestroyContext (display, context);

                context  = EGL_NO_CONTEXT;
            }
        }

    }}}

#endif
