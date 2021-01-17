/*
 * APPLICATION
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712290050
 */

#include "Android_Application.hpp"

namespace basics
{

    namespace internal
    {

        Android_Application application;

    }

    Application & Application::get_instance ()
    {
        return internal::application;
    }

    Application & application = Application::get_instance ();

}
