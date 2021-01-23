/*
 * AUDIO DATA
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_AUDIO_DATA_HEADER
#define BASICS_AUDIO_DATA_HEADER

    #include <cassert>
    #include <vector>
    #include <basics/types>

    namespace basics
    {

        /** Representa un conjunto de datos de audio descomprimido (en algún formato RAW) alojados
          * en la memoria RAM.
          */
        class Audio_Data
        {
        public:

            struct Format
            {

                enum Channels
                {
                    MONO = 1, STEREO = 2
                };

                enum Interleaving
                {
                    INTERLEAVED, PLANAR
                };

                enum Sample_Type
                {
                    INT, UINT, FLOAT
                };

                enum Endianness
                {
                    LITTLE_ENDIAN, BIG_ENDIAN
                };

            public:

                unsigned      channels;                 ///< number of channels
                Interleaving  interleaving;             ///< channel interleaving mode

                unsigned      sample_rate;              ///< number of samples per second
                Sample_Type   sample_type;              ///< data type of a sample
                unsigned      sample_size;              ///< size of a sample in bytes
                Endianness    sample_endianness;        ///< endianness of a multi-byte sample

            public:

                bool is_interleaved   () const { return interleaving      == INTERLEAVED;   }
                bool is_planar        () const { return interleaving      == PLANAR;        }
                bool is_little_endian () const { return sample_endianness == LITTLE_ENDIAN; }
                bool is_big_endian    () const { return sample_endianness == BIG_ENDIAN;    }

            };

            typedef std::vector< byte > Buffer;

        private:

            struct
            {
                Format format;
                Buffer buffer;
                size_t number_of_samples;
            }
            attributes;

        public:

            Audio_Data(const Format & format, size_t number_of_samples)
            :
                attributes{ format, {}, number_of_samples }
            {
                assert(format.channels    > 0);
                assert(format.sample_rate > 0);
                assert(format.sample_size > 0);

                attributes.buffer.resize (format.channels * format.sample_size * number_of_samples);
            }

            const Format & format () const
            {
                return attributes.format;
            }

            size_t number_of_samples () const
            {
                return attributes.number_of_samples;
            }

            byte * data ()
            {
                return attributes.buffer.data ();
            }

            const byte * data () const
            {
                return attributes.buffer.data ();
            }

        };

        /** Sampling rate values provided as help to type common standard values.
          */
        enum Sample_Rate : unsigned
        {
            SAMPLE_RATE_8000   =   8000U,       // telephone, walkie-talkie, etc.
            SAMPLE_RATE_11025  =  11025U,       // a quarter of audio CD
            SAMPLE_RATE_16000  =  16000U,       // modern VoIP
            SAMPLE_RATE_22050  =  22050U,       // half of audio CD
            SAMPLE_RATE_32000  =  32000U,       // analog TV, video camcorders and others
            SAMPLE_RATE_44100  =  44100U,       // audio CD
            SAMPLE_RATE_48000  =  48000U,       // professional digital video
            SAMPLE_RATE_88200  =  88200U,       // double of audio CD
            SAMPLE_RATE_96000  =  96000U,       // audio DVD, Blu-ray and others
            SAMPLE_RATE_192000 = 192000U,       // audio DVD, Blu-ray and others
        };

    }

#endif
