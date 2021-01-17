/*
 * ANDROID APPLICATION
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712291010
 */

#ifndef BASICS_ANDROID_APPLICATION_HEADER
#define BASICS_ANDROID_APPLICATION_HEADER

    #include <atomic>
    #include <basics/Application>

    namespace basics { namespace internal
    {

        class Android_Application : public Application
        {

            std::atomic< Application::State > state;

        public:

            State get_state () const override
            {
                return state;
            }

            void set_state (State new_state)
            {
                state = new_state;
            }

            void clear_events ()
            {
                event_queue.clear ();
            }

        };

        extern Android_Application application;

    }}

#endif
