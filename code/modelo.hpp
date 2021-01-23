/*
 * Modelo
 * Copyright © 2020+ Carlos Casado
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * rexitah@gmail.com
 */

#include <cstdlib>
#include <ctime>

namespace model {

    class Casilla
    {
    private:

        int posX = 0;                           // Coordenada X
        int posY = 0;                           // Coordenada Y

        int valorBomba;                         // Si es 0 no es una bomba, si es 1 lo es
                                                // (podría ser un bool pero lo prefiero en int para conteo de bombas)
        int valorMultp;                         // Multiplica la puntuación total por si mismo
        bool estaDesvelada = false;             // false = sin desvelar

        int percentBomba = 25;                  // 0%-percentBomba% -> porcentaje de bomba = 1

        int percentMultp[3] = {75, 90, 100};    //  0% -[0]% -> porcentaje de multp = 1
                                                // [0]%-[1]% -> porcentaje de multp = 2
                                                // [1]%-[2]% -> porcentaje de multp = 3
    public:

        /**
          * Constructor por defecto, asigna valores random a valorBomba y valorMultp
          */
        Casilla();

        /**
          * Constructor que recibe un array que altera los porcentajes de aleatoriedad
          * @param percent[] Array de 4 ints con números del 0 al 100.
          * Los 3 primeros alteran percentMultp y el 4o altera percentBomba.
          */
        Casilla(int percent[]);  // Recibe un array con porcentajes para alterar los actuales (por si se requiere cambiarlos)

        int getX()          { return posX;          }

        int getY()          { return posY;          }

        int getValorBomba() { return valorBomba;    }

        int getValorMultp() { return valorMultp;    }

        bool getDesvelada() { return estaDesvelada; }

        //int *getPercents()
        //{
        //    int percents[4] = {percentMultp[0], percentMultp[1], percentMultp[2], percentBomba};
        //    return percents;
        //}

        void setX(int x)          { posX = x;         }

        void setY(int y)          { posY = y;         }

        void setValorBomba(int b){ valorBomba    = b; }

        void setValorMultp(int m){ valorMultp    = m; }

        void setDesvelada(bool e){ estaDesvelada = e; }

        void setPercents(int p[])
        {
            for (int i = 0; i < 3; ++i) {
                percentMultp[i] = p[i];
            }
            percentBomba = p[3];
        }

    };

    class Tablero
    {

    private:
        int ladoLength = 5 - 1;
        int height = 0;
        int width  = 0;

    public:
        Casilla matrizTablero[5][5];
        int mayoresUno = 0;
        int bombasFila[5]    = {0, 0, 0, 0, 0};
        int bombasColumna[5] = {0, 0, 0, 0, 0};
        int puntosFila[5]    = {0, 0, 0, 0, 0};
        int puntosColumna[5] = {0, 0, 0, 0, 0};

        Tablero()
        {
            calcDatos();
        }


        //Tablero(int percent[]) : Casilla{percent}//--------------> Problemas aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        //{
        //}

    private:

        /**
          * Rellena el array bombas fila con el número de bombas por cada fila (nºfila = pos en el array)
          * Rellena el array puntos fila con el número de puntos por cada fila (nºfila = pos en el array)
          */
        void calcDatosFila();

        /**
          * Rellena el array bombas columna con el número de bombas por cada columna (nºcolumna = pos en el array)
          * Rellena el array puntos columna con el número de puntos por cada columna (nºcolumna = pos en el array)
          */
        void calcDatosColumna();

        /**
          * Copia la información de la matriz Casillas de un tablero recibido por referencia
          * @param tablero Puntero al tablero cuya información se necesita. No debe ser nullptr.
          */
        void copiarCasillas(Tablero & tablero);

        /**
          * Copia la información de los arrays de un tablero recibido por referencia
          * @param tablero Puntero al tablero cuya información se necesita. No debe ser nullptr.
          */
        void copiarArrays(Tablero & tablero);

    public:

        /**
          * Actualiza la información de los arrays de un tablero
          */
        void calcDatos()
        {
            calcDatosFila();
            calcDatosColumna();
        }

        /**
          * Operador igualar, copia los datos de otro tablero dentro de si mismo
          * @param tablero Puntero al tablero cuya información se necesita. No debe ser nullptr.
          */
        Tablero & operator = (Tablero & tablero) // Debería ser constante para no modificarlo
        {
            copiarCasillas(tablero);
            copiarArrays  (tablero);

            return *this;
        }
    };
}