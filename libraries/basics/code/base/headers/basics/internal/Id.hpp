/*
 * ID
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712211447
 */

#ifndef BASICS_ID_HEADER
#define BASICS_ID_HEADER

    #include <basics/fnv>

    #define ID(X) FNV(X)

    namespace basics
    {

        typedef unsigned int Id;

    }

#endif
