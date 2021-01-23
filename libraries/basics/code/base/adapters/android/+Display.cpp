/*
 * Display
 * Copyright © 2021+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Display>
#include "Native_Activity.hpp"

namespace basics
{

    bool Display::set_prevent_sleep (bool status)
    {
        if (internal::native_activity)
        {
            ANativeActivity_setWindowFlags
            (
               &internal::native_activity->get_activity (),
                status == true  ? AWINDOW_FLAG_KEEP_SCREEN_ON : 0,
                status == false ? AWINDOW_FLAG_KEEP_SCREEN_ON : 0
            );

            return true;
        }
        
        return false;
    }

}
