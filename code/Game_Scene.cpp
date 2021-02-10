/*
 * GAME SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 * Modified by Carlos Casado (Copyright © 2020+)
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 * rexitah@gmail.com
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
        { ID(loading),  "sprites/loading.png"        },
        { ID(nextround),"sprites/nextround.png"      },
        { ID(exiting),  "sprites/exiting.png"        },
        { ID(gameover), "sprites/gameover.png"       },
        { ID(help),     "menu-scene/help.png"        },
        { ID(credits),  "menu-scene/credits.png"     },
        { ID(down),     "sprites/0facedown.png"      },
        { ID(one),      "sprites/1point.png"         },
        { ID(two),      "sprites/2point.png"         },
        { ID(three),    "sprites/3point.png"         },
        { ID(bomb),     "sprites/4bomb.png"          },
        { ID(info),     "sprites/5info.png"          },
        { ID(blank),    "sprites/6blank.png"         },
        { ID(pause),    "sprites/7pause.png"         },
        { ID(play),     "sprites/8play.png"          },
        { ID(back),     "sprites/9back.png"          },
        { ID(red),      "sprites/10red.png"          },
    };

    // Para determinar el número de items en el array textures_data, se divide el tamaño en bytes
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

        posXTablero = canvas_width / 5;

        // Inicializa a falso el menú de pausa
        for (auto & option : options) option.is_pressed = false;

        // Carga las puntuaciones anteriores
        load_scores();
        //check_scores();

        walkman = audio_player.play ("sounds/song1.ogg", true);
        walkman ->stop();

        return true;
    }

    void Game_Scene::finalize ()
    {
        walkman ->stop();
        //save_scores();
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::suspend ()
    {
        walkman->pause ();
        suspended = true;               // Se marca que la escena ha pasado a primer plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::resume ()
    {
        if (walkman->get_status () != Audio_Player::Playback_Controller::STOPPED) walkman->play ();
        suspended = false;              // Se marca que la escena ha pasado a segundo plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::handle (Event & event)
    {
        if (state == RUNNING)  // Se descartan los eventos cuando la escena no está en RUNNING
        {
            switch (event.id)
            {
                case ID(touch-started):
                {
                    initial_x = *event[ID(x)].as< var::Float > ();
                    initial_y = *event[ID(y)].as< var::Float > ();
                    initial_point = {initial_x, initial_y};

                    timer.reset();
                    break;
                }

                case ID(touch-ended):  // El usuario deja de tocar la pantalla
                {
                    //rondaAcabada = false;
                    //gameOver = false;
                    final_x = *event[ID(x)].as< var::Float > ();
                    final_y = *event[ID(y)].as< var::Float > ();
                    final_point = {final_x, final_y};  // Guarda el último punto tocado

                    //Se desvela la carta de haber sido tocada una

                    unveil_card();

                    if(pausaSpr->contains(final_point))
                    {
                        playSpr ->show();
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

                    for (int index = 0; index < nOptions; ++index)
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

                    //Se gestiona la pulsación del user
                    click_menu(touch_location);

                    break;
                }
            }
        }
    }

    void Game_Scene::unveil_card ()
    {
        for (int i = 0; i < nCasillas; ++i) //Recorre las casillas y sus sprites asociados
        {
            // Si el punto está contenido en el sprite && la casilla no ha sido desvelada
            if(casillasSpr[i]->contains(final_point) && !casillas[i]->getDesvelada())
            {
                // Si el usuario no mantiene pulsado para marcar la casilla como peligrosa
                if(!set_warn(i))
                {
                    //"Desvela" la casilla
                    casillas[i]->setDesvelada(true);
                    Id id = check_ID(casillas[i]);
                    Sprite_Handle card(new Sprite(textures[id].get()));
                    card->set_position({casillas[i]->getX(), casillas[i]->getY()});
                    card->set_scale(scaleCards);
                    casillasSpr[i] ->hide();      // Oculta el gráfico de carta bocaabajo
                    casillasSpr[i] = card.get(); // Guarda el puntero a la nueva carta
                    sprites.push_back(card);


                    // Si la casilla es una bomba, activa gameOver para que termine la partida
                    if(casillas[i]->getValorBomba() == 1)
                    {
                        gameOver = true;
                        state = NEWROUND;
                        timer.reset();
                    }
                    else
                    {
                        // Obtiene el valor del multiplicador de la casilla y aumenta contador si es <1
                        controlador.multiplicarPunt(casillas[i]->getValorMultp());
                        if (casillas[i]->getValorMultp() > 1) ++counter;
                    }
                    current_score = controlador.getTotalScore() + controlador.getScore();

                    if(counter >= tablero.mayoresUno)// Si ha desvelado todos los 2 y 3, pasa a la siguiente ronda
                    {
                        rondaAcabada = true;
                        state = NEWROUND;
                        timer.reset();
                    }
                }
            }
        }
    }

    bool Game_Scene::set_warn(int i)
    {
        // Si la casilla lleva más de 0.2s pulsada
        if(casillasSpr[i]->contains(initial_point) && timer.get_elapsed_seconds () > 0.2f)
        {
            // Si la casilla no ha sido marcada
            if(casillas[i]->getMarcada() == false)
            {
                casillas[i]->setMarcada(true);
                if (warnsSpr[i] == nullptr) // Si no hay un sprite creado, lo crea
                {
                    Sprite_Handle warn(new Sprite(textures[ID(red)].get()));
                    warn->set_position({casillas[i]->getX() - 25, casillas[i]->getY() + 25});
                    warn->set_scale(scaleCards * 0.3f);
                    warnsSpr[i] = warn.get();  // Guarda referencia al warn en el mismo índice en el que está la casilla
                    sprites.push_back(warn);
                }
                else warnsSpr[i]->show(); // Reutiliza el sprite si ya lo había creado
            }
            else // Si ya está marcada, esconde el sprite al mantener pulsado, por si el usuario cambia de opinión
            {
                casillas[i]->setMarcada(false);
                warnsSpr[i]->hide();
            }
            return true; // Ha puesto (o ocultado) un warn
        }
        return false;   // No ha puesto un warn
    }

    void Game_Scene::click_menu (Point2f touch_location)
    {
        if(!seenScores && !seenHelp && !seenCredits) // Si no está en ningún submenu de la pausa
        {
            // Si está en pausa y toca algo que no sea EXIT
            if (option_at(touch_location) > -1 && option_at(touch_location) < 3)
            {
                playSpr  ->hide();
                pausaSpr ->hide();
                gobackSpr->show();
            }

            // Si está en pausa y toca SCORES
            if      (option_at(touch_location) == SCORES)  seenScores  = true;

            // Si está en pausa y toca HELP
            else if (option_at(touch_location) == HELP)    seenHelp    = true;

            // Si está en pausa y toca CREDITS
            else if (option_at(touch_location) == CREDITS) seenCredits = true;

            // Si está en pausa y toca EXIT
            else if(option_at(touch_location) == EXIT)
            {
                save_scores();
                state = EXITING;
                director.run_scene (shared_ptr< Scene >(new menu::Menu_Scene));
            }

        }

        if (playSpr->contains(touch_location))
        {
            if(!seenScores && !seenHelp && !seenCredits) // Si no está en ningún submenu de la pausa vuelve al juego
            {
                playSpr  ->hide();
                pausaSpr ->show();
                gobackSpr->hide();
                state = RUNNING;
            }
            else
            {
                if      (seenScores)  seenScores  = false; // Sale de SCORES
                else if (seenHelp)    seenHelp    = false; // Sale de HELP
                else if (seenCredits) seenCredits = false; // Sale de CREDITS
                playSpr  ->show();
                pausaSpr ->hide();
                gobackSpr->hide();

            }
        }
    }


    // ---------------------------------------------------------------------------------------------

    void Game_Scene::update (float time)
    {
        if (!suspended) switch (state)
        {
            case LOADING:   load_textures     ();  break;
            case RUNNING:   check_endCondition();  break;
            case NEWROUND:  check_endCondition();  break;
            case NEXTROUND: check_endCondition();  break;
            case GAMEOVER:  check_endCondition();  break;
            case PAUSE:     run_pause         ();  break;
            case EXITING:                          break;
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
                    case RUNNING:   render_playfield ( *canvas); break;
                    case NEWROUND:  render_playfield ( *canvas); break;
                    case NEXTROUND: render_afterRounds(*canvas); break;
                    case GAMEOVER:  render_afterRounds(*canvas); break;
                    case PAUSE:     render_pause     ( *canvas); break;
                    case EXITING:   render_afterRounds(*canvas); break;
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
        if (timer.get_elapsed_seconds () > 0.5f)
        {
            //Si las texturas se han cargado muy rápido se espera desde el inicio de la carga
            // antes de pasar al juego para que el mensaje de carga no aparezca y desaparezca demasiado rápido.
            restart_game   ();
            create_sprites ();
            state = RUNNING;
            if (walkman->get_status () == Audio_Player::Playback_Controller::STOPPED) walkman->play ();
        }
    }


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
            casillas[i]->setX(x); // Setea la x de la casilla para cuando tenga que invocarse algo en su posición
            casillas[i]->setY(y); // Setea la y de la casilla para cuando tenga que invocarse algo en su posición

            // No es necesario pero comprueba el id de las casillas y si han sido desveladas antes de crearlas
            // Es útil para hacer pruebas y desvelar el tablero durante estas
            if(!casillas[i]->getDesvelada()) id = ID(down);
            else id = check_ID(casillas[i]);

            // Genera la carta boca abajo y guarda una referencia a esta
            Sprite_Handle card(new Sprite(textures[id].get()));
            card->set_position({x, y});
            card->set_scale(scaleCards);
            casillasSpr[i] = card.get();
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

        // Se crean las 10 casillas de información que envuelven el tablero y la de ronda
        for (int i = 0; i < 6; ++i, ++yCol, ++xFil)
        {
            //---------------------------------Columna Info---------------------------------------//
            if(i < 5) // Evita crear 2 veces la esquina inferior derecha (casilla blanca)
            {
                xLoc = xCol + 1; // Ajustar las posiciones para multiplicar con ellas
                yLoc = yCol + 1; //

                Sprite_Handle card(new Sprite(textures[id].get()));
                card->set_position({posXTablero + xLoc * escalar, posYTablero + yLoc * escalar});
                card->set_scale(scaleCards);
                sprites.push_back(card);
            }

            //-----------------------------------Fila Info----------------------------------------//
            xLoc = xFil + 1; // Ajustar las posiciones para multiplicar con ellas
            yLoc = yFil + 1; //

            if(i == 5) id = ID(blank);

            Sprite_Handle card(new Sprite(textures[id].get()));
            card->set_position({posXTablero + xLoc * escalar, posYTablero + yLoc * escalar});
            card->set_scale(scaleCards);
            sprites.push_back(card);
        }

        //Crea los 3 botones que se usan para pausar, y moverse por el menú de pausa
        id = ID(back);
        Sprite_Handle atras(new Sprite(textures[id].get()));
        atras->set_position({1220, 60});
        atras->set_scale(scaleCards);
        spritesPause.push_back(atras);
        gobackSpr = atras.get();
        gobackSpr->hide();

        id = ID(play);
        Sprite_Handle jugar(new Sprite(textures[id].get()));
        jugar->set_position({1220, 60});
        jugar->set_scale(scaleCards);
        spritesPause.push_back(jugar);
        playSpr = jugar.get();
        playSpr->hide();

        id = ID(pause);
        Sprite_Handle pausa(new Sprite(textures[id].get()));
        pausa->set_position({1220, 60});
        pausa->set_scale(scaleCards);
        spritesPause.push_back(pausa);
        pausaSpr = pausa.get();
    }

    void Game_Scene::print_round()
    {
        wstring ronda = to_wstring(controlador.getRound());
        Text_Layout ronda_text(*blackfont, L"R-" +  ronda);
        if (controlador.getRound() > 9) canvas->draw_text({posXTablero + 5.6f * escalar, posYTablero + 0.3f * escalar}, ronda_text);
        else canvas->draw_text({posXTablero + 5.7f * escalar, posYTablero + 0.3f * escalar}, ronda_text);
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

        // Escribe las puntuaciones a la derecha del tablero
        print_scores();

        // Escribe la ronda actual en el cuadrado de la esquina inferior derecha
        print_round();

        //Columna
        int colX = 5;           // Posiciones números columna
        int colY = 0;           //

        //Fila
        int filX = 0;           // Posiciones números fila
        int filY = -1;          //

        for (int i = 0; i < 5; ++i, ++colY, ++filX)
        {
            //----------------------------Números por Columna-------------------------------------//

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

            //-------------------------------Números por Fila-------------------------------------//

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

    void Game_Scene::save_scores ()
    {
        // Comprueba si la puntuación del jugador entra en high scores
        check_scores();

        //for (int i = 0; i < nScore; ++i) highscores[i] = 0;  //<--- Borrar datos para testear

        // Genera un path donde el writter escribirá los datos
        string path = application.get_internal_data_path () + "/scores.data";
        ofstream writer(path, ofstream::binary | ofstream::trunc);

        if (writer)
        {
            for (int i = 0; i < nScore; ++i)
            {
                // Se escriben las puntuaciones una a una en formato binario
                unsigned score  = highscores[i];
                writer.write (reinterpret_cast< char * >(&score), sizeof(score));
            }
        }
    }


    void Game_Scene::load_scores ()
    {
        int i = 0;
        for (i = 0; i < nScore; ++i) highscores[i] = 0;  // Inicializa el array a 0

        // Genera el path donde el reader leerá los datos
        string path = application.get_internal_data_path () + "/scores.data";
        ifstream reader(path, ofstream::binary);

        if (reader)
        {
            for (i = 0; i < nScore; ++i)
            {
                // Se leen y cargan las puntuaciones una a una en formato binario
                unsigned score;
                reader.read (reinterpret_cast< char * >(&score), sizeof(score));

                if (!reader.fail () && !reader.bad ()) highscores[i] = score; // Si no hay problemas
                else highscores[i] = 0; // Si los hay
            }
        }
    }

    void Game_Scene::check_scores()
    {
        unsigned aux = 0;
        unsigned score = current_score;

        for (int i = 0; i < nScore; ++i)
        {
            // Si la score es mejor que la highscore i, guarda esta en su posición y
            // baja todas las demás un puesto hacia abajo
            if(highscores[i] <= score && score > 0)
            {
                aux = highscores[i];
                highscores[i] = score;
                score = aux;
            }
        }
    }

    void Game_Scene::next_round()
    {
        rondaAcabada = false;
        controlador.siguienteRonda();

        clear_round();
    }

    void Game_Scene::game_over()
    {
        save_scores();
        gameOver = false;
        controlador.gameOver();

        clear_round();
    }

    void Game_Scene::clear_round()
    {
        // Reinicia valores que no se reinician por si mismos durante la carga de una nueva ronda
        counter = 0;
        current_score = 0;
        sprites.clear();
        spritesPause.clear();
        for (int i = 0; i < nCasillas; ++i) warnsSpr[i] = nullptr;
        timer.reset();
        state = LOADING;
    }


    void Game_Scene::restart_game()
    {
        // Genera un nuevo tablero para cada ronda
        Tablero tab;
        tablero = tab;

        // Genera un controlador en caso de no haber uno
        if(!controlador)
        {
            Controlador cont;
            controlador = cont;
        }
        else current_score = controlador.getScore() + controlador.getTotalScore();
    }


    void Game_Scene::check_endCondition ()
    {
        // Si alguna de las 2 condiciones para terminar una ronda se cumplen, cambia de estado e
        // invoca a la función correspondiente
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

        switch (state) // Según el estado de la partida, pinta una textura u otra
        {
            case LOADING:   id = ID(loading);     break;
            case NEXTROUND: id = ID(nextround);   break;
            case GAMEOVER:  id = ID(gameover);    break;
            case EXITING:   id = ID(exiting);     break;
        }

        Texture_2D * textureRound = textures[id].get ();

        if (textureRound)
        {
            canvas.fill_rectangle
            (
                    { canvas_width * .5f, canvas_height * .5f },
                    {textureRound->get_width (), textureRound->get_height () }, textureRound
            );

            // Muestra las puntuaciones del jugador entre rondas siempre y cuando no sea la primera carga
            if(state == NEXTROUND || (controlador.getRound() > 1 && state == LOADING)) print_scores();
        }
    }

    void Game_Scene::print_scores()
    {
        // Si el juego no está en pausa, printea las puntuación de ronda y total
        if(state != PAUSE)
        {
            wstring puntos = to_wstring(controlador.getScore());
            Text_Layout puntos_text(*whitefont, L"Round Score\n" + puntos);
            canvas->draw_text({990, 490}, puntos_text);

            wstring puntosTotales = to_wstring(controlador.getTotalScore());
            Text_Layout totales_text(*whitefont, L"Total Score\n" + puntosTotales);
            canvas->draw_text({990, 360}, totales_text);
        }
        else // Si está en pausa printea las high scores
        {
            int x = 390;
            int y = 740;

             for (int i = 0; i < nScore; ++i)
             {
                 y -= 70;
                 wstring record = to_wstring(highscores[i]);
                 Text_Layout highscore_text(*whitefont, L"High Score  " + to_wstring(i + 1));
                 Text_Layout record_text(*whitefont, L"-   " + record);
                 canvas->draw_text({x, y}, highscore_text);
                 canvas->draw_text({x + 300, y}, record_text);
             }
        }
    }


    // Se dibujan todos los sprites que conforman la escena y los textos de esta.
    void Game_Scene::render_playfield (Canvas & canvas)
    {
        for (auto & sprite : sprites)      sprite->render (canvas);
        for (auto & sprite : spritesPause) sprite->render (canvas);

        create_text();
    }

    void Game_Scene::render_pause (Canvas & canvas)
    {
        // Se dibuja el slice de cada una de las opciones del menú de pausa mientras
        // no se entre en ninguno de sus submenús
        if(!seenScores && !seenHelp && !seenCredits)
        {
            for (auto &option : options)
            {
                canvas.set_transform
                        (
                                scale_then_translate_2d
                                        (
                                                option.is_pressed ? 0.75f : 1.f,
                                                {option.position[0], option.position[1]}
                                        )
                        );

                canvas.fill_rectangle({0.f, 0.f}, {option.slice->width, option.slice->height},
                                      option.slice, CENTER | TOP);
            }

            // Se restablece la transformación aplicada a las opciones para que no afecte a
            // dibujos posteriores realizados con el mismo canvas:
            canvas.set_transform (Transformation2f());
        }
        else if (seenScores) print_scores(); // Muestra las highscores durante la pausa
        else if (seenHelp)                   // Muestra el tutorial durante la pausa
        {
            Id id = ID(help);
            Texture_2D * textureHelp = textures[id].get ();

            if (textureHelp)
            {
                canvas.fill_rectangle
                        (
                                {canvas_width * .5f, canvas_height * .5f},
                                {textureHelp->get_width(), textureHelp->get_height()}, textureHelp
                        );
            }
        }
        else if (seenCredits)                // Muestra los créditos durante la pausa
        {
            Id id = ID(credits);
            Texture_2D * textureHelp = textures[id].get ();

            if (textureHelp)
            {
                canvas.fill_rectangle
                        (
                                {canvas_width * .5f, canvas_height * .5f},
                                {textureHelp->get_width(), textureHelp->get_height()}, textureHelp
                        );
            }
        }

        // Renderiza solo los sprites que se usan durante la pausa
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

        for (unsigned index = 0; index < nOptions; ++index) {
            options[index].position = Point2f{canvas_width / 2.f, option_top};

            option_top -= options[index].slice->height;
        }
        // Se restablece la presión de cada opción en initialize
    }

    int Game_Scene::option_at (const Point2f & point)
    {
        // Comprueba en que opción está el jugador en el menú de pausa
        for (int index = 0; index < nOptions; ++index)
        {
            const Option & option = options[index];

            if
            (
                point[0] > option.position[0] - option.slice->width  &&
                point[0] < option.position[0] + option.slice->width  &&
                point[1] > option.position[1] - option.slice->height &&
                point[1] < option.position[1] + option.slice->height
            ) return index;
        }
        return -1;
    }
}

//canvas.draw_segment ({    0,   1 }, { 1280,   1 });                   // Borde inferior
//canvas.draw_segment ({    0, 720 }, { 1280, 720 });                   // Borde superior
//canvas.draw_segment ({    1,   0 }, {    1, 720 });                   // Borde izquierdo
//canvas.draw_segment ({ 1280,   0 }, { 1280, 720 });                   // Borde derecho
//canvas.draw_segment ({    0,   360 }, { 1280/4,   360 });             // Medio derecha
//canvas.draw_segment ({    1280 * 0.75,   360 }, { 1280,   360 });     // Medio izquierda