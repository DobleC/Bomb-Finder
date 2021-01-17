/*
 * GRAPHICS RESOURCE CACHE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801211747
 */

#ifndef BASICS_GRAPHICS_RESOURCE_CACHE_HEADER
#define BASICS_GRAPHICS_RESOURCE_CACHE_HEADER

    #include <list>
    #include <basics/Graphics_Resource>

    namespace basics
    {

        /**
         * Mantiene punteros weak a recursos que están en uso en situaciones en las que el contexto
         * gráfico se puede destruir y volver a crear.
         */
        class Graphics_Resource_Cache
        {

            typedef std::list< std::weak_ptr< Graphics_Resource > > Graphics_Resource_List;

        public:

            typedef Graphics_Resource_List::iterator Iterator;

        public:

            Graphics_Resource_List resources;

        public:

            Iterator begin ()
            {
                return resources.begin ();
            }

            Iterator end ()
            {
                return resources.end ();
            }

        };

    }

#endif
