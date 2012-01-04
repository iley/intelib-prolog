#ifndef INTELIB_PROLOG_DATA_HPP_SENTRY
#define INTELIB_PROLOG_DATA_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/svector.hpp"
#include "../sexpress/shashtbl.hpp"
#include "../sexpress/gensref.hpp"

#include "exceptions.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "../sexpress/sstring.hpp"
#endif

typedef const char *(*NameGeneratorFunction)();

class PlgReference;
class PlgContext;
class PlgContinuation;
class PlgExpressionContinuation;

class PlgObject
{
public:
    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual PlgReference RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const;
    virtual PlgReference Evaluate(const PlgReference &self, PlgContext &context) const;
};

typedef GenericSReference<SExpression, IntelibX_not_a_prolog_object> PlgRef;

class PlgReference : public PlgRef
{
public:
    PlgReference() {}
    PlgReference(const PlgReference &ref) : PlgRef(ref) {}
    PlgReference(const SReference &ref) : PlgRef(ref) {}
    PlgReference(SExpression *p) : PlgRef(p) {}

    bool Unify(const PlgReference &other, PlgContext &context) const;

    PlgReference RenameVars(PlgContext &context, SHashTable &existingVars) const {
        PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
        INTELIB_ASSERT(obj, IntelibX_not_a_prolog_object(*this));
        return obj->RenameVars(*this, context, existingVars);
    }

    PlgReference Evaluate(PlgContext &context) const {
        PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
        INTELIB_ASSERT(obj, IntelibX_not_a_prolog_object(*this));
        return obj->Evaluate(*this, context);
    }

    ~PlgReference() {}
};

extern PlgReference PlgUnbound;

// Clause

class PlgExpressionClause : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClause(const PlgReference &hd, const PlgReference &bd) : SExpression(TypeId), head(hd), body(bd) {}
    PlgReference Head() const { return head; }
    PlgReference Body() const { return body; }

    virtual PlgReference RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const;
    virtual PlgReference Evaluate(const PlgReference &self, PlgContext &context) const;

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

// Predicate

class PlgAtom;
class PlgExpressionPredicate : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    virtual bool Apply(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) = 0;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PREDICATE>"; }
#endif

protected:
    explicit PlgExpressionPredicate(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}
};

typedef bool (*UserPredicate)(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

class PlgExpressionUserPredicate : public PlgExpressionPredicate
{
public:
    explicit PlgExpressionUserPredicate(const UserPredicate &func) : function(func) {}

    virtual bool Apply(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

private:
    UserPredicate function;
};

typedef GenericSReference<PlgExpressionPredicate, IntelibX_not_a_prolog_predicate> PlgPredicate_Super;

class PlgPredicate : public PlgPredicate_Super
{
public:
    PlgPredicate() {}
    PlgPredicate(const UserPredicate &func) : PlgPredicate_Super(new PlgExpressionUserPredicate(func)) {}
    PlgPredicate(const SReference &sex) : PlgPredicate_Super(sex) {}
};

// Atom

class PlgExpressionAtom : public SExpressionLabel, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    explicit PlgExpressionAtom(const char *name, bool infix) : SExpressionLabel(TypeId, name), isInfix(infix) {}

    PlgExpressionAtom(const char *name, int arity, const PlgPredicate &pred, bool infix)
        : SExpressionLabel(TypeId, name), isInfix(infix) { SetPredicate(arity, pred); }

    PlgExpressionAtom(const char *name, const PlgPredicate &pred, bool infix)
        : SExpressionLabel(TypeId, name), isInfix(infix) { SetPredicate(pred); }

    PlgPredicate GetPredicate(int arity) const;

    // fixed-arity predicate
    void SetPredicate(int arity, const PlgPredicate &pred) { predicates[arity] = pred; }

    // variable-arity predicate
    void SetPredicate(const PlgPredicate &pred) { varArgPredicate = pred; }

    bool IsInfix() const { return isInfix; }

protected:
    PlgExpressionAtom(const IntelibTypeId &typeId, const char *name, bool infix) : SExpressionLabel(typeId, name), isInfix(infix) {}

    SVector predicates;
    PlgPredicate varArgPredicate;
    bool isInfix;
};

typedef GenericSReference<PlgExpressionAtom, IntelibX_not_a_prolog_atom> PlgAtom_Super;

class PlgAtom : public PlgAtom_Super
{
public:
    explicit PlgAtom(const char *name, bool infix = false) : PlgAtom_Super(new PlgExpressionAtom(name, infix)) {}
    explicit PlgAtom(const char *name, int arity, const PlgPredicate &pred, bool infix = false) : PlgAtom_Super(new PlgExpressionAtom(name, arity, pred, infix)) {}
    explicit PlgAtom(const char *name, const PlgPredicate &pred, bool infix = false) : PlgAtom_Super(new PlgExpressionAtom(name, pred, infix)) {}
    PlgAtom(const SReference &s) : PlgAtom_Super(s) {}

    PlgReference operator () (const PlgReference &arg1);
    PlgReference operator () (const PlgReference &arg1, const PlgReference &arg2);
    PlgReference operator () (const PlgReference &arg1, const PlgReference &arg2, const PlgReference &arg3);
    // TODO more args
};

// Variable Name

class PlgExpressionVariableName : public SExpressionLabel, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    explicit PlgExpressionVariableName(const char *name) : SExpressionLabel(TypeId, name) {}

    virtual PlgReference RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const;
};

typedef GenericSReference<PlgExpressionVariableName, IntelibX_not_a_prolog_variable_name> PlgVariableName_Super;

class PlgVariableName : public PlgVariableName_Super
{
public:
    explicit PlgVariableName(const char *name) : PlgVariableName_Super(new PlgExpressionVariableName(name)) {}
};

// Variable index

class PlgExpressionVariableIndex : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;
    explicit PlgExpressionVariableIndex(int val) : SExpression(TypeId), value(val) {}
    explicit PlgExpressionVariableIndex(PlgContext &ctx);

    int GetValue() const { return value; }
    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual PlgReference Evaluate(const PlgReference &self, PlgContext &context) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    int value;
};

typedef GenericSReference<PlgExpressionVariableIndex, IntelibX_not_a_prolog_variable_index> PlgVariableIndex_Super;

class PlgVariableIndex : public PlgVariableIndex_Super
{
public:
    explicit PlgVariableIndex(PlgContext &ctx) : PlgVariableIndex_Super(new PlgExpressionVariableIndex(ctx)) {}
    explicit PlgVariableIndex(int value) : PlgVariableIndex_Super(new PlgExpressionVariableIndex(value)) {}
    PlgVariableIndex(const SReference &s) : PlgVariableIndex_Super(s) {}
};

// Term

class PlgExpressionTerm : public SExpression, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionTerm(const PlgAtom &fn, const SReference &as);

    const PlgAtom &Functor() const { return functor; }
    const SReference &Args() const { return args; }
    int Arity() const;

    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual PlgReference RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const;
    virtual PlgReference Evaluate(const PlgReference &self, PlgContext &context) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgExpressionTerm(const IntelibTypeId &typeId, const PlgAtom &fn, const SReference &as);

    PlgAtom functor;
    SReference args;
};

typedef GenericSReference<PlgExpressionTerm, IntelibX_not_a_prolog_atom> PlgTerm_Super;

class PlgTerm : public PlgTerm_Super
{
public:
    PlgTerm(const PlgAtom &functor, const SReference &args) : PlgTerm_Super(new PlgExpressionTerm(functor, args)) {}
    PlgTerm(const SReference &other) : PlgTerm_Super(other) {}
};


#endif
