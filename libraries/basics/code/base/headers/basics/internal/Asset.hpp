/*
 * ASSET
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801220045
 */

#ifndef BASICS_ASSET_HEADER
#define BASICS_ASSET_HEADER

    #include <memory>
    #include <vector>
    #include <string>
    #include <basics/types>

    namespace basics
    {

        class Asset
        {
        public:

            enum Anchor
            {
                BEGINNING,
                CURRENT,
                END
            };

        public:

            static std::shared_ptr< Asset > open (const std::string & path);
            static bool exists (const std::string & path);
            static size_t size (const std::string & path);

        protected:

            Asset() = default;
            virtual ~Asset() = default;

        public:

            virtual bool   good () const = 0;
            virtual bool   fail () const = 0;
            virtual bool   eof  () const = 0;

        public:

            virtual size_t size () const = 0;
            virtual bool   seek (ptrdiff_t offset, Anchor = CURRENT) = 0;
            virtual size_t tell () const = 0;
            virtual byte   read () = 0;
            virtual bool   read_all (std::vector< byte > & buffer) = 0;
            virtual bool   read_all (std::string & buffer) = 0;

        };

    }

#endif
