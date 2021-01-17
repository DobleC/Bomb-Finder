/*
 * TIMER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801271656
 */

#ifndef BASICS_TIMER_HEADER
#define BASICS_TIMER_HEADER

    #include <chrono>

    namespace basics
    {

        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;

        /**
         * La clase Timer sirve para cronometrar intervalos de tiempo en alta resolución.
         */
        class Timer
        {

            high_resolution_clock::time_point start_time;

        public:

            /**
             * El constructor por defecto inicia el cronometraje, por lo que no es necesario llamar
             * al método start() justo a continuación de crear un objeto Timer.
             */
            Timer()
            {
                reset ();
            }

            /**
             * Restablece el cronometraje iniciando una nueva medida desde el momento en que se llama.
             */
            void reset ()
            {
                start_time = high_resolution_clock::now ();
            }

            /**
             * Retorna el número de segundos que han transcurrido desde que se inició la medida de tiempo.
             * @tparam NUMERIC_TYPE Es el tipo de dato del valor de retorno (float por defecto). Se pueden
             *     usar tipos enteros para contar segundos enteros o tipos de coma flotante (float o
             *     double) para contar segundos con precisión.
             * @return El número de segundos transcurridos desde que se inició la medida de tiempo.
             */
            template< typename NUMERIC_TYPE = float >
            NUMERIC_TYPE get_elapsed_seconds () const
            {
                return duration_cast< duration< NUMERIC_TYPE > >
                (
                    high_resolution_clock::now () - start_time
                )
                .count ();
            }

        };

    }

#endif
