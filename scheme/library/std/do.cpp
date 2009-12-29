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

DECLARE_SPECFORM(SchFunctionDo, "#<FUNCTION DO>", "DO")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

class SchExpressionDoIterator : public SExpressionGenericIterator {
    SReference test;
    SReference body;
    SReference *varstepvector;
    int varstepcount;
public:
    static IntelibTypeId TypeId;

    SchExpressionDoIterator(const SReference &atest, const SReference &abody)
        : SExpressionGenericIterator(TypeId), test(atest), body(abody),
          varstepvector(0), varstepcount(0)
    {}
    ~SchExpressionDoIterator() { if(varstepvector) delete[] varstepvector; }

    void SetIterationVector(int n, SReference *vec)
    {
        varstepcount = n;
        varstepvector = vec;
    }

    void ScheduleTest(IntelibContinuation& lf)
    {
        lf.PushTodo(IntelibContinuation::just_evaluate, test.Car());
    }

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        SReference res;
        lf.PopResult(res);
        return res.GetPtr() == PTheSchemeBooleanFalse->GetPtr();
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        ScheduleTest(lf);
        for(int i = 0; i < varstepcount; i++) {
            lf.PushTodo(IntelibContinuation::assign_to, varstepvector[2*i]);
            lf.PushTodo(IntelibContinuation::just_evaluate, varstepvector[2*i+1]);
        }
        lf.PushTodo(IntelibContinuation::drop_result);
        lf.PushTodo(IntelibContinuation::evaluate_progn, body);
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {
        // nothing to do
    }

    void ReturnFinalValue(IntelibContinuation &lf) {
        if(test.Cdr().IsEmptyList()) {
            lf.ReturnUnspecified();
        } else {
            lf.PushTodo(IntelibContinuation::evaluate_progn, test.Cdr());
        }
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const { return "#<-DO ITERATOR->"; }
#endif
};

IntelibTypeId SchExpressionDoIterator::TypeId(&SExpression::TypeId,true);

static void analyze_vars(const SReference& vars,
                         SchContextRef& tempcontext,
                         IntelibContinuation& lf,
                         int& varcnt,
                         SReference **varstepvec,
                         int depth)
{
    SExpressionCons *dp = vars.DynamicCastGetPtr<SExpressionCons>();
    if(!dp) { // end of list? let's not check
        varcnt = depth;
        *varstepvec = depth ? new SReference[2*depth] : 0;
    } else {
        SReference &vl = dp->Car();
        SReference &vname = vl.Car();
        SReference &vl2 = vl.Cdr();
        SReference &init = vl2.Car(); 
        SExpressionCons *dp2 = vl2.Cdr().DynamicCastGetPtr<SExpressionCons>();

        analyze_vars(dp->Cdr(), tempcontext, lf, varcnt, varstepvec,
                     depth + (dp2 ? 1 : 0));

        SchExpressionSymbol *symb =
            vname.DynamicCastGetPtr<SchExpressionSymbol>();
        if(!symb) throw IntelibX_scheme_not_a_symbol(vname);
        SReference *loc = tempcontext->ProvideBinding(symb);
        SReference locref(new SExpressionLocation(tempcontext, loc));
        lf.PushTodo(IntelibContinuation::assign_to, locref);
        lf.PushTodo(IntelibContinuation::just_evaluate, init);
        
        if(dp2) {
            (*varstepvec)[2*depth] = locref;
            (*varstepvec)[2*depth+1] = dp2->Car();
        }
    } 
}

void SchFunctionDo::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
        // the very last step is the context restoration
    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());

    SReference &vars = paramsv.Car();
    SReference &p2 = paramsv.Cdr();
    SReference &test = p2.Car();
    SReference &body = p2.Cdr();

    SchContextRef tempcontext(new SchExpressionContext(lf.GetContext()));

    SchExpressionDoIterator *iter = new SchExpressionDoIterator(test, body);
    lf.PushTodo(IntelibContinuation::generic_iteration, SReference(iter));

    iter->ScheduleTest(lf);
    lf.PushTodo(IntelibContinuation::set_context, tempcontext);

    int varcnt;
    SReference *varstepvec;

    analyze_vars(vars, tempcontext, lf, varcnt, &varstepvec, 0);

    iter->SetIterationVector(varcnt, varstepvec);

}

#endif
