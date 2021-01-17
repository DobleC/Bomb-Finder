/*
 *  VECTOR
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_VECTOR_HEADER
#define BASICS_VECTOR_HEADER

    #include <cmath>
    #include "Coordinates.hpp"

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE = float, Coordinate_System COORDINATE_SYSTEM = CARTESIAN >
        class Vector
        {
        public:

            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            typedef Coordinates< DIMENSION, NUMERIC_TYPE, COORDINATE_SYSTEM > Coordinates;

        public:

            Coordinates coordinates;

        public:

            Vector() = default;
            Vector(const Vector & other) = default;

            Vector(const Coordinates & given_coordinates) : coordinates(given_coordinates)
            {
            }

            Vector(const Number (& given_values)[Coordinates::value_count]) : coordinates(given_values)
            {
            }

            template< typename... PARAMETERS >
            Vector(const PARAMETERS &... parameters) : coordinates(parameters...)
            {
            }

        public:

            Number length () const
            {
                return std::sqrt (length_squared ());
            }

            Number length_squared () const
            {
                Number length_squared = Number(0);

                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    length_squared += coordinates[i] * coordinates[i];
                }

                return length_squared;
            }

            Vector & normalize ()
            {
                Number divisor = Number(1) / length ();

                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    coordinates[i] *= divisor;
                }

                return *this;
            }

            Vector normalized () const
            {
                return Vector(*this).normalize ();
            }

        public:

            Number & operator [] (const unsigned index)
            {
                return coordinates[index];
            }

            const Number & operator [] (const unsigned index) const
            {
                return coordinates[index];
            }

        public:

            Vector & operator += (const Vector & other)
            {
                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    this->coordinates[i] += other.coordinates[i];
                }

                return *this;
            }

            Vector & operator -= (const Vector & other)
            {
                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    this->coordinates[i] -= other.coordinates[i];
                }

                return *this;
            }

            Vector operator + (const Vector & other) const
            {
                return Vector(*this) += other;
            }

            Vector operator - (const Vector & other) const
            {
                return Vector(*this) += other;
            }

            const Vector & operator + () const
            {
                return *this;
            }

            Vector operator - () const
            {
                Vector result(*this);

                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    coordinates[i] = -coordinates[i];
                }

                return *this;
            }

        public:

            ENABLE_IF(COORDINATE_SYSTEM == CARTESIAN)
            Number operator * (const Vector & other) const
            {
                Number result = Number(0);

                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    result += this->coordinates[i] * other.coordinates[i];
                }

                return result;
            }

            Vector & operator *= (const Number & number)
            {
                for (unsigned i = 0; i < Coordinates::value_count; ++i)
                {
                    coordinates[i] *= number;
                }

                return *this;
            }

            Vector operator * (const Number & number) const
            {
                return Vector(*this) *= number;
            }

        public:

            bool operator == (const Vector & other) const
            {
                return this->coordinates == other.coordinates;
            }

            bool operator != (const Vector & other) const
            {
                return this->coordinates != other.coordinates;
            }

        public:

            operator Coordinates & ()
            {
                return coordinates;
            }

            operator const Coordinates & () const
            {
                return coordinates;
            }

        };

        typedef Vector< 1,     int,   CARTESIAN > Vector1i;
        typedef Vector< 1,   float,   CARTESIAN > Vector1f;
        typedef Vector< 1,  double,   CARTESIAN > Vector1d;
        typedef Vector< 2,     int,   CARTESIAN > Vector2i;
        typedef Vector< 2,   float,   CARTESIAN > Vector2f;
        typedef Vector< 2,  double,   CARTESIAN > Vector2d;
        typedef Vector< 3,     int,   CARTESIAN > Vector3i;
        typedef Vector< 3,   float,   CARTESIAN > Vector3f;
        typedef Vector< 3,  double,   CARTESIAN > Vector3d;
        typedef Vector< 4,     int,   CARTESIAN > Vector4i;
        typedef Vector< 4,   float,   CARTESIAN > Vector4f;
        typedef Vector< 4,  double,   CARTESIAN > Vector4d;

        typedef Vector< 1,     int,   CARTESIAN > Vector1ic;
        typedef Vector< 1,   float,   CARTESIAN > Vector1fc;
        typedef Vector< 1,  double,   CARTESIAN > Vector1dc;
        typedef Vector< 2,     int,   CARTESIAN > Vector2ic;
        typedef Vector< 2,   float,   CARTESIAN > Vector2fc;
        typedef Vector< 2,  double,   CARTESIAN > Vector2dc;
        typedef Vector< 3,     int,   CARTESIAN > Vector3ic;
        typedef Vector< 3,   float,   CARTESIAN > Vector3fc;
        typedef Vector< 3,  double,   CARTESIAN > Vector3dc;
        typedef Vector< 4,     int,   CARTESIAN > Vector4ic;
        typedef Vector< 4,   float,   CARTESIAN > Vector4fc;
        typedef Vector< 4,  double,   CARTESIAN > Vector4dc;

        typedef Vector< 1,     int, HOMOGENEOUS > Vector1ih;
        typedef Vector< 1,   float, HOMOGENEOUS > Vector1fh;
        typedef Vector< 1,  double, HOMOGENEOUS > Vector1dh;
        typedef Vector< 2,     int, HOMOGENEOUS > Vector2ih;
        typedef Vector< 2,   float, HOMOGENEOUS > Vector2fh;
        typedef Vector< 2,  double, HOMOGENEOUS > Vector2dh;
        typedef Vector< 3,     int, HOMOGENEOUS > Vector3ih;
        typedef Vector< 3,   float, HOMOGENEOUS > Vector3fh;
        typedef Vector< 3,  double, HOMOGENEOUS > Vector3dh;
        typedef Vector< 4,     int, HOMOGENEOUS > Vector4ih;
        typedef Vector< 4,   float, HOMOGENEOUS > Vector4fh;
        typedef Vector< 4,  double, HOMOGENEOUS > Vector4dh;

    }

#endif
