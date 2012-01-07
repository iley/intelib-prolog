#ifndef INTELIB_PLGTEST_SENTRY
#define INTELIB_PLGTEST_SENTRY

#include <stdio.h>
#include "../tests.hpp"

#include "../../sexpress/sexpress.hpp"
#include "../../sexpress/sstring.hpp"
#include "../../prolog/prolog.hpp"
#include "../../prolog/utils.hpp"

void printContext(const PlgContext &context) {
    return;
    printf("--- context dump start ---\n");
    printf("%s", DumpContext(context).c_str());
    printf("---  context dump end  ---\n");
}

void Ok(PlgDatabase &db, const PlgReference &query, const PlgReference &var, const SReference &results) {
    PlgContinuation cont = db.Query(query);
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

void Ok(PlgDatabase &db, const PlgReference &query) {
    SListConstructor S;
    return Ok(db, query, *PTheEmptyList, (S| *PTheEmptyList));
}

void Fail(PlgDatabase &db, const PlgReference &query) {
    Ok(db, query, *PTheEmptyList, *PTheEmptyList);
}

namespace Hooks {
    void Call(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        printf("* Call %s\n", PlgTerm(functor, args)->TextRepresentation().c_str());
        printContext(cont.Context());
    }

    void Exit(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont, bool result) {
        printf("* %s %s\n", (result ? "Exit" : "Fail"), PlgTerm(functor, args)->TextRepresentation().c_str());
        printContext(cont.Context());
    }

    void UnifyCall(const PlgReference &left, const PlgReference &right, PlgContext &ctx) {
        printf("* Unify %s with %s\n", left->TextRepresentation().c_str(), right->TextRepresentation().c_str());
        printContext(ctx);
    }

    void UnifyExit(const PlgReference &left, const PlgReference &right, PlgContext &ctx, bool result) {
        printf("* Unification %s: %s with %s\n", (result ? "success" : "fail"), left->TextRepresentation().c_str(), right->TextRepresentation().c_str());
        printContext(ctx);
    }

    void EnableAll() {
        PlgGlobalHooks.Call = Call;
        PlgGlobalHooks.Exit = Exit;
        PlgGlobalHooks.UnifyCall = UnifyCall;
        PlgGlobalHooks.UnifyExit = UnifyExit;
    }
}

#endif
