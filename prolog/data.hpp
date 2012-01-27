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

class PlgAtom;
class PlgReference;
class PlgContext;
class PlgContinuation;
class PlgExpressionContinuation;

struct PlgHooks {
    typedef void (*PredicateCallFunc)(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);
    typedef void (*PredicateExitFunc)(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont, bool result);
    typedef void (*UnifyCallFunc)(const PlgReference &left, const PlgReference &right, PlgContext &ctx);
    typedef void (*UnifyExitFunc)(const PlgReference &left, const PlgReference &right, PlgContext &ctx, bool result);
    typedef void (*RenameFunc)(const PlgReference &from, const PlgReference &to, PlgContext &ctx);
    typedef void (*EvaluateFunc)(const PlgReference &object, PlgContext &ctx);

    PredicateCallFunc Call;
    PredicateExitFunc Exit;
    UnifyCallFunc UnifyCall;
    UnifyExitFunc UnifyExit;
    RenameFunc Rename;
    EvaluateFunc Evaluate;

    PlgHooks() : Call(0), Exit(0), UnifyCall(0), UnifyExit(0), Rename(0) {}
};

extern PlgHooks PlgGlobalHooks;

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

    PlgReference RenameVars(PlgContext &context, SHashTable &existingVars) const;
    PlgReference Evaluate(PlgContext &context) const;

    PlgReference Functor() const;
    PlgReference Head() const;
    PlgReference Body() const;

    ~PlgReference() {}
};

extern PlgReference PlgUnbound;

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

class PlgExpressionAtom : public SExpressionString, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    explicit PlgExpressionAtom(const char *name, bool infix) : SExpressionString(TypeId, name), isInfix(infix) {}

    PlgPredicate GetPredicate(int arity) const;

    // fixed-arity predicate
    void SetPredicate(int arity, const PlgPredicate &pred) { predicates[arity] = pred; }

    // variable-arity predicate
    void SetPredicate(const PlgPredicate &pred) { varArgPredicate = pred; }

    bool IsInfix() const { return isInfix; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    SVector predicates;
    PlgPredicate varArgPredicate;
    bool isInfix;

    PlgExpressionAtom(const IntelibTypeId &typeId, const char *name, bool infix) : SExpressionString(typeId, name), isInfix(infix) {}
    virtual bool SpecificEql(const SExpression* other) const;
};

typedef GenericSReference<PlgExpressionAtom, IntelibX_not_a_prolog_atom> PlgAtom_Super;

class PlgAtom : public PlgAtom_Super
{
public:
    explicit PlgAtom(const char *name, bool infix = false);
    explicit PlgAtom(const char *name, int arity, const PlgPredicate &pred, bool infix = false);
    explicit PlgAtom(const char *name, const PlgPredicate &pred, bool infix = false);

    PlgAtom(const SReference &s) : PlgAtom_Super(s) {}

    PlgReference operator () (const SReference &arg1);
    PlgReference operator () (const SReference &arg1, const SReference &arg2);
    PlgReference operator () (const SReference &arg1, const SReference &arg2, const SReference &arg3);
    PlgReference operator () (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4);
    PlgReference operator () (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4, const SReference &arg5);
    // TODO more args

private:
    void Init(const char *name, bool infix);
};

// Variable Name

class PlgExpressionVariable : public SExpressionString, public PlgObject
{
public:
    static IntelibTypeId TypeId;

    explicit PlgExpressionVariable(const char *name) : SExpressionString(TypeId, name) {}

    virtual PlgReference RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const;
    virtual bool Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const;
    virtual PlgReference Evaluate(const PlgReference &self, PlgContext &context) const;

    virtual bool SpecificEql(const SExpression* other) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
};

typedef GenericSReference<PlgExpressionVariable, IntelibX_not_a_prolog_variable_name> PlgVariable_Super;

class PlgVariable : public PlgVariable_Super
{
public:
    explicit PlgVariable(const char *name);
    PlgVariable(const SReference &s) : PlgVariable_Super(s) {}

    PlgReference is(const PlgReference &expr);
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
