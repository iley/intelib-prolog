//   InteLib                                    http://www.intelib.org
//   The file sexpress/sstring.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
