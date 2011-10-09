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

template <class T>
class GenericPlgReference : public GenericSReference<T, IntelibX_not_a_prolog_object>
{
public:
    typedef GenericSReference<T, IntelibX_not_a_prolog_object> Super;

    GenericPlgReference(T* ptr) : Super(ptr) {}

    template <class Y>
    GenericPlgReference(const GenericPlgReference<Y> &ref) : Super((const GenericPlgReference<T>&)ref) {}
    //TODO: check statically if Y is subtype of T
};

// Reference types

class PlgExpressionImpl;
class PlgExpression : public GenericPlgReference<PlgExpressionImpl> 
{
public:
    typedef GenericPlgReference<PlgExpressionImpl> Super;

    //PlgResult Solve() const;
};

class PlgDisjunctionImpl;
class PlgDisjunction : public GenericPlgReference<PlgDisjunctionImpl>
{
public:
    typedef GenericPlgReference<PlgDisjunctionImpl> Super;
};

class PlgConjunctionImpl;
class PlgConjunction : public GenericPlgReference<PlgConjunctionImpl>
{
public:
    typedef GenericPlgReference<PlgConjunctionImpl> Super;
};

class PlgTermImpl;
class PlgTerm : public GenericPlgReference<PlgTermImpl>
{
public:
    typedef GenericPlgReference<PlgTermImpl> Super;

    template <class Y>
    PlgTerm(const GenericPlgReference<Y> &ref) : Super(ref) {}
};

class PlgAtom;
class PlgCompoundTermImpl;
class PlgCompoundTerm : public GenericPlgReference<PlgCompoundTermImpl> 
{
public:
    typedef GenericPlgReference<PlgCompoundTermImpl> Super;

    PlgCompoundTerm(const PlgAtom &functor, const SReference &args);
    const PlgAtom &GetFunctor() const;
    const SReference &GetArguments() const;
};

class PlgAtomImpl;
class PlgAtom : public GenericPlgReference<PlgAtomImpl> 
{
public:
    typedef GenericPlgReference<PlgAtomImpl> Super;

    PlgAtom(const char *value);
    const char *GetName() const;

    PlgCompoundTerm operator() (const PlgTerm &term1);
    PlgCompoundTerm operator() (const PlgTerm &term1, const PlgTerm &term2);
    PlgCompoundTerm operator() (const PlgTerm &term1, const PlgTerm &term2, const PlgTerm &term3);
    //TODO: continue
};

class PlgClauseImpl;
class PlgClause : public GenericPlgReference<PlgClauseImpl> 
{
public:
    typedef GenericPlgReference<PlgClauseImpl> Super;

    PlgClause(const PlgCompoundTerm &head, const PlgDisjunction &body);

    const PlgCompoundTerm &GetHead() const;
    const PlgDisjunction &GetBody() const;
};

inline PlgClause operator <<= (const PlgCompoundTerm &head, const PlgDisjunction &body) {
    return PlgClause(head, body);
}

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

// Prolog Disjunction
class PlgDisjunctionImpl : public PlgExpressionImpl
{
    friend class PlgDisjunction;
public:
    static IntelibTypeId TypeId;

protected:
    PlgDisjunctionImpl(const IntelibTypeId &typeId = TypeId) : PlgExpressionImpl(typeId) {}
};

// Prolog Conjunction
class PlgConjunctionImpl : public PlgDisjunctionImpl
{
    friend class PlgConjunction;
public:
    static IntelibTypeId TypeId;

protected:
    PlgConjunctionImpl() : PlgDisjunctionImpl(TypeId) {}
};

PlgDisjunction operator|| (const PlgDisjunction&, const PlgDisjunction&);
PlgConjunction operator&& (const PlgConjunction&, const PlgConjunction&);

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

// Prolog Atom, immutable
class PlgAtomImpl : public PlgTermImpl {
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

// Prolog Compound Term, i.e. functor(arguments), immutable
class PlgCompoundTermImpl : public PlgTermImpl 
{
    friend class PlgCompoundTerm;
public:
    static IntelibTypeId TypeId;

    const PlgAtom& GetFunctor() const { return functor_; }
    const SReference& GetArguments() const { return args_; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return functor_->TextRepresentation() + " :- " + args_->TextRepresentation(); }
#endif

private:
    PlgAtom functor_;
    SReference args_;

    PlgCompoundTermImpl(const PlgAtom &functor, const SReference &args) 
        : PlgTermImpl(TypeId), functor_(functor), args_(args) {}

    ~PlgCompoundTermImpl() {}
};

#endif //INTELIB_PROLOG_HPP_SENTRY
