/*
 * EVENT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712171705
 */

#ifndef BASICS_EVENT_HEADER
#define BASICS_EVENT_HEADER

    #include <map>
    #include <basics/fnv>
    #include <basics/Id>
    #include <basics/Var>

    namespace basics
    {

        struct Event
        {
        public:

            typedef std::map< Id, Var > Property_List;      // REEMPLAZAR POR UN MAPA "LIGERO"

        public:

            Id            id;
            int           priority;
            Property_List properties;

        public:

            Event(Id id = 0) : id(id), priority(0)
            {
            }

            Var & operator [] (const Id & id)
            {
                return properties[id];
            }

            bool operator < (const Event & other) const
            {
                return this->priority < other.priority;
            }

        };

    }

#endif
