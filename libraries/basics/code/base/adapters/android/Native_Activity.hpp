/*
 * NATIVE ACTIVITY
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712162207
 */

#ifndef BASICS_NATIVE_ACTIVITY_HEADER
#define BASICS_NATIVE_ACTIVITY_HEADER

    #include <atomic>
    #include <condition_variable>
    #include <memory>
    #include <mutex>
    #include <thread>

    #include "Native_Window.hpp"

    #include <android/configuration.h>
    #include <android/input.h>
    #include <android/native_activity.h>
    #include <android/native_window.h>
    #include <android/sensor.h>

    namespace basics { namespace internal
    {

        using std::atomic;
        using std::condition_variable;
        using std::lock_guard;
        using std::mutex;
        using std::thread;

        using std::shared_ptr;
        using std::unique_ptr;

        class Native_Activity final
        {
        private:

            ANativeActivity           & activity;
            shared_ptr< Native_Window > window;

            struct
            {
                AConfiguration     * configuration = nullptr;
                AInputQueue        * input_queue   = nullptr;
                ARect                viewport      = { 0, 0, 0, 0 };
            }
            android;

            struct
            {
                unique_ptr< thread > instance;
                mutex                mutex;
            }
            main_thread;

            struct
            {
                unique_ptr< thread > instance;
                mutex                mutex;
                condition_variable   barrier;
                atomic< bool >       ready;
                ALooper            * looper = nullptr;
            }
            input_thread;

            // Esto habría que llevarlo a Android_Sensor_Manager...
            struct
            {
                unique_ptr< thread > instance;
                mutex                mutex;
                condition_variable   barrier;
                atomic< bool >       started;
                atomic< bool >       ready;
                ALooper            * looper = nullptr;
            }
            sensor_thread;

            struct
            {
                mutex mutex;
            }
            state;

        public:

            Native_Activity(ANativeActivity & activity)
            :
                activity(activity)
            {
            }

           ~Native_Activity();

        public:

            ANativeActivity & get_activity ()
            {
                return activity;
            }

            std::weak_ptr< Native_Window > get_window ()
            {
                return window;
            }

            bool sensor_thread_is_ready () const
            {
                return sensor_thread.ready;
            }

            bool start_sensor_thread ();

        private:

            void   main_thread_function ();
            void  input_thread_function ();
            void sensor_thread_function ();

        public:

            /// NativeActivity has started.
            /// See Java documentation for Activity.on_start() for more information.
            void on_create (void * saved_activity_state, size_t saved_state_size);

            /// NativeActivity has started.
            /// See Java documentation for Activity.on_start() for more information.
            void on_start ();

            /// NativeActivity has resumed.
            /// See Java documentation for Activity.onResume() for more information.
            void on_resume ();

            /// NativeActivity has paused.
            /// See Java documentation for Activity.onPause() for more information.
            void on_pause ();

            /// NativeActivity has stopped.
            /// See Java documentation for Activity.onStop() for more information.
            void on_stop ();

            /// NativeActivity is being destroyed.
            /// See Java documentation for Activity.onDestroy() for more information.
            void on_destroy ();

            /// The system is running low on memory.
            void on_low_memory ();

            /// The current device AConfiguration has changed.
            void on_configuration_changed ();

            /// Framework is asking NativeActivity to save its current instance state.
            /// See Java documentation for Activity.onSaveInstanceState() for more information.
            void * on_save_activity_state (size_t & out_size);

            /// The drawing window for this native activity has been created.
            void on_window_created (ANativeWindow * native_window);

            /// The drawing window for this native activity is going to be destroyed.
            void on_window_destroyed (ANativeWindow * native_window);

            /// The drawing window for this native activity needs to be redrawn.
            void on_window_redraw (ANativeWindow * native_window);

            /// The drawing window for this native activity has been resized.
            void on_window_resized (ANativeWindow * native_window);

            /// The rectangle in the window in which content should be placed has changed.
            void on_viewport_resized (const ARect * rect);

            /// Focus has changed in this NativeActivity's window.
            void on_window_focus_changed (bool has_focus);

            /// The input queue for this native activity's window has been created.
            void on_input_queue_created (AInputQueue * queue);

            /// The input queue for this native activity's window is being destroyed.
            void on_input_queue_destroyed  (AInputQueue * queue);

        };

        extern Native_Activity * native_activity;

    }}

#endif
