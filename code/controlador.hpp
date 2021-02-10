/*
 * Controlador
 * Copyright © 2020+ Carlos Casado
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * rexitah@gmail.com
 */

#include "modelo.hpp"


using namespace model;

namespace controller
{
    class Controlador
    {
    private:

        unsigned score      = 0;                // Puntuación de la ronda
        unsigned totalScore = 0;                // Puntuación de la partida (suma de las rondas)
        unsigned round      = 0;                // Ronda actual

    public:

        /**
          * Constructor, hace que ronda sea = 1
          */
        Controlador() { round = 1; }

        int  getScore()                { return score;        }
        void setScore(int score)       { this->score = score; }

        int  getTotalScore()           { return totalScore;   }
        void setTotalScore(int score)  { totalScore = score;  }

        int  getRound()                { return round;        }
        void setRound(int round)       { this->round = round; }


        /**
          * Recibe un multiplo y lo multiplica por la puntuación de la ronda
          */
        void multiplicarPunt(int multiplo)
        {
            if(score == 0) score += multiplo;
            else score *= multiplo;
        };

        /**
          * Pasa a la siguiente ronda y suma la puntuación de esta a la total
          */
        void siguienteRonda()
        {
            ++round;
            totalScore += score;
            score = 0;
        }

        /**
          * Reinicia los valores del controlador en caso de game over
          */
        void gameOver()
        {
            totalScore = 0;
            score = 0;
            round = 1;
        }

        /**
          * Devuelve verdadero siempre que ronda es mayor que 0
          * (comprueba si se ha iniciado el controlador)
          */
        explicit operator bool () const { return round != 0; }
    };
}