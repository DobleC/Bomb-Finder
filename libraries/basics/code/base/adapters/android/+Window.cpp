/*
 * WINDOW
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712180050
 */

#include <basics/Window>
#include "Native_Activity.hpp"

namespace basics
{

    #pragma RETAIN(Window::can_be_instantiated)

    const bool Window::can_be_instantiated __attribute__((__used__)) = false;

    Window::Handle Window::create_window (Id )
    {
        return Handle();
    }

    bool Window::destroy_window (Id )
    {
        return false;
    }

    Window::Handle Window::get_window (Id id)
    {
        if (internal::native_activity && id == default_window_id)
        {
            return Handle(internal::native_activity->get_window ());
        }

        return Handle();
    }

}
