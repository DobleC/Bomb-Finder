/*
 *  TRANSFORMATION
 *  Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_TRANSFORMATION_HEADER
#define BASICS_TRANSFORMATION_HEADER

    #include "Matrix.hpp"
    #include "Vector.hpp"

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        class Transformation
        {
        public:

            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            static  constexpr unsigned dimension = DIMENSION;
            static  constexpr unsigned size      = dimension + 1;

            typedef Matrix< size, size, Numeric_Type > Matrix;

        public:

            Matrix matrix;

        public:

            Transformation()
            :
                matrix(Matrix::identity)
            {
            }

            Transformation(const Matrix & matrix)
            :
                matrix(matrix)
            {
            }

        public:

            Transformation operator * (const Transformation & other) const
            {
                return this->matrix *  other.matrix;
            }

            operator const Matrix & () const
            {
                return matrix;
            }

        };

        typedef Transformation< 2, int    > Transformation2i;
        typedef Transformation< 2, float  > Transformation2f;
        typedef Transformation< 2, double > Transformation2d;

        typedef Transformation< 3, int    > Transformation3i;
        typedef Transformation< 3, float  > Transformation3f;
        typedef Transformation< 3, double > Transformation3d;

        template< typename NUMERIC_TYPE >
        Transformation< 2, NUMERIC_TYPE > rotate_then_translate_2d (float angle, const Vector< 2, NUMERIC_TYPE > & displacement)
        {
            Transformation< 2, NUMERIC_TYPE > transformation;

            NUMERIC_TYPE sin = NUMERIC_TYPE(std::sin (angle));
            NUMERIC_TYPE cos = NUMERIC_TYPE(std::cos (angle));

            transformation.matrix[0][2] =  displacement.coordinates.x ();
            transformation.matrix[1][2] =  displacement.coordinates.y ();
            transformation.matrix[0][0] =  cos;
            transformation.matrix[0][1] = -sin;
            transformation.matrix[1][0] =  sin;
            transformation.matrix[1][1] =  cos;

            return transformation;
        }

        template< typename NUMERIC_TYPE >
        Transformation< 2, NUMERIC_TYPE > scale_then_translate_2d (NUMERIC_TYPE scale_x, NUMERIC_TYPE scale_y, const Vector< 2, NUMERIC_TYPE > & displacement)
        {
            Transformation< 2, NUMERIC_TYPE > transformation;

            transformation.matrix[0][2] = displacement.coordinates.x ();
            transformation.matrix[1][2] = displacement.coordinates.y ();
            transformation.matrix[0][0] = scale_x;
            transformation.matrix[1][1] = scale_y;

            return transformation;
        }

        template< typename NUMERIC_TYPE >
        inline Transformation< 2, NUMERIC_TYPE > scale_then_translate_2d (NUMERIC_TYPE scale, const Vector< 2, NUMERIC_TYPE > & displacement)
        {
            return scale_then_translate_2d (scale, scale, displacement);
        }

        template< typename NUMERIC_TYPE >
        Transformation< 2, NUMERIC_TYPE > translate_then_scale_2d (const Vector< 2, NUMERIC_TYPE > & displacement, NUMERIC_TYPE scale_x, NUMERIC_TYPE scale_y)
        {
            Transformation< 2, NUMERIC_TYPE > transformation;

            transformation.matrix[0][2] = displacement.coordinates.x () * scale_x;
            transformation.matrix[1][2] = displacement.coordinates.y () * scale_y;
            transformation.matrix[0][0] = scale_x;
            transformation.matrix[1][1] = scale_y;

            return transformation;
        }

        template< typename NUMERIC_TYPE >
        inline Transformation< 2, NUMERIC_TYPE > translate_then_scale_2d (const Vector< 2, NUMERIC_TYPE > & displacement, NUMERIC_TYPE scale)
        {
            return translate_then_scale_2d (displacement, scale, scale);
        }

    };

#endif
