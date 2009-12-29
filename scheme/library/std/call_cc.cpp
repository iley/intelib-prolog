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
