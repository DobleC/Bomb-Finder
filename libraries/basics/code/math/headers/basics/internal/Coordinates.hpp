/*
 *  COORDINATES
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_COORDINATES_HEADER
#define BASICS_COORDINATES_HEADER

    #include <algorithm>
    #include <limits>
    #include <basics/type_traits>

    namespace basics
    {

        // USAR UNA ENUMERACIÓN LIMITA QUE TERCEROS PUEDAN HACER ESPECIALIZACIONES DISTINTAS, PERO
        // RESULTA ÚTIL PARA PODER COMPARAR/COMPROBAR.
        // PODRÍA SER INTERESTANTE (AUNQUE QUIZÁS POCO ÚTIL) QUE UN SISTEMA DE COORDENADAS APORTASE
        // INFORMACIÓN ADICIONAL COMO ES ESPACIO SOBRE EL QUE TRABAJA (EUCLÍDEO, PROYECTIVO, ETC.).
        enum Coordinate_System
        {
            CARTESIAN,
            HOMOGENEOUS,
            POLAR,
            CYLINDRICAL,
        };

        // -----------------------------------------------------------------------------------------

        template< unsigned DIMENSION, typename NUMERIC_TYPE = float, Coordinate_System = CARTESIAN >
        class Coordinates;

        // -----------------------------------------------------------------------------------------

        namespace internal
        {

            template< unsigned DIMENSION, unsigned VALUE_COUNT, typename NUMERIC_TYPE >
            class Coordinates
            {
                static_assert(DIMENSION > 0, "The template parameter DIMENSION of Coordinates can't be 0.");

            public:

                typedef NUMERIC_TYPE Numeric_Type;
                typedef Numeric_Type Number;

                static  constexpr unsigned dimension   = DIMENSION;
                static  constexpr unsigned value_count = VALUE_COUNT;

            public:

                static Number min_value ()
                {
                    return std::numeric_limits< Numeric_Type >::min ();
                }

                static Number max_value ()
                {
                    return std::numeric_limits< Numeric_Type >::max ();
                }

            protected:

                Number values[value_count];

            public:

                Coordinates() = default;
                Coordinates(const Coordinates & ) = default;

                Coordinates(const Number (& given_values)[value_count])
                {
                    std::copy_n (given_values, value_count, this->values);
                }

                ENABLE_IF(value_count == 1)
                Coordinates(const Number & a)
                {
                    values[0] = a;
                }

                ENABLE_IF(value_count == 2)
                Coordinates(const Number & a, const Number & b)
                {
                    values[0] = a;
                    values[1] = b;
                }

                ENABLE_IF(value_count == 3)
                Coordinates(const Number & a, const Number & b, const Number & c)
                {
                    values[0] = a;
                    values[1] = b;
                    values[2] = c;
                }

                ENABLE_IF(value_count == 4)
                Coordinates(const Number & a, const Number & b, const Number & c, const Number & d)
                {
                    values[0] = a;
                    values[1] = b;
                    values[2] = c;
                    values[4] = d;
                }

            public:

                Number & operator [] (const unsigned index)
                {
                    return values[index];
                }

                const Number & operator [] (const unsigned index) const
                {
                    return values[index];
                }

                bool operator == (const Coordinates & other) const
                {
                    return std::equal (this->values, this->values + dimension, other.values);
                }

                bool operator != (const Coordinates & other) const
                {
                    return !(*this == other);
                }

                explicit operator Numeric_Type * ()
                {
                    return values;
                }

                explicit operator const Numeric_Type * () const
                {
                    return values;
                }

            };

        }

        // -----------------------------------------------------------------------------------------

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        class Coordinates< DIMENSION, NUMERIC_TYPE, CARTESIAN > : public internal::Coordinates< DIMENSION, DIMENSION, NUMERIC_TYPE >
        {
            typedef internal::Coordinates< DIMENSION, DIMENSION, NUMERIC_TYPE > Base;

            using typename Base::Number;
            using typename Base::Numeric_Type;
            using          Base::dimension;
            using          Base::values;

        public:

            Coordinates() = default;
            Coordinates(const Coordinates & ) = default;

            template< typename... PARAMETERS >
            Coordinates(const PARAMETERS &... parameters) : Base(parameters...)
            {
            }

        public:

            ENABLE_IF(dimension >= 1)       Number & x ()       { return values[0]; }
            ENABLE_IF(dimension >= 1) const Number & x () const { return values[0]; }
            ENABLE_IF(dimension >= 2)       Number & y ()       { return values[1]; }
            ENABLE_IF(dimension >= 2) const Number & y () const { return values[1]; }
            ENABLE_IF(dimension >= 3)       Number & z ()       { return values[2]; }
            ENABLE_IF(dimension >= 3) const Number & z () const { return values[2]; }
            ENABLE_IF(dimension >= 4)       Number & t ()       { return values[3]; }
            ENABLE_IF(dimension >= 4) const Number & t () const { return values[3]; }

        };

        typedef Coordinates< 1,      int, CARTESIAN > Coordinates1ic;
        typedef Coordinates< 1, unsigned, CARTESIAN > Coordinates1uc;
        typedef Coordinates< 1,    float, CARTESIAN > Coordinates1fc;
        typedef Coordinates< 1,   double, CARTESIAN > Coordinates1dc;
        typedef Coordinates< 2,      int, CARTESIAN > Coordinates2ic;
        typedef Coordinates< 2, unsigned, CARTESIAN > Coordinates2uc;
        typedef Coordinates< 2,    float, CARTESIAN > Coordinates2fc;
        typedef Coordinates< 2,   double, CARTESIAN > Coordinates2dc;
        typedef Coordinates< 3,      int, CARTESIAN > Coordinates3ic;
        typedef Coordinates< 3, unsigned, CARTESIAN > Coordinates3uc;
        typedef Coordinates< 3,    float, CARTESIAN > Coordinates3fc;
        typedef Coordinates< 3,   double, CARTESIAN > Coordinates3dc;
        typedef Coordinates< 4,      int, CARTESIAN > Coordinates4ic;
        typedef Coordinates< 4, unsigned, CARTESIAN > Coordinates4uc;
        typedef Coordinates< 4,    float, CARTESIAN > Coordinates4fc;
        typedef Coordinates< 4,   double, CARTESIAN > Coordinates4dc;

        // -----------------------------------------------------------------------------------------

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        class Coordinates< DIMENSION, NUMERIC_TYPE, HOMOGENEOUS > : public internal::Coordinates< DIMENSION, DIMENSION + 1, NUMERIC_TYPE >
        {

            typedef internal::Coordinates< DIMENSION, DIMENSION + 1, NUMERIC_TYPE > Base;

            using typename Base::Number;
            using typename Base::Numeric_Type;
            using          Base::dimension;
            using          Base::values;

        public:

            Coordinates() = default;
            Coordinates(const Coordinates & ) = default;

            template< typename... PARAMETERS >
            Coordinates(const PARAMETERS &... parameters) : Base(parameters...)
            {
            }

        public:

            ENABLE_IF(dimension >= 1)       Number & x ()       { return values[0]; }
            ENABLE_IF(dimension >= 1) const Number & x () const { return values[0]; }
            ENABLE_IF(dimension >= 2)       Number & y ()       { return values[1]; }
            ENABLE_IF(dimension >= 2) const Number & y () const { return values[1]; }
            ENABLE_IF(dimension >= 3)       Number & z ()       { return values[2]; }
            ENABLE_IF(dimension >= 3) const Number & z () const { return values[2]; }
            ENABLE_IF(dimension >= 1)       Number & w ()       { return values[dimension]; }
            ENABLE_IF(dimension >= 1) const Number & w () const { return values[dimension]; }

        };

        typedef Coordinates< 1,      int, HOMOGENEOUS > Coordinates1ih;
        typedef Coordinates< 2,      int, HOMOGENEOUS > Coordinates2ih;
        typedef Coordinates< 3,      int, HOMOGENEOUS > Coordinates3ih;
        typedef Coordinates< 4,      int, HOMOGENEOUS > Coordinates4ih;
        typedef Coordinates< 1, unsigned, HOMOGENEOUS > Coordinates1uh;
        typedef Coordinates< 2, unsigned, HOMOGENEOUS > Coordinates2uh;
        typedef Coordinates< 3, unsigned, HOMOGENEOUS > Coordinates3uh;
        typedef Coordinates< 4, unsigned, HOMOGENEOUS > Coordinates4uh;
        typedef Coordinates< 1,    float, HOMOGENEOUS > Coordinates1fh;
        typedef Coordinates< 2,    float, HOMOGENEOUS > Coordinates2fh;
        typedef Coordinates< 3,    float, HOMOGENEOUS > Coordinates3fh;
        typedef Coordinates< 4,    float, HOMOGENEOUS > Coordinates4fh;
        typedef Coordinates< 1,   double, HOMOGENEOUS > Coordinates1dh;
        typedef Coordinates< 2,   double, HOMOGENEOUS > Coordinates2dh;
        typedef Coordinates< 3,   double, HOMOGENEOUS > Coordinates3dh;
        typedef Coordinates< 4,   double, HOMOGENEOUS > Coordinates4dh;

    }

#endif
