/*
 * EVENT QUEUE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712171705
 */

#ifndef BASICS_EVENT_QUEUE_HEADER
#define BASICS_EVENT_QUEUE_HEADER

    #include <queue>
    #include <mutex>
    #include <basics/Event>

    namespace basics
    {

        class Event_Queue
        {

            std::queue< Event > queue;
            std::mutex          mutex;

        public:

            void clear ()
            {
                std::queue< Event >().swap (queue);
            }

            void push (const Event & event)
            {
                std::lock_guard< std::mutex > lock(mutex);

                queue.push (event);
            }

            void push (Event && event)
            {
                std::lock_guard< std::mutex > lock(mutex);

                queue.push (event);
            }

            bool poll (Event & event)
            {
                std::lock_guard< std::mutex > lock(mutex);

                if (queue.size () > 0)
                {
                    event = queue.front ();

                    queue.pop ();

                    return true;
                }

                return false;
            }

            bool peek (Event & event)
            {
                std::lock_guard< std::mutex > lock(mutex);

                if (queue.size () > 0)
                {
                    event = queue.front ();

                    return true;
                }

                return false;
            }

        };

    }

#endif
