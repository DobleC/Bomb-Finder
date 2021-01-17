/*
 *  MATRIX
 *  Copyright © 2012+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_MATRIX_HEADER
#define BASICS_MATRIX_HEADER

    #include <algorithm>

    namespace basics
    {

        template< unsigned M, unsigned N, typename NUMERIC_TYPE >
        class Matrix
        {
        public:

            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            static  constexpr unsigned number_of_rows    = M;
            static  constexpr unsigned number_of_columns = N;

        public:

            class Row
            {
            private:

                Number * const values;

            public:

                Row(Matrix * const given_matrix, const unsigned row_index)
                :
                    values(given_matrix->values + row_index * N)
                {
                }

                Number & operator [] (const unsigned column_index)
                {
                    return values[column_index];
                }

                const Number & operator [] (const unsigned column_index) const
                {
                    return values[column_index];
                }

                Row & operator = (const Row & other)
                {
                    for (unsigned index = 0; index < N; ++index)
                    {
                        this->values[index] = other.values[index];
                    }

                    return *this;
                }

                template< unsigned OTHER_N >
                Row & operator = (const typename Matrix< N, OTHER_N, Number >::Column & other_column)
                {
                    for (unsigned index = 0; index < N; ++index)
                    {
                        this->values[index] = other_column.values[index * N];
                    }

                    return *this;
                }

            };

            class Column
            {
            private:

                Number * const values;

            public:

                Column(Matrix * const given_matrix, const unsigned column_index)
                :
                    values(given_matrix->values + column_index)
                {
                }

                Number & operator [] (const unsigned row_index)
                {
                    return values[row_index * N];
                }

                const Number & operator [] (const unsigned row_index) const
                {
                    return values[row_index * N];
                }

                Column & operator = (const Column & other)
                {
                    for (unsigned index = 0; index < M; ++index)
                    {
                        this->values[index * N] = other.values[index * N];
                    }

                    return *this;
                }

                template< unsigned OTHER_M >
                Column & operator = (const typename Matrix< OTHER_M, M, Number >::Row & other_row)
                {
                    for (unsigned index = 0; index < M; ++index)
                    {
                        this->values[index * N] = other_row.values[index];
                    }

                    return *this;
                }

            };

        public:

            static const Matrix identity;

        public:

            Number values[M * N];

        public:

            Matrix() = default;
            Matrix(const Matrix & ) = default;

        private:

            enum Identity
            {
                IDENTITY
            };

            Matrix(const Identity & )
            {
                std::fill_n (values, M * N, Number(0));

                for (unsigned offset = 0; offset < M * N; offset += N + 1)
                {
                    values[offset] = Number(1);
                }
            }

        public:

            Row row (const unsigned row_index)
            {
                return Row(this, row_index);
            }

            const Row row (const unsigned row_index) const
            {
                return Row(this, row_index);
            }

            Column column (const unsigned column_index)
            {
                return Column(this, column_index);
            }

            const Column column (const unsigned column_index) const
            {
                return Column(this, column_index);
            }

            Row operator [] (const unsigned row_index)
            {
                return Row(this, row_index);
            }

            const Row operator [] (const unsigned row_index) const
            {
                return Row(const_cast< Matrix * >(this), row_index);
            }

        public:

            template< unsigned P >
            const Matrix< M, P, Numeric_Type > operator * (const Matrix< N, P, Numeric_Type > & other) const
            {
                Matrix< M, P, Numeric_Type > result;

                for (unsigned r = M; r-- > 0; )
                {
                    for (unsigned c = P; c-- > 0; )
                    {
                        Numeric_Type total = Numeric_Type(0);

                        for (unsigned index = N; index-- > 0; )
                        {
                            total += (*this)[r][index] * other[index][c];
                        }

                        result[r][c] = total;
                    }
                }

                return result;
            }

            template< unsigned A,  unsigned B >
            bool operator == (const Matrix< A, B, Numeric_Type > & other) const
            {
                return M == A && N == B && std::equal (this->values, this->values + M * N, other.values);
            }

            template< unsigned A,  unsigned B >
            bool operator != (const Matrix< A, B, Numeric_Type > & other) const
            {
                return !(*this == other);
            }

        };

        // -----------------------------------------------------------------------------------------

        template< unsigned M, unsigned N, typename NUMERIC_TYPE >
        const Matrix< M, N, NUMERIC_TYPE > Matrix< M, N, NUMERIC_TYPE >::identity(IDENTITY);

        template< typename  NUMERIC_TYPE >
        class Matrix< 0, 0, NUMERIC_TYPE >;

        // -----------------------------------------------------------------------------------------

        typedef Matrix< 2, 2,    int > Matrix22i;
        typedef Matrix< 2, 2,  float > Matrix22f;
        typedef Matrix< 2, 2, double > Matrix22d;

        typedef Matrix< 3, 3,    int > Matrix33i;
        typedef Matrix< 3, 3,  float > Matrix33f;
        typedef Matrix< 3, 3, double > Matrix33d;

        typedef Matrix< 4, 4,    int > Matrix44i;
        typedef Matrix< 4, 4,  float > Matrix44f;
        typedef Matrix< 4, 4, double > Matrix44d;

        typedef Matrix< 2, 1,    int > Matrix21i;
        typedef Matrix< 2, 1,  float > Matrix21f;
        typedef Matrix< 2, 1, double > Matrix21d;

        typedef Matrix< 3, 1,    int > Matrix31i;
        typedef Matrix< 3, 1,  float > Matrix31f;
        typedef Matrix< 3, 1, double > Matrix31d;

        typedef Matrix< 4, 1,    int > Matrix41i;
        typedef Matrix< 4, 1,  float > Matrix41f;
        typedef Matrix< 4, 1, double > Matrix41d;

    }

#endif
