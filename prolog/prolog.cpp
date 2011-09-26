#include "prolog.hpp"
#include "../sexpress/sstring.hpp"

IntelibX_not_a_prolog_object::IntelibX_not_a_prolog_object(SReference a_param) : IntelibX("Not a prolog object", a_param) {}

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


class PlgRuleImpl : public SExpression 
{
    friend class PlgRule;
public:
    static IntelibTypeId TypeId;

    const PlgTerm &GetSignature() const { return signature_; }
    const PlgTerm &GetBody() const { return body_; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG RULE>"; } //TODO
#endif

private:
    PlgTerm signature_, body_;

    PlgRuleImpl(const PlgTerm &signature, const PlgTerm &body) :
        SExpression(TypeId), signature_(signature), body_(body) {}
};

IntelibTypeId PlgRuleImpl::TypeId(&SExpression::TypeId, true);

PlgRule::PlgRule(const PlgTerm &signature, const PlgTerm &body) : Super(new PlgRuleImpl(signature, body)) {}

class PlgTermImpl : public SExpression 
{
public:
    static IntelibTypeId TypeId;

    PlgResult Solve() { return PlgResult(); } //TODO

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const { return "<PROLOG TERM>"; } //TODO
#endif
};

IntelibTypeId PlgTermImpl::TypeId(&SExpression::TypeId, true);

PlgResult PlgTerm::Solve() const 
{ 
    return (*this)->Solve(); 
}

PlgRule PlgTerm::operator <<= (const PlgTerm &body)
{
    return PlgRule(*this, body);
}
