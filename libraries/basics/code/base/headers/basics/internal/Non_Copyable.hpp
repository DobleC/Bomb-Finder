/*
 * NON COPYABLE
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1111081338
 */

// https://msdn.microsoft.com/en-us/library/dn457344.aspx
// http://www.boost.org/doc/libs/1_57_0/boost/core/noncopyable.hpp
// http://boost.2283326.n4.nabble.com/ADL-comment-in-noncopyable-td2558478.html

#ifndef BASICS_NON_COPYABLE_HEADER
#define BASICS_NON_COPYABLE_HEADER

    #include <basics/macros>

    namespace basics
    {

        namespace non_copyable
        {

            // Defining Non_Copyable within its own namespace prevents function/template argument dependent
            // lookup within the namespace basics for identifiers defined outside it that use Non_Copyable.

            class Non_Copyable
            {
            protected:

                Non_Copyable() = default;
               ~Non_Copyable() = default;

            private:

                Non_Copyable(const Non_Copyable & ) = delete;
                Non_Copyable & operator = (const Non_Copyable & ) = delete;

            };

        }

        using namespace non_copyable;

    }

    #if not defined(BASICS_DONT_EXPORT_TYPES_TO_GLOBAL_NAMESPACE)

        // The use of Non_Copyable is more clear when it can be accessed from the
        // global namespace (e.g. "class X : Non_Copyable").

        using basics::Non_Copyable;

    #endif

#endif
