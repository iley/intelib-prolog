#include "prolog.hpp"
#include "../sexpress/sstring.hpp"

IntelibX_not_a_prolog_object::IntelibX_not_a_prolog_object(SReference a_param) : IntelibX("Not a prolog object", a_param) {}

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

// Prolog Rule

class PlgRuleImpl : public SExpression 
{
    friend class PlgRule;
public:
    static IntelibTypeId TypeId;

    const PlgTerm &GetHead() const { return head_; }
    const PlgTerm &GetBody() const { return body_; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG RULE>"; } //TODO
#endif

private:
    PlgTerm head_, body_;

    PlgRuleImpl(const PlgTerm &head, const PlgTerm &body) :
        SExpression(TypeId), head_(head), body_(body) {}
};

IntelibTypeId PlgRuleImpl::TypeId(&SExpression::TypeId, true);

PlgRule::PlgRule(const PlgTerm &head, const PlgTerm &body) : Super(new PlgRuleImpl(head, body)) {}

const PlgTerm &PlgRule::GetHead() const { return (*this)->GetHead(); }
const PlgTerm &PlgRule::GetBody() const { return (*this)->GetBody(); }

// Abstract Prolog Term, mutable ?

class PlgTermImpl : public SExpression 
{
public:
    static IntelibTypeId TypeId;

    PlgTermImpl(const IntelibTypeId &typeId) : SExpression(typeId) {}
    PlgTermImpl() : SExpression(TypeId) {}
    virtual PlgResult Solve() { return PlgResult(); } //FIXME: STUB
    virtual ~PlgTermImpl() = 0;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG TERM>"; } //TODO
#endif
};

IntelibTypeId PlgTermImpl::TypeId(&SExpression::TypeId, true);

PlgTermImpl::~PlgTermImpl() {}

PlgResult PlgTerm::Solve() const 
{ 
    return (*this)->Solve(); 
}

PlgRule PlgTerm::operator <<= (const PlgTerm &body)
{
    return PlgRule(*this, body);
}

// Prolog Atom, immutable

class PlgAtomImpl : public PlgTermImpl {
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    const char *GetName() { return label_.GetPtr()->GetName(); }

private:
    SLabel label_;

    PlgAtomImpl(const char *name) : PlgTermImpl(TypeId), label_(name) {}
};

IntelibTypeId PlgAtomImpl::TypeId(&PlgTermImpl::TypeId, false);

PlgAtom::PlgAtom(const char *name) : Super(new PlgAtomImpl(name)) {}

const char *PlgAtom::GetName() const { return (*this)->GetName(); }

// Prolog Compound Term, i.e. functor + arguments, immutable

class PlgCompoundTermImpl : public PlgTermImpl {
    friend class PlgCompoundTerm;
public:
    static IntelibTypeId TypeId;

    PlgCompoundTermImpl(const PlgAtom &functor, const SReference &args) 
        : PlgTermImpl(TypeId), functor_(functor), args_(args) {}

    ~PlgCompoundTermImpl() {}

    const PlgAtom& GetFunctor() const { return functor_; }
    const SReference& GetArguments() const { return args_; }

private:
    PlgAtom functor_;
    SReference args_;
};

PlgCompoundTerm::PlgCompoundTerm(const PlgAtom &functor, const SReference &args)
    : Super(new PlgCompoundTermImpl(functor, args)) {}


const PlgAtom &PlgCompoundTerm::GetFunctor() const { return (*this)->GetFunctor(); }
const SReference &PlgCompoundTerm::GetArguments() const { return (*this)->GetArguments(); }

IntelibTypeId PlgCompoundTermImpl::TypeId(&PlgTermImpl::TypeId, false);
