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
class GenericPlgReference : public GenericSReference<T, IntelibX_not_a_prolog_object> {
public:
    typedef GenericSReference<T, IntelibX_not_a_prolog_object> Super;

    GenericPlgReference(T* ptr) : Super(ptr) {}

    template <class Y>
    GenericPlgReference(const GenericPlgReference<Y> &ref) : Super(ref) {}
};

class PlgResultImpl;
class PlgResult : public GenericPlgReference<PlgResultImpl> {
public:
    typedef GenericPlgReference<PlgResultImpl> Super;

    PlgResult();
    void Next();
};

class PlgTerm;
class PlgRuleImpl;
class PlgRule : public GenericPlgReference<PlgRuleImpl> {
public:
    typedef GenericPlgReference<PlgRuleImpl> Super;

    PlgRule(const PlgTerm &signature, const PlgTerm &body);
};

class PlgTermImpl;
class PlgTerm : public GenericPlgReference<PlgTermImpl> {
public:
    typedef GenericPlgReference<PlgTermImpl> Super;

    PlgResult Solve() const;
    PlgRule operator <<= (const PlgTerm &body);
};

class PlgAtomImpl;
class PlgAtom : public GenericPlgReference<PlgAtomImpl> {
public:
    typedef GenericPlgReference<PlgAtomImpl> Super;
};

#endif //INTELIB_PROLOG_HPP_SENTRY
