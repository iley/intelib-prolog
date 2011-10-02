#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/custmem.hpp"

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
    GenericPlgReference(const GenericPlgReference<Y> &ref) : Super(ref) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif
};

class PlgResult;
class PlgExpressionImpl;
class PlgExpression : public GenericPlgReference<PlgExpressionImpl> 
{
public:
    typedef GenericPlgReference<PlgExpressionImpl> Super;

    PlgResult Solve() const;
};

class PlgResultImpl;
class PlgResult : public GenericPlgReference<PlgResultImpl> 
{
public:
    typedef GenericPlgReference<PlgResultImpl> Super;

    PlgResult();
    void Next();
};

class PlgConjuncitonImpl;
class PlgConjunciton : public GenericPlgReference<PlgConjuncitonImpl>
{
public:
    typedef GenericPlgReference<PlgConjuncitonImpl> Super;
};

class PlgDisjunctionImpl;
class PlgDisjunction : public GenericPlgReference<PlgDisjunctionImpl>
{
public:
    typedef GenericPlgReference<PlgDisjunctionImpl> Super;
};

class PlgCompoundTerm;
class PlgRuleImpl;
class PlgRule : public GenericPlgReference<PlgRuleImpl> 
{
public:
    typedef GenericPlgReference<PlgRuleImpl> Super;

    PlgRule(const PlgCompoundTerm &head, const PlgDisjunction &body);

    const PlgCompoundTerm &GetHead() const;
    const PlgDisjunction &GetBody() const;
};

inline PlgRule operator <<= (const PlgCompoundTerm &head, const PlgDisjunction &body) {
    return PlgRule(head, body);
}


class PlgTermImpl;
class PlgTerm : public GenericPlgReference<PlgTermImpl>
{
public:
    typedef GenericPlgReference<PlgTermImpl> Super;
};

class PlgAtomImpl;
class PlgAtom : public GenericPlgReference<PlgAtomImpl> 
{
public:
    typedef GenericPlgReference<PlgAtomImpl> Super;

    PlgAtom(const char *value);
    const char *GetName() const;
};

class PlgCompoundTermImpl;
class PlgCompoundTerm : public GenericPlgReference<PlgCompoundTermImpl> 
{
public:
    typedef GenericPlgReference<PlgCompoundTermImpl> Super;

    PlgCompoundTerm(const PlgAtom &functor, const SReference &args);
    const PlgAtom &GetFunctor() const;
    const SReference &GetArguments() const;
};

#endif //INTELIB_PROLOG_HPP_SENTRY
