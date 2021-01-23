/*
 * AUDIO PLAYER
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_AUDIO_PLAYER_HEADER
#define BASICS_AUDIO_PLAYER_HEADER

    #include <memory>
    #include <string>
    #include <basics/Audio_Data>
    #include <basics/Audio_Source>
    #include <basics/Id>
    #include <basics/Non_Copyable>

    namespace basics
    {

        /** Se encarga de coordinar la reproducción de audio con el mixer.
          */
        class Audio_Player : Non_Copyable
        {
        public:

            /** Permite controlar la reproducción un sonido.
              */
            class Playback_Controller
            {
            public:

                enum Status
                {
                    UNINITIALIZED, PLAYING, PAUSED, STOPPED
                };

            protected:

                Status status = UNINITIALIZED;

            public:

                bool is_ok     () const { return status != UNINITIALIZED; }
                bool is_not_ok () const { return status == UNINITIALIZED; }

            public:

                virtual Status get_status () = 0;

                virtual bool play  () = 0;
                virtual bool pause () = 0;
                virtual bool stop  () = 0;

            };

            typedef std::shared_ptr< Playback_Controller > Playback_Controller_Ptr;

        public:

            static Audio_Player & instance ();

        public:

            virtual bool is_ready () const = 0;

        public:

            virtual Playback_Controller_Ptr play (const Audio_Data   & audio, bool loop = false) = 0;
            virtual Playback_Controller_Ptr play (const Audio_Source & audio, bool loop = false) = 0;
            virtual Playback_Controller_Ptr play (const std::string  & file , bool loop = false) = 0;

        };

        extern Audio_Player & audio_player;

    }

#endif
