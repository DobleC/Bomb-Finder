/*
 * APPLICATION
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712171705
 */

#ifndef BASICS_APPLICATION_HEADER
#define BASICS_APPLICATION_HEADER

    #include <memory>
    #include <basics/Event_Queue>

    namespace basics
    {

        class Application
        {
        public:

            enum State
            {
                ACTIVE,                         ///< Since the application is created until it becomes interactive (it should be preparing for it)
                INTERACTIVE,                    ///< Once the application is presented to the user
                SUSPENDED,                      ///< When the application goes to the background and should standby
                DESTROYED                       ///< The application should shutdown and terminate all threads still running
            };

            enum Event_Id
            {
                RESTART,
                RESUME,
                SUSPEND,
                SQUEEZE,
                QUIT,
                WINDOW_CREATED,
                WINDOW_DESTROYED,
                CONFIGURATION_CHANGED
            };

        public:

            static Application & get_instance ();

        protected:

            Event_Queue event_queue;

        protected:

            Application() = default;
            virtual ~Application() = default;

        public:

            virtual State get_state () const = 0;

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

        };

        extern Application & application;

    }

#endif
