/*
 * ANDROID AUDIO PLAYER
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_ANDROID_AUDIO_PLAYER_HEADER
#define BASICS_ANDROID_AUDIO_PLAYER_HEADER

    #include <map>
    #include <memory>
    #include <SLES/OpenSLES.h>
    #include <SLES/OpenSLES_Android.h>
    #include <basics/Audio_Player>

    namespace basics { namespace internal
    {

        class Android_Audio_Player final : public Audio_Player
        {
        private:

            class File_Playback_Controller final : public Playback_Controller
            {
                SLObjectItf player_object;
                SLPlayItf   player_interface;
                SLSeekItf   seeker_interface;

            public:

                File_Playback_Controller(const std::string & file, bool loop);
               ~File_Playback_Controller();

                Status get_status () override;
                bool   play       () override;
                bool   pause      () override;
                bool   stop       () override;
                void   finalize   ();

            private:

                bool setup_player ();

            };

            typedef std::map< Id, Playback_Controller_Ptr > Playback_Controller_Map;

        public:

            static Android_Audio_Player & instance ()
            {
                static Android_Audio_Player android_audio_player;
                return android_audio_player;
            }

        private:

            bool        initialized;

            SLObjectItf audio_engine_object;
            SLEngineItf audio_engine_interface;
            SLObjectItf output_mixer_object;

            Playback_Controller_Map playback_controllers;

        private:

            Android_Audio_Player();
           ~Android_Audio_Player();

        public:

            bool initialize ();
            void finalize   ();
            bool is_ready   () const override { return initialized; }

            SLObjectItf & get_audio_engine_object    () { return audio_engine_object;    }
            SLEngineItf & get_audio_engine_interface () { return audio_engine_interface; }
            SLObjectItf & get_output_mixer_object    () { return output_mixer_object;    }

        public:

            Playback_Controller_Ptr play (const Audio_Data   & audio, bool loop = false) override;
            Playback_Controller_Ptr play (const Audio_Source & audio, bool loop = false) override;
            Playback_Controller_Ptr play (const std::string  & file , bool loop = false) override;

        };

    }}

#endif
