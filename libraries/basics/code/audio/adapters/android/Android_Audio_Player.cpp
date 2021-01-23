/*
 * ANDROID AUDIO PLAYER
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Android_Audio_Player.hpp"
    #include "../../../base/adapters/android/Native_Activity.hpp"
    #include <android/asset_manager.h>
    #include <basics/internal/module.hpp>
    #include <basics/Log>

    namespace basics { namespace internal
    {

        // -----------------------------------------------------------------------------------------

        Android_Audio_Player::File_Playback_Controller::File_Playback_Controller(const std::string & file, bool loop)
        :
            player_object   (nullptr),
            player_interface(nullptr),
            seeker_interface(nullptr)
        {
            auto & audio_player = Android_Audio_Player::instance ();

            if (audio_player.is_ready ())
            {
                auto asset_handle = AAssetManager_open
                (
                    internal::native_activity->get_activity ().assetManager,
                    file.c_str (),
                    AASSET_MODE_UNKNOWN
                );

                if (asset_handle)
                {
                    SLDataLocator_AndroidFD data_locator;
                    SLDataFormat_MIME       data_format;
                    SLDataSource            data_source;
                    SLDataLocator_OutputMix data_output;
                    SLDataSink              data_target;

                    off_t asset_offset, asset_size;

                    data_locator.locatorType = SL_DATALOCATOR_ANDROIDFD;
                    data_locator.fd          = AAsset_openFileDescriptor (asset_handle, &asset_offset, &asset_size);
                    data_locator.offset      = asset_offset;
                    data_locator.length      = asset_size;

                    if (data_locator.fd >= 0)
                    {
                        data_format.formatType    = SL_DATAFORMAT_MIME;
                        data_format.mimeType      = nullptr;
                        data_format.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

                        data_source.pLocator      = &data_locator;
                        data_source.pFormat       = &data_format;

                        data_output.locatorType   = SL_DATALOCATOR_OUTPUTMIX;
                        data_output.outputMix     = audio_player.get_output_mixer_object ();

                        data_target.pLocator      = &data_output;
                        data_target.pFormat       = nullptr;

                        static const SLInterfaceID interface_ids     [] = { SL_IID_PLAY,     SL_IID_SEEK     };
                        static const SLboolean     interface_required[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

                        auto result = (*audio_player.get_audio_engine_interface ())->CreateAudioPlayer
                        (
                            audio_player.get_audio_engine_interface (),
                            &player_object,
                            &data_source,
                            &data_target,
                            2,
                            interface_ids,
                            interface_required
                        );

                        if (result == SL_RESULT_SUCCESS && setup_player () == true)
                        {
                            status =  STOPPED;
                        }
                    }

                    AAsset_close (asset_handle);
                }
            }

            if (this->is_not_ok ())
            {
                finalize ();
            }
        }

        // -----------------------------------------------------------------------------------------

        Android_Audio_Player::File_Playback_Controller::~File_Playback_Controller()
        {
            finalize ();
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Audio_Player::File_Playback_Controller::play ()
        {
            if (this->is_ok ())
            {
                if (get_status () == PLAYING)       // Esta llamada sincroniza con el estado real del player_object
                {
                    return true;
                }
                else
                {
                    // Si la aplicación ha regresado de estar en segundo plano, quizás sea necesario
                    // recargar los recursos necesarios antes de poder reanudar la reproducción:

                    SLuint32 player_object_state = 0;

                    (*player_object)->GetState (player_object, &player_object_state);

                    if (player_object_state != SL_OBJECT_STATE_REALIZED)
                    {
                        auto result = (*player_object)->Realize (player_object, SL_BOOLEAN_FALSE);

                        if (result != SL_RESULT_SUCCESS)
                        {
                            return false;
                        }
                    }

                    auto   result = (*player_interface)->SetPlayState (player_interface, SL_PLAYSTATE_PLAYING);

                    return result == SL_RESULT_SUCCESS;
                }
            }

            return false;
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Audio_Player::File_Playback_Controller::pause ()
        {
            if (this->is_ok ())
            {
                if (get_status () == PAUSED)        // Esta llamada sincroniza con el estado real del player_object
                {
                    return true;
                }
                else
                {
                    auto   result = (*player_interface)->SetPlayState (player_interface, SL_PLAYSTATE_PAUSED);

                    return result == SL_RESULT_SUCCESS;
                }
            }

            return false;
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Audio_Player::File_Playback_Controller::stop ()
        {
            if (this->is_ok ())
            {
                if (get_status () == STOPPED)       // Esta llamada sincroniza con el estado real del player_object
                {
                    return true;
                }
                else
                {
                    auto   result = (*player_interface)->SetPlayState (player_interface, SL_PLAYSTATE_STOPPED);

                    return result == SL_RESULT_SUCCESS;
                }
            }

            return false;
        }

        // -----------------------------------------------------------------------------------------

        Android_Audio_Player::Playback_Controller::Status Android_Audio_Player::File_Playback_Controller::get_status ()
        {
            if (this->is_ok ())
            {
                SLuint32 player_state;

                if ((*player_interface)->GetPlayState (player_interface, &player_state) == SL_RESULT_SUCCESS)
                {
                    switch (player_state)
                    {
                        case SL_PLAYSTATE_PLAYING: status = PLAYING; break;
                        case SL_PLAYSTATE_PAUSED:  status = PAUSED;  break;
                        case SL_PLAYSTATE_STOPPED: status = STOPPED; break;
                    }
                }
            }

            return status;
        }

        // -----------------------------------------------------------------------------------------

        void Android_Audio_Player::File_Playback_Controller::finalize ()
        {
            if (player_object)
            {
                // Dado el caso, es necesario detener la reproducción antes de destruir el player_object:

                if (player_interface)
                {
                    SLuint32 player_object_state;
                    SLresult result = (*player_object)->GetState (player_object, &player_object_state);

                    if (result == SL_RESULT_SUCCESS && player_object_state == SL_OBJECT_STATE_REALIZED)
                    {
                        SLuint32 player_state;
                        SLresult result = (*player_interface)->GetPlayState (player_interface, &player_state);

                        if (result == SL_RESULT_SUCCESS && player_state == SL_PLAYSTATE_PLAYING)
                        {
                            (*player_interface)->SetPlayState (player_interface, SL_PLAYSTATE_PAUSED);
                        }
                    }

                    player_interface = nullptr;
                }

                (*player_object)->Destroy (player_object);

                player_object = nullptr;
                status        = UNINITIALIZED;
            }
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Audio_Player::File_Playback_Controller::setup_player ()
        {
            SLresult result = (*player_object)->Realize (player_object, SL_BOOLEAN_FALSE);

            if (result == SL_RESULT_SUCCESS)
            {
                result = (*player_object)->GetInterface (player_object, SL_IID_PLAY, &player_interface);

                if (result == SL_RESULT_SUCCESS)
                {
                    result = (*player_object)->GetInterface (player_object, SL_IID_SEEK, &seeker_interface);

                    if (result == SL_RESULT_SUCCESS)
                    {
                        result = (*seeker_interface)->SetLoop (seeker_interface, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);

                        if (result == SL_RESULT_SUCCESS)
                        {
                            result = (*player_interface)->SetPlayState(player_interface, SL_PLAYSTATE_PLAYING);

                            if (result == SL_RESULT_SUCCESS)
                            {
                                return true;
                            }
                        }
                    }
                }
            }

            return false;
        }

        // -----------------------------------------------------------------------------------------

        Android_Audio_Player::Android_Audio_Player()
        {
            initialized            = false;
            audio_engine_object    = nullptr;
            audio_engine_interface = nullptr;
            output_mixer_object    = nullptr;

            initialize ();          // Al ser singleton, esta llamada solo se ejecuta una vez

            Log log;
            if (initialized)
                log.i ("AUDIO INITIALIZED");
            else
                log.e ("AUDIO INITIALIZATION FAILED");
        }

        // -----------------------------------------------------------------------------------------

        Android_Audio_Player::~Android_Audio_Player()
        {
            log.i ("FINALIZING AUDIO...");
            finalize ();
        }

        // -----------------------------------------------------------------------------------------

        bool Android_Audio_Player::initialize ()
        {
            if (not initialized)
            {
                // Se crea el engine de audio:

                static const SLInterfaceID audio_engine_interface_ids     [] = { SL_IID_ENGINE   };
                static const SLboolean     audio_engine_interface_required[] = { SL_BOOLEAN_TRUE };

                auto result = slCreateEngine
                (
                    &audio_engine_object,
                    0,
                    nullptr,
                    1,
                    audio_engine_interface_ids,
                    audio_engine_interface_required
                );

                if (result == SL_RESULT_SUCCESS)
                {
                    // Se alojan los recursos que necesita el engine de audio de modo síncrono para
                    // poder usarlo. Ello puede producir un bloqueo durante un tiempo indeterminado
                    // pero presumiblemente corto:

                    result = (*audio_engine_object)->Realize (audio_engine_object, SL_BOOLEAN_FALSE);

                    if (result == SL_RESULT_SUCCESS)
                    {
                        // Se obtiene la interfaz que dará accedo a los métodos del engine:

                        result = (*audio_engine_object)->GetInterface
                        (
                            audio_engine_object,
                            SL_IID_ENGINE,
                            &audio_engine_interface
                        );

                        if (result == SL_RESULT_SUCCESS)
                        {
                            // Se crea el mixer de salida de audio:

                            static const SLInterfaceID output_mixer_interface_ids     [] = { SL_IID_VOLUME    };
                            static const SLboolean     output_mixer_interface_required[] = { SL_BOOLEAN_FALSE };

                            result = (*audio_engine_interface)->CreateOutputMix
                            (
                                audio_engine_interface,
                                &output_mixer_object,
                                0,
                                nullptr,
                                nullptr
                            );

                            if (result == SL_RESULT_SUCCESS)
                            {
                                // Se alojan los recursos que necesita el engine de audio de modo síncrono
                                // para poder usarlo.

                                result = (*output_mixer_object)->Realize (output_mixer_object, SL_BOOLEAN_FALSE);

                                if (result == SL_RESULT_SUCCESS)
                                {
                                    // Llegados a este punto ya se puede dar por inicializado el audio:

                                    initialized = true;
                                }
                            }
                        }
                    }
                }

                // Si el intento de inicialización ha fallado, se intenta liberar todos los recursos
                // que se hayan llegado a alojar:

                if (not initialized)
                {
                    finalize ();
                }
            }

            return initialized;
        }

        // -----------------------------------------------------------------------------------------

        void Android_Audio_Player::finalize ()
        {
            // Se detienen todos los audios que puedan estar reproduciéndose:

            for (auto & item : playback_controllers)
            {
                auto      playback_controller = item.second.get ();
                auto file_playback_controller = dynamic_cast< File_Playback_Controller * >(playback_controller);

                if  (file_playback_controller)
                {
                     file_playback_controller->finalize ();
                }
            }

            playback_controllers.clear ();

            if (output_mixer_object)
            {
                (*output_mixer_object)->Destroy (output_mixer_object);
                  output_mixer_object = nullptr;
            }

            if (audio_engine_object)
            {
                (*audio_engine_object)->Destroy (audio_engine_object);
                  audio_engine_object = nullptr;
            }
        }

        // -----------------------------------------------------------------------------------------

        Audio_Player::Playback_Controller_Ptr Android_Audio_Player::play (const Audio_Data & audio, bool loop)
        {

            return 0;
        }

        // -----------------------------------------------------------------------------------------

        Audio_Player::Playback_Controller_Ptr Android_Audio_Player::play (const Audio_Source & audio, bool loop)
        {

            return 0;
        }

        // -----------------------------------------------------------------------------------------

        Audio_Player::Playback_Controller_Ptr Android_Audio_Player::play (const std::string & file, bool loop)
        {
            Id id = fnv32 (file);

            std::shared_ptr< Playback_Controller > controller(new File_Playback_Controller(file, loop));

            if (controller->is_ok ())
            {
                playback_controllers[id] = controller;

                log.i (">>>>>>>>>>>>>>>>>>>>>>>>>>> GOOD!");

                return controller;
            }

            log.i (">>>>>>>>>>>>>>>>>>>>>>>>>>> :(");

            return Playback_Controller_Ptr{};
        }

    }}

#endif
