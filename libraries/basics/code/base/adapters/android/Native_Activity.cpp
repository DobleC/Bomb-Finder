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

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Android_Application.hpp"
    #include "Android_Sensor_Manager.hpp"
    #include "Android_Accelerometer.hpp"
    #include "Native_Activity.hpp"

    #include <basics/Log>
    using namespace basics;

    using namespace std;

    // ---------------------------------------------------------------------------------------------

    int main ();

    // ---------------------------------------------------------------------------------------------

    namespace basics { namespace internal
    {

        void android_input_dispatcher (AInputQueue * input_queue);

        Native_Activity * native_activity = nullptr;

        // -----------------------------------------------------------------------------------------

        Native_Activity::~Native_Activity()
        {
            if (sensor_thread.instance &&
                sensor_thread.instance->joinable ()) sensor_thread.instance->join ();
            if ( input_thread.instance->joinable ())  input_thread.instance->join ();
            if (  main_thread.instance->joinable ())   main_thread.instance->join ();

            android_sensor_manager.shut_down ();
        }

        // -----------------------------------------------------------------------------------------

        bool Native_Activity::start_sensor_thread ()
        {
            if (!sensor_thread.ready)
            {
                // Start the sensor thread and wait for it to be ready:

                sensor_thread.ready   = false;
                sensor_thread.started = false;

                sensor_thread.instance.reset
                (
                    new thread(&Native_Activity::sensor_thread_function, this)
                );

                while (!sensor_thread.started)
                {
                    unique_lock< mutex > sync (sensor_thread.mutex);
                    sensor_thread.barrier.wait (sync);
                }
            }

            return sensor_thread.ready;
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::main_thread_function ()
        {
            main ();

            lock_guard< mutex > lock(state.mutex);

            ANativeActivity_finish (&activity);
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::input_thread_function ()
        {
            // Creates a looper for the input thread:

            input_thread.looper = ALooper_prepare (ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
            input_thread.ready  = true;

            // And then notifies the onCreate() callback that the input thread is up and ready:

            input_thread.barrier.notify_all ();

            // This loop dispatches the input events to the appropriate handlers:

            while (application.get_state () != Application::DESTROYED)
            {
                ALooper_pollAll (-1, nullptr, nullptr, nullptr);
                android_input_dispatcher  (android.input_queue);
            }

            input_thread.ready = false;
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::sensor_thread_function ()
        {
            // Creates a looper for the sensor thread:

            ASensorEventQueue * sensor_queue = android_sensor_manager.create_event_queue
            (
                sensor_thread.looper = ALooper_prepare (ALOOPER_PREPARE_ALLOW_NON_CALLBACKS)
            );

            sensor_thread.ready   = sensor_queue != nullptr;
            sensor_thread.started = true;

            // And then notifies the onCreate() callback that the sensor thread is up and ready:

            sensor_thread.barrier.notify_all ();

            if (sensor_thread.ready)
            {
                // This loop dispatches the sensor events to the appropriate handlers:

                Accelerometer * accelerometer = Accelerometer::get_instance ();

                while (application.get_state () != Application::DESTROYED)
                {
                    ALooper_pollAll (-1, nullptr, nullptr, nullptr);

                    ASensorEvent event;

                    while (ASensorEventQueue_hasEvents (sensor_queue) == 1)
                    {
                        if (ASensorEventQueue_getEvents (sensor_queue, &event, 1) >= 0)
                        {
                            switch (event.type)
                            {
                                case ASENSOR_TYPE_ACCELEROMETER:
                                {
                                    if (accelerometer)
                                    {
                                        accelerometer->set_state
                                        (
                                            event.acceleration.x,
                                            event.acceleration.y,
                                            event.acceleration.z
                                        );
                                    }

                                    break;
                                }

                                case ASENSOR_TYPE_GYROSCOPE:
                                {
                                    break;
                                }

                                default: break;
                            }
                        }
                    }

                }
            }
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_create (void * saved_activity_state, size_t saved_state_size)
        {
            lock_guard< mutex > lock(state.mutex);

            // Initialize the Android configuration object:

            android.configuration = AConfiguration_new ();

            AConfiguration_fromAssetManager (android.configuration, activity.assetManager);

            // Clear the Android input queue pointer:

            android.input_queue = nullptr;

            // Initialize the Application state:

            application.clear_events ();
            application.set_state    (Application::ACTIVE);

            // AÑADIR UN EVENTO RESTART CUANDO CORRESPONDA...

            // Se incializa el gestor de sensores:

            android_sensor_manager.wake_up ();

            sensor_thread.ready   = false;
            sensor_thread.started = false;

            // Start the main thread:

            main_thread.instance.reset
            (
                new thread(&Native_Activity::main_thread_function, this)
            );

            // Start the input thread and wait for it to be ready:

            input_thread.ready = false;

            input_thread.instance.reset
            (
                new thread(&Native_Activity::input_thread_function, this)
            );

            while (!input_thread.ready)
            {
                unique_lock< mutex > sync (input_thread.mutex);
                input_thread.barrier.wait (sync);
            }
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_start ()
        {
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_resume ()
        {
            lock_guard< mutex > lock(state.mutex);

            application.set_state (Application::INTERACTIVE);

            application.push (Event(Application::Event_Id::RESUME));
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_pause ()
        {
            lock_guard< mutex > lock(state.mutex);

            application.set_state (Application::SUSPENDED);

            application.push (Event(Application::Event_Id::SUSPEND));
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_stop ()
        {
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_destroy ()
        {
            lock_guard< mutex > lock(state.mutex);

            application.set_state (Application::DESTROYED);

            application.push (Event(Application::Event_Id::QUIT));

            // This should wake the looper from the input thread and then terminate that thread:

            ALooper_wake (input_thread.looper);
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_low_memory ()
        {
            lock_guard< mutex > lock(state.mutex);

            application.push (Event(Application::Event_Id::SQUEEZE));
        }

        // -----------------------------------------------------------------------------------------
        // The new configuration can be retrieved from assetManager.

        void Native_Activity::on_configuration_changed ()
        {
            lock_guard< mutex > lock(state.mutex);

            AConfiguration_fromAssetManager (android.configuration, activity.assetManager);

            application.push (Event(Application::Event_Id::CONFIGURATION_CHANGED));
        }

        // -----------------------------------------------------------------------------------------
        // The returned pointer needs to be created with malloc(). The framework will call free()
        // on it for you. You also must fill in out_size with the number of bytes in the allocation.
        // Note that the saved state will be persisted, so it can not contain any active entities
        // (pointers to memory, file descriptors, etc).

        void * Native_Activity::on_save_activity_state (size_t & out_size)
        {
            out_size = 0;
            return nullptr;
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_window_created (ANativeWindow * native_window)
        {
            lock_guard< mutex > lock(state.mutex);

            if (!window)
            {
                log.d ("reset window");

                window.reset
                (
                    new Native_Window(default_window_id, native_window)
                );
            }
            else
            {
                window->reset_window_resource (native_window);
            }

            application.push (Event(Application::Event_Id::WINDOW_CREATED));
        }

        // -----------------------------------------------------------------------------------------
        // Notes from the documentation about the onNativeWindowDestroyed() callback:
        // You MUST ensure that you do not touch the window object after returning from this
        // function: in the common case of drawing to the window from another thread, that means
        // the implementation of this callback must properly synchronize with the other thread to
        // stop its drawing before returning from here.

        void Native_Activity::on_window_destroyed (ANativeWindow * )
        {
            lock_guard< mutex > lock(state.mutex);

            if (window->has_graphics_context ())
            {
                // There exist Android implementations where the window surface seems to be destroyed
                // before this callback returns (against what can be inferred from the official
                // documentation), therefore the graphics context is invalidated as soon as possible,
                // but it is only destroyed once it can be locked:

                window->get_graphics_context ()->invalidate ();

                // Wait until the graphics context can be locked before destroying it.
                // The constructor of Accessor waits for the lock and the Accessor object keeps it
                // locked until it's destroyed. Waiting for the lock acquisition may halt this thread
                // for "some" time (presumably while other thread finishes rendering).
                // The Accessor also keeps the graphics context alive until leaving this block. Then
                // it's finally destroyed before it can be locked by other thread.

                Graphics_Context::Accessor graphics_context = window->lock_graphics_context ();

                // Release the window resource and its graphics context so that they can be destroyed:

                window->reset_window_resource ();
            }
            else
            {
                window->reset_window_resource ();
            }

            application.push (Event(Application::Event_Id::WINDOW_DESTROYED));
        }

        // -----------------------------------------------------------------------------------------
        // To avoid transient artifacts during screen changes (such resizing after rotation),
        // applications should not return from this function until they have finished drawing their
        // window in its current state.

        void Native_Activity::on_window_redraw (ANativeWindow * )
        {
            lock_guard< mutex > lock(state.mutex);

            if (window)
            {
                window->push (Event(Window::Event_Id::REDRAW));
            }
        }

        // -----------------------------------------------------------------------------------------
        // You should retrieve the new size from the window and ensure that your rendering in it
        // now matches.

        void Native_Activity::on_window_resized (ANativeWindow * )
        {
            lock_guard< mutex > lock(state.mutex);

            if (window)
            {
                window->push (Event(Window::Event_Id::RESIZED));
            }
        }

        // -----------------------------------------------------------------------------------------
        // The rectangle in the window in which content should be placed has changed.

        void Native_Activity::on_viewport_resized (const ARect * rect)
        {
            lock_guard< mutex > lock(state.mutex);

            if (window)
            {
                window->push (Event(Window::Event_Id::VIEWPORT_RESIZED));
            }
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_window_focus_changed (bool has_focus)
        {
            lock_guard< mutex > lock(state.mutex);

            if (window)
            {
                window->set_focused (has_focus);

                window->push
                (
                    Event(has_focus ? Window::Event_Id::GOT_FOCUS : Window::Event_Id::LOST_FOCUS)
                );
            }
        }

        // -----------------------------------------------------------------------------------------

        void Native_Activity::on_input_queue_created (AInputQueue * queue)
        {
            lock_guard< mutex > lock(state.mutex);

            if (android.input_queue != nullptr)
            {
                AInputQueue_detachLooper (android.input_queue);
            }

            android.input_queue = queue;

            AInputQueue_attachLooper
            (
                android.input_queue,
                input_thread.looper,
                1,
                nullptr,
                nullptr
            );
        }

        // -----------------------------------------------------------------------------------------
        // You should no longer try to reference this object upon returning from this function.

        void Native_Activity::on_input_queue_destroyed (AInputQueue * )
        {
            lock_guard< mutex > lock(state.mutex);

            if (android.input_queue != nullptr)
            {
                AInputQueue_detachLooper (android.input_queue);

                android.input_queue  = nullptr;
            }
        }

    }}

#endif
