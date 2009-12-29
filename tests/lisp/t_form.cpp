// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "genlisp/lispform.hpp"
#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lcont.hpp"
#include "lisp/llambda.hpp"

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

LListConstructor L;

class LSampleCFunction : public SExpressionFunction {
public:
    LSampleCFunction() : SExpressionFunction(-1, -1) {}
    ~LSampleCFunction()  {}
    virtual SString TextRepresentation() const
        { return SString("#<FORM LSampleCFunction>"); }
    virtual void DoApply(int paramsc,
                        const SReference paramsv[],
                        IntelibContinuation &lf) const
    {
        //printf("* entered Sample::DoCall\n");
        LReference res = (L|paramsv[0]);
        for(int i = 1; i< paramsc; i++)  {
            res.operator,(paramsv[i]);
        }
        lf.RegularReturn(res);
    }
};

LFunctionalSymbol<LSampleCFunction> SAMPLE("SAMPLE");

int main()
{
    poc();
    try {
        TestSection("Forms");
        TestSubsection("LExpressionCFunction");
        {
            LSymbol A("A");
            LReference res = (L|SAMPLE, 1, 2, ~A).Evaluate();
            TEST("sample_lcfunction", res->TextRepresentation().c_str(),
                 "(1 2 A)");
            res = (L|SAMPLE, ~SAMPLE, 1, 2).Evaluate();
            TEST("sample_lcfunction2", res->TextRepresentation().c_str(),
                 "(SAMPLE 1 2)");
        }
        TestSubsection("LExpressionLambda");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            B->SetDynamicValue("shit");
            LReference l(L|A);
            LReference b((L|(L|SAMPLE, (L|SAMPLE, 1, 2, A), B, A)));
            LContextRef cont(new LExpressionContext);
            cont->AddBinding(B.GetPtr(), (L|"teststring"));
            LReference fun(new LExpressionLambda(cont, l, b));
            F->SetFunction(fun);
            LReference res = (L| F, 100).Evaluate();
            TESTTR("lisp_function", res, "((1 2 100) (\"teststring\") 100)");
        }
        TestSubsection("LExpressionMacro");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            A->SetDynamicValue("strA");
            B->SetDynamicValue("strB");
            {
                LReference l(L|A);
                LReference b((L|A));
                LContextRef cont(new LExpressionContext);
                LReference fun(new LExpressionMacro(cont, l, b));
                cont->AddBinding(B.GetPtr(), (L|"teststring"));
                F->SetFunction(fun);
                LReference res = (L| F, B).Evaluate();
                TESTTR("lisp_macro", res, "\"strB\"");
                res = (L| F, 999).Evaluate();
                TESTTR("lisp_macro2", res, "999");
            }
            {
                LReference l(L|A);
                LReference b((L|(L|SAMPLE, ~SAMPLE, A, A)));
                LContextRef cont(new LExpressionContext);
                LReference fun(new LExpressionMacro(cont, l, b));
                cont->AddBinding(B.GetPtr(), (L|"teststring"));
                F->SetFunction(fun);
                LReference res = (L| F, B).Evaluate();
                TESTTR("lisp_macro3", res, "(\"strB\" \"strB\")");
            }
        }
        TestScore();
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


