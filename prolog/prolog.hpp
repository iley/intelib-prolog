#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/custmem.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "../sexpress/sstring.hpp"
#endif

class IntelibX_not_a_prolog_object : public IntelibX
{
public:
    IntelibX_not_a_prolog_object(SReference a_param);
};

#include "plgrefs.hpp"

// Abstract Prolog Expression
class PlgExpressionImpl : public SExpression 
{
    friend class PlgExpression;
public:
    static IntelibTypeId TypeId;

    //virtual PlgResult Solve() { return PlgResult(); } //FIXME: STUB

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG EXPRESSION>"; } //TODO
#endif

protected:
    PlgExpressionImpl(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}
    virtual ~PlgExpressionImpl() = 0;
};

// Prolog Term
class PlgTermImpl : public PlgExpressionImpl
{
    friend class PlgTerm;
public:
    static IntelibTypeId TypeId;
protected:
    PlgTermImpl(const IntelibTypeId &typeId = TypeId) : PlgExpressionImpl(typeId) {}
    virtual ~PlgTermImpl() = 0;
};

// Prolog Disjunction
class PlgDisjunctionImpl : public PlgExpressionImpl
{
    friend class PlgDisjunction;
public:
    static IntelibTypeId TypeId;

    const PlgConjunction &GetHead() const { return head_; }
    const PlgDisjunction &GetTail() const { return tail_; }
    PlgDisjunction Append(const PlgDisjunction &rest) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgDisjunctionImpl(const PlgConjunction &head, const IntelibTypeId &typeId = TypeId)
        : PlgExpressionImpl(typeId), head_(head), tail_() {}

    PlgDisjunctionImpl(const PlgConjunction &head, const PlgDisjunction &tail, const IntelibTypeId &typeId = TypeId)
        : PlgExpressionImpl(typeId), head_(head), tail_(tail)  {}

private:
    PlgConjunction head_;
    PlgDisjunction tail_;
};

// Prolog Conjunction
class PlgConjunctionImpl : public PlgExpressionImpl
{
    friend class PlgConjunction;
public:
    static IntelibTypeId TypeId;

    const PlgTerm &GetHead() const { return head_; }
    const PlgConjunction &GetTail() const { return tail_; }
    PlgConjunction Append(const PlgConjunction &rest) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgConjunctionImpl(const PlgTerm &head, const IntelibTypeId &typeId = TypeId)
        : PlgExpressionImpl(typeId), head_(head), tail_() {}

    PlgConjunctionImpl(const PlgTerm &head, const PlgConjunction &tail, const IntelibTypeId &typeId = TypeId)
        : PlgExpressionImpl(typeId), head_(head), tail_(tail)  {}

private:
    PlgTerm head_;
    PlgConjunction tail_;
};

// Prolog Clause
class PlgClauseImpl : public PlgExpressionImpl 
{
    friend class PlgClause;
public:
    static IntelibTypeId TypeId;

    const PlgCompoundTerm &GetHead() const { return head_; }
    const PlgDisjunction &GetBody() const { return body_; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG RULE>"; } //TODO
#endif

private:
    PlgCompoundTerm head_;
    PlgDisjunction body_;

    PlgClauseImpl(const PlgCompoundTerm &head, const PlgDisjunction &body) 
        : PlgExpressionImpl(TypeId), head_(head), body_(body) {}
};

// Prolog Atom, immutable
class PlgAtomImpl : public PlgTermImpl
{
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    const char *GetName() { return ((SExpressionLabel&)*label_).GetName(); }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return label_->TextRepresentation(); }
#endif

private:
    SLabel label_;
    PlgAtomImpl(const char *name) : PlgTermImpl(TypeId), label_(name) {}
    ~PlgAtomImpl() {}
};

// Prolog variable
class PlgVarImpl : public PlgTermImpl
{
    friend class PlgVar;
public:
    static IntelibTypeId TypeId;

    const char *GetName() { return ((SExpressionLabel&)*label_).GetName(); }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return label_->TextRepresentation(); }
#endif

private:
    SLabel label_;
    PlgVarImpl(const char *name) : PlgTermImpl(TypeId), label_(name) {}
    ~PlgVarImpl() {}
};

// Prolog Compound Term, i.e. functor(arguments), immutable
class PlgCompoundTermImpl : public PlgTermImpl 
{
    friend class PlgCompoundTerm;
public:
    static IntelibTypeId TypeId;

    const PlgAtom& GetFunctor() const { return functor_; }
    const SReference& GetArguments() const { return args_; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return functor_->TextRepresentation() + args_->TextRepresentation(); }
#endif

private:
    PlgAtom functor_;
    SReference args_;

    PlgCompoundTermImpl(const PlgAtom &functor, const SReference &args) 
        : PlgTermImpl(TypeId), functor_(functor), args_(args) {}

    ~PlgCompoundTermImpl() {}
};

#endif //INTELIB_PROLOG_HPP_SENTRY
