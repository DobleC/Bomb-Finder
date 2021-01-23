/*
 * DISPLAY
 * Copyright © 2021+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_DISPLAY_HEADER
#define BASICS_DISPLAY_HEADER

    #include <basics/Non_Copyable>

    namespace basics
    {

        /** Esta clase permite configurar algunas características del display.
          */
        class Display : Non_Copyable
        {
        public:

            static Display & get_instance ()
            {
                static Display display;
                return display;
            }

        public:

            /** Permite evitar que la iluminación de la pantalla se apague automáticamente pasado un tiempo.
              * @param status true si se desea evitar el apagado automático o false en caso contrario.
              * @return true si se ha podido configurar el apagado automático.
              */
            bool set_prevent_sleep (bool status);

        };

        extern Display & display;

    }

#endif
