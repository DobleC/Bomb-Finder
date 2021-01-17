/*
 * GRAPHICS RESOURCE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801210040
 */

#ifndef BASICS_GRAPHICS_RESOURCE_HEADER
#define BASICS_GRAPHICS_RESOURCE_HEADER

    #include <memory>

    namespace basics
    {

        class Graphics_Resource
        {
        protected:

            bool initialized;

        protected:

            Graphics_Resource()
            {
                initialized = false;
            }

            virtual ~Graphics_Resource() = default;

        public:

            virtual bool initialize (/*Graphics_Context & context*/) = 0;
            virtual void finalize   () = 0;

        };

    }

#endif
