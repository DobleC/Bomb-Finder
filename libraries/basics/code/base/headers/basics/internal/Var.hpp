/*
 * VAR
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712190125
 */

#ifndef BASICS_VAR_HEADER
#define BASICS_VAR_HEADER

    #include <basics/Id>

    namespace basics
    {

        class Var final
        {
        public:

            class Type
            {
            public:

                struct Info
                {
                    const Id            id;
                    const char        * name;
                    const void (Type::* clear) ();
                    // * DEFAULT VALUE? SERÍA DE TIPO "Type"
                    // * TABLA DE PUNTEROS A FUNCIONES DE CONVERSIÓN A TIPOS BÁSICOS (posiblemente es mejor usar tipos de C++ que Var::Type)
                };


            protected:

                      byte   blob[8];
                const Info * info;

            public:

                Type(const Info * const info) : info(info)
                {
                }

               ~Type()
                {
                    if (info->clear) (this->*(info->clear)) ();
                }

            public:

                const Info & type_info () const
                {
                    return *info;
                }

            protected:

                template< typename TYPE >
                TYPE & data ()
                {
                    static_assert(sizeof(TYPE) <= sizeof(blob), "basics::Var:Type::data() error: TYPE is bigger than blob.");

                    return reinterpret_cast< TYPE & >(*blob);
                }

                template< typename TYPE >
                const TYPE & data () const
                {
                    return const_cast< Type * >(this)->data< TYPE > ();
                }

            };

        public:

            template< typename TYPE >
            struct Conversion
            {
                typedef TYPE Type;

                Type value;
                bool ok;

                operator       Type & ()       { return value; }
                operator const Type & () const { return value; }
            };

        private:

            Type value;

        public:

            Var();

            template< typename TYPE >
            bool is () const
            {
                return value.type_info ().id == TYPE::id;
            }

            template< typename TYPE >
            TYPE * as ()
            {
                return value.type_info ().id == TYPE::id ? static_cast< TYPE * >(&value) : nullptr;
            }

            // AL CONTRARIO QUE EL MÉTODO AS(), EL MÉTODO TO() REALIZA CONVERSIÓN ENTRE TIPOS.
            // UNA PLANTILLA INDEX_OF<TYPE> DEVOLVERÍA EL ÍNDICE EN LA TABLA DE CONVERSIÓN DE UN TIPO
            // CUALQUIERA EN TIEMPO DE COMPILACIÓN. SI NO EXISTE EL ÍNDICE O SI LA ENTRADA EN DICHO
            // ÍNDICE ES NULA, SE DEVUELVE UN VALOR POR DEFECTO Y OK A FALSE. EN OTRO CASO, SE INVOCA
            // A LA FUNCIÓN DE CONVERSIÓN.
            template< typename TYPE >
            Conversion< TYPE > to ();

            template< typename TYPE >
            Var & operator = (const TYPE & new_value)
            {
                return value = new_value, *this;
            }

        };

        // -----------------------------------------------------------------------------------------

        namespace var
        {

            class Void : public Var::Type
            {
            public:

                static constexpr Id   id = ID(basics::var::Void);
                static const     Info info;

            public:

                Void() : Type(&info)
                {
                }

            };

            // -------------------------------------------------------------------------------------

            class Bool : public Var::Type
            {
            public:

                static constexpr Id   id = ID(basics::var::Bool);
                static const     Info info;

            public:

                Bool() : Type(&info)
                {
                }

                Bool(bool x) : Type(&info)
                {
                    *this = x;
                }

                Bool & operator = (const bool value)
                {
                    return data< bool > () = value, *this;
                }

                operator const bool & () const
                {
                    return data< bool > ();
                }
            };

            // -------------------------------------------------------------------------------------

            class Int32 : public Var::Type
            {
            public:

                static constexpr Id   id = ID(basics::var::Int32);
                static const     Info info;

            public:

                Int32() : Type(&info)
                {
                }

                Int32(int32_t x) : Type(&info)
                {
                    *this = x;
                }

                Int32 & operator = (const int32_t value)
                {
                    return data< int32_t > () = value, *this;
                }

                operator const int32_t & () const
                {
                    return data< int32_t > ();
                }
            };

            // -------------------------------------------------------------------------------------

            class Float : public Var::Type
            {
            public:

                static constexpr Id   id = ID(basics::var::Float);
                static const     Info info;

            public:

                Float() : Type(&info)
                {
                }

                Float(float x) : Type(&info)
                {
                    *this = x;
                }

                Float & operator = (const float value)
                {
                    return data< float > () = value, *this;
                }

                operator const float & () const
                {
                    return data< float > ();
                }
            };

            // -------------------------------------------------------------------------------------

            // Tipos simples:

            class Void;
            class Bool;
            class Byte;
            class Word;
            class Char;
            class WChar;
            class Int;
            class Unsigned;
            class Int8;
            class Int16;
            class Int32;
            class Int64;
            class UInt8;
            class UInt16;
            class UInt32;
            class UInt64;
            class Float;
            class Double;

            // Tipos derivados:

            class Pointer;              // template< typename T > class Pointer : public Type...

            // Tipos complejos:

            class String;
            class Array;
            class Map;

        }

        // -----------------------------------------------------------------------------------------

        inline Var::Var() : value(var::Void())
        {
        }

        // -----------------------------------------------------------------------------------------

        template< > inline Var & Var::operator = < bool    > (const bool    & x) { return value = var::Bool (x), *this; }
        template< > inline Var & Var::operator = < int32_t > (const int32_t & x) { return value = var::Int32(x), *this; }
        template< > inline Var & Var::operator = < float   > (const float   & x) { return value = var::Float(x), *this; }

    }

#endif
