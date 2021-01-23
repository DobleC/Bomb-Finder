/*
 * NATIVE ACTIVITY GLUE
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
    #include "Native_Activity.hpp"
    #include <basics/Application>

    /*REMOVE*/#include <basics/Log>/****/

    using basics::internal::native_activity;
    using basics::internal::Native_Activity;

    namespace
    {

        inline Native_Activity * instance_of (ANativeActivity *activity)
        {
            return reinterpret_cast< Native_Activity * >(activity->instance);
        }

        // -----------------------------------------------------------------------------------------

        void on_start (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON START");
            instance_of (activity)->on_start ();
        }

        void on_resume (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON RESUME");
            instance_of (activity)->on_resume ();
        }

        void on_pause (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON PAUSE");
            instance_of (activity)->on_pause ();
        }

        void on_stop (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON STOP");
            instance_of (activity)->on_stop ();
        }

        // ESTE MÉTODO PODRÍA NO SER LLAMADO? DESTRUIR LA ACTIVITY AL SALIR DE MAIN THREAD?
        void on_destroy (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON DESTROY");
            instance_of (activity)->on_destroy ();

            // The Native_Activity for this activity was created in ANativeActivity_onCreate()
            // and must now be deleted:

            native_activity = nullptr;

            delete instance_of (activity);
        }

        void on_low_memory (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON LOW MEMORY");
            instance_of (activity)->on_low_memory ();
        }

        // -----------------------------------------------------------------------------------------

        void on_configuration_changed (ANativeActivity * activity)
        {
            /*REMOVE*/basics::log.i ("ON CONFIGURATION CHANGED");
            instance_of (activity)->on_configuration_changed ();
        }

        void * on_save_activity_state (ANativeActivity * activity, size_t * out_size)
        {
            /*REMOVE*/basics::log.i ("ON SAVE ACTIVITY STATE");
            return instance_of (activity)->on_save_activity_state (*out_size);
        }

        // -----------------------------------------------------------------------------------------

        void on_window_created (ANativeActivity * activity, ANativeWindow * window)
        {
            /*REMOVE*/basics::log.i ("ON WINDOW CREATED");
            instance_of (activity)->on_window_created (window);
        }

        void on_window_destroyed (ANativeActivity * activity, ANativeWindow * window)
        {
            /*REMOVE*/basics::log.i ("ON WINDOW DESTROYED");
            instance_of (activity)->on_window_destroyed (window);
        }

        void on_window_redraw (ANativeActivity * activity, ANativeWindow * window)
        {
            /*REMOVE*/basics::log.i ("ON WINDOW REDRAW");
            instance_of (activity)->on_window_redraw (window);
        }

        void on_window_resized (ANativeActivity * activity, ANativeWindow * window)
        {
            /*REMOVE*/basics::log.i ("ON WINDOW RESIZED");
            instance_of (activity)->on_window_resized (window);
        }

        void on_viewport_resized (ANativeActivity * activity, const ARect * rect)
        {
            /*REMOVE*/basics::log.i ("ON VIEWPORT RESIZED");
            instance_of (activity)->on_viewport_resized (rect);
        }

        void on_window_focus_changed (ANativeActivity * activity, int has_focus)
        {
            /*REMOVE*/basics::log.i ("ON WINDOW FOCUS CHANGED");
            instance_of (activity)->on_window_focus_changed (has_focus != 0);
        }

        // -----------------------------------------------------------------------------------------

        void on_input_queue_created (ANativeActivity * activity, AInputQueue * queue)
        {
            /*REMOVE*/basics::log.i ("ON INPUT QUEUE CREATED");
            instance_of (activity)->on_input_queue_created (queue);
        }

        void on_input_queue_destroyed (ANativeActivity * activity, AInputQueue * queue)
        {
            /*REMOVE*/basics::log.i ("ON INPUT QUEUE DESTROYED");
            instance_of (activity)->on_input_queue_destroyed (queue);
        }

    }

    // ---------------------------------------------------------------------------------------------
    // This is the default function that NativeActivity looks for when launching its native code.
    // You can specify android.app.func_name string meta-data in your manifest to use a different
    // function.
    // This function should be explicitly preserved to prevent it from being removed by the linker
    // as it is not called directly within this library:
    // set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    // See https://github.com/android-ndk/ndk/issues/381.

    void ANativeActivity_onCreate
    (
        ANativeActivity * activity,
        void            * saved_activity_state,
        size_t            saved_state_size
    )
    {
        /*REMOVE*/basics::log.i ("ON CREATE");

        // LIMPIAR LOS EVENTOS DE APPLICATION...

        activity->instance = native_activity = new Native_Activity(*activity);

        activity->callbacks->onStart                    = on_start;
        activity->callbacks->onResume                   = on_resume;
        activity->callbacks->onPause                    = on_pause;
        activity->callbacks->onStop                     = on_stop;
        activity->callbacks->onDestroy                  = on_destroy;
        activity->callbacks->onLowMemory                = on_low_memory;

        activity->callbacks->onConfigurationChanged     = on_configuration_changed;
        activity->callbacks->onSaveInstanceState        = on_save_activity_state;

        activity->callbacks->onNativeWindowCreated      = on_window_created;
        activity->callbacks->onNativeWindowDestroyed    = on_window_destroyed;
        activity->callbacks->onNativeWindowRedrawNeeded = on_window_redraw;
        activity->callbacks->onNativeWindowResized      = on_window_resized;
        activity->callbacks->onContentRectChanged       = on_viewport_resized;
        activity->callbacks->onWindowFocusChanged       = on_window_focus_changed;

        activity->callbacks->onInputQueueCreated        = on_input_queue_created;
        activity->callbacks->onInputQueueDestroyed      = on_input_queue_destroyed;

        auto * android_application = dynamic_cast< basics::internal::Android_Application * >(&basics::application);

        assert(android_application);

        android_application->set_internal_data_path (activity->internalDataPath);
        android_application->set_external_data_path (activity->externalDataPath);

        native_activity->on_create (saved_activity_state, saved_state_size);
    }

#endif
