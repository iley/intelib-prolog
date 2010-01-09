//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/call_cc.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "std_inc.h"

DECLARE_CFUNCTION(SchFunctionCall_cc, 1, 1, "#<FUNCTION CALL/CC>", "CALL/CC")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

class SchExpressionEscapeFunction : public SExpressionFunction {
    SchemeContinuation cont;
public:
    static IntelibTypeId TypeId;

    SchExpressionEscapeFunction(IntelibContinuation &a)
        : SExpressionFunction(TypeId, 1, 1),
          cont((SchemeContinuation&)a, true) {}
    ~SchExpressionEscapeFunction() {}
    
    virtual void DoApply(int paramc,
                         const SReference *paramv,
                         IntelibContinuation &lf) const
    {
        SReference arg = paramv[0];
        lf.ReplaceContinuation(cont); // destroys the locations of paramsv
        lf.RegularReturn(arg);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const
        { return "#<CONTINUATION PROCEDURE>"; }
#endif
};

IntelibTypeId
SchExpressionEscapeFunction::TypeId(&SExpressionFunction::TypeId, false);

void SchFunctionCall_cc::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference esc(new SchExpressionEscapeFunction(lf));
    lf.PushTodo(1);   // call with one argument
    lf.PushTodo(IntelibContinuation::quote_parameter, esc);
    lf.PushTodo(IntelibContinuation::quote_parameter, paramsv[0]);
}
#endif
