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




#include "m_inc.h"

DECLARE_CFUNCTION(SFunctionExpt,  2,  2,  "#<FUNCTION EXPT>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <math.h>

void SFunctionExpt::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0]->TermType() == SExpressionInt::TypeId &&
       paramsv[1]->TermType() == SExpressionInt::TypeId)
    {
        intelib_integer_t op2 = paramsv[1].GetInt();
        if(op2>=0) {
            intelib_integer_t res=1;
            if(op2>0) {
                intelib_integer_t op1 = paramsv[0].GetInt();
                intelib_integer_t i=1;
                res = op1;
                while(i*2<=op2) {
                    res *= res;
                    i *= 2;
                }
                while(i<op2) {
                    res *= op1;
                    i++;
                }
            }
            lf.RegularReturn(SReference(new SExpressionInt(res))); 
            return;
        }
    }
    intelib_float_t op1 = paramsv[0].GetFloat();
    intelib_float_t op2 = paramsv[1].GetFloat();
    lf.RegularReturn(SReference(new SExpressionFloat(exp(op2*log(op1))))); 
}

#endif

