/*
 *  POINT
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_POINT_HEADER
#define BASICS_POINT_HEADER

    #include "Coordinates.hpp"

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE = float, Coordinate_System COORDINATE_SYSTEM = CARTESIAN >
        class Point
        {
        public:

            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            typedef Coordinates< DIMENSION, NUMERIC_TYPE, COORDINATE_SYSTEM > Coordinates;

        public:

            Coordinates coordinates;

        public:

            Point() = default;
            Point(const Point & other) = default;

            template< typename... PARAMETERS >
            Point(const PARAMETERS &... parameters) : coordinates(parameters...)
            {
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

            bool operator == (const Point & other) const
            {
                return this->coordinates == other.coordinates;
            }

            bool operator != (const Point & other) const
            {
                return this->coordinates != other.coordinates;
            }

            operator Coordinates & ()
            {
                return coordinates;
            }

            operator const Coordinates & () const
            {
                return coordinates;
            }

        };

        typedef Point< 1,      int,   CARTESIAN > Point1i;
        typedef Point< 1,    float,   CARTESIAN > Point1f;
        typedef Point< 1,   double,   CARTESIAN > Point1d;
        typedef Point< 2,      int,   CARTESIAN > Point2i;
        typedef Point< 2, unsigned,   CARTESIAN > Point2u;
        typedef Point< 2,    float,   CARTESIAN > Point2f;
        typedef Point< 2,   double,   CARTESIAN > Point2d;
        typedef Point< 3,      int,   CARTESIAN > Point3i;
        typedef Point< 3,    float,   CARTESIAN > Point3f;
        typedef Point< 3,   double,   CARTESIAN > Point3d;
        typedef Point< 4,      int,   CARTESIAN > Point4i;
        typedef Point< 4,    float,   CARTESIAN > Point4f;
        typedef Point< 4,   double,   CARTESIAN > Point4d;

        typedef Point< 1,     int,   CARTESIAN > Point1ic;
        typedef Point< 1,   float,   CARTESIAN > Point1fc;
        typedef Point< 1,  double,   CARTESIAN > Point1dc;
        typedef Point< 2,     int,   CARTESIAN > Point2ic;
        typedef Point< 2,   float,   CARTESIAN > Point2fc;
        typedef Point< 2,  double,   CARTESIAN > Point2dc;
        typedef Point< 3,     int,   CARTESIAN > Point3ic;
        typedef Point< 3,   float,   CARTESIAN > Point3fc;
        typedef Point< 3,  double,   CARTESIAN > Point3dc;
        typedef Point< 4,     int,   CARTESIAN > Point4ic;
        typedef Point< 4,   float,   CARTESIAN > Point4fc;
        typedef Point< 4,  double,   CARTESIAN > Point4dc;

        typedef Point< 1,     int, HOMOGENEOUS > Point1ih;
        typedef Point< 1,   float, HOMOGENEOUS > Point1fh;
        typedef Point< 1,  double, HOMOGENEOUS > Point1dh;
        typedef Point< 2,     int, HOMOGENEOUS > Point2ih;
        typedef Point< 2,   float, HOMOGENEOUS > Point2fh;
        typedef Point< 2,  double, HOMOGENEOUS > Point2dh;
        typedef Point< 3,     int, HOMOGENEOUS > Point3ih;
        typedef Point< 3,   float, HOMOGENEOUS > Point3fh;
        typedef Point< 3,  double, HOMOGENEOUS > Point3dh;
        typedef Point< 4,     int, HOMOGENEOUS > Point4ih;
        typedef Point< 4,   float, HOMOGENEOUS > Point4fh;
        typedef Point< 4,  double, HOMOGENEOUS > Point4dh;

    }

#endif
