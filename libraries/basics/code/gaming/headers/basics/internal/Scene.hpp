/*
 *  SCENE
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801072242
 */

#ifndef BASICS_SCENE_HEADER
#define BASICS_SCENE_HEADER

    #include <basics/Event>
    #include <basics/Graphics_Context>
    #include <basics/Size>

    namespace basics
    {

        class Scene
        {
        private:

            float frame_duration;

        public:

            Scene()
            {
                frame_duration = -1.f;
            }

            virtual ~Scene() = default;

        public:

            virtual bool initialize () { return true; }
            virtual void suspend    () { }
            virtual void resume     () { }
            virtual void finalize   () { }

            virtual void handle     (Event & event) { }
            virtual void update     (float time) { }
            virtual void render     (Graphics_Context::Accessor & context) { }

            virtual Size2u get_view_size () = 0;

        public:

            bool set_frame_rate (int fps)
            {
                return fps > 0 ? frame_duration = 1.f / float(fps), true : false;
            }

            float get_frame_duration () const
            {
                return frame_duration;
            }

        };

    }

#endif
