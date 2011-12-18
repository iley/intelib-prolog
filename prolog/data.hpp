#ifndef INTELIB_PROLOG_DATA_HPP_SENTRY
#define INTELIB_PROLOG_DATA_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/shashtbl.hpp"
#include "../sexpress/gensref.hpp"

#include "exceptions.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "../sexpress/sstring.hpp"
#endif

typedef const char *(*NameGeneratorFunction)();

class PlgReference;
class PlgContext;
class PlgExpressionContinuation;

class PlgObject
{
public:
    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;
};

typedef GenericSReference<SExpression, IntelibX_not_a_prolog_expression> PlgRef;

class PlgReference : public PlgRef
{
public:
    PlgReference() {}
    PlgReference(const PlgReference &ref) : PlgRef(ref) {}
    PlgReference(const SReference &ref) : PlgRef(ref) {}
    PlgReference(SExpression *p) : PlgRef(p) {}

    bool Unify(const PlgReference &other, PlgContext &context) const;

    bool Solve(PlgExpressionContinuation &continuation) const {
        PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
        INTELIB_ASSERT(obj, IntelibX_bug()); //FIXME: use proper exception type
        return obj->Solve(*this, continuation);
    }

    virtual PlgReference RenameVars(NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
        PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
        INTELIB_ASSERT(obj, IntelibX_bug()); //FIXME: use proper exception type
        return obj->RenameVars(*this, nameGenerator, existingVars);
    }

    ~PlgReference() {}
};

extern PlgReference PlgUnbound;

class PlgExpressionTruthValue : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionTruthValue() : SExpression(TypeId) {}

    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "true"; }
#endif
};

typedef GenericSReference<PlgExpressionTruthValue, IntelibX_not_a_prolog_truth> PlgTruthValue_Super;

class PlgTruthValue : public PlgTruthValue_Super
{
public:
    PlgTruthValue() : PlgTruthValue_Super(new PlgExpressionTruthValue()) {}
};

extern PlgTruthValue PlgTrue;

// Clause

class PlgExpressionClause : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClause(const PlgReference &hd, const PlgReference &bd) : SExpression(TypeId), head(hd), body(bd) {}
    PlgReference Head() const { return head; }
    PlgReference Body() const { return body; }
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    PlgReference head;
    PlgReference body;
};

typedef GenericSReference<PlgExpressionClause, IntelibX_not_a_prolog_clause> PlgClause_Super;

class PlgClause : public PlgClause_Super
{
public:
    PlgClause(const PlgReference &hd, const PlgReference &bd) : PlgClause_Super(new PlgExpressionClause(hd, bd)) {}
    PlgClause(const SReference &sex) : PlgClause_Super(sex) {}
};

PlgClause operator <<= (const PlgReference &head, const PlgReference &body);

// Atom

class PlgExpressionAtom : public SExpressionLabel, public PlgObject
{
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    PlgExpressionAtom(const char *name) : SExpressionLabel(TypeId, name) {}

protected:
    PlgExpressionAtom(const IntelibTypeId &typeId, const char *name) : SExpressionLabel(typeId, name) {}
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
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;
};

typedef GenericSReference<PlgExpressionVariableName, IntelibX_not_a_prolog_variable_name> PlgVariableName_Super;

class PlgVariableName : public PlgVariableName_Super
{
public:
    explicit PlgVariableName(const char *name) : PlgVariableName_Super(new PlgExpressionVariableName(name)) {}
};

// Term

class PlgExpressionTerm : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionTerm(const PlgAtom &fn, const SReference &as);

    const PlgAtom &Functor() const { return functor; }
    const SReference &Args() const { return args; }
    const int Arity() const { return arity; }

    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

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



class PlgExpressionList : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    const SReference& List() const { return list; }
protected:
    PlgExpressionList(const IntelibTypeId &typeId = TypeId, const SReference &ls = *PTheEmptyList) 
        : SExpression(typeId), list(ls) {};

    SReference list;
};

// Disjunction

class PlgExpressionDisjunction : public PlgExpressionList
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionDisjunction(const SReference &ls) : PlgExpressionList(TypeId, ls) {} 

    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

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

    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

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
