/*
 * WINDOW
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712171705
 */

#ifndef BASICS_WINDOW_HEADER
#define BASICS_WINDOW_HEADER

    #include <atomic>
    #include <memory>
    #include <basics/Size>
    #include <basics/Event_Queue>
    #include <basics/Graphics_Context>

    namespace basics
    {

        class Window
        {
            // -------------------------------------------------------------------------------------

        public:

            class Accessor
            {
            protected:

                std::shared_ptr< Window > window;

            public:

                Accessor() = default;
                Accessor(Accessor && ) = default;
                Accessor(const Accessor & ) = default;
                Accessor(std::shared_ptr< Window > && window) : window(window) { }

            public:

                Window * operator -> ()
                {
                    return window.get ();
                }

                const Window * operator -> () const
                {
                    return window.get ();
                }

            public:

                operator bool () const
                {
                    return bool(window);
                }

            };

            // -------------------------------------------------------------------------------------

        public:

            class Handle final
            {

                std::weak_ptr< Window > watch;

            public:

                Handle() = default;
                Handle(Handle && ) = default;
                Handle(const Handle & ) = default;
                Handle(const std::weak_ptr< Window > & watch) : watch(watch) { }

            public:

                Accessor lock ()
                {
                    return Accessor(watch.lock ());
                }

                Handle & operator = (const Handle & ) = default;

                /** Checks whether the Handle object has been initialized with a valid Window pointer or not.
                 * @return true if the Handle object has been initialized with a valid Window pointer or
                 *  false otherwise.
                 */
                operator bool () const
                {
                    return std::weak_ptr< Window >{}.owner_before (watch) || watch.expired ();
                }

            };

            // -------------------------------------------------------------------------------------

        public:

            enum Event_Id : Id
            {
                REDRAW           = FNV( window-redraw           ),
                RESIZED          = FNV( window-resized          ),
                VIEWPORT_RESIZED = FNV( window-viewport-resized ),
                GOT_FOCUS        = FNV( window-got-focus        ),
                LOST_FOCUS       = FNV( window-lost-focus       ),
              //CLOSED           = FNV( window-closed           ),
                LOST_GRAPHICS_CONTEXT
            };

            // -------------------------------------------------------------------------------------

        public:

            static const bool can_be_instantiated;

            // UNIFORMIZAR QUITANDO SUFIJOS:
            static Handle   create_window (Id id);
            static bool    destroy_window (Id id);
            static Handle      get_window (Id id);

            // -------------------------------------------------------------------------------------

        public:

            const  Id id;

        protected:

            /** Holds true if the real window resource (managed by the window manager and not always
             *  controllable by the app) is available at a given moment.
             */
            std::atomic< bool > available;
            std::atomic< bool > focused;

            Event_Queue event_queue;

            struct
            {
                std::shared_ptr< Graphics_Context > context;
                std::shared_ptr< std::mutex       > mutex;
            }
            graphics;

            // -------------------------------------------------------------------------------------

        private:

            Window() = delete;
            Window(Window && ) = delete;
            Window(const Window & ) = delete;

        protected:

            Window(Id id) : id(id)
            {
                available = false;
                focused   = false;
            }

            virtual ~Window () = default;

            // -------------------------------------------------------------------------------------

        public:

            virtual bool     set_full_screen (bool status) = 0;

            virtual Size2u   get_size   () = 0;
            virtual unsigned get_width  () = 0;
            virtual unsigned get_height () = 0;

        public:

            bool is_available () const
            {
                return available;
            }

            bool is_focused ()
            {
                return focused;
            }

            bool has_graphics_context () const
            {
                return graphics.context != nullptr;
            }

        public:

            void reset_graphics_context ()
            {
                graphics.context.reset ();
            }

            bool set_graphics_context (const std::shared_ptr< Graphics_Context > & context)
            {
                if (available && !graphics.context)
                {
                    graphics.mutex.reset (new std::mutex);

                    graphics.context = context;

                    return true;
                }

                return false;
            }

            Graphics_Context::Accessor lock_graphics_context ()
            {
                if (available && graphics.context)
                {
                    return Graphics_Context::Accessor(graphics.context, *graphics.mutex);
                }

                return Graphics_Context::Accessor();
            }

            Graphics_Context::Accessor try_lock_graphics_context ()
            {
                if (available && graphics.context)
                {
                    return Graphics_Context::Accessor(graphics.context, *graphics.mutex, std::try_to_lock);
                }

                return Graphics_Context::Accessor();
            }

        public:

            void push (const Event & event)
            {
                event_queue.push (event);
            }

            void push (Event && event)
            {
                event_queue.push (event);
            }

            bool poll (Event & event)
            {
                return event_queue.poll (event);
            }

            bool peek (Event & event)
            {
                return event_queue.peek (event);
            }

        };

        constexpr Id default_window_id = FNV(default-window);

    }

#endif
