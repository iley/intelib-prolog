//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_eval.cpp
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
#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lcont.hpp"
#include "lfun_std.hpp"



LReference square(int paramsc, const SReference *paramsv)
{
    int i = paramsv[0].GetInt();
    return LReference(i*i);
}

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    try {
        poc();
        TestSection("LispEvaluations");
        TestSubsection("ConstantEvaluations");
        {
            LReference intref(25);
            TESTC("int_evaluating", intref.Evaluate(), intref);
            LReference floatref(25.55);
            TESTC("float_evaluating", floatref.Evaluate(), floatref);
            LReference stringref("string");
            TESTC("string_evaluating", stringref.Evaluate(), stringref);

        }
        poc();
        TestSubsection("LSymbol");
        {
            LSymbol A("A");
            LReference nullref;
            TESTC("unassigned_symbol", A->GetDynamicValue(), nullref);
            LReference ref(2552);
            A->SetDynamicValue(ref);
            TESTC("dynamic_value", A->GetDynamicValue(), ref);
            TESTC("dynamic_value_eval", LReference(A).Evaluate(), ref);
        }
        poc();
        TestSubsection("LExpressionContext");
        {
            LSymbol SS("SS");
            LReference ref25(25);
            SS->SetDynamicValue(ref25);
            TESTC("setq_without_context", LReference(SS).Evaluate(), ref25);
            LContextRef pcont(new LExpressionContext);
            LReference unbound;
            TESTB("no_lex_binding", pcont->GetBinding(SS.GetPtr())==0);
            LReference ref36(36);
            pcont->AddBinding(SS.GetPtr(), ref36);
            SReference *pref;
            pref = pcont->GetBinding(SS.GetPtr());
            TESTC("lex_binding_playback", *pref, ref36);
            TESTC("dynamic_value_unaffected", LReference(SS).Evaluate(), ref25);

            LContextRef emptycontext(new LExpressionContext);
            LispContinuation cont;
            cont.SetContext(emptycontext);
            TESTC("dynamic_value_when_no_lex",
                  LReference(SS).Evaluate(cont), ref25);

            cont.SetContext(pcont);
            TESTC("lexical_value", LReference(SS).Evaluate(cont), ref36);
            TEST("lexical_value_txt", LReference(SS).Evaluate(cont)->
                 TextRepresentation().c_str(),
                 "36");
            // Now create an inner lexical context
            LContextRef pcont2(new LExpressionContext(pcont));
            // And evaluate within it
            cont.SetContext(pcont2);
            TESTC("lexical_value_in_inner_context",
                  LReference(SS).Evaluate(cont), ref36);

#if 0
            // Now lets change the value of SS
            LReference ref49(49);
            SS->Setq(ref49, pcont2);
            // We've just changed the EXISTING binding
            TESTC("inner_setq_changes_same_binding", *pref, ref49);
            // Let's shadow that binding by creating a new one
            LReference ref64(64);
            pcont2->AddLexicalBinding(SS.GetPtr(), ref64);
            // Now we test that it is in effect, 
            //   then return to the outer context
            //   and see THAT binding is still unchanged
            TESTC("shadow_lexical_binding",
                  LReference(SS).Evaluate(pcont2), ref64);
            TESTC("shadowed_binding_untouched",
                  LReference(SS).Evaluate(pcont), ref49);
#endif
        }
        poc();
        TestSubsection("Dynamic_binding");
        {
            LSymbol SS("SS");
            LSymbol DD("DD");
            TESTB("symbol_is_lexical_by_default", !(SS->IsDynamic()));
            SS->SetDynamicBinding();
            TESTB("switch_to_dynamic_binding", SS->IsDynamic());
            SS->SetDynamicBinding(false);
            TESTB("switch_back_to_lexical", !(SS->IsDynamic()));
            // Lets make DD dynamic
            DD->SetDynamicBinding();
            // Let's assign a dynamic value to both symbols
            LReference ref25(25);
            SS->SetDynamicValue(ref25);
            DD->SetDynamicValue(ref25);
            // Now let's make a lexical context
            LContextRef pcont(new LExpressionContext);
            // We try to bind both symbols lexically,
            // and for DD that must have no visible effect
            //         (unless we restore lexical binding style for it)
            LReference ref36(36);
            pcont->AddBinding(SS.GetPtr(), ref36);
            pcont->AddBinding(DD.GetPtr(), ref36);
            LispContinuation cont;
            cont.SetContext(pcont);
            TESTC("lex_symbol_obeys_lex_binding",
                  LReference(SS).Evaluate(cont), ref36);
            TESTC("dyn_symbol_ignores_lex_binding",
                  LReference(DD).Evaluate(cont), ref25);
#if 0
            // Now we Setq the both symbols and see that SS'dynamic value
            //   is still untouched while DD's changed
            LReference ref49(49);
            SS->Setq(ref49, pcont);
            DD->Setq(ref49, pcont);
            // They now both evaluate to ref49 within the context
            TESTC("lex_symbol_setq", LReference(SS).Evaluate(pcont), ref49);
            TESTC("dyn_symbol_setq", LReference(DD).Evaluate(pcont), ref49);
            // ... but for SS it is lexical value while for DD is dynamic
            TESTC("lex_symbol_lex", SS->GetDynamicValue(), ref25);
            TESTC("dyn_symbol_dyn", DD->GetDynamicValue(), ref49);
            // Lets now return to the top-level and check if everything's Ok
            LContextRef topcont(new LExpressionContext);
            TESTC("lex_symbol_value_survived",
                  LReference(SS).Evaluate(topcont), ref25);
            TESTC("dyn_symbol_value_changed",
                  LReference(DD).Evaluate(topcont), ref49);
#endif
        }
        poc();
        TestSubsection("Apply");
        {
            SListConstructor L;
            LReference plus = new LFunctionPlus;
            LReference res = plus.Apply((L|1,2,3));
            TESTTR("apply", res, "6");
        }
        TestSubsection("Form_evaluation");
        {
            SListConstructor L;
            LSymbol A("A");
            LReference frm = (L|QUOTE, A);
            LReference res = frm.Evaluate();
            LReference must_be = A;
            TEST("evaluate_quote", res->TextRepresentation().c_str(),
                 must_be->TextRepresentation().c_str());
            frm = (L|QUOTE,(L|1, 2, 3));
            TEST("evaluate_quoted_list",
                 frm.Evaluate()->TextRepresentation().c_str(),
                 "(1 2 3)");
        }
        TestSubsection("UserLispFunctions");
        {
            LListConstructor L;
            LSymbol f("f");
            f->SetFunction(square);
            LReference fref = (L| f, 15);
            TEST("user_function", fref.Evaluate().GetInt(), 225);
            LReference fref2 = (L| square, 25);
            try {
                TEST("user_function_by_value", fref2.Evaluate().GetInt(), 625);
            }
            catch(...) {
                TESTB("user_function_by_value", false);
            }
        }
        TestScore();
        poc();
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


