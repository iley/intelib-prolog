//   InteLib                                    http://www.intelib.org
//   The file tests/refal/t_eval.cpp
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
#include "sexpress/sexpress.hpp"
#include "sexpress/sstring.hpp"

#include "refal/refal.hpp"
#include "refal/rvars.hpp"
#include "refal/rclause.hpp"



void doprint(const SExpression *ref)
{
    printf("%s\n", ref ? ref->TextRepresentation().c_str() : "(unbound)");
}

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

RfFormConstructor R;

RfWith RWITH;

extern RfLibSymbol Prout;
extern RfLibSymbol Sub;

int main()
{
    try
    {
        poc();
        TestSection("RefalEvaluation");
        TestSubsection("clauses");
        {
            int before = SExpression::object_counter;
            {
                RfSymbol Sym("Sym");
                Sym || ((~R) ^ (R<Sym>R)) ;
            }
            TEST("no_leaks_due_backreferenced_symbols",
                 SExpression::object_counter, before);
        }
        TestSubsection("Palindrom");
        {
            RfVariable_S s1("s1");
            RfVariable_E e2("e2");
            RfSymbol Pal("Pal");

            Pal
            || (~R)         ^ (R|"True")
            || (R|s1)       ^ (R|"True")
            || (R|s1,e2,s1) ^ (R| R<Pal, e2>R )
            || (R|e2)       ^ (R|e2);

            TEST("palindrom_text_rep",
                 Pal->GetFunction().TextRepresentation().c_str(),
                 " = \"True\";\n"
                 "s1 = \"True\";\n"
                 "s1 e2 s1 = <Pal e2>;\n"
                 "e2 = e2"
                );

            RfReference res;

            res = (R| R<Pal, "b">R);
            res.Evaluate();
            TESTTR("palindrom1", res, "\"True\"");

            res = (R| R<Pal, "aba">R);
            res.Evaluate();
            TESTTR("palindrom3", res, "\"True\"");

            res = (R| R<Pal, "abcdefedcba">R);
            res.Evaluate();
            TESTTR("palindrom11", res, "\"True\"");
        }
        TestSubsection("WhereTest");
        {
          RfVariable_E e_1("e.1");
          RfVariable_S s_2("s.2");
          RfVariable_E e_3("e.3");
          RfVariable_E e_4("e.4");
          RfVariable_S s_5("s.5");
          RfVariable_E e_6("e.6");
          RfVariable_E e_Foo("e.Foo");

          RfSymbol Strange("Strange");
          RfSymbol IsGood("IsGood");
          RfSymbol False("False");

          Strange
          ||
          (R|e_1, s_2, e_3) & (R|R<IsGood, s_2>R) | (R|e_4, s_5, e_6) &
          (R|R<Sub, s_5, 1>R) | (R|7)  ^ (R|s_2)
          ||
          (R|e_Foo)       ^ (R|False);

          IsGood
          ||
          (R|1)   ^ (R|2)
          ||
          (R|2)   ^ (R|4)
          ||
          (R|3)   ^ (R|6)
          ||
          (R|4)   ^ (R|2, 4, 6, 8, 10, 12, 14, 16, 18, 20)
          ||
          (R|5)   ^ (R|10)
          ||
          (R|e_Foo)       ^ (R|False);

          RfReference ref = (R|R<Strange, 1, 2, 3, 4, 5>R);
          ref.Evaluate();
          TESTTR("where_test", ref, "4");
        }
        TestSubsection("WithTest");
        {
          RfVariable_E e_Foo("e.Foo");
          RfVariable_S s_1("s.1");
          RfVariable_E e_Bar("e.Bar");
          RfVariable_E e_New("e.New");

          RfSymbol Withtest("Withtest");
          RfSymbol Not("Not");

          RfSymbol True("True");
          RfSymbol False("False");

          Withtest
          ||
          (R|e_Foo, s_1, e_Bar) & (R|R<Not, s_1>R) |  RWITH(
          RWITH
          ||
          (R|False, e_Bar)        ^ (R|True, R<Prout, e_Bar>R)
          ||
          (R|False, e_New) & (R|R<Not, e_New>R) |  RWITH(
          RWITH
          ||
          (R|True)        ^ (R|True, e_Bar)
          ||
          (R|False)       ^ (R|R<Prout, False, e_Bar>R)
          )
          ||
          (R|e_Foo)       ^ (R|False)
          );

          Not
          ||
          (R|1)   ^ (R|False, 2)
          ||
          (R|2)   ^ (R|True)
          ||
          (R|3)   ^ (R|1);

          RfReference ref = (R|R<Withtest,1,2,3>R);
          ref.Evaluate();
          TESTTR("with_test", ref, "True 2 3");
        }
        TestSubsection("NothingToNothing");
        {
            RfVariable_S s1("s1");
            RfVariable_E e2("e2");
            RfSymbol fun("fun");

            fun || (~R)         ^ (~R);

            TEST("nothing2nothing_text_rep",
                 fun->GetFunction().TextRepresentation().c_str(),
                 " = "
                );

            RfReference res;

            res = (R| R<fun>R);
            res.Evaluate();
            TESTTR("nothing_to_nothing", res, "");
        }
        TestSubsection("Crash");
        {
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");
            RfVariable_E e_3("e_3");
            RfVariable_E e_4("e_4");

            RfSymbol BuildVariablesList("BuildVariablesList");
            RfSymbol RemoveDupes("RemoveDupes");

            SLabel Extern_("Extern_");
            SLabel Entry_("Entry_");
            SLabel Var_("Var_");
            SLabel Id_("Id_");

            BuildVariablesList
            || (R|(R|e_1), e_2, (R|Var_, e_3), e_4)
            ^ (R|R<BuildVariablesList, (R|e_1, (R|e_3)), e_4>R)
            || (R|(R|e_1), e_2)
            ^ (R|(R|R<RemoveDupes, e_1>R)) ;

            RemoveDupes || (~R)         ^ (~R);

            RfReference res((R<BuildVariablesList,
                             (~R), Extern_, (R|Id_, "xxx")>R));
            res.Evaluate();
            TESTTR("no_crash", res, "()");
        }
        TestScore();
    }
    catch(const IntelibX &ex)
    {
        printf("Caught IntelibX: %s\n", ex.Description());
        doprint(ex.Parameter().GetPtr());
        printf("\n");
    }
    catch(...)
    {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


