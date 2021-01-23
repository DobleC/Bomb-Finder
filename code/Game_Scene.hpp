/*
 * GAME SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef GAME_SCENE_HEADER
#define GAME_SCENE_HEADER

    #include <map>
    #include <list>
    #include <memory>

    #include <basics/Application>
    #include <basics/Canvas>
    #include <basics/Id>
    #include <basics/Scene>
    #include <basics/Texture_2D>
    #include <basics/Timer>
    #include <basics/Raster_Font>
    #include <iostream>
    #include <fstream>

    #include "Sprite.hpp"
    #include "controlador.hpp" // Incluye modelo



using namespace graphics;
using namespace model;
using namespace controller;
using namespace std;


    namespace game
    {

        using basics::Id;
        using basics::Timer;
        using basics::Canvas;
        using basics::Texture_2D;

        class Game_Scene : public basics::Scene
        {

            // Estos typedefs pueden ayudar a hacer el código más compacto y claro:

            typedef shared_ptr < Tablero            >  Tablero_Handle;
            typedef map< string, unsigned           >  Scores_Handle;
            typedef unique_ptr< basics::Raster_Font >  Font_Handle;
            typedef shared_ptr < Sprite             >  Sprite_Handle;
            typedef list< Sprite_Handle             >  Sprite_List;
            typedef shared_ptr< Texture_2D          >  Texture_Handle;
            typedef map< Id, Texture_Handle         >  Texture_Map;
            typedef basics::Graphics_Context::Accessor Context;

            /**
             * Representa el estado de la escena en su conjunto.
             */
            enum State
            {
                LOADING,
                RUNNING,
                NEXTROUND,
                ERROR
            };

            /**
             * Representa el estado del juego cuando el estado de la escena es RUNNING.
             */
            enum Gameplay_State
            {
                UNINITIALIZED,
                WAITING_TO_START,
                PLAYING,
            };

        private:

            /**
             * Array de estructuras con la información de las texturas (Id y ruta) que hay que cargar.
             */
            static struct   Texture_Data { Id id; const char * path; } textures_data[];

            /**
             * Número de items que hay en el array textures_data.
             */
            static unsigned textures_count;

        private:

            State               state;                    ///< Estado de la escena.
            Gameplay_State      gameplay;                 ///< Estado del juego cuando la escena está RUNNING.
            bool                suspended;                ///< true cuando la escena está en segundo plano y viceversa.
            bool                rondaAcabada = false;

            unsigned            canvas_width;             ///< Ancho de la resolución virtual usada para dibujar.
            unsigned            canvas_height;            ///< Alto  de la resolución virtual usada para dibujar.

            float               scene_x;                  ///< Denota la coordenada X de donde pulsa el jugador
            float               scene_y;                  ///< Denota la coordenada X de donde pulsa el jugador

            Texture_Map         textures;                 ///< Mapa  en el que se guardan shared_ptr a las texturas cargadas.
            Sprite_List         sprites;                  ///< Lista en la que se guardan shared_ptr a los sprites creados.
            Font_Handle         blackfont;                ///< Fuente para escribir por pantalla
            Font_Handle         whitefont;                ///< Fuente para escribir por pantalla en blanco

            unsigned            posXTablero = 256;        ///< Anchura para colocar en horizontal el tablero.
            const unsigned      posYTablero =  85;        ///< Altura para colocar en vertical el tablero
            const unsigned      escalar     = 110;        ///< Distancia entre casillas

            Casilla             *casillas[25];            ///< Guarda los punteros a las 25 casillas
            Sprite              *grafico_casillas[25];    ///< Guarda punteros a los sprites de las casillas
            Canvas              *canvas;                  ///< Guarda puntero al canvas
            Tablero             tablero;                  ///< Guarda la información del tablero
            Controlador         controlador;
            unsigned            counter = 0;                  ///< Cuenta los multiplicadores desvelados que son >1
            string              player_name;              ///< Nombre del player
            Scores_Handle       scores;                   ///< Record del player
            Timer               timer;                    ///< Cronómetro usado para medir intervalos de tiempo

        public:

            /**
             * Solo inicializa los atributos que deben estar inicializados la primera vez, cuando se
             * crea la escena desde cero.
             */
            Game_Scene();

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
            void suspend () override;

            /**
             * Este método lo invoca Director automáticamente cuando el juego pasa a primer plano.
             */
            void resume () override;

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
            void render (Context & context) override;

        private:

            /**
            * En este método crea el tablero cuando termina la carga de texturas.
            */
            void create_tablero ();

            /**
             * En este método crea la información del tablero cuando termina su construcción.
             */
            void create_info ();

            /**
             * En este método crea la información del tablero cuando termina su construcción.
             */
            void create_text ();

            /**
             * Este método avanza a la siguiente ronda
             */
            void next_round ();

            /**
             * Este método guarda las 10 mejores puntuaciones del usuario en un archivo binario
             */
            void save_scores ();

            /**
             * Este método carga las puntuaciones de un archivo binario
             */
            void load_scores ();

            /**
             * Este método gestiona el array de mejores puntuaciones
             */
            void check_scores ();

            /**
             * Comprueba la información de una casilla para asociarle una ID de textura.
             */
            Id check_ID(Casilla *casilla);

            /**
             * En este método se cargan las texturas (una cada fotograma para facilitar que la
             * propia carga se pueda pausar cuando la aplicación pasa a segundo plano).
             */
            void load_textures ();

            /**
             * En este método se crean los sprites cuando termina la carga de texturas.
             */
            void create_sprites ();

            /**
             * Se llama cada vez que se debe reiniciar el juego. En concreto la primera vez y cada
             * vez que un jugador pierde.
             */
            void restart_game ();

            /**
             * Cuando se ha reiniciado el juego y el usuario toca la pantalla por primera vez se
             * pone la bola en movimiento en una dirección al azar.
             */
            void start_playing ();

            /**
             * Actualiza el estado del juego cuando el estado de la escena es RUNNING.
             */
            void run_simulation (float time);

            /**
             * Hace que el player derecho se mueva hacia el punto de la pantalla que toca el usuario.
             */
            void update_user ();



            /**
             * Dibuja la textura con el mensaje de carga mientras el estado de la escena es LOADING.
             * La textura con el mensaje se carga la primera para mostrar el mensaje cuanto antes.
             * @param canvas Referencia al Canvas con el que dibujar la textura.
             */
            void render_loading (Canvas & canvas);

            /**
             * Dibuja la escena de juego cuando el estado de la escena es RUNNING.
             * @param canvas Referencia al Canvas con el que dibujar.
             */
            void render_playfield (Canvas & canvas);


        };

    }

#endif
