/*
 * TYPES
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1407160000
 */

#ifndef BASICS_TYPES_HEADER
#define BASICS_TYPES_HEADER

    #include <cstddef>
    #include <cstdint>
    #include <basics/macros>
    #include <basics/Non_Instantiable>

    namespace basics
    {

   /* --------------------------------------------------------------------------------------------- +
                                          Define byte and word
    + --------------------------------------------------------------------------------------------- */

        typedef uint8_t byte;

        #if   BASICS_WORD_SIZE == 4

            typedef uint32_t word;

        #elif BASICS_WORD_SIZE == 8

            typedef uint64_t word;

        #else

            #error "basics++: unsupported word size."

        #endif

   /* --------------------------------------------------------------------------------------------- +
                                              Trinary logic
    + --------------------------------------------------------------------------------------------- */

    constexpr class unknown_t {} unknown;

    class trilean
    {
        int value;

    public:

        trilean() = default;
        trilean(unknown_t ) : value(   -1) { }
        trilean(bool value) : value(value) { }

        operator bool ()  const { return value == true; }
        operator int  ()  const { return value;         }

        bool operator == (const trilean & other) const  { return this->value == other.value; }
        bool operator != (const trilean & other) const  { return this->value != other.value; }
    };

   /* --------------------------------------------------------------------------------------------- +
                                      Define chars of specific sizes
    + --------------------------------------------------------------------------------------------- */

        typedef uint_least8_t char8_t;                      // Complements char16_t and char32_t

   /* --------------------------------------------------------------------------------------------- +
                              Define floating point types of specific sizes
    + --------------------------------------------------------------------------------------------- */

        typedef float  float32_t;
        typedef double float64_t;

   /* --------------------------------------------------------------------------------------------- +
                Define abstraction class templates by size for various fundamental types
    + --------------------------------------------------------------------------------------------- */

        template< unsigned BITS > struct Sized_Type;

        template< > struct Sized_Type< 8 >  final : Non_Instantiable
        {
            typedef   char8_t  char_t;
            typedef    int8_t   int_t;
            typedef   uint8_t  uint_t;
        };

        template< > struct Sized_Type< 16 > final : Non_Instantiable
        {
            typedef  char16_t  char_t;
            typedef   int16_t   int_t;
            typedef  uint16_t  uint_t;
        };

        template< > struct Sized_Type< 32 > final : Non_Instantiable
        {
            typedef  char32_t  char_t;
            typedef   int32_t   int_t;
            typedef  uint32_t  uint_t;
            typedef float32_t float_t;
        };

        template< > struct Sized_Type< 64 > final : Non_Instantiable
        {
            typedef   int64_t   int_t;
            typedef  uint64_t  uint_t;
            typedef float64_t float_t;
        };

    }

   /* --------------------------------------------------------------------------------------------- +
                         Export some basics identifiers into the global namespace
    + --------------------------------------------------------------------------------------------- */

    #if not defined(BASICS_DONT_EXPORT_TYPES_TO_GLOBAL_NAMESPACE)

        using basics::byte;
        using basics::word;
        using basics::char8_t;
        using basics::float32_t;
        using basics::float64_t;

    #endif

#endif
