/*
 *  TRANSLATION
 *  Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_TRANSLATION_HEADER
#define BASICS_TRANSLATION_HEADER

    #include "Vector.hpp"
    #include "Transformation.hpp"

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE = float >
        class Translation;

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        class Translation< 2, NUMERIC_TYPE > : public Transformation< 2, NUMERIC_TYPE >
        {

            typedef Transformation< 2, NUMERIC_TYPE > Base;

            using          Base::matrix;
            using typename Base::Number;
            using typename Base::Numeric_Type;

        public:

            Translation() = default;
            Translation(const Translation & ) = default;

            Translation(const Vector< 2, Numeric_Type > & displacement)
            {
                set (displacement);
            }

            Translation(const Number & displacement_x, const Number & displacement_y)
            {
                set (displacement_x, displacement_y);
            }

        public:

            template< Coordinate_System COORDINATE_SYSTEM >
            void set (const Vector< 2, Numeric_Type, COORDINATE_SYSTEM > & displacement)
            {
                matrix[0][2] = displacement.coordinates.x ();
                matrix[1][2] = displacement.coordinates.y ();
            }

            void set (const Number & displacement_x, const Number & displacement_y)
            {
                matrix[0][2] = displacement_x;
                matrix[1][2] = displacement_y;
            }

        };

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        class Translation< 3, NUMERIC_TYPE > : public Transformation< 3, NUMERIC_TYPE >
        {

            typedef Transformation< 3, NUMERIC_TYPE > Base;

            using          Base::matrix;
            using typename Base::Number;
            using typename Base::Numeric_Type;

        public:

            Translation() = default;
            Translation(const Translation & ) = default;

            Translation(const Vector< 3, Numeric_Type > & displacement)
            {
                set (displacement);
            }

            Translation(const Number & displacement_x, const Number & displacement_y, const Number & displacement_z)
            {
                set (displacement_x, displacement_y, displacement_z);
            }

        public:

            template< Coordinate_System COORDINATE_SYSTEM >
            void set (const Vector< 3, Numeric_Type, COORDINATE_SYSTEM > & displacement)
            {
                matrix[0][3] = displacement.coordinates.x ();
                matrix[1][3] = displacement.coordinates.y ();
                matrix[2][3] = displacement.coordinates.z ();
            }

            void set (const Number & displacement_x, const Number & displacement_y, const Number & displacement_z)
            {
                matrix[0][3] = displacement_x;
                matrix[1][3] = displacement_y;
                matrix[2][3] = displacement_z;
            }

        };

        // -----------------------------------------------------------------------------------------

        typedef Translation< 2, int    > Translation2i;
        typedef Translation< 2, float  > Translation2f;
        typedef Translation< 2, double > Translation2d;

        typedef Translation< 3, int    > Translation3i;
        typedef Translation< 3, float  > Translation3f;
        typedef Translation< 3, double > Translation3d;

    }

#endif
