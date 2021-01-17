/*
 * ANDROID ASSET
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801220100
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Android_Asset.hpp"
    #include <android/asset_manager.h>
    #include "Native_Activity.hpp"

    namespace basics { namespace internal
    {

        Android_Asset::Android_Asset(const std::string & path)
        {
            handle = AAssetManager_open
            (
                internal::native_activity->get_activity ().assetManager,
                path.c_str (),
                AASSET_MODE_UNKNOWN
            );

            cursor = 0;
            failed = handle == nullptr;
            at_end = false;
        }

        Android_Asset::~Android_Asset()
        {
            if (handle != nullptr)
            {
                AAsset_close (handle), handle = nullptr;
            }
        }

        bool Android_Asset::good () const
        {
            return not failed;
        }

        bool Android_Asset::fail () const
        {
            return failed;
        }

        bool Android_Asset::eof () const
        {
            return at_end;
        }

        size_t Android_Asset::size () const
        {
            return good () ? size_t(AAsset_getLength (handle)) : 0;
        }

        bool Android_Asset::seek (ptrdiff_t offset, Anchor anchor)
        {
            if (good ())
            {
                off_t new_offset = AAsset_seek
                (
                    handle,
                    off_t(offset),
                    anchor == BEGINNING ? SEEK_SET : anchor == END ? SEEK_END : SEEK_CUR
                );

                if (new_offset >= 0)
                {
                    cursor = size_t(new_offset);

                    return true;
                }
            }

            return false;
        }

        size_t Android_Asset::tell () const
        {
            return cursor;
        }

        byte Android_Asset::read ()
        {
            byte data = 0;

            if (good ())
            {
                read (&data, 1);
            }

            return data;
        }

        bool Android_Asset::read_all (std::vector< byte > & buffer)
        {
            if (good ())
            {
                size_t s = size ();

                buffer.resize (s);

                return read (buffer.data (), s);
            }

            return false;
        }

        bool Android_Asset::read_all (std::string & buffer)
        {
            if (good ())
            {
                size_t s = size ();

                buffer.resize (s);

                return read ((uint8_t *)buffer.data (), s);
            }

            return false;
        }

        bool Android_Asset::read (uint8_t * buffer, size_t size)
        {
            if (size > 0)
            {
                int result = AAsset_read (handle, buffer, size);

                if (size_t(result) == size)
                {
                    cursor += size;

                    return true;
                }
                else
                if (result == 0)
                {
                    at_end = true;
                }
                else
                    failed = true;

                return false;
            }

            return true;
        }

    }}

#endif
