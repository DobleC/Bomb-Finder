/*
 *  ASSERT
 *  Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1110252259
 */

#ifndef BASICS_ASSERT_HEADER
#define BASICS_ASSERT_HEADER

    // Esta cabecera pretende ser un sustituto de la cabecera <cassert> de la biblioteca estándar.
    // La idea es que esta cabecera puede suprimir las aserciones en configuraciones release en
    // algunas situaciones en las que la macro NDEBUG no queda definida y <cassert> las podría
    // llegar a dejar activas.

    #include <basics/macros>
    #include <cassert>

#endif
