//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/m/expt.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "m_inc.h"

DECLARE_CFUNCTION(SFunctionExpt,  2,  2,  "#<FUNCTION EXPT>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION


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

