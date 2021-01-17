/*
 *  DIRECTOR
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801072242
 */

#ifndef BASICS_DIRECTOR_HEADER
#define BASICS_DIRECTOR_HEADER

    #include <memory>
    #include <basics/declarations>
    #include <basics/Event_Queue>
    #include <basics/Graphics_Context>
    #include <basics/Graphics_Resource_Cache>
    #include <basics/Window>

    namespace basics
    {

        class Director final
        {
        public:

            typedef bool (* Graphics_Context_Factory) (Window::Accessor & window, Graphics_Resource_Cache * cache);

        public:

            static Director & get_instance ()
            {
                static Director director;
                return director;
            }

        private:

            struct
            {
                bool running;
                bool exit;
            }
            kernel;

            struct
            {
                bool active   = false;
                bool focused  = false;
                bool graphics = false;

                operator bool () const
                {
                    return active && focused && graphics;
                }
            }
            state;

            std::shared_ptr< Scene > current_scene;
            std::shared_ptr< Scene >  target_scene;

            Event_Queue event_queue;

            float surface_width;
            float surface_height;

            Graphics_Context_Factory graphics_context_factory;
            Graphics_Resource_Cache  graphics_resource_cache;

        private:

            Director();

        public:

            void set_graphics_context_factory (Graphics_Context_Factory factory)
            {
                graphics_context_factory = factory;
            }

            Graphics_Context::Accessor lock_graphics_context ();

        public:

            void run_scene (const std::shared_ptr< Scene > & new_scene);

            void stop ()
            {
                kernel.exit = kernel.running;
            }

            void handle (const Event & event)
            {
                event_queue.push (event);
            }

        private:

            void run_kernel ();
            bool check_scene ();
            void reset_viewport (Window::Accessor & window);

        };

        extern Director & director;

    }

#endif
