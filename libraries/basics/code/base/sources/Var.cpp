/*
 * VAR
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712190125
 */

#include <basics/Var>

namespace basics
{

    namespace var
    {

        const Var::Type::Info  Void::info{  Void::id,  "Void", nullptr };
        const Var::Type::Info  Bool::info{  Bool::id,  "Bool", nullptr };
        const Var::Type::Info Int32::info{ Int32::id, "Int32", nullptr };
        const Var::Type::Info Float::info{ Float::id, "Float", nullptr };

    }

}
