/*
 * CANVAS ES 2
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712200139
 */

#ifndef BASICS_OPENGLES_CANVAS_ES2_HEADER
#define BASICS_OPENGLES_CANVAS_ES2_HEADER

    #include <memory>
    #include <basics/Canvas>
    #include <basics/Transformation>

    namespace basics { namespace opengles
    {

        class Shader_Program;

        class Canvas_ES2 : public basics::Canvas
        {
        private:

            static const char * internal_vertex_shader_f;
            static const char * internal_vertex_shader_t;
            static const char * internal_fragment_shader_f;
            static const char * internal_fragment_shader_t;

        public:

            static Canvas * create (Id id, Graphics_Context::Accessor & context, const Options & options);

        public:

            static void enable ()
            {
                register_factory (ID(opengles2), Canvas_ES2::create);
            }

        private:

            Size2f size;
            Size2f half_size;

            Transformation2f transform;
            Transformation2f projection;

            std::shared_ptr< Shader_Program > shader_program_f;
            std::shared_ptr< Shader_Program > shader_program_t;

            int  transform_f_id;
            int projection_f_id;
            int      color_f_id;
            int    opacity_f_id;
            int  transform_t_id;
            int projection_t_id;
            int    sampler_t_id;
            int    opacity_t_id;

            unsigned   vertex_position_location_f;
            unsigned   vertex_position_location_t;
            unsigned vertex_texture_uv_location_t;

        public:

            Canvas_ES2(Graphics_Context::Accessor & context, const Size2u & viewport_size);

        public:

            void reset_state     () override;

        public:

            void set_size        (const Size2u & size) override;

        public:

            void set_clear_color (float r, float g, float b) override;
            void set_color       (float r, float g, float b) override;
            void set_opacity     (float opacity) override;
            void set_transform   (const Transformation2f & transform) override;
            void apply_transform (const Transformation2f & transform) override;

        public:

            void clear           () override;
            void draw_point      (const Point2f & position) override;
            void draw_segment    (const Point2f & a, const Point2f & b) override;
            void draw_triangle   (const Point2f & a, const Point2f & b, const Point2f & c) override;
            void fill_triangle   (const Point2f & a, const Point2f & b, const Point2f & c) override;
            void draw_rectangle  (const Point2f & bottom_left, const Size2f & size) override;
            void fill_rectangle  (const Point2f & bottom_left, const Size2f & size) override;
            void fill_rectangle  (const Point2f & where, const Size2f & size, const basics::Texture_2D * texture, int handling = CENTER) override;
            void fill_rectangle  (const Point2f & where, const Size2f & size, const Atlas::Slice * slice, int handling = CENTER) override;
            void fill_circle     (const Point2f & center, float radius) override;

        };

    }}

#endif
