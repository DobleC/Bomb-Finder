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

    class Casilla {
    private:
        int valorBomba;                         // Si es 0 no es una bomba, si es 1 lo es
                                                // (podría ser un bool pero lo prefiero en int para conteo de bombas)
        int valorMultp;                         // Multiplica la puntuación total por si mismo
        bool estaDesvelada = false;             // false = sin desvelar

        int percentBomba = 25;                  // 0%-percentBomba% -> porcentaje de bomba = 1
        int percentMultp[3] = {45, 80, 100};
        // 0%-[0]%   -> porcentaje de multp = 1
        // [0]%-[1]% -> porcentaje de multp = 2
        // [1]%-[2]% -> porcentaje de multp = 3


    public:
        int getValorBomba() { return valorBomba; }

        int getValorMultp() { return valorMultp; }

        bool getDesvelada() { return estaDesvelada; }

        int *getPercents() {
            int percents[4] = {percentMultp[0], percentMultp[1], percentMultp[2], percentBomba};
            return percents;
        }

        void setValorBomba(int b) { valorBomba = b; }

        void setValorMultp(int m) { valorMultp = m; }

        void setDesvelada(bool e) { estaDesvelada = e; }

        void setPercents(int p[]) {
            for (int i = 0; i < 3; ++i) {
                percentMultp[i] = p[i];
            }
            percentBomba = p[3];
        }

        Casilla() {
            // srand (time(NULL)); // No funciona, se mantiene con el mismo generador en siempre (debe ir en el main)

            int randomMult = rand() % 100;
            int randomBomba = rand() % 100;

            // Según el valor de randomMult asocia un multiplicador del 1 al 3
            if (randomMult < percentMultp[0]) valorMultp = 1;
            else if (randomMult < percentMultp[1]) valorMultp = 2;
            else if (randomMult < percentMultp[2]) valorMultp = 3;

            // Según el valor de randomBomba vuelve a la casilla una bomba, en cuyo caso hace su multiplicador 0
            if (randomBomba < percentBomba) {
                valorBomba = 1;
                valorMultp = 0;
            } else valorBomba = 0;
        }

        Casilla(int percent[])  // Recibe un array con porcentajes para alterar los actuales (por si se requiere cambiarlos)
        {
            setPercents(percent);
            Casilla();
        }

    };

    class Tablero {

    private:
        int ladoLength = 5 - 1;

    public:
        Casilla matrizTablero[5][5];
        int bombasFila[5] = {0, 0, 0, 0, 0};
        int bombasColumna[5] = {0, 0, 0, 0, 0};
        int puntosFila[5] = {0, 0, 0, 0, 0};
        int puntosColumna[5] = {0, 0, 0, 0, 0};

        Tablero() = default;

        //Tablero(int percent[]) : Casilla{percent}//--------------> Problemas aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        //{
        //}

    private:
        void
        calcDatosFila() // Rellena el array bombas fila con el número de bombas por cada fila (nºfila = pos en el array)
        {                    // Rellena el array puntos fila con el número de puntos por cada fila (nºfila = pos en el array)
            int f = 0; // #fila
            int c = 0; // #columna
            for (; c <= ladoLength; ++c) // Recorre la matriz fila a fila de izquierda a derecha
            {
                bombasFila[f] += matrizTablero[f][c].getValorBomba();
                puntosFila[f] += matrizTablero[f][c].getValorMultp();

                if (c == ladoLength &&
                    f < ladoLength) // Salta a la siguiente fila siempre que haya una
                {
                    c = -1;
                    ++f;
                }
            }
        }

        void
        calcDatosColumna() // Rellena el array bombas columna con el número de bombas por cada columna (nºcolumna = pos en el array)
        {                       // Rellena el array puntos columna con el número de puntos por cada columna (nºcolumna = pos en el array)
            int f = 0; // #fila
            int c = 0; // #columna
            for (; f <= 5; ++f) // Recorre la matriz columna a columna de arriba a abajo
            {
                bombasColumna[c] += matrizTablero[f][c].getValorBomba();
                puntosColumna[c] += matrizTablero[f][c].getValorMultp();

                if (f == ladoLength &&
                    c < ladoLength) // Salta a la siguiente columna siempre que haya una
                {
                    f = -1;
                    ++c;
                }
            }
        }

    public:
        void calcDatos() {
            calcDatosFila();
            calcDatosColumna();
        }

    };
}