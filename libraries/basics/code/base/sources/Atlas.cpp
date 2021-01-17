/*
 * ATLAS
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1802012123
 */

#include <basics/assert>
#include <basics/Asset>
#include <basics/Atlas>
#include <cstring>

#include <basics/Log>

using namespace std;
using namespace rapidxml;

namespace basics
{

    Atlas::Atlas(const string & path, Graphics_Context::Accessor & context)
    {
        shared_ptr< Asset > slices_file = Asset::open (path);

        if (slices_file->good ())
        {
            Buffer slices_data;

            if (slices_file->read_all (slices_data))
            {
                parse (slices_data, path, context);
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    Atlas::Atlas(const Texture_Handle & texture)
    :
        texture(texture)
    {
    }

    // ---------------------------------------------------------------------------------------------

    Atlas::Slice * Atlas::add_slice (Id id, const Point2f & position, const Size2f & size)
    {
        if (slices.count (id) == 0)
        {
            return &
            (
                slices[id] =
                {
                    this,
                    position.coordinates.x (), position.coordinates.x () + size.width,
                    position.coordinates.y (), position.coordinates.y () + size.height,
                    size.width,                size.height
                }
            );
        };

        return nullptr;
    }

    // ---------------------------------------------------------------------------------------------

    void Atlas::parse (Buffer & slices_data, const std::string & path, Graphics_Context::Accessor & context)
    {
        // Se pone un caracter nulo al final para que el parseador de rapidxml sepa dónde está el
        // final de los datos:

        slices_data.push_back (0);

        // Se parsea el xml de datos de slices:

        xml_document<> xml;

        xml.parse< 0 > (reinterpret_cast< char * >(slices_data.data ()));

        // Se comprueba si se ha podido parsear el xml y, si se ha podido, se empieza a analizar el tag raíz:

        xml_node<> * img_tag = xml.first_node ("img");

        if (img_tag)
        {
            parse_img (img_tag, path, context);
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Atlas::parse_img (rapidxml::xml_node<> * img_tag, const std::string & path, Graphics_Context::Accessor & context)
    {
        // Se busca el atributo "name" del tag "img", el cual indica el nombre del archivo de la textura:

        xml_attribute<> * name_attribute = img_tag->first_attribute ("name");

        if (name_attribute)
        {
            // Se determina la ruta de la textura:

            size_t slash     = path.find_last_of ('/' );
            size_t backslash = path.find_last_of ('\\');
            string texture_path;

            if (slash != string::npos && backslash != string::npos)
            {
                texture_path = path.substr (0, std::max (slash, backslash + 1));
            }
            else
            if (slash != string::npos)
            {
                texture_path = path.substr (0, slash + 1);
            }
            else
            if (backslash != string::npos)
            {
                texture_path = path.substr (0, backslash + 1);
            }

            // Se intenta cargar la textura:

            texture = Texture_2D::create (0, context, texture_path + name_attribute->value ());

            assert(texture);

            if (texture)
            {
                context->add (texture);

                // Se comprueba que las dimensiones de la textura coinciden con lo que indica el XML:

                //xml_attribute<> * w_attribute = img_tag->first_attribute ("w");
                //xml_attribute<> * h_attribute = img_tag->first_attribute ("h");

                //assert(!w_attribute || texture->get_width  () == std::atoi (w_attribute->value ()));
                //assert(!h_attribute || texture->get_height () == std::atoi (h_attribute->value ()));

                // Se busca el tag "definitions" (anidado en el tag "img"):

                xml_node<> * definitions_tag = img_tag->first_node ();

                if (definitions_tag && definitions_tag->name () == string("definitions"))
                {
                    // Se buscan y parsean todos los tags "dir" anidados dentro de "definitions":

                    for (xml_node<> * dir_tag = definitions_tag->first_node ("dir"); dir_tag; dir_tag = dir_tag->next_sibling ("dir"))
                    {
                        parse_dir (dir_tag);
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Atlas::parse_dir (rapidxml::xml_node<> * dir_tag, const string & prefix)
    {
        for (xml_node<> * child = dir_tag->first_node (); child; child = child->next_sibling ())
        {
            if (child->type () == node_element)
            {
                // Se espera que un tag anidado en "dir" sea otro "dir" o un "spr" y debe tener
                // atributo "name" para ser tenido en cuenta:

                xml_attribute<> * name_attribute = child->first_attribute ("name");

                if (name_attribute)
                {
                    // Se determina el id del nodo añadiendo al prefijo el nombre propio:

                    string id = prefix + name_attribute->value ();

                    // Se interpreta el "dir" o "spr":

                    if (child->name () == string("dir"))
                    {
                        // Si se trata de un "dir" raíz con un nombre por defecto, se descarta su id.
                        // En otro caso, se le añade un punto como separador:

                        if (id == "/") id.clear (); else id += ".";

                        parse_dir (child, id);
                    }
                    else
                    if (child->name () == string("spr"))
                    {
                        parse_spr (child, id);
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Atlas::parse_spr (rapidxml::xml_node<> * spr_tag, const std::string & id)
    {
        // Se extraen todos los atributos básicos:

        xml_attribute<> * x_attribute = spr_tag->first_attribute ("x");
        xml_attribute<> * y_attribute = spr_tag->first_attribute ("y");
        xml_attribute<> * w_attribute = spr_tag->first_attribute ("w");
        xml_attribute<> * h_attribute = spr_tag->first_attribute ("h");

        if (x_attribute && y_attribute && w_attribute && h_attribute)
        {
            float x = std::atoi (x_attribute->value ());
            float y = std::atoi (y_attribute->value ());
            float w = std::atoi (w_attribute->value ());
            float h = std::atoi (h_attribute->value ());

            Slice * slice = add_slice (fnv32 (id), { x, y }, { w, h });

            assert (slice && w && h);
        }
    }

}
