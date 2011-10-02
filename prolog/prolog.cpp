#include "prolog.hpp"
#include "../sexpress/sstring.hpp"

IntelibX_not_a_prolog_object::IntelibX_not_a_prolog_object(SReference a_param) : IntelibX("Not a prolog object", a_param) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
template <class T>
SString GenericPlgReference<T>::TextRepresentation() const { 
    return ((SExpression*)this->GetPtr())->TextRepresentation(); 
}
#endif

// Prolog Solution Result : STUB

class PlgResultImpl : public SExpression 
{
    friend class PlgResult;
public:
    static IntelibTypeId TypeId;

    PlgResultImpl() : SExpression(TypeId) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG RESULT>"; } //TODO
#endif
};

IntelibTypeId PlgResultImpl::TypeId(&SExpression::TypeId, true);

PlgResult::PlgResult() : Super(new PlgResultImpl()) {}

void PlgResult::Next() 
{
    // TODO
}

// Abstract Prolog Expression

class PlgExpressionImpl : public SExpression 
{
    friend class PlgExpression;
public:
    static IntelibTypeId TypeId;

    virtual PlgResult Solve() { return PlgResult(); } //FIXME: STUB

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG EXPRESSION>"; } //TODO
#endif

protected:
    PlgExpressionImpl(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}
    virtual ~PlgExpressionImpl() = 0;
};

IntelibTypeId PlgExpressionImpl::TypeId(&SExpression::TypeId, false);

PlgExpressionImpl::~PlgExpressionImpl() {}

PlgResult PlgExpression::Solve() const 
{ 
    return (*this)->Solve(); 
}

// Prolog Disjunction

class PlgDisjunctionImpl : public PlgExpressionImpl
{
    friend class PlgDisjunction;
public:
    static IntelibTypeId TypeId;

protected:
    PlgDisjunctionImpl(const IntelibTypeId &typeId = TypeId) : PlgExpressionImpl(typeId) {}
};

IntelibTypeId PlgDisjunctionImpl::TypeId(&PlgExpressionImpl::TypeId, false);

// Prolog Conjunction

class PlgTermListImpl : public PlgDisjunctionImpl
{
    friend class PlgTermList;
public:
    static IntelibTypeId TypeId;

protected:
    PlgTermListImpl() : PlgDisjunctionImpl(TypeId) {}
};

IntelibTypeId PlgTermListImpl::TypeId(&PlgDisjunctionImpl::TypeId, false);

// Prolog Rule

class PlgRuleImpl : public SExpression 
{
    friend class PlgRule;
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

    PlgRuleImpl(const PlgCompoundTerm &head, const PlgDisjunction &body) 
        : SExpression(TypeId), head_(head), body_(body) {}
};

IntelibTypeId PlgRuleImpl::TypeId(&SExpression::TypeId, true);

PlgRule::PlgRule(const PlgCompoundTerm &head, const PlgDisjunction &body) : Super(new PlgRuleImpl(head, body)) {}

const PlgCompoundTerm &PlgRule::GetHead() const { return (*this)->GetHead(); }
const PlgDisjunction &PlgRule::GetBody() const { return (*this)->GetBody(); }

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

IntelibTypeId PlgTermImpl::TypeId(&PlgExpressionImpl::TypeId, false);

PlgTermImpl::~PlgTermImpl() {}

// Prolog Atom, immutable

class PlgAtomImpl : public PlgTermImpl {
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    const char *GetName() { return label_.GetPtr()->GetName(); }

private:
    SLabel label_;
    PlgAtomImpl(const char *name) : PlgTermImpl(TypeId), label_(name) {}
    ~PlgAtomImpl() {}
};

IntelibTypeId PlgAtomImpl::TypeId(&PlgTermImpl::TypeId, false);

PlgAtom::PlgAtom(const char *name) : Super(new PlgAtomImpl(name)) {}

const char *PlgAtom::GetName() const { return (*this)->GetName(); }

// Prolog Compound Term, i.e. functor + arguments, immutable

class PlgCompoundTermImpl : public PlgTermImpl 
{
    friend class PlgCompoundTerm;
public:
    static IntelibTypeId TypeId;

    const PlgAtom& GetFunctor() const { return functor_; }
    const SReference& GetArguments() const { return args_; }

private:
    PlgAtom functor_;
    SReference args_;

    PlgCompoundTermImpl(const PlgAtom &functor, const SReference &args) 
        : PlgTermImpl(TypeId), functor_(functor), args_(args) {}

    ~PlgCompoundTermImpl() {}
};

IntelibTypeId PlgCompoundTermImpl::TypeId(&PlgTermImpl::TypeId, false);

PlgCompoundTerm::PlgCompoundTerm(const PlgAtom &functor, const SReference &args)
    : Super(new PlgCompoundTermImpl(functor, args)) {}


const PlgAtom &PlgCompoundTerm::GetFunctor() const { return (*this)->GetFunctor(); }
const SReference &PlgCompoundTerm::GetArguments() const { return (*this)->GetArguments(); }

