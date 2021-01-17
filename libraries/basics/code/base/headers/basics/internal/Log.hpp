/*
 * LOG
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712172039
 */

// Requisitos:
// Rapidez, uso de poca memoria, versatilidad.
// Sorporte de salida estándar, archivos u otros canales de modo extensible.
// Salida multicanal (ej. salida estándar y archivo simultáneamente, etc.).
// Concurrencia.
// Formateo de tipos de datos básicos.
// Desactivación dinámica o estática de diferentes niveles de log.

#ifndef BASICS_LOG_HEADER
#define BASICS_LOG_HEADER

    #include <string>

    namespace basics
    {

        class Log final
        {
        private:

            enum Level
            {
                VERBOSE,
                DEBUG,
                INFO,
                WARNING,
                ERROR,
                FATAL
            };

        // -----------------------------------------------------------------------------------------

        public:

          /*template< typename >
            class Channel
            {
            public:

                void dump (Level level, const char * tag, const char * chars);

            };*/

        // -----------------------------------------------------------------------------------------

        private:

            class Pass_Gate final
            {
            private:

                Log &  log;
                Level  level;
                bool   is_open;
              //Buffer buffer;                  // DEBE HABER UN BUFFER POR LEVEL

            public:

                Pass_Gate(Log & log, Level level) : log(log), level(level)
                {
                    open ();
                }

            public:

                void open ()
                {
                    is_open = true;
                }

                void close ()
                {
                    is_open = false;
                }

                // SOBRECARGAR EL OPERADOR << PARA DIFERENTES TIPOS DE DATOS, IR ACUMULANDO
                // EN UN BUFFER EL RESULTADO (LLAMANDO A CIERTO MÉTODO) Y TERMINAR CON ENDL.

                // PERMITIR QUE EL OPERADOR () ACEPTE UN NÚMERO VARIABLE DE PARÁMETROS DE TIPOS
                // DISTINTOS SEPARADOS POR COMAS. TRAS EL ÚLTIMO PARÁMETRO SE ENVÍA A LOG.DUMP.

                Pass_Gate & operator () (const char * message)
                {
                    if (is_open) accept (message);

                    return *this;
                }

                Pass_Gate & operator () (const std::string & message)
                {
                    if (is_open) accept (message.c_str ());

                    return *this;
                }

                // PODRÍA SER CONVENIENTE QUE ESTE MÉTODO NO SEA INLINE PARA QUE EL CÓDIGO MÁQUINA
                // SEA MÁS COMPACTO Y PARA QUE TENGA MÁS POSIBILIDADES DE RESIDIR EN EL L1 DE CÓDIGO.
                void accept (const char * message)
                {
                    log.dump (level, nullptr, message);
                }

            };

        // -----------------------------------------------------------------------------------------

        private:

            // PASS_GATE Y NULL_GATE DEBEN TENER LA MISMA INTERFAZ, PERO NO DEBE USARSE LA HERENCIA
            // PARA EVITAR QUE SE CREE UNA TABLA DE MÉTODOS VIRTUALES Y ASÍ PERMITIR QUE LOS MÉTODOS
            // VACÍOS DE NULL_GATE NO GENEREN CÓDIGO.

            class Null_Gate final
            {
            public:

                Null_Gate(Log & , Level )
                {
                }

                void open  () { }
                void close () { }

                Null_Gate & operator () (const char        * ) { return *this; }
                Null_Gate & operator () (const std::string & ) { return *this; }

            };

        // -----------------------------------------------------------------------------------------

        public:

            // MEDIANTE PREPROCESADO USAR PASS_GATE O NULL_GATE SEGÚN SE QUIERAN ACTIVAR
            // O DESACTIVAR EN TIEMPO DE COMPILACIÓN. EN MODO RELASE, DESACTIVAR DEBUG Y
            // VERBOSE POR DEFECTO.

            Pass_Gate v;                        ///< Log gate for verbose messages.
            Pass_Gate d;                        ///< Log gate for debug messages.
            Pass_Gate i;                        ///< Log gate for information messages.
            Pass_Gate w;                        ///< Log gate for warnings.
            Pass_Gate e;                        ///< Log gate for error messages.
            Pass_Gate f;                        ///< Log gate for fatal error messages.

        // -----------------------------------------------------------------------------------------

        private:

            //std::list< Channel? > channels;

        public:

            Log()
            :
                v(*this, VERBOSE),
                d(*this, DEBUG  ),
                i(*this, INFO   ),
                w(*this, WARNING),
                e(*this, ERROR  ),
                f(*this, FATAL  )
            {
            }

        // -----------------------------------------------------------------------------------------

        private:

            void dump (Level level, const char * tag, const char * cstring);

        };

        // -----------------------------------------------------------------------------------------

        extern Log log;

    }

#endif
