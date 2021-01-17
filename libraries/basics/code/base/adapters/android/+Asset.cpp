/*
 * ASSET
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801220105
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include <android/asset_manager.h>
    #include <basics/Asset>
    #include "Android_Asset.hpp"
    #include "Native_Activity.hpp"

    namespace basics
    {

        std::shared_ptr< Asset > Asset::open (const std::string & path)
        {
            std::shared_ptr< Asset > asset(new internal::Android_Asset(path));

            if (!asset->good ())
            {
                 asset.reset ();
            }

            return asset;
        }

        bool Asset::exists (const std::string & path)
        {
            return internal::Android_Asset(path).good ();
        }

        size_t Asset::size (const std::string & path)
        {
            return internal::Android_Asset(path).size ();
        }

    }

#endif
