//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_lsymb.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "lisp/lsymbol.hpp"


int main()
{
    try {
        TestSection("LispSymbols");
        TestSubsection("LSymbol");
        {
            LSymbol A("A");
            LReference a_ref(A);
            TESTC("lsymbol_creation", a_ref->TermType(),
                  LExpressionSymbol::TypeId);
            TEST("lsymbol_name", a_ref->TextRepresentation().c_str(), "A");
            TESTB("lsymbol_is_label", a_ref->TermType().
                 IsSubtypeOf(SExpressionLabel::TypeId));
        }
        TestSubsection("TheSymbolQuote");
        {
            LListConstructor L;
            LReference ref((L| QUOTE, (L|1)));
            TEST("quote_text_representation", ref.TextRepresentation().c_str(),
                 "'(1)");
            LReference ref2 = ~(L|1,2,3);
            TEST("tilde_operator", ref2.TextRepresentation().c_str(),
                 "'(1 2 3)");
            LSymbol B("B");
            LReference ref3 = ~B;
            TEST("tilde_for_lsymbol", ref3.TextRepresentation().c_str(),
                 "'B");
            LReference ref4 = (L| QUOTE, 1, 2, 3);
            TEST("quote_many_params_text_rep", ref4.TextRepresentation().c_str(),
                 "(QUOTE 1 2 3)");
            LReference ref5 = (L| QUOTE || 1);
            TEST("quote_dotted_text_rep", ref5.TextRepresentation().c_str(),
                 "(QUOTE . 1)");
            LReference ref6 = (L| QUOTE);
            TEST("quote_noparams_text_rep", ref6.TextRepresentation().c_str(),
                 "(QUOTE)");
            LReference ref7 = (L|1, 2, ~~~~B);
            TEST("quote_text_rep_recursive", ref7.TextRepresentation().c_str(),
                 "(1 2 ''''B)");
        }
        TestSubsection("");
        {
            TEST("boolean_true",
                 (*PTheLispBooleanTrue)->TextRepresentation().c_str(),
                 "T"
                );

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
    return 0;
}


