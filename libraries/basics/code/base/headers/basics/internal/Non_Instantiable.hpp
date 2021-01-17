/*
 * NON INSTANTIABLE
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1602091422
 */

#ifndef BASICS_NON_INSTANTIABLE_HEADER
#define BASICS_NON_INSTANTIABLE_HEADER

    namespace basics
    {

        namespace non_instantiable
        {

            // Defining Non_Instantiable within its own namespace prevents function/template argument dependent
            // lookup within the namespace basics for identifiers defined outside it that use Non_Instantiable.

            class Non_Instantiable
            {
                  Non_Instantiable() { }
            };

        }

        using namespace non_instantiable;

    }

    #if not defined(BASICS_DONT_EXPORT_TYPES_TO_GLOBAL_NAMESPACE)

        // The use of Non_Instantiable is more clear when it can be accessed from the
        // global namespace (e.g. "class X : Non_Instantiable").

        using basics::Non_Instantiable;

    #endif

#endif
