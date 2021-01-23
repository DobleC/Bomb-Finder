/*
 * MODULES
 * Copyright © 2020+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_MODULES_HEADER
#define BASICS_MODULES_HEADER

    #include <basics/Id>

    namespace basics { namespace internal
    {

        enum class ModuleId : Id
        {
            AUDIO = ID(audio)
        };
        
        template< ModuleId >
        bool module_is_linked ()
        {
            return false;
        }
        
        template< ModuleId >
        void initialize_module ()
        {
        }
        
        template< ModuleId >
        void finalize_module ()
        {
        }

    }}

#endif
