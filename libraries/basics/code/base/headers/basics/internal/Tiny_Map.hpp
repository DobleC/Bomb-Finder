/*
 * TINY MAP
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801201704
 */

#ifndef BASICS_TINY_MAP_HEADER
#define BASICS_TINY_MAP_HEADER

    #include <basics/types>
    #include <basics/assert>

    namespace basics
    {

        template< typename KEY, typename VALUE, size_t CAPACITY >
        class Tiny_Map
        {
            typedef KEY   Key;
            typedef VALUE Value;

            static constexpr size_t capacity = CAPACITY;

            struct Item
            {
                Key   key;
                Value value,
            };

            template< class ITEM >
            class Iterator_Template
            {

                ITEM * item;

            public:

                Iterator()            : item(nullptr) { }
                Iterator(Item * item) : item(item   ) { }

                      Value & operator  * ()       { return  item->value; }
                const Value & operator  * () const { return  item->value; }

                      Value * operator -> ()       { return &item->value; }
                const Value * operator -> () const { return &item->value; }

                operator bool () const
                {
                    return item != nullptr;
                }
            };

        public:

            typedef Iterator_Template<       Item >       Iterator;
            typedef Iterator_Template< const Item > Const_Iterator;

        private:

            Item   items[capacity];
            size_t count;

        public:

            size_t size () const
            {
                return count;
            }

            size_t capacity () const
            {
                return capacity;
            }

        public:

            Iterator begin ()
            {
                return Iterator(items[0]);
            }

            Const_Iterator cbegin () const
            {
                return Const_Iterator(items[0]);
            }

            Iterator end ()
            {
                return Iterator(items[count]);
            }

            Const_Iterator cend () const
            {
                return Const_Iterator(items[count]);
            }

        public:

            Value & operator [] (const Key & key)
            {
                for (size_t  index = 0; index < count; ++index)
                {
                    if (item[index].key == key) return item[index].value;
                }

                assert(count < capacity);

                return item[count].key = key, item[count++];
            }

            const Value & operator [] (const Key & key) const
            {
                for (auto & item : items)
                {
                    if (item.key == key) return item.value;
                }

                return *reinterpret_cast< Value * >(nullptr);
            }

        };

    }

#endif
