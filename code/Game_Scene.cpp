/*
 * GAME SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Game_Scene.hpp"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;
using namespace model;

namespace game
{
    // ---------------------------------------------------------------------------------------------
    // ID y ruta de las texturas que se deben cargar para esta escena. La textura con el mensaje de
    // carga está la primera para poder dibujarla cuanto antes:

    Game_Scene::Texture_Data Game_Scene::textures_data[] =
    {
        { ID(loading),"high/sprites/loading.png"          },
        { ID(down),   "high/sprites/0facedown.png"        },
        { ID(one),    "high/sprites/1point.png"           },
        { ID(two),    "high/sprites/2point.png"           },
        { ID(three),  "high/sprites/3point.png"           },
        { ID(bomb),   "high/sprites/4bombshadow.png"      },
    };

    // Pâra determinar el número de items en el array textures_data, se divide el tamaño en bytes
    // del array completo entre el tamaño en bytes de un item:

    unsigned Game_Scene::textures_count = sizeof(textures_data) / sizeof(Texture_Data);

    // ---------------------------------------------------------------------------------------------
    // Definiciones de los atributos estáticos de la clase:

    //constexpr float Game_Scene::  ball_speed;

    // ---------------------------------------------------------------------------------------------

    Game_Scene::Game_Scene()
    {
        // Se establece la resolución virtual (independiente de la resolución virtual del dispositivo).
        // En este caso no se hace ajuste de aspect ratio, por lo que puede haber distorsión cuando
        // el aspect ratio real de la pantalla del dispositivo es distinto.

        canvas_width  = 1280;
        canvas_height =  720;

        // Se inicia la semilla del generador de números aleatorios:

        srand (unsigned(time(nullptr)));

        // Se inicializan otros atributos:

        initialize ();
    }

    // ---------------------------------------------------------------------------------------------
    // Algunos atributos se inicializan en este método en lugar de hacerlo en el constructor porque
    // este método puede ser llamado más veces para restablecer el estado de la escena y el constructor
    // solo se invoca una vez.

    bool Game_Scene::initialize ()
    {
        state     = LOADING;
        suspended = true;
        gameplay  = UNINITIALIZED;

        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::suspend ()
    {
        suspended = true;               // Se marca que la escena ha pasado a primer plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::resume ()
    {
        suspended = false;              // Se marca que la escena ha pasado a segundo plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::handle (Event & event) // Ejemplos de touch
    {
        if (state == RUNNING)               // Se descartan los eventos cuando la escena está LOADING
        {
            if (gameplay == WAITING_TO_START)
            {
                //start_playing ();           // Se empieza a jugar cuando el usuario toca la pantalla por primera vez
            }
            else switch (event.id)
            {
                //case ID(touch-started):     // El usuario toca la pantalla
                //case ID(touch-moved):
                //{
                //    user_target_y = *event[ID(y)].as< var::Float > ();
                //    follow_target = true;
                //    break;
                //}
//
                //case ID(touch-ended):       // El usuario deja de tocar la pantalla
                //{
                //    follow_target = false;
                //    break;
                //}
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::update (float time)
    {
        if (!suspended) switch (state)
        {
            case LOADING: load_textures  ();     break;
            case RUNNING: run_simulation (time); break;
            case ERROR:   break;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::render (Context & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();

                switch (state)
                {
                    case LOADING: render_loading   (*canvas); break;
                    case RUNNING: render_playfield (*canvas); break;
                    case ERROR:   break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    // En este método solo se carga una textura por fotograma para poder pausar la carga si el
    // juego pasa a segundo plano inesperadamente. Otro aspecto interesante es que la carga no
    // comienza hasta que la escena se inicia para así tener la posibilidad de mostrar al usuario
    // que la carga está en curso en lugar de tener una pantalla en negro que no responde durante
    // un tiempo.

    void Game_Scene::load_textures ()
    {
        if (textures.size () < textures_count)          // Si quedan texturas por cargar...
        {
            // Las texturas se cargan y se suben al contexto gráfico, por lo que es necesario disponer
            // de uno:

            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                // Se carga la siguiente textura (textures.size() indica cuántas llevamos cargadas):

                Texture_Data   & texture_data = textures_data[textures.size ()];
                Texture_Handle & texture      = textures[texture_data.id] = Texture_2D::create (texture_data.id, context, texture_data.path);

                // Se comprueba si la textura se ha podido cargar correctamente:

                if (texture) context->add (texture); else state = ERROR;

                // Cuando se han terminado de cargar todas las texturas se pueden crear los sprites que
                // las usarán e iniciar el juego:
            }
        }
        else
        if (timer.get_elapsed_seconds () > 1.f)         // Si las texturas se han cargado muy rápido
        {                                               // se espera un segundo desde el inicio de
            restart_game   ();                          // la carga antes de pasar al juego para que
            create_sprites ();                          // el mensaje de carga no aparezca y desaparezca
                                                        // demasiado rápido.

            state = RUNNING;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::create_sprites ()
    {
        int f = 0; // #fila
        int c = 0; // #columna
        int i = 0; // indice
        int xLoc, yLoc; // Posiciones en X e Y

        for (; c <= 4; ++c, ++i) // Recorre la matriz fila a fila de izquierda a derecha
        {
            xLoc = f + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = c + 1; //
            casillas[i] = &tablero.matrizTablero[f][c]; // Guardar las direcciones de las casillas del tablero


            if(!casillas[i]->getDesvelada())
            {
                Sprite_Handle card(new Sprite(textures[ID(down)].get()));
                card->set_position({50 + xLoc * 110, 0 + yLoc * 110});
                card->set_scale(0.4f);
                grafico_casillas[i] = card.get();
                sprites.push_back(card);
            }


            if (c == 4 && f < 4) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }

    }



    // ---------------------------------------------------------------------------------------------
    // Cuando el juego se inicia por primera vez o cuando se reinicia porque un jugador pierde, se
    // llama a este método para restablecer la posición y velocidad de los sprites:

    void Game_Scene::restart_game()
    {
        Tablero tab;
        tablero = tab;
        tablero.calcDatos();

        gameplay = WAITING_TO_START;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::start_playing ()
    {
        // Se genera un vector de dirección al azar:

        Vector2f random_direction
        (
            float(rand () % int(canvas_width ) - int(canvas_width  / 2)),
            float(rand () % int(canvas_height) - int(canvas_height / 2))
        );

        // Se hace unitario el vector y se multiplica un el valor de velocidad para que el vector
        // resultante tenga exactamente esa longitud:

        //ball->set_speed (random_direction.normalized () * ball_speed);

        gameplay = PLAYING;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::run_simulation (float time)
    {
        // Se actualiza el estado de todos los sprites:

        for (auto & sprite : sprites)
        {
            sprite->update (time);
        }

        //update_user ();

    }


    // ---------------------------------------------------------------------------------------------
    // Se hace que el player dechero se mueva hacia arriba o hacia abajo según el usuario esté
    // tocando la pantalla por encima o por debajo de su centro. Cuando el usuario no toca la
    // pantalla se deja al player quieto.

    void Game_Scene::update_user () // Ejemplo intersects
    {
      //  if (right_player->intersects (*top_border))
      //  {
      //      // Si el player está tocando el borde superior, no puede ascender:
//
      //      right_player->set_position_y (top_border->get_bottom_y () - right_player->get_height () / 2.f);
      //      right_player->set_speed_y (0);
      //  }
      //  else
      //  if (right_player->intersects (*bottom_border))
      //  {
      //      // Si el player está tocando el borde inferior, no puede descender:
//
      //      right_player->set_position_y (bottom_border->get_top_y () + right_player->get_height () / 2.f);
      //      right_player->set_speed_y (0);
      //  }
      //  else
      //  if (follow_target)
      //  {
      //      // Si el usuario está tocando la pantalla, se determina si está tocando por encima o por
      //      // debajo de su centro para establecer si tiene que subir o bajar:
//
      //      float delta_y = user_target_y - right_player->get_position_y ();
//
      //      if (delta_y < 0.f) right_player->set_speed_y (-player_speed); else
      //      if (delta_y > 0.f) right_player->set_speed_y (+player_speed);
      //  }
      //  else
      //      right_player->set_speed_y (0);
    }

    void Game_Scene::render_loading (Canvas & canvas)
    {
        Texture_2D * loading_texture = textures[ID(loading)].get ();

        if (loading_texture)
        {
            canvas.fill_rectangle
            (
                { canvas_width * .5f, canvas_height * .5f },
                { loading_texture->get_width (), loading_texture->get_height () },
                  loading_texture
            );
        }
    }

    // ---------------------------------------------------------------------------------------------
    // Simplemente se dibujan todos los sprites que conforman la escena.

    void Game_Scene::render_playfield (Canvas & canvas)
    {
        for (auto & sprite : sprites)
        {
            sprite->render (canvas);
        }
    }
}

/*
 *  if (casillas[i]->getValorBomba() == 1)
 *          {
 *              Sprite_Handle card(new Sprite(textures[ID(bomb)].get()));
 *              card->set_position({50 + xLoc * 110, 0 + yLoc * 110});
 *              card->set_scale(0.4f);
 *              grafico_casillas[i] = card.get();
 *              sprites.push_back(card);
 *          }
 *          else if (casillas[i]->getValorMultp() == 1)
 *          {
 *              Sprite_Handle card(new Sprite(textures[ID(one)].get()));
 *              card->set_position({50 +xLoc * 110, 0 + yLoc * 110});
 *              card->set_scale(0.4f);
 *              grafico_casillas[i] = card.get();
 *              sprites.push_back(card);
 *          }
 *          else if (casillas[i]->getValorMultp() == 2)
 *          {
 *              Sprite_Handle card(new Sprite(textures[ID(two)].get()));
 *              card->set_position({50 + xLoc * 110, 0 + yLoc * 110});
 *              card->set_scale(0.4f);
 *              grafico_casillas[i] = card.get();
 *              sprites.push_back(card);
 *          }
 *          else if (casillas[i]->getValorMultp() == 3)
 *          {
 *              Sprite_Handle card(new Sprite(textures[ID(three)].get()));
 *              card->set_position({50 + xLoc * 110, 0 + yLoc * 110});
 *              card->set_scale(0.4f);
 *              grafico_casillas[i] = card.get();
 *              sprites.push_back(card);
 *          }

            ----------------------------------------------------------------------------------------------

            void Game_Scene::traverse_tablero(Tablero & tablero) {

        int f = 0; // #fila
        int c = 0; // #columna
        int i = 0; // indice
        for (; c <= 4; ++c, ++i) // Recorre la matriz fila a fila de izquierda a derecha
        {
            casillas[i] = &tablero.matrizTablero[f][c]; // Guarda las direcciones de las casillas del tablero

            if (c == 4 && f < 4) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }
    }

 */
