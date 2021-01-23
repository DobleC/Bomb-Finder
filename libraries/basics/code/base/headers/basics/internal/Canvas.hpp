/*
 * CANVAS
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801161230
 */

#ifndef BASICS_CANVAS_HEADER
#define BASICS_CANVAS_HEADER

    #include <basics/Atlas>
    #include <basics/Graphics_Context>
    #include <basics/Point>
    #include <basics/Renderer>
    #include <basics/Size>
    #include <basics/Text_Layout>
    #include <basics/Texture_2D>
    #include <basics/Transformation>

    namespace basics
    {

        enum Anchor
        {
            TOP    = 4,
            LEFT   = 1,
            CENTER = 0,
            RIGHT  = 2,
            BOTTOM = 8,
        };

        enum
        {
            FLIP_HORIZONTAL = 16,
            FLIP_VERTICAL   = 32,
        };

        struct Canvas : public Renderer
        {
        public:

            enum Blending
            {
                NONE,
                TRANSPARENCY,
                MULTIPLY,
                ADD
            };

            struct Options
            {
                Size2u size;
            };

        public:

            typedef Canvas * (* Factory) (Id id, Graphics_Context::Accessor & context, const Options & options);

        private:

            static Id      canvas_specialization_ids      [10];
            static Factory canvas_specialization_factories[10];
            static size_t  canvas_specialization_count;

        protected:

            static void register_factory (Id id, Factory factory)
            {
                canvas_specialization_ids      [canvas_specialization_count] = id;
                canvas_specialization_factories[canvas_specialization_count] = factory;
                canvas_specialization_count++;
            }

        public:

            static Canvas * create (Id id, Graphics_Context::Accessor & context, const Options & options);

        protected:

            virtual ~Canvas() = default;

        public:

            virtual void reset_state     () { }

        public:

            virtual void set_size        (const Size2u & size) { }

        public:

            virtual void set_clear_color (float r, float g, float b) { }
            virtual void set_color       (float r, float g, float b) { }
            virtual void set_opacity     (float opacity) { }
            virtual void set_blending    (Blending blending) { }
            virtual void set_transform   (const Transformation2f & transform) { }
            virtual void apply_transform (const Transformation2f & transform) { }

        public:

            virtual void clear           () { }

            virtual void draw_point      (const Point2f & position) { }
            virtual void draw_segment    (const Point2f & a, const Point2f & b) { }
            virtual void draw_triangle   (const Point2f & a, const Point2f & b, const Point2f & c) { }
            virtual void draw_rectangle  (const Point2f & bottom_left, const Size2f & size) { }

            virtual void fill_triangle   (const Point2f & a, const Point2f & b, const Point2f & c) { }
            virtual void fill_rectangle  (const Point2f & bottom_left, const Size2f & size) { }
            virtual void fill_rectangle  (const Point2f & where, const Size2f & size, const Texture_2D   * texture, int handling = CENTER) { }
            virtual void fill_rectangle  (const Point2f & where, const Size2f & size, const Atlas::Slice * slice,   int handling = CENTER) { }
            virtual void fill_circle     (const Point2f & center, float radius) { }

            virtual void draw_text       (const Point2f & where, const Text_Layout & text_layout, int handling = TOP | LEFT);

        };

    }

#endif
