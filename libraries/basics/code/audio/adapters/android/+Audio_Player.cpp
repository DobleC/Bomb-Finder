/*
 * AUDIO PLAYER
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/macros>
#include <basics/Log>

#if defined(BASICS_ANDROID_OS)

    #include "Android_Audio_Player.hpp"

    namespace basics
    {

        Audio_Player & audio_player = Audio_Player::instance ();

        Audio_Player & Audio_Player::instance ()
        {
            log.i ("--------------- CREATING AUDIO PLAYER INSTANCE");
            return internal::Android_Audio_Player::instance ();
        }

    }

#endif
