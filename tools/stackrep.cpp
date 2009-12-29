// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <stdio.h> // for snprintf
#include "../sexpress/sstring.hpp"

SString stack_representation(SReference stack)
{
    if(!stack.GetPtr()) return "# No stack available\n";
    SString s("# Stack:\n");
    int f = 0;
    for(SReference r=stack; !r.IsEmptyList(); r=r.Cdr()) {
         char buf[20];
         snprintf(buf, sizeof(buf), "#%d:    ", f++);
         s += buf;
         s += r.Car()->TextRepresentation();
         s += "\n";
    }
    s += "\n";
    return s;
}
