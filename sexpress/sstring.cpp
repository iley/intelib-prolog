// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <string.h>
#include "sstring.hpp"

//////////////////////////////////
// SString

#if 0
SString::SString(const SReference &ref)
    : SReference(ref)
{
    if(!ref.GetPtr() || 
        !ref->TermType().IsSubtypeOf(SExpressionString::TypeId)) 
    {
        throw IntelibX_not_a_string(ref);
    }
}
SString& SString::operator=(const SString &s)
{
    this->SReference::operator=(s);
    return *this;
}
#endif

SString& SString::operator=(const char *s)
{
    this->SReference::operator=(SReference(s));
    //this->SReference::operator=(new SExpressionString(s));
    return *this;
}

SString SString::operator+(const SString &other) const
{
    return SString(new SExpressionString(c_str(), other.c_str()));
}

void SString::operator+=(const SString &other)
{
    this->operator=(new SExpressionString(c_str(), other.c_str()));
}

SString SString::operator+(const char *s) const
{
    return SString(new SExpressionString(c_str(), s));
}

void SString::operator+=(const char *s)
{
    this->operator=(new SExpressionString(c_str(), s));
}

void SString::operator+=(const char c)
{
    char buf[2]; 
    buf[0] = c; buf[1] = 0;
    this->operator=(new SExpressionString(c_str(), buf));
}

const char* SString::c_str() const
{
    return (static_cast<const SExpressionString*>(GetPtr()))->GetValue();
}

bool SString::operator==(const SString &other) const
{
    return 0 == strcmp(c_str(), other.c_str());
}

bool SString::operator!=(const SString &other) const
{
    return !(*this == other);
}

unsigned int SString::length() const
{
    return strlen(c_str());
}
