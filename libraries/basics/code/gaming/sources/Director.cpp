/*
 * DIRECTOR
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801072305
 */

#include <basics/Application>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scene>
#include <basics/Timer>
#include <basics/Window>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/Context>

namespace basics
{

    Director & director = Director::get_instance ();

    // ---------------------------------------------------------------------------------------------

    Director::Director()
    {
        kernel.running           = false;
        graphics_context_factory = opengles::Context::create;
    }

    // ---------------------------------------------------------------------------------------------

    Graphics_Context::Accessor Director::lock_graphics_context ()
    {
        Window::Accessor window = Window::get_window (default_window_id).lock ();

        if (window)
        {
            return window->lock_graphics_context ();
        }

        return Graphics_Context::Accessor();
    }

    // ---------------------------------------------------------------------------------------------

    void Director::run_scene (const std::shared_ptr< Scene > & new_scene)
    {
        if (new_scene)
        {
            target_scene = new_scene;

            if (!kernel.running)
            {
                run_kernel ();
            }
        }
        else
        {
            kernel.exit = true;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Director::run_kernel ()
    {
        kernel.running = true;
        kernel.exit    = false;

        Window::Handle window_handle;

        if (Window::can_be_instantiated)
        {
            Window::create_window (default_window_id);
        }

        float time = 1.f / 60.f;
        Event event;

        do
        {
            Timer timer;
            bool  reset_canvas = false;

            // Check if the current scene must be replaced:

            if (target_scene)
            {
                // If the current scene must be replaced, then it is first finalized:

                if (current_scene) current_scene->finalize ();

                // And then possibly destroyed:

                current_scene.reset ();

                // The new scene is then initialized:

                if (target_scene->initialize ())
                {
                    // If the initialization succeeded, then it is made current:

                    current_scene = target_scene;

                    // The target pointer is cleared:

                    target_scene.reset ();

                    // Suspend of resume the scene depending on the current state:

                    if (state) current_scene->resume (); else current_scene->suspend ();

                    // Initialize the frame time limit:

                    time = current_scene->get_frame_duration ();

                    if (time <= 0.f) time = 1.f / 60.f;

                    reset_canvas = true;
                }
            }

            bool previously_active = state;

            while (application.poll (event))
            {
                switch (event.id)
                {
                    case Application::Event_Id::RESUME:
                    {
                        state.active = true;
                        break;
                    }

                    case Application::Event_Id::SUSPEND:
                    {
                        state.active = false;
                        break;
                    }

                    case Application::Event_Id::WINDOW_CREATED:
                    {
                        window_handle = Window::get_window (default_window_id);

                        Window::Accessor window = window_handle.lock ();

                        if (graphics_context_factory)
                        {
                            if (!window->has_graphics_context ())
                            {
                                if (!graphics_context_factory (window, &graphics_resource_cache))
                                {
                                    log.e ("ERROR: failed to initialize the OpenGL ES context!");

                                    return;
                                }
                            }

                            reset_viewport (window);

                            state.graphics = true;
                        }

                        break;
                    }

                    case Application::Event_Id::WINDOW_DESTROYED:
                    {
                        state.graphics = false;
                        break;
                    }

                    case Application::Event_Id::CONFIGURATION_CHANGED:
                    {
                        Window::Accessor window = window_handle.lock ();

                        reset_viewport  (window);

                        break;
                    }

                    case Application::Event_Id::QUIT:
                    {
                        kernel.exit = true;
                        break;
                    }
                }
            }

            if (!kernel.exit)
            {
                Window::Accessor window = window_handle.lock ();

                if (window)
                {
                    while (window->poll (event))
                    {
                        switch (event.id)
                        {
                            case Window::GOT_FOCUS:             state.focused = true;    break;
                            case Window::LOST_FOCUS:            state.focused = false;   break;
                            case Window::LOST_GRAPHICS_CONTEXT:                          break;
                            case Window::RESIZED:
                            case Window::VIEWPORT_RESIZED:      reset_viewport (window); break;
                        }
                    }

                    if (current_scene)
                    {
                        bool  currently_active = state;

                        if (!previously_active &&  currently_active) current_scene->resume  (); else
                        if ( previously_active && !currently_active) current_scene->suspend ();

                        if (currently_active)
                        {
                            Size2u scene_view_size = current_scene->get_view_size ();

                            float  h_ratio = float(scene_view_size.width ) / surface_width;
                            float  v_ratio = float(scene_view_size.height) / surface_height;

                            while (event_queue.poll (event))
                            {
                                switch (event.id)
                                {
                                    case ID(touch-started):
                                    case ID(touch-moved):
                                    case ID(touch-ended):
                                    {
                                        float x = *event.properties[ID(x)].as< var::Float > ();
                                        float y = *event.properties[ID(y)].as< var::Float > ();

                                        event.properties[ID(x)] = x * h_ratio;
                                        event.properties[ID(y)] = (surface_height - y) * v_ratio;

                                        break;
                                    }
                                }

                                current_scene->handle (event);
                            }

                            current_scene->update (time);

                            Graphics_Context::Accessor graphics_context = window->lock_graphics_context ();

                            if (graphics_context)
                            {
                                if (reset_canvas)
                                {
                                    Canvas * canvas = graphics_context->get_renderer< Canvas > (ID(canvas));

                                    if (canvas) canvas->reset_state ();
                                }

                                current_scene->render (graphics_context);

                                graphics_context->flush_and_display ();
                            }
                        }
                    }
                }
            }

            time = timer.get_elapsed_seconds ();
        }
        while (!kernel.exit && current_scene);

        if (current_scene)
        {
            current_scene->finalize ();

            current_scene.reset ();
        }

        kernel.running = false;
    }

    // ---------------------------------------------------------------------------------------------

    void Director::reset_viewport (Window::Accessor & window)
    {
        Graphics_Context::Accessor graphics_context = window->lock_graphics_context ();

        if (graphics_context)
        {
            graphics_context->reset_viewport ();

            surface_width  = graphics_context->get_surface_width  ();
            surface_height = graphics_context->get_surface_height ();
        }
    }

}
