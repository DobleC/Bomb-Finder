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

#include <basics/log> // basics::log.d("message");

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
        { ID(loading),"sprites/loading.png"          },
        { ID(nextround),"sprites/nextround.png"      },
        { ID(gameover),"sprites/gameover.png"        },
        { ID(down),   "sprites/0facedown.png"        },
        { ID(one),    "sprites/1point.png"           },
        { ID(two),    "sprites/2point.png"           },
        { ID(three),  "sprites/3point.png"           },
        { ID(bomb),   "sprites/4bombshadow.png"      },
        { ID(info),   "sprites/5info.png"            },
        { ID(blank),  "sprites/6blank.png"           },
        { ID(pause),  "sprites/7pause.png"           },
        { ID(play),   "sprites/8play.png"            },
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

        // Se inicializan otros atributos:

        initialize ();
    }

    // ---------------------------------------------------------------------------------------------
    // Algunos atributos se inicializan en este método en lugar de hacerlo en el constructor porque
    // este método puede ser llamado más veces para restablecer el estado de la escena y el constructor
    // solo se invoca una vez.

    bool Game_Scene::initialize ()
    {
        // Se inicia la semilla del generador de números aleatorios:
        srand (unsigned(time(nullptr)));

        state     = LOADING;
        suspended = true;
        //gameplay  = UNINITIALIZED;

        for (auto & option : options)
        {
            option.is_pressed = false;
        }

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
            switch (event.id)
            {
                case ID(touch-ended):  // El usuario deja de tocar la pantalla
                {
                    rondaAcabada = false;
                    gameOver = false;
                    scene_x = *event[ID(x)].as< var::Float > ();
                    scene_y = *event[ID(y)].as< var::Float > ();
                    Point2f touched_point{scene_x, scene_y};  // Guarda el último punto tocado

                    for (int i = 0; i < 25; ++i) //Recorre las casillas y sus sprites asociados
                    {
                        // Si el punto está contenido en el sprite && la casilla no ha sido desvelada
                        if(grafico_casillas[i]->contains(touched_point) && !casillas[i]->getDesvelada())
                        {
                            casillas[i]->setDesvelada(true);
                            Id id = check_ID(casillas[i]);
                            Sprite_Handle card(new Sprite(textures[id].get()));
                            card->set_position({casillas[i]->getX(), casillas[i]->getY()});
                            card->set_scale(0.40f);
                            sprites.push_back(card);

                            if(casillas[i]->getValorBomba() == 1)
                            {
                                gameOver = true;
                                state = NEWROUND;
                                timer.reset();
                            }
                            else
                            {
                                controlador.multiplicarPunt(casillas[i]->getValorMultp());
                                if (casillas[i]->getValorMultp() > 1) ++counter;
                            }
                        }

                        if(counter >= tablero.mayoresUno && !gameOver)
                        {
                            rondaAcabada = true;
                            state = NEWROUND;
                            timer.reset();
                        }
                        //else rondaAcabada = false;
                    }

                    if(pausaSpr->contains(touched_point))
                    {
                        playSpr->show();
                        pausaSpr->hide();
                        state = PAUSE;
                    }
                    break;
                }
            }
        }
        else if (state == PAUSE) {
            switch (event.id) {
                case ID(touch-started):         // El usuario toca la pantalla
                case ID(touch-moved):
                {
                    // Se determina qué opción se ha tocado:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int     option_touched = option_at (touch_location);

                    // Solo se puede tocar una opción a la vez (para evitar selecciones múltiples),
                    // por lo que solo una se considera presionada (el resto se "sueltan"):

                    for (int index = 0; index < number_of_options; ++index)
                    {
                        options[index].is_pressed = index == option_touched;
                    }
                    break;
                }

                case ID(touch-ended):           // El usuario deja de tocar la pantalla
                {
                    // Se "sueltan" todas las opciones:

                    for (auto &option : options) option.is_pressed = false;

                    // Se determina qué opción se ha dejado de tocar la última y se actúa como corresponda:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    // if (option_at (touched_point) == PLAY)
                    // {
                    //     director.run_scene (shared_ptr< Scene >(new Game_Scene));
                    // }

                    if (playSpr->contains(touch_location))
                    {
                        playSpr->hide();
                        pausaSpr->show();
                        state = RUNNING;
                    }

                    break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::update (float time)
    {
        if (!suspended) switch (state)
        {
            case LOADING:   load_textures  ();  break;
            case RUNNING:
                check_endCondition();  break;
            case NEWROUND:
                check_endCondition();  break;
            case NEXTROUND:
                check_endCondition();  break;
            case GAMEOVER:
                check_endCondition();  break;
            case PAUSE:     run_pause      ();  break;
            case ERROR:                         break;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::render (Context & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            canvas = context->get_renderer< Canvas > (ID(canvas));

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
                    case LOADING:   render_afterRounds(*canvas); break;
                    case RUNNING:   render_playfield (*canvas);  break;
                    case NEWROUND:  render_playfield (*canvas);  break;
                    case NEXTROUND: render_afterRounds(*canvas); break;
                    case GAMEOVER:  render_afterRounds(*canvas); break;
                    case PAUSE:     render_pause     (*canvas);  break;
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
                blackfont.reset (new Raster_Font("fonts/impact.fnt",      context));
                whitefont.reset (new Raster_Font("fonts/impactwhite.fnt", context));

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
        if (timer.get_elapsed_seconds () > 0.5f)         // Si las texturas se han cargado muy rápido
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
        create_tablero();
        create_info();
    }

    Id Game_Scene::check_ID(Casilla *casilla)
    {
        Id id;

        if(casilla->getValorBomba() == 1) id = ID(bomb);  // Se valora si es una bomba
        else
        {
            switch (casilla->getValorMultp()) // En caso de no ser bomba, se valora su multiplicador
            {
                case 1: id = ID(one  ); break;
                case 2: id = ID(two  ); break;
                case 3: id = ID(three); break;
            }
        }
        return id;
    }

    void Game_Scene::create_tablero ()
    {
        int f = 0;      // #fila
        int c = 0;      // #columna
        int i = 0;      // indice
        int xLoc, yLoc; // Posiciones en X e Y
        int x, y;       // Valores absolutos en X e Y
        Id id;          // Id

        for (; c <= 4; ++c, ++i) // Recorre la matriz fila a fila de izquierda a derecha
        {

            xLoc = f + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = c + 1; //
            x = posXTablero + xLoc * escalar;
            y = posYTablero + yLoc * escalar;


            casillas[i] = &tablero.matrizTablero[f][c]; // Guardar las direcciones de las casillas del tablero
            casillas[i]->setX(x);
            casillas[i]->setY(y);

            //casillas[i]->setDesvelada(true);

            if(!casillas[i]->getDesvelada()) id = ID(down);
            else
            {
                id = check_ID(casillas[i]);
            }

            Sprite_Handle card(new Sprite(textures[id].get()));
            card->set_position({x, y});
            card->set_scale(0.40f);
            grafico_casillas[i] = card.get();
            sprites.push_back(card);

            if (c == 4 && f < 4) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }
    }

    void Game_Scene::create_info ()
    {
        int xLoc = 0;
        int yLoc = 0;
        Id id = ID(info);

        //Columna
        int xCol = 5;
        int yCol = 0;

        //Fila
        int xFil = 0;
        int yFil = -1;

        for (int i = 0; i < 6; ++i, ++yCol, ++xFil)
        {
            //---------------------------------Columna Info---------------------------------------//
            if(i < 5) // Evita crear 2 veces la esquina inferior derecha (casilla blanca)
            {
                xLoc = xCol + 1; // Ajustar las posiciones para multiplicar con ellas
                yLoc = yCol + 1; //

                Sprite_Handle card(new Sprite(textures[id].get()));
                card->set_position({posXTablero + xLoc * escalar, posYTablero + yLoc * escalar});
                card->set_scale(0.40f);
                sprites.push_back(card);
            }

            //-----------------------------------Fila Info----------------------------------------//
            xLoc = xFil + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = yFil + 1; //

            if(i == 5) id = ID(blank);

            Sprite_Handle card(new Sprite(textures[id].get()));
            card->set_position({posXTablero + xLoc * escalar, posYTablero + yLoc * escalar});
            card->set_scale(0.40f);
            sprites.push_back(card);
        }

        id = ID(play);
        Sprite_Handle jugar(new Sprite(textures[id].get()));
        jugar->set_position({1220, 60});
        jugar->set_scale(0.40f);
        spritesPause.push_back(jugar);
        playSpr = jugar.get();
        playSpr->hide();

        id = ID(pause);
        Sprite_Handle pausa(new Sprite(textures[id].get()));
        pausa->set_position({1220, 60});
        pausa->set_scale(0.40f);
        spritesPause.push_back(pausa);
        pausaSpr = pausa.get();

    }

    void Game_Scene::create_text ()
    {
        int xLoc = 0;
        int yLoc = 0;
        int x = 0;
        int y = 0;

        int puntosMargen;       // Márgenes dinámicos para recolocar los números
        int bombasMargen;       // en caso de tener 2 cifras


        wstring puntosCol;      // Texto a escribir
        wstring bombasCol;      //
        wstring puntosFil;      //
        wstring bombasFil;      //

        print_score();

        wstring ronda = to_wstring(controlador.getRound());
        Text_Layout ronda_text(*blackfont, L"R-" +  ronda);
        if (controlador.getRound() > 9) canvas->draw_text({posXTablero + 5.6f * escalar, posYTablero + 0.3f * escalar}, ronda_text);
        else canvas->draw_text({posXTablero + 5.7f * escalar, posYTablero + 0.3f * escalar}, ronda_text);
        //{posXTablero + xLoc * escalar, posYTablero + yLoc * escalar}

        //Columna
        int colX = 5;           // Posiciones números columna
        int colY = 0;           //

        //Fila
        int filX = 0;           // Posiciones números fila
        int filY = -1;          //

        for (int i = 0; i < 5; ++i, ++colY, ++filX)
        {
            //-------------------------------Números Columna--------------------------------------//

            xLoc = colX + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = colY + 1; //

            puntosCol = to_wstring(tablero.puntosColumna[i]);
            bombasCol = to_wstring(tablero.bombasColumna[i]);

            Text_Layout puntos_textCol(*blackfont, puntosCol);
            Text_Layout bombas_textCol(*blackfont, bombasCol);

            if(tablero.puntosColumna[i] > 9) puntosMargen = 3;
            else puntosMargen = 15;

            if(tablero.bombasColumna[i] > 9) bombasMargen = 3;
            else bombasMargen = 15;

            x = puntosMargen + posXTablero + xLoc * escalar;
            y = 57 + posYTablero + yLoc * escalar;
            canvas->draw_text({x, y}, puntos_textCol);

            x = bombasMargen + posXTablero + xLoc * escalar;
            y = 6 + posYTablero + yLoc * escalar;
            canvas->draw_text({x, y}, bombas_textCol);

            //----------------------------------Números Fila--------------------------------------//

            xLoc = filX + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = filY + 1; //

            puntosFil = to_wstring(tablero.puntosFila[i]);
            bombasFil = to_wstring(tablero.bombasFila[i]);

            Text_Layout puntos_textFil(*blackfont, puntosFil);
            Text_Layout bombas_textFil(*blackfont, bombasFil);

            if(tablero.puntosFila[i] > 9) puntosMargen = 3;
            else puntosMargen = 15;

            if(tablero.bombasFila[i] > 9) bombasMargen = 3;
            else bombasMargen = 15;

            x = puntosMargen + posXTablero + xLoc * escalar;
            y = 57 + posYTablero + yLoc * escalar;
            canvas->draw_text({x, y}, puntos_textFil);

            x = bombasMargen + posXTablero + xLoc * escalar;
            y = 6 + posYTablero + yLoc * escalar;
            canvas->draw_text({x, y}, bombas_textFil);
        }
    }

    /*void Game_Scene::save_scores () ///////////////////////
    {
        string path = application.get_internal_data_path () + "/save.data";

        ofstream writer(path, ofstream::binary | ofstream::trunc);

        if (writer)
        {
            size_t number_of_scores = scores.size ();

            writer.write (reinterpret_cast< char * >(&number_of_scores), sizeof(number_of_scores));

            for (auto & item : scores)
            {
                string   player = item.first;
                unsigned score  = item.second;

                size_t   player_length = player.length();

                writer.write (reinterpret_cast< char * >(&player_length), sizeof(player_length));
                writer.write (player.data (), player.length ());
                writer.write (reinterpret_cast< char * >(&score), sizeof(score));
            }
        }
    }


    void Game_Scene::load_scores () /////////////////////
    {
        string path = application.get_internal_data_path () + "/scores.data";

        ifstream reader(path, ofstream::binary);

        if (reader)
        {
            scores.clear ();

            size_t number_of_scores;

            reader.read (reinterpret_cast<char*>(&number_of_scores), sizeof(number_of_scores));

            for (int item = 0; item < number_of_scores; ++item)
            {
                string   player;
                unsigned score;
                size_t   player_length;

                reader.read (reinterpret_cast< char * >(&player_length), sizeof(player_length));

                player.resize (player_length);

                reader.read (&player.front (), player_length);
                reader.read (reinterpret_cast< char * >(&score), sizeof(score));

                if (!reader.fail () && !reader.bad ())
                {
                    scores[player] = score;
                }
                else
                {
                    //basics::log.e ("ERROR READING THE SCORES!");
                    scores.clear();
                    break;
                }
            }
        }
    }*/

    void Game_Scene::check_scores()
    {


    }

    void Game_Scene::next_round()
    {
        rondaAcabada = false;
        counter = 0;
        controlador.siguienteRonda();
        sprites.clear();
        timer.reset();
        state = LOADING;
    }

    void Game_Scene::game_over()
    {
        gameOver = false;
        counter = 0;
        controlador.gameOver();
        sprites.clear();
        timer.reset();
        state = LOADING;

    }


    // ---------------------------------------------------------------------------------------------
    // Cuando el juego se inicia por primera vez o cuando se reinicia porque un jugador pierde, se
    // llama a este método para restablecer la posición y velocidad de los sprites:

    void Game_Scene::restart_game()
    {
        Tablero tab;
        tablero = tab;

        if(!controlador)
        {
            Controlador cont;
            controlador = cont;
        }

        posXTablero = canvas_width / 5;

        //gameplay = WAITING_TO_START;
    }


    void Game_Scene::check_endCondition ()
    {
        if(rondaAcabada)
        {
            if (timer.get_elapsed_seconds () > 1.f) state = NEXTROUND;
            if (timer.get_elapsed_seconds () > 2.f) next_round();
        }
        if(gameOver)
        {
            if (timer.get_elapsed_seconds () > 1.f) state = GAMEOVER;
            if (timer.get_elapsed_seconds () > 2.f) game_over();
        }
    }

    void Game_Scene::render_afterRounds(Canvas & canvas)
    {
        Id id;

        switch (state)
        {
            case LOADING:   id = ID(loading);     break;
            case NEXTROUND: id = ID(nextround);   break;
            case GAMEOVER:  id = ID(gameover);    break;
        }

        Texture_2D * loading_texture = textures[id].get ();

        if (loading_texture)
        {
            canvas.fill_rectangle
            (
                { canvas_width * .5f, canvas_height * .5f },
                { loading_texture->get_width (), loading_texture->get_height () }, loading_texture
            );

            if(state == NEXTROUND || (controlador.getRound() > 1 && state == LOADING)) print_score();
        }
    }

    void Game_Scene::print_score()
    {
        wstring puntos = to_wstring(controlador.getScore());
        Text_Layout puntos_text(*whitefont, L"Score\n" + puntos);
        canvas->draw_text({1000, 490}, puntos_text);

        wstring puntosTotales = to_wstring(controlador.getTotalScore());
        Text_Layout totales_text(*whitefont, L"Total Score\n" + puntosTotales);
        canvas->draw_text({1000, 350}, totales_text);
    }

    // ---------------------------------------------------------------------------------------------
    // Simplemente se dibujan todos los sprites que conforman la escena.

    void Game_Scene::render_playfield (Canvas & canvas)
    {
        canvas.draw_segment ({    0,   1 }, { 1280,   1 });     // Borde inferior
        canvas.draw_segment ({    0, 720 }, { 1280, 720 });     // Borde superior
        canvas.draw_segment ({    1,   0 }, {    1, 720 });     // Borde izquierdo
        canvas.draw_segment ({ 1280,   0 }, { 1280, 720 });     // Borde derecho


        canvas.draw_segment ({    0,   360 }, { 1280/4,   360 });           // Medio derecha
        canvas.draw_segment ({    1280 * 0.75,   360 }, { 1280,   360 });   // Medio izquierda

        for (auto & sprite : sprites)      sprite->render (canvas);
        for (auto & sprite : spritesPause) sprite->render (canvas);

        create_text();
    }

    void Game_Scene::render_pause (Canvas & canvas)
    {
        // Se dibuja el slice de cada una de las opciones del menú:

        for (auto & option : options)
        {
            canvas.set_transform
                    (
                            scale_then_translate_2d
                                    (
                                            option.is_pressed ? 0.75f : 1.f,                     // Escala de la opción
                                            { option.position[0], option.position[1] }      // Traslación
                                    )
                    );

            canvas.fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
        }

        // Se restablece la transformación aplicada a las opciones para que no afecte a
        // dibujos posteriores realizados con el mismo canvas:

        canvas.set_transform (Transformation2f());


        for (auto & sprite : spritesPause) sprite->render (canvas);

    }

    void Game_Scene::run_pause ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context();

            if (context) {
                // Se carga el atlas:

                atlas.reset(new Atlas("menu-scene/pause-menu.sprites", context));

                // Si el atlas está disponible, se inicializan los datos de las opciones del menú:

                if (atlas->good())  configure_options();
            }
        }
    }

    void Game_Scene::configure_options ()
    {
        // Se asigna un slice del atlas a cada opción del menú según su ID:

        options[EXIT   ].slice = atlas->get_slice(ID(exit)   );
        options[CREDITS].slice = atlas->get_slice(ID(credits));
        options[HELP   ].slice = atlas->get_slice(ID(help)   );
        options[SCORES ].slice = atlas->get_slice(ID(scores) );

        // Se calcula la altura total del menú:

        float pause_height = 0;

        for (auto & option : options) pause_height += option.slice->height;

        // Se calcula la posición del borde superior del menú en su conjunto de modo que
        // quede centrado verticalmente:

        float option_top = canvas_height / 2.f + pause_height / 2.f;

        // Se establece la posición del borde superior de cada opción:

        for (unsigned index = 0; index < number_of_options; ++index) {
            options[index].position = Point2f{canvas_width / 2.f, option_top};

            option_top -= options[index].slice->height;
        }

        // Se restablece la presión de cada opción en initialize

    }

    int Game_Scene::option_at (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
            (
                point[0] > option.position[0] - option.slice->width  &&
                point[0] < option.position[0] + option.slice->width  &&
                point[1] > option.position[1] - option.slice->height &&
                point[1] < option.position[1] + option.slice->height
            )
            {
                return index;
            }
        }

        return -1;
    }

}


// ---------------------------------------------------------------------------------------------
// Se hace que el player dechero se mueva hacia arriba o hacia abajo según el usuario esté
// tocando la pantalla por encima o por debajo de su centro. Cuando el usuario no toca la
// pantalla se deja al player quieto.

//void Game_Scene::update_user () // Ejemplo intersects
// {
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
// }