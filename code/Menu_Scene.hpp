/*
 * MENU SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 * Modified by Carlos Casado (Copyright © 2020+)
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 * rexitah@gmail.com
 */

#ifndef MENU_SCENE_HEADER
#define MENU_SCENE_HEADER

    #include <memory>
    #include <map>
    #include <basics/Atlas>
    #include <basics/Canvas>
    #include <basics/Id>
    #include <basics/Point>
    #include <basics/Scene>
    #include <basics/Size>
    #include <basics/Timer>
    #include <basics/Texture_2D>
    #include "Sprite.hpp"

    namespace menu
    {

        using namespace basics;
        //using basics::Atlas;
        //using basics::Timer;
        //using basics::Canvas;
        //using basics::Point2f;
        //using basics::Size2f;
        //using basics::Texture_2D;
        //using basics::Id;
        //using basics::Graphics_Context;

        class Menu_Scene : public basics::Scene
        {

            typedef std::shared_ptr < graphics::Sprite  > Sprite_Handle;
            typedef std::list       < Sprite_Handle     > Sprite_List;
            typedef std::shared_ptr < Texture_2D        > Texture_Handle;
            typedef std::map        < Id, Texture_Handle> Texture_Map;

            /**
             * Representa el estado de la escena en su conjunto.
             */
            enum State
            {
                LOADING,
                READY,
                SEENSCORE,
                SEENHELP,
                SEENCREDITS,
                ERROR
            };

            enum Option_Id
            {
                PLAY,
                HELP,
                SCORES,
                CREDITS,
            };

            struct Option
            {
                const Atlas::Slice * slice;
                Point2f position;
                float   is_pressed;
            };

            static struct   Texture_Data { Id id; const char * path; } textures_data[];

            static const unsigned number_of_options = 4;        ///< Número de opciones
            static const unsigned nScore = 9;                   ///< Número de highscores que se guardan

        private:

            State    state;                                     ///< Estado de la escena.
            bool     suspended;                                 ///< true cuando la escena está en segundo plano y viceversa.
            bool     scoresLoaded = false;                      ///< Verifica si se han cargado las puntuaciones guardadas

            Canvas * canvas;                                    ///< Puntero al canvas
            unsigned canvas_width;                              ///< Ancho de la resolución virtual usada para dibujar.
            unsigned canvas_height;                             ///< Alto  de la resolución virtual usada para dibujar.

            Option   options[number_of_options];                ///< Datos de las opciones del menú
            std::unique_ptr< Atlas > atlas;                     ///< Atlas que contiene las imágenes de las opciones del menú

            std::unique_ptr< basics::Raster_Font > whitefont;   ///< Fuente para escribir por pantalla en blanco
            unsigned highscores[nScore];                        ///< Colección de records del player

            Texture_Handle        gobackTexture;                ///< Texturas y sprites usados en el menú
            Texture_Handle        helpTexture;                  ///<
            Texture_Handle        creditsTexture;               ///<
            graphics::Sprite      *gobackSpr;                   ///<
            graphics::Sprite      *helpSpr;                     ///<
            graphics::Sprite      *creditsSpr;                  ///<
            Sprite_List           sprites;                      ///< Lista en la que se guardan shared_ptr a los sprites creados.

        public:

            Menu_Scene();

            /**
             * Este método lo llama Director para conocer la resolución virtual con la que está
             * trabajando la escena.
             * @return Tamaño en coordenadas virtuales que está usando la escena.
             */
            basics::Size2u get_view_size () override
            {
                return { canvas_width, canvas_height };
            }

            /**
             * Aquí se inicializan los atributos que deben restablecerse cada vez que se inicia la escena.
             * @return
             */
            bool initialize () override;

            /**
             * Este método lo invoca Director automáticamente cuando el juego pasa a segundo plano.
             */
            void suspend () override
            {
                suspended = true;
            }

            /**
             * Este método lo invoca Director automáticamente cuando el juego pasa a primer plano.
             */
            void resume () override
            {
                suspended = false;
            }

            /**
             * Este método se invoca automáticamente una vez por fotograma cuando se acumulan
             * eventos dirigidos a la escena.
             */
            void handle (basics::Event & event) override;

            /**
             * Este método se invoca automáticamente una vez por fotograma para que la escena
             * actualize su estado.
             */
            void update (float time) override;

            /**
             * Este método se invoca automáticamente una vez por fotograma para que la escena
             * dibuje su contenido.
             */
            void render (Graphics_Context::Accessor & context) override;

        private:

            /**
             * Establece las propiedades de cada opción si se ha podido cargar el atlas.
             */
            void configure_options ();

            /**
             * Devuelve el índice de la opción que se encuentra bajo el punto indicado.
             * @param point Punto que se usará para determinar qué opción tiene debajo.
             * @return Índice de la opción que está debajo del punto o -1 si no hay alguna.
             */
            int option_at (const Point2f & point);

            /**
             * Carga las scores guardadas por la game_scene
             */
            void load_scores();

            /**
             * Printea los records del jugador
             */
            void print_scores();

        };

    }

#endif
