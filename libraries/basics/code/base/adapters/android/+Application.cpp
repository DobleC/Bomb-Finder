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
#include "Native_Activity.hpp"

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

    std::string Application::get_internal_data_path () const
    {
        return internal::application.get_internal_data_path ();
    }

    std::string Application::get_external_data_path () const
    {
        return internal::application.get_external_data_path ();
    }

    Application & application = Application::get_instance ();

}
