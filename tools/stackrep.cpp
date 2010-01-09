//   InteLib                                    http://www.intelib.org
//   The file tools/stackrep.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
