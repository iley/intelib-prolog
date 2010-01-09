//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/permut/perm.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#ifndef PERM_HPP_SENTRY
#define PERM_HPP_SENTRY


#include <intelib/sexpress/sexpress.hpp>
#include <intelib/genlisp/lispform.hpp>
#include <intelib/lisp/lisp.hpp>
#include <intelib/lisp/lsymbol.hpp>

class SExpressionPermGen : public SExpression {
    class PermutationGenerator *pg;
public:
    static IntelibTypeId TypeId;

    SExpressionPermGen(int n);
    ~SExpressionPermGen();

    virtual SString TextRepresentation() const;

    class PermutationGenerator *Get() const;
};


class LFunctionCreatepg : public SExpressionFunction {
public:
    LFunctionCreatepg() : SExpressionFunction(1, 1) {}
    virtual void DoApply(int paramsc, 
                         const SReference paramsv[],
                         IntelibContinuation &lf) const;
    virtual SString TextRepresentation() const;
};


class LFunctionNextpermutation : public SExpressionFunction {
public:
    LFunctionNextpermutation() : SExpressionFunction(1, 1) {}
    virtual void DoApply(int paramsc, 
                         const SReference paramsv[],
                         IntelibContinuation &lf) const;
    virtual SString TextRepresentation() const;
};

#endif
