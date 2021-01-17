/*
 * SIZE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712250337
 */

#ifndef BASICS_SIZE_HEADER
#define BASICS_SIZE_HEADER

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        struct Size;

        // USAR ARRAYS PARA LOS VALORES Y MÉTODOS DE ACCESO IGUAL QUE EN COORDINATES?

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 1, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            Number length;      // USAR ARRAYS PARA LOS DATOS Y FUNCIONES PARA LOS ACCESSORS

            Size operator * (const Number & value) const
            {
                return { length * value };
            }

            Number operator [] (unsigned index)
            {
                return length;
            }

            const Number operator [] (unsigned index) const
            {
                return length;
            }
        };

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 2, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            Number width;       // USAR ARRAYS PARA LOS DATOS Y FUNCIONES PARA LOS ACCESSORS
            Number height;

            Size operator * (const Number & value) const
            {
                return { width * value, height * value };
            }

            Number & operator [] (unsigned index)
            {
                return index == 0 ? width : height;
            }

            const Number & operator [] (unsigned index) const
            {
                return index == 0 ? width : height;
            }
        };

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 3, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            Number width;       // USAR ARRAYS PARA LOS DATOS Y FUNCIONES PARA LOS ACCESSORS
            Number height;
            Number depth;

            Size operator * (const Number & value) const
            {
                return { width * value, height * value, depth * value };
            }

            Number & operator [] (unsigned index)
            {
                return index == 0 ? width : index == 1 ? height : depth;
            }

            const Number & operator [] (unsigned index) const
            {
                return index == 0 ? width : index == 1 ? height : depth;
            }
        };

        // -----------------------------------------------------------------------------------------

        typedef Size< 1, int      > Size1i;
        typedef Size< 1, unsigned > Size1u;
        typedef Size< 1, float    > Size1f;
        typedef Size< 1, double   > Size1d;

        typedef Size< 2, int      > Size2i;
        typedef Size< 2, unsigned > Size2u;
        typedef Size< 2, float    > Size2f;
        typedef Size< 2, double   > Size2d;

        typedef Size< 3, int      > Size3i;
        typedef Size< 3, unsigned > Size3u;
        typedef Size< 3, float    > Size3f;
        typedef Size< 3, double   > Size3d;

    }

#endif
