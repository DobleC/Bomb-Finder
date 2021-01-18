/*
 * Modelo
 * Copyright © 2020+ Carlos Casado
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * rexitah@gmail.com
 */

#include "modelo.hpp"

namespace model {

    Casilla::Casilla() {
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
    
    Casilla::Casilla(int percent[])
    {
        setPercents(percent);
        Casilla();
    }


    void Tablero::calcDatosFila()
    {
        int f = 0; // #fila
        int c = 0; // #columna
        for (; c <= ladoLength; ++c) // Recorre la matriz fila a fila de izquierda a derecha
        {
            bombasFila[f] += matrizTablero[f][c].getValorBomba();
            puntosFila[f] += matrizTablero[f][c].getValorMultp();

            if (c == ladoLength && f < ladoLength) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }
    }

    void Tablero::calcDatosColumna()
    {
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

    void Tablero::copiarCasillas(Tablero & T) {
        int f = 0; // #fila
        int c = 0; // #columna
        for (; c <= ladoLength; ++c) // Recorre la matriz fila a fila de izquierda a derecha
        {
            matrizTablero[f][c] = T.matrizTablero[f][c];

            if (c == ladoLength && f < ladoLength) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }
    }

    void Tablero::copiarArrays(Tablero & T) {
        for (int i = 0; i < 5; ++i) {
            bombasFila[i]    = T.bombasFila[i];
            bombasColumna[i] = T.bombasColumna[i];
            puntosFila[i]    = T.puntosFila[i];
            puntosColumna[i] = T.puntosColumna[i];
        }

    }

};
