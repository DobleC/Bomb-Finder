/*
 * TYPE TRAITS
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801082225
 */

#ifndef BASICS_TYPE_TRAITS_HEADER
#define BASICS_TYPE_TRAITS_HEADER

    #include <type_traits>

    namespace basics
    {

        #define ENABLE_IF(CONDITION) template< typename X = void, typename = typename std::enable_if< CONDITION, X >::type >

    }

#endif
