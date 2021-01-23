/*
 * ASSET
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801220045
 */

#ifndef BASICS_ASSET_HEADER
#define BASICS_ASSET_HEADER

    #include <memory>
    #include <vector>
    #include <string>
    #include <basics/types>

    namespace basics
    {

        /** Esta clase permite conocer detalles de archivos que se encuentran en la carpeta de assets
          * y leer su contenido (no permite modificarlo).
          */
        class Asset
        {
        public:

            /** Representa un punto de referencia cuando se desea mover el puntero de lectura.
              */
            enum Anchor
            {
                BEGINNING,      ///< Mover el puntero de lectura desde el principio del archivo
                CURRENT,        ///< Mover el puntero de lectura en relación a la posición actual
                END             ///< Mover el puntero de lectura en relación al final del archivo
            };

        public:

            /** Crea un objeto de clase Asset que permite leer el contenido de un archivo que se
              * encuentra en la carpeta de assets.
              * @param path Ruta relativa del archivo considerando la carpeta de assets como base.
              * @return Retorna un shared_ptr<Asset> que apunta al objeto de clase Asset que
              *     permitirá leer el contenido del archivo. Si no se pudo abrir, dicho puntero
              *     será nulo.
              */
            static std::shared_ptr< Asset > open (const std::string & path);
            
            /** Permite comprobar si un archivo existe en cierta ruta dentro de la carpeta de assets.
              * @param path Ruta relativa del archivo considerando la carpeta de assets como base.
              * @return true si el archivo existe en la ruta indicada.
              */
            static bool exists (const std::string & path);
            
            /** Permite conocer el tamaño de un archivo ubicado en cierta ruta dentro de la carpeta de assets.
              * @param path Ruta relativa del archivo considerando la carpeta de assets como base.
              * @return El tamaño del archivo o 0 si no existe.
              */
            static size_t size (const std::string & path);

        protected:

            Asset() = default;
            virtual ~Asset() = default;

        public:

            /** Permite comprobar si la última operación de lectura terminó con éxito.
              * @return true si la última operación de lectura terminó con éxito.
              */
            virtual bool good () const = 0;

            /** Permite comprobar si se produjo algún error en la última operación de lectura.
              * @return true si se produjo algún error en la última operación de lectura.
              */
            virtual bool fail () const = 0;

            /** Permite comprobar si se llegó al final del archivo tras la última operación de lectura.
              * @return true si se llegó al final del archivo tras la última operación de lectura.
              */
            virtual bool eof () const = 0;

        public:

            /** Permite conocer el tamaño del archivo abierto.
              * @return Retorna el tamaño del archivo abierto.
              */
            virtual size_t size () const = 0;

            /** Mueve el puntero de lectura a cierta posición en relación al punto de referencia que
              * se indique (por defecto la posición actual del puntero de lectura).
              * @param offset posición relativa al punto de referencia (puede llegar a ser un número
              *     positivo o negativo).
              * @param Punto de referencia desde el que se moverá el puntero de lectura  (por defecto
              *     la posición actual del puntero de lectura). Ver los posibles valores de Asset::Anchor.
              * @return true si la operación se pudo completar con éxito.
              */
            virtual bool seek (ptrdiff_t offset, Anchor = CURRENT) = 0;
            
            /** Devuelve la posición actual del puntero de lectura.
              * @return La posición actual del puntero de lectura.
              */
            virtual size_t tell () const = 0;
            
            /** Lee un único byte del archivo. Después de llamar a esta función es conveniente llamar
              * a la función good() o fail() para conocer si la operación lectura se realizó correctamente.
              * Si la operación de lectura falló el valor que retorna se debería descartar.
              * @return Un byte leído del archivo.
              * 
              */
            virtual byte read () = 0;
            
            /** Lee todo el contenido del archivo y lo coloca dentro de un vector de bytes. El contenido
              * que pueda tener el vector se sustituirá por el contenido leído.
              * @param buffer Vector en el que se colocarán los bytes leídos del archivo.
              * @return true si la operación de lectura se completó con éxito.
              */
            virtual bool read_all (std::vector< byte > & buffer) = 0;
            
            /** Lee todo el contenido del archivo y lo coloca dentro de una string (de char). El contenido
              * que pueda tener la cadena se sustituirá por el contenido leído.
              * @param buffer String en la que se colocarán los chars leídos del archivo.
              * @return true si la operación de lectura se completó con éxito.
              */
            virtual bool read_all (std::string & buffer) = 0;

        };

    }

#endif
