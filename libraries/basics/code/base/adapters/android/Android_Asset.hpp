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

#ifndef BASICS_ANDROID_ASSET_HEADER
#define BASICS_ANDROID_ASSET_HEADER

    #include <basics/Asset>

    class AAsset;

    namespace basics { namespace internal
    {

        class Android_Asset final : public Asset
        {

            AAsset * handle;
            size_t   cursor;
            bool     failed;
            bool     at_end;

        public:

            Android_Asset(const std::string & path);
           ~Android_Asset();

        public:

            bool   good () const override;
            bool   fail () const override;
            bool   eof  () const override;

            size_t size () const override;
            bool   seek (ptrdiff_t offset, Anchor = CURRENT) override;
            size_t tell () const override;
            byte   read () override;
            bool   read_all (std::vector< byte > & buffer) override;
            bool   read_all (std::string & buffer) override;

        private:

            bool read (uint8_t * buffer, size_t size);

        };

    }}

#endif
