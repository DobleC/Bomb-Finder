/*
 * ACCELEROMETER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_ACCELEROMETER_HEADER
#define BASICS_ACCELEROMETER_HEADER

    namespace basics
    {

        /** Permite leer el estado del acelerómetro en cualquier punto del programa.
          */
        class Accelerometer
        {
        public:

            /** Guarda los valores de aceleración leídos del acelerómetro cuando se llama al método
              * get_state().
              */
            struct State
            {
                float x;
                float y;
                float z;
            };

        public:

            /** Permite conocer si el acelerómetro está disponible en el dispositivo en el que se
              * está ejecutando el programa.
              * @return true si el sensor de aceleración está disponible y es posible leer su estado.
              */
            static bool is_available ();
            
            /** Permite acceder a la instancia única de la clase Accelerometer.
              */
            static Accelerometer * get_instance ();

        protected:

            State state;

        public:

            /** Realiza una lectura del 
              */
            const State & get_state ()
            {
                return state;
            }

            /** Permite establecer manualmente los valores de aceleración, lo cual puede ser útil para
              * simular ciertos comportamientos del acelerómetro desde el propio programa.
              */
            void set_state (float new_x, float new_y, float new_z)
            {
                state.x = new_x;
                state.y = new_y;
                state.z = new_z;
            }

        public:

            /** Permite activar la lectura del sensor de aceleración.
              * Al hacerlo aumentará el consumo de energía y se ejecutará un hilo que consumirá cierta
              * capacidad de cómputo.
              * @return true si se pudo activar la lectura del acelerómetro.
              */
            virtual bool switch_on  () = 0;
            
            /** Permite desactivar la lectura del sensor de aceleración.
              * Ello puede permitir ahorrar energía cuando no se necesita usarlo.
              */
            virtual void switch_off () = 0;

        };

        //extern Accelerometer * const accelerometer;

    }

#endif
