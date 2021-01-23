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

        unsigned score = 0;
        unsigned totalScore = 0;
        unsigned round = 0;

    public:

        Controlador()
        {
            ++round;
        }

        int  getScore()           { return score;        }
        void setScore(int score)  { this->score = score; }

        int  getTotalScore()           { return totalScore;  }
        void setTotalScore(int score)  { totalScore = score; }

        int  getRound()           { return round;        }
        void setRound(int round)  { this->round = round; }

        void multiplicarPunt(int multiplo)
        {
            if(score == 0) score += multiplo;
            else score *= multiplo;
        };

        void siguienteRonda()
        {
            ++round;
            totalScore += score;
            score = 0;
        }

        void gameOver() {  }

        explicit operator bool () const
        {
            return round != 0;
        }


    };
}