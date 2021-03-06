#ifndef INTELIB_PLGTEST_SENTRY
#define INTELIB_PLGTEST_SENTRY

#include <stdio.h>
#include "../tests.hpp"

#include "../../sexpress/sexpress.hpp"
#include "../../sexpress/sstring.hpp"
#include "../../prolog/prolog.hpp"
#include "../../prolog/plgutils.hpp"

bool PrintContextEnabled = false;

void printContext(const PlgContext &context)
{
    if (PrintContextEnabled)
        printf("%s\n", DumpContext(context).c_str());
}

void Ok(const PlgReference &query, const PlgReference &var, const SReference &results)
{
    PlgContinuation cont = query.Query();
    for (SReference p = results; !p.IsEmptyList(); p = p.Cdr()) {
        bool result = cont->Next();
        TESTB((SString("solving ") + query->TextRepresentation()).c_str(), result);
        printContext(cont->Context());
        if (var.GetPtr()) {
            PlgReference expectedResult = p.Car();
            PlgReference value = cont->GetValue(var);
            TESTTR((SString("evaluate ") + var->TextRepresentation() + SString(" to ") + expectedResult->TextRepresentation()).c_str(),
                value,
                expectedResult->TextRepresentation().c_str());
        }
    }
    TESTB((SString("fail ") + query->TextRepresentation()).c_str(), !cont->Next());
    printContext(cont->Context());
}

void Ok(const PlgReference &query)
{
    SListConstructor S;
    return Ok(query, *PTheEmptyList, (S| *PTheEmptyList));
}

void Fail(const PlgReference &query)
{
    Ok(query, *PTheEmptyList, *PTheEmptyList);
}

namespace Hooks
{
    void Call(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        printf("* Call %s\n", PlgTerm(functor, args)->TextRepresentation().c_str());
        printContext(cont.Context());
    }

    void Exit(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont, bool result)
    {
        printf("* %s %s\n", (result ? "Exit" : "Fail"), PlgTerm(functor, args)->TextRepresentation().c_str());
        printContext(cont.Context());
    }

    void UnifyCall(const PlgReference &left, const PlgReference &right, PlgContext &ctx)
    {
        printf("* Unify %s with %s\n", left->TextRepresentation().c_str(), right->TextRepresentation().c_str());
        printContext(ctx);
    }

    void UnifyExit(const PlgReference &left, const PlgReference &right, PlgContext &ctx, bool result)
    {
        printf("* Unification %s: %s with %s\n",
                (result ? "success" : "fail"),
                left->TextRepresentation().c_str(),
                right->TextRepresentation().c_str()
        );
        printContext(ctx);
    }

    void Rename(const PlgReference &from, const PlgReference &to, PlgContext &ctx)
    {
        printf("* Rename %s --> %s\n", from->TextRepresentation().c_str(), to->TextRepresentation().c_str());
        printContext(ctx);
    }

    void Evaluate(const PlgReference &object, PlgContext &ctx)
    {
        printf("* Evaluate %s\n", object->TextRepresentation().c_str());
        printContext(ctx);
    }

    void EnableMain()
    {
        PlgGlobalHooks.Call = Call;
        PlgGlobalHooks.Exit = Exit;
        PlgGlobalHooks.UnifyCall = UnifyCall;
        PlgGlobalHooks.UnifyExit = UnifyExit;
    }

    void EnableAux()
    {
        PlgGlobalHooks.Rename = Rename;
        PlgGlobalHooks.Evaluate = Evaluate;
    }

    void EnableAll()
    {
        EnableMain();
        EnableAux();
    }
}

#endif
