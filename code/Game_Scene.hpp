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
    #include <basics/Atlas>
    #include <basics/Audio_Player>
    #include <iostream>
    #include <fstream>

    #include "Sprite.hpp"
    #include "Menu_Scene.hpp"
    #include "controlador.hpp" // Incluye modelo



using namespace graphics;
using namespace model;
using namespace controller;
using namespace std;
using namespace basics;


    namespace game
    {

        //using basics::Id;
        //using basics::Timer;
        //using basics::Canvas;
        //using basics::Texture_2D;
        //using basics::Atlas;

        class Game_Scene : public basics::Scene
        {

            // Estos typedefs pueden ayudar a hacer el código más compacto y claro:

            typedef unique_ptr< basics::Raster_Font >  Font_Handle;
            typedef shared_ptr < Sprite             >  Sprite_Handle;
            typedef list< Sprite_Handle             >  Sprite_List;
            typedef shared_ptr< Texture_2D          >  Texture_Handle;
            typedef map< Id, Texture_Handle         >  Texture_Map;
            typedef unique_ptr< Atlas               >  Atlas_Handle;
            typedef Audio_Player::Playback_Controller_Ptr   Walkman;
            typedef Graphics_Context::Accessor              Context;

            /**
             * Representa el estado de la escena en su conjunto.
             */
            enum State
            {
                LOADING,
                RUNNING,
                NEWROUND,
                NEXTROUND,
                GAMEOVER,
                PAUSE,
                EXITING,
                ERROR
            };

            /**
             * Opciones del menú de pausa.
             */
            enum Option_Id
            {
                HELP,
                SCORES,
                CREDITS,
                EXIT,
            };

            /**
             * Estructura opción.
             */
            struct Option
            {
                const Atlas::Slice * slice;
                Point2f position;
                float   is_pressed;
            };



        private:

            /**
             * Array de estructuras con la información de las texturas (Id y ruta) que hay que cargar.
             */
            static struct   Texture_Data { Id id; const char * path; } textures_data[];

            static unsigned        textures_count;         ///< Número de items que hay en el array textures_data.
            static const unsigned  nOptions = 4;           ///< Número de opciones en el menú de pausa
            static const unsigned  nScore = 9;             ///< Número de highscores que se guardan
            static const unsigned  nCasillas = 25;         ///< Número de casillas del tablero
            static constexpr float scaleCards = 0.40f;     ///< Número de casillas del tablero

            State               state;                     ///< Estado de la escena.
            bool                suspended;                 ///< True cuando la escena está en segundo plano y viceversa.
            bool                rondaAcabada    = false;   ///< Cuando se vuelve true, se acaba la ronda y se pasa a la siguiente.
            bool                gameOver        = false;   ///< Cuando se vuelve true, se acaba la partida y se reinicia
            bool                seenScores      = false;
            bool                seenHelp        = false;
            bool                seenCredits     = false;

            unsigned            canvas_width;              ///< Ancho de la resolución virtual usada para dibujar.
            unsigned            canvas_height;             ///< Alto  de la resolución virtual usada para dibujar.

            float               initial_x;                 ///< Denota la coordenada X de donde pulsa el jugador
            float               initial_y;                 ///< Denota la coordenada Y de donde pulsa el jugador
            Point2f             initial_point;
            float               final_x;                   ///< Denota la coordenada X de donde deja de pulsar el jugador
            float               final_y;                   ///< Denota la coordenada Y de donde deja de pulsar el jugador
            Point2f             final_point;

            Texture_Map         textures;                  ///< Mapa  en el que se guardan shared_ptr a las texturas cargadas.
            Sprite_List         sprites;                   ///< Lista en la que se guardan shared_ptr a los sprites creados.
            Sprite_List         spritesPause;              ///< Lista en la que se guardan shared_ptr a los sprites creados.
            Font_Handle         blackfont;                 ///< Fuente para escribir por pantalla
            Font_Handle         whitefont;                 ///< Fuente para escribir por pantalla en blanco

            unsigned            posXTablero = 256;         ///< Anchura para colocar en horizontal el tablero.
            const unsigned      posYTablero =  85;         ///< Altura para colocar en vertical el tablero
            const unsigned      escalar     = 110;         ///< Distancia entre casillas

            Casilla             *casillas[nCasillas];      ///< Guarda los punteros a las 25 casillas
            Sprite              *casillasSpr[nCasillas];   ///< Guarda punteros a los sprites de las casillas
            Sprite              *warnsSpr[nCasillas];      ///< Guarda punteros a los sprites de los cuadrados rojos
            Sprite              *pausaSpr;
            Sprite              *playSpr;
            Sprite              *gobackSpr;
            Canvas              *canvas;                   ///< Guarda puntero al canvas
            Tablero             tablero;                   ///< Guarda la información del tablero
            Controlador         controlador;               ///< Guarda el controlador del juego
            unsigned            counter = 0;               ///< Cuenta los multiplicadores desvelados que son >1

            unsigned            current_score = 0;         ///< Record del player actual
            unsigned            highscores[nScore];        ///< Colección de records del player
            Timer               timer;                     ///< Cronómetro usado para medir intervalos de tiempo
            Walkman             walkman;                   ///< Reproductor de audio

            Option              options[nOptions];         ///< Datos de las opciones del menú de pausa
            Atlas_Handle        atlas;                     ///< Atlas que contiene las imágenes de las opciones de pausa

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
             * Aquí se guarda la partida si el jugador fuerza el cerrado de la aplicación
             */
            void finalize () override;

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
             * En este método crea las casillas de información del tablero cuando termina su construcción.
             */
            void create_info ();

            /**
             * En este método crea y actualiza los textos del juego (números y las scores).
             */
            void create_text ();

            /**
             * Desvela una carta al ser tocada
             */
            void unveil_card();

            /**
             * Comprueba que el usuario no esté mantenido pulsada la misma casilla, en cuyo caso
             * crea un sprite rojo encima de ella para marcarla como casilla no segura
             * @param Índice para comprobar en que casilla se encuentra
             * @return true si se han cumplido los requerimientos para marcar la casilla.
             */
            bool set_warn(int i);

            /**
             * En este gestiona las pulsaciones en el menú de pausa
             */
            void click_menu(Point2f touched_point);

            /**
             * Este método avanza a la siguiente ronda
             */
            void next_round ();

            /**
             * Este método termina la partida
             */
            void game_over ();

            /**
             * Este método restablece los valores entre rondas / tras un gameover
             */
            void clear_round ();

            /**
             * Este método guarda las 10 mejores puntuaciones del usuario en un archivo binario
             */
            void save_scores ();

            /**
             * Este método carga las puntuaciones de un archivo binario
             */
            void load_scores ();

            /**
             * Este método gestiona el array de mejores puntuaciones para introducir nuevas high scores
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
             * Actualiza el estado del juego cuando el jugador acaba una ronda
             */
            void check_endCondition ();

            /**
             * Printea la score por pantalla
             */
            void print_scores ();

            /**
             * Dibuja la textura con el mensaje de carga mientras el estado de la escena es LOADING.
             * Dibuja la textura con el mensaje de nextround mientras el estado de la escena es NEXTROUND.
             * Dibuja la textura con el mensaje de gameover mientras el estado de la escena es GAMEOVER.
             * @param canvas Referencia al Canvas con el que dibujar la textura.
             */
            void render_afterRounds (Canvas & canvas);

            /**
             * Dibuja la escena de juego cuando el estado de la escena es RUNNING.
             * @param canvas Referencia al Canvas con el que dibujar.
             */
            void render_playfield (Canvas & canvas);

            /**
            * Dibuja la escena de juego cuando el estado de la escena es PAUSE.
            * @param canvas Referencia al Canvas con el que dibujar.
            */
            void render_pause (Canvas & canvas);

            /**
            * Genera el atlas cuando el estado de la escena es PAUSE.
            */
            void run_pause ();

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

        };

    }

#endif
