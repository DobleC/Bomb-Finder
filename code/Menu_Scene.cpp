/*
 * MENU SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

using namespace basics;
using namespace std;
using namespace game;

namespace menu
{
    Menu_Scene::Menu_Scene()
    {
        state         = LOADING;
        suspended     = true;
        canvas_width  = 1280;
        canvas_height =  720;
    }

    // ---------------------------------------------------------------------------------------------

    bool Menu_Scene::initialize ()
    {
        for (auto & option : options)
        {
            option.is_pressed = false;
        }

        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::handle (basics::Event & event)
    {
        if (state == READY)                     // Se descartan los eventos cuando la escena está LOADING
        {
            switch (event.id)
            {
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

                    for (auto & option : options) option.is_pressed = false;

                    // Se determina qué opción se ha dejado de tocar la última y se actúa como corresponda:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    if (option_at (touch_location) == PLAY)
                    {
                        director.run_scene (shared_ptr< Scene >(new Game_Scene));
                    }
                    else if (option_at (touch_location) == SCORES)
                    {
                        gobackSpr->show();
                        state = SEENSCORE;
                    }
                    else if (option_at (touch_location) == HELP)
                    {
                        helpSpr  ->show();
                        gobackSpr->show();
                        state = SEENHELP;
                    }
                    else if (option_at (touch_location) == CREDITS)
                    {
                        creditsSpr->show();
                        gobackSpr ->show();
                        state = SEENCREDITS;
                    }
                    break;
                }
            }
        }
        else if (state == SEENSCORE || state == SEENHELP || state == SEENCREDITS)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-ended):
                {
                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    if (gobackSpr->contains(touch_location))
                    {
                        gobackSpr ->hide();
                        helpSpr   ->hide();
                        creditsSpr->hide();
                        state = READY;
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::update (float time)
    {
        if (!suspended)
            if (state == LOADING)
        {
                Graphics_Context::Accessor context = director.lock_graphics_context();

                if (context) {

                    whitefont.reset(new Raster_Font("fonts/impactwhite.fnt", context));
                    gobackTexture    = Texture_2D::create (ID(back),    context, "sprites/9back.png"      );
                    helpTexture      = Texture_2D::create (ID(help),    context, "menu-scene/help.png"    );
                    creditsTexture   = Texture_2D::create (ID(credits), context, "menu-scene/credits.png" );

                    context->add(gobackTexture );
                    context->add(helpTexture   );
                    context->add(creditsTexture);

                    Sprite_Handle ayuda(new Sprite(helpTexture.get()));
                    ayuda->set_position({canvas_width * 0.5f, canvas_height * 0.5f});
                    helpSpr = ayuda.get();
                    helpSpr->hide();
                    sprites.push_back(ayuda);

                    Sprite_Handle creditos(new Sprite(creditsTexture.get()));
                    creditos->set_position({canvas_width * 0.5f, canvas_height * 0.5f});
                    creditsSpr = creditos.get();
                    creditsSpr->hide();
                    sprites.push_back(creditos);

                    Sprite_Handle atras(new Sprite(gobackTexture.get()));
                    atras->set_position({1220, 60});
                    atras->set_scale(0.40f);
                    gobackSpr = atras.get();
                    gobackSpr->hide();
                    sprites.push_back(atras);

                    // Se carga el atlas:
                    atlas.reset(new Atlas("menu-scene/main-menu.sprites", context));

                    // Si el atlas se ha podido cargar el estado es READY y, en otro caso, es ERROR:
                    state = atlas->good() ? READY : ERROR;

                    // Si el atlas está disponible, se inicializan los datos de las opciones del menú:
                    if (state == READY) {
                        configure_options();
                    }
                }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::render (Graphics_Context::Accessor & context)
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

                for (auto & sprite : sprites)      sprite->render (*canvas);

                if (state == READY)
                {
                    // Se dibuja el slice de cada una de las opciones del menú:

                    for (auto & option : options)
                    {
                        canvas->set_transform
                        (
                            scale_then_translate_2d
                            (
                                  option.is_pressed ? 0.75f : 1.f,                     // Escala de la opción
                                { option.position[0], option.position[1] }      // Traslación
                            )
                        );

                       canvas->fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
                    }

                    // Se restablece la transformación aplicada a las opciones para que no afecte a
                    // dibujos posteriores realizados con el mismo canvas:

                    canvas->set_transform (Transformation2f());
                }

                else if (state == SEENSCORE) print_scores();
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::configure_options ()
    {
        // Se asigna un slice del atlas a cada opción del menú según su ID:

        options[PLAY   ].slice = atlas->get_slice (ID(play)   );
        options[CREDITS].slice = atlas->get_slice (ID(credits));
        options[HELP   ].slice = atlas->get_slice (ID(help)   );
        options[SCORES ].slice = atlas->get_slice (ID(scores) );

        // Se calcula la altura total del menú:

        float menu_height = 0;

        for (auto & option : options) menu_height += option.slice->height;

        // Se calcula la posición del borde superior del menú en su conjunto de modo que
        // quede centrado verticalmente:

        float option_top = canvas_height / 2.f + menu_height / 2.f;

        // Se establece la posición del borde superior de cada opción:

        for (unsigned index = 0; index < number_of_options; ++index)
        {
            options[index].position = Point2f{ canvas_width / 2.f, option_top };

            option_top -= options[index].slice->height;
        }

        // Se restablece la presión de cada opción:

        initialize ();
    }

    // ---------------------------------------------------------------------------------------------

    int Menu_Scene::option_at (const Point2f & point)
    {
        // Comprueba en que opción está el jugador en el menú de pausa
        for (int index = 0; index < number_of_options; ++index)
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

    //----------------------------------------------------------------------------------------------

    void Menu_Scene::load_scores ()
    {
        int i = 0;
        for (i = 0; i < nScore; ++i) highscores[i] = 0;  // Inicializa el array a 0

        // Genera el path donde el reader escribirá los datos
        string path = application.get_internal_data_path () + "/scores.data";
        ifstream reader(path, ofstream::binary);

        if (reader)
        {
            for (i = 0; i < nScore; ++i)
            {
                unsigned score;
                reader.read (reinterpret_cast< char * >(&score), sizeof(score));

                if (!reader.fail () && !reader.bad ()) highscores[i] = score; // Si no hay problemas
                else highscores[i] = 0; // Si los hay
            }
        }
        scoresLoaded = true;
    }

    void Menu_Scene::print_scores()
    {
        if(!scoresLoaded) load_scores();

        int x = 390;
        int y = 740;

        // Printea las high scores
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
