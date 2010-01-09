//   InteLib                                    http://www.intelib.org
//   The file tools/prprint.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/sstring.hpp"
#include "prprint.hpp"



static SString make_indent(int indent)
{
    SString res = "";
    for(int i=0; i<indent; i++) res += " ";
    return res;
}


bool pretty_print(SReference ref, 
                  prettyprint_callback_function fun, 
                  void *userdata, 
                  int indent, int margin, int indentstep)
{
    SString res = make_indent(indent);

    SExpressionCons *dp = ref.DynamicCastGetPtr<SExpressionCons>();

    if(!dp) {
        res += ref->TextRepresentation();
        return fun(res.c_str(), userdata);
    }

    /* well... dot pair */
    SString rep = ref->TextRepresentation();
    if(indent+rep.length()<= (unsigned int) margin) {
        res += rep;
        return fun(res.c_str(), userdata);
    }

    /* can't print at once... */

    res+="(";
    /* now check the length of the first element */
    rep = dp->Car()->TextRepresentation();
    if(indent+rep.length()+1/*brace*/ <= (unsigned int)margin) {
        res += rep;   
        ref = dp->Cdr();
    } 
    if(!fun(res.c_str(), userdata)) return false;
    res = "";

    while((dp=ref.DynamicCastGetPtr<SExpressionCons>())) {
        if(!pretty_print(dp->Car(), fun, userdata, 
                         indent+indentstep, margin, indentstep))
        {
            return false;
        }
        ref = dp->Cdr();
    }
    if(!ref.IsEmptyList()) {
        /* dotted list */
        res = make_indent(indent+indentstep+1)+".";
        if(!fun(res.c_str(), userdata)) return false;
        if(!pretty_print(ref, fun, userdata, 
                         indent+indentstep, margin, indentstep))
        {
            return false;
        }
    }
    res = make_indent(indent)+")";
    return fun(res.c_str(), userdata);
}
