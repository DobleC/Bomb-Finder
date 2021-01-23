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
    #include <string>
    #include <basics/Event_Queue>

    namespace basics
    {

        /** Permite conocer el estado de la aplicación (ver Application::State) y diversos eventos 
          * relativos al ciclo de vida (ver Application::Event_Id) entre otros. Tanto el estado como
          * los eventos son administrados automáticamente.
          * A efectos prácticos en el desarrollo de un videojuego, la clase Director es la encargada
          * de consultar los cambios de estado de la aplicación y notificar a las escenas directamente.
          */
        class Application
        {
        public:

            /** Representa los posibles estados de la aplicación en un momento dado.
              */
            enum State
            {
                ACTIVE,                         ///< Since the application is created until it becomes interactive (it should be preparing for it)
                INTERACTIVE,                    ///< Once the application is presented to the user
                SUSPENDED,                      ///< When the application goes to the background and should standby
                DESTROYED                       ///< The application should shutdown and terminate all threads still running
            };

            /** Representa los posibles eventos relacionados con la aplicación que se pueden dar.
              */
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

            /** Devuelve una referencia a la instancia única de la clase Application.
              * Se puede usar la referencia basics::application en su lugar.
              * @return Referencia a la instancia única de la clase Application.
              */
            static Application & get_instance ();

        protected:

            Event_Queue event_queue;

        protected:

            Application() = default;
            virtual ~Application() = default;

        public:

            virtual State get_state () const = 0;

            std::string get_internal_data_path () const;
            std::string get_external_data_path () const;

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
