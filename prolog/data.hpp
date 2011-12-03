#ifndef INTELIB_PROLOG_DATA_HPP_SENTRY
#define INTELIB_PROLOG_DATA_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/gensref.hpp"

#include "exceptions.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "../sexpress/sstring.hpp"
#endif

class PlgReference;
class PlgContext;
class PlgExpressionContinuation;

class PlgExpression : public SExpression
{
    friend class PlgReference;
public:
    static IntelibTypeId TypeId;

    PlgExpression(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual bool Solve(PlgExpressionContinuation &continuation) const;
};

typedef GenericSReference<PlgExpression, IntelibX_not_a_prolog_expression> PlgRef;

class PlgReference : public PlgRef
{
public:
    PlgReference() {}
    PlgReference(const PlgReference &ref) : PlgRef(ref) {}
    PlgReference(const SReference &ref) : PlgRef(ref) {}
    PlgReference(SExpression *p) : PlgRef(p) {}

    bool Unify(const PlgReference &other, PlgContext &context) const;
    bool Solve(PlgExpressionContinuation &continuation) const;

    ~PlgReference() {}
};

extern PlgReference PlgUnbound;

// Clause

class PlgExpressionClause : public PlgExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClause(const PlgReference &hd, const PlgReference &bd) : PlgExpression(TypeId), head(hd), body(bd) {}
    PlgReference Head() const { return head; }
    PlgReference Body() const { return body; }

private:
    PlgReference head;
    PlgReference body;
};

typedef GenericSReference<PlgExpressionClause, IntelibX_not_a_prolog_clause> PlgClause_Super;

class PlgClause : public PlgClause_Super
{
public:
    PlgClause(const PlgReference &hd, const PlgReference &bd) : PlgClause_Super(new PlgExpressionClause(hd, bd)) {}
};

// Atom

class PlgExpressionAtom : public PlgExpression
{
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    PlgExpressionAtom(const char *name) : PlgExpression(TypeId), label(name) {}
    const SLabel &Label() const { return label; }
    const char *GetName() const { return label.GetPtr()->GetName(); }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgExpressionAtom(const IntelibTypeId &typeId, const char *name) : PlgExpression(typeId), label(name) {}
    SLabel label;
};

typedef GenericSReference<PlgExpressionAtom, IntelibX_not_a_prolog_atom> PlgAtom_Super;

class PlgAtom : public PlgAtom_Super
{
public:
    PlgAtom(const char *name) : PlgAtom_Super(new PlgExpressionAtom(name)) {}

    PlgReference operator () (const PlgReference &arg1);
    PlgReference operator () (const PlgReference &arg1, const PlgReference &arg2);
    PlgReference operator () (const PlgReference &arg1, const PlgReference &arg2, const PlgReference &arg3);
    // TODO more args
};

// Variable Name

class PlgExpressionVariableName : public PlgExpressionAtom
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionVariableName(const char *name) : PlgExpressionAtom(TypeId, name) {}
    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
};

typedef GenericSReference<PlgExpressionVariableName, IntelibX_not_a_prolog_variable_name> PlgVariableName_Super;

class PlgVariableName : public PlgVariableName_Super
{
public:
    PlgVariableName(const char *name) : PlgVariableName_Super(new PlgExpressionVariableName(name)) {}
};

// Term

class PlgExpressionTerm : public PlgExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionTerm(const PlgAtom &fn, const SReference &as);

    const PlgAtom &Functor() const { return functor; }
    const SReference &Args() const { return args; }
    const int Arity() const { return arity; }

    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual bool Solve(PlgExpressionContinuation &continuation) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    PlgAtom functor;
    SReference args;
    int arity;
};

typedef GenericSReference<PlgExpressionTerm, IntelibX_not_a_prolog_atom> PlgTerm_Super;

class PlgTerm : public PlgTerm_Super
{
public:
    PlgTerm(const PlgAtom &functor, const SReference &args) : PlgTerm_Super(new PlgExpressionTerm(functor, args)) {}
};



class PlgExpressionList : public PlgExpression
{
public:
    static IntelibTypeId TypeId;

    const SReference& List() const { return list; }
protected:
    PlgExpressionList(const IntelibTypeId &typeId = TypeId, const SReference &ls = *PTheEmptyList) 
        : PlgExpression(typeId), list(ls) {};

    SReference list;
};

// Disjunction

class PlgExpressionDisjunction : public PlgExpressionList
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionDisjunction(const SReference &ls) : PlgExpressionList(TypeId, ls) {} 

    virtual bool Solve(PlgExpressionContinuation &continuation) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

typedef GenericSReference<PlgExpressionDisjunction, IntelibX_not_a_prolog_disjunction> PlgDisjunction_Super;

class PlgDisjunction : public PlgDisjunction_Super
{
public:
    PlgDisjunction(const SReference &ls) : PlgDisjunction_Super(new PlgExpressionDisjunction(ls)) {}
};

PlgDisjunction operator | (const PlgReference &left, const PlgReference &right);

// Conjunction

class PlgExpressionConjunction : public PlgExpressionList
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionConjunction(const SReference &ls) : PlgExpressionList(TypeId, ls) {}

    virtual bool Solve(PlgExpressionContinuation &continuation) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

typedef GenericSReference<PlgExpressionConjunction, IntelibX_not_a_prolog_conjunction> PlgConjunction_Super;

class PlgConjunction : public PlgConjunction_Super
{
public:
    PlgConjunction(const SReference &ls) : PlgConjunction_Super(new PlgExpressionConjunction(ls)) {}
};

PlgConjunction operator & (const PlgReference &left, const PlgReference &right);

#endif
