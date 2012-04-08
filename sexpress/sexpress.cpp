//   InteLib                                    http://www.intelib.org
//   The file sexpress/sexpress.cpp
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
#include <stdlib.h>

#include "../version.h"

#include "iexcept.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#include <stdio.h>
#include "sstring.hpp"
#endif

/* ---------------------------------------------------------------------- */
/* Version identification and copyright information. DON'T REMOVE !!!     */


#ifndef INTELIB_VERSION
# error The symbol INTELIB_VERSION must be defined
#endif

const char the_intelib_title[] =
    "The   I n t e L i b   library version " INTELIB_VERSION;

const char the_intelib_copyright[] =
    "Copyright (c) Andrey Vikt. Stolyarov, 1999-2009.\n"
    "Copyright (c) Andrey Vikt. Stolyarov et al, 2010";

const char the_intelib_license[] =
    "This is free software. You may copy and distribute it as well as any "
    "software developed using it under the terms and conditions of the "
    "GNU General Public License, version 2. "
    "The files that form the library as such are also available under the "
    "terms and conditions of GNI Lesser General Public License, vers. 2.1. "
    "See the file COPYING for details. "
    " !!! WARNING: There's NO WARRANTY of any kind !!!";

const int the_intelib_version_id = INTELIB_VERSIONID;

/* ---------------------------------------------------------------------- */

// IntelibTypeId implementation

bool IntelibTypeId::IsSubtypeOf(const IntelibTypeId &op) const
{
    if(&op == this) {
        return true;
    } else if (!prev) {
        return false;
    } else {
        return prev->IsSubtypeOf(op);
    }
}

// SExpression class

IntelibTypeId SExpression::TypeId;


SExpression::SExpression(const IntelibTypeId &the_type)
{
    term_type_id = &the_type;
#if INTELIB_DEBUG_COUNTERS == 1
    object_counter++;
    term_type_id->object_counter++;
#endif
}

SExpression::~SExpression()
{
#if INTELIB_DEBUG_COUNTERS == 1
    object_counter--;
    term_type_id->object_counter--;
#endif
}


#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpression::TextRepresentation() const
{
    return SString("#<UNKNOWN_TERM>");
}
#endif




#if INTELIB_DEBUG_COUNTERS == 1
long SExpression::object_counter = 0;
#endif


// SExpressionInt class

IntelibTypeId SExpressionInt::TypeId(&SExpression::TypeId);

bool SExpressionInt::SpecificEql(const SExpression *expr) const
{
    return data == static_cast<const SExpressionInt*>(expr)->data;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionInt::TextRepresentation() const
{
    static char buf[40];
    //snprintf(buf, sizeof(buf), INTELIB_INTEGER_FORMAT, data);
    sprintf(buf, INTELIB_INTEGER_FORMAT, data);
    return SString(buf);
}
#endif

// SExpressionFloat class

IntelibTypeId SExpressionFloat::TypeId(&SExpression::TypeId);

bool SExpressionFloat::SpecificEql(const SExpression *expr) const
{
    /*INTELIB_ASSERT(ref->TermType() == TypeId, LispX_bug());*/
    return data == static_cast<const SExpressionFloat*>(expr)->data;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionFloat::TextRepresentation() const
{
    char buf[50];
    //snprintf(buf, sizeof(buf), INTELIB_FLOAT_FORMAT, data);
    sprintf(buf, INTELIB_FLOAT_FORMAT, data);
    return SString(buf);
}
#endif

// SExpressionChar class

IntelibTypeId SExpressionChar::TypeId(&SExpression::TypeId);

bool SExpressionChar::SpecificEql(const SExpression *ref) const
{
    INTELIB_ASSERT(TermType() == ref->TermType(), IntelibX_bug());
    return s[0] == static_cast<const SExpressionChar*>(ref)->s[0];
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionChar::TextRepresentation() const
{
    // names taken from CLISP
    static const char *charnames[] = {
        "#\\Null",      "#\\Soh",    "#\\Stx",     "#\\Etx",
        "#\\Eot",       "#\\Enq",    "#\\Ack",     "#\\Bell",
        "#\\Backspace", "#\\Tab",    "#\\Newline", "#\\Vt",
        "#\\Page",      "#\\Return", "#\\So",      "#\\Si",
        "#\\Dle",       "#\\Dc1",    "#\\Dc2",     "#\\Dc3",
        "#\\Dc4",       "#\\Nak",    "#\\Syn",     "#\\Etb",
        "#\\Can",       "#\\Em",     "#\\Sub",     "#\\Escape",
        "#\\Fs",        "#\\Gs",     "#\\Rs",      "#\\Us",
        "#\\Space"
    };
    if(s[0] < 33 && s[0] >= 0) return charnames[(int)(s[0])];
    if(s[0] == 127) return "#\\Rubout";
    char res[] = "#\\.";
    res[2] = s[0];
    return res;
}
#endif


// SExpressionString class

IntelibTypeId SExpressionString::TypeId(&SExpression::TypeId);

SExpressionString::SExpressionString(const char *s)
        : SExpression(TypeId)
{
    StringSetup(s);
}

SExpressionString::
SExpressionString(const IntelibTypeId &tid, const char *s)
        : SExpression(tid)
{
    StringSetup(s);
}

SExpressionString::SExpressionString(const char *s1,
                                     const char *s2)
        : SExpression(TypeId)
{
    unsigned int len1 = strlen(s1);
    unsigned int lensum = len1 + strlen(s2);
    char *p;
    is_indirect = (lensum + 1 >= intelib_simple_string_limit);
    if(is_indirect) {
        p = str.p = new char[lensum+1];
    } else {
        p = str.s;
    }
    strncpy(p, s1, len1+1);
    strncpy(p+len1, s2, lensum+1-len1);
}

void SExpressionString::StringSetup(const char *s)
{
    unsigned int len = strlen(s);
    is_indirect = (len+1 >= intelib_simple_string_limit);
    if(is_indirect) {
        str.p = new char[len+1];
        strncpy(str.p, s, len+1);
    } else {
        strncpy(str.s, s, len+1);
    }
}

SExpressionString::~SExpressionString() {
    if(is_indirect)
        delete [] str.p;
}

bool SExpressionString::SpecificEql(const SExpression *ref) const
{
    INTELIB_ASSERT(TermType() == ref->TermType(), IntelibX_bug());
    return
        strcmp(GetValue(), ((const SExpressionString*)(ref))->GetValue()) == 0;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionString::TextRepresentation() const
{
    SString res;
    const char *src = is_indirect ? str.p : str.s;
    int len = strlen(src);
    res="\"";
    for(int i = 0; i< len; i++) {
        switch(src[i]) {  // in accordance to B.Stroustroup, 3rd ed., C.3.2
            case '\n': res+="\\n"; break;
            case '\t': res+="\\t"; break;
            case '\v': res+="\\v"; break;
            case '\b': res+="\\b"; break;
            case '\r': res+="\\r"; break;
            case '\f': res+="\\f"; break;
            case '\a': res+="\\a"; break;
            case '\\': res+="\\\\"; break;
                //case '\?': res+="\\?"; break; //senseless
                //case '\'': res+="\\'"; break; //senseless
            case '\"': res+="\\\""; break;
            default: {
                    char tmp[2];
                    tmp[0] = src[i];
                    tmp[1] = 0;
                    res+=tmp;
                }
        }
    }
    res+="\"";
    return res;
}
#endif

// Single character object bank

class IntelibSingleCharBank {
    SReference refs[256];
public:
    IntelibSingleCharBank() {
        for(int i=0; i<256; i++) {
            refs[i] = ::new SExpressionChar(i);
        }
    }
    ~IntelibSingleCharBank() {}
    const SReference& operator[](int ch) { return refs[ch % 256]; }
};

static IntelibSingleCharBank TheSingleCharBank;

const SReference& GetSingleCharExpression(int ch)
{
    return TheSingleCharBank[ch];
}

// SExpressionClassicAtom class

IntelibTypeId SExpressionClassicAtom::TypeId(&SExpressionString::TypeId);

SString SExpressionClassicAtom::TextRepresentation() const 
{
    return SString(this->GetValue());
}

// SExpressionCons class

IntelibTypeId SExpressionCons::TypeId(&SExpression::TypeId, true);

SExpression* SExpressionCons::Clone() const
{
    return new SExpressionCons(car.Clone(), cdr.Clone());
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
static SString SafeTextRepresentation(const SReference &ref)
{
    if(ref.GetPtr())
        return ref->TextRepresentation();
    else
        return "#<UNBOUND>";
}

SString SExpressionCons::
CoreTextRepresentation(const char *delim,
                       const char *dot_delim,
                       SString (*repfun)(const SReference &)) const
{
    SString res = repfun ? repfun(car) : SafeTextRepresentation(car);
    if(cdr.IsEmptyList()) {
        // end of list
    } else {
        SExpressionCons *dp = cdr.DynamicCastGetPtr<SExpressionCons>();
        if(dp) {
            // Print as a list, without a dot...
            res += delim;
            res += dp->CoreTextRepresentation(delim, dot_delim, repfun);
        } else {
            res += dot_delim;
            res += repfun ? repfun(cdr) : SafeTextRepresentation(cdr);
        }
    }
    return res;
}
SString SExpressionCons::TextRepresentation() const
{
    SString res("(");
    res+= CoreTextRepresentation(" ", " . ");
    res+= ")";
    return res;
}
#endif


// SExpressionLabel class

IntelibTypeId SExpressionLabel::TypeId(&SExpression::TypeId);


#if INTELIB_TEXT_REPRESENTATIONS == 1
SExpressionLabel::SExpressionLabel(const IntelibTypeId &the_type,
                                   const char *a_name)
        : SExpression(the_type)
{
    name = strdup(a_name);
}
SExpressionLabel::SExpressionLabel(const char *a_name)
        : SExpression(TypeId)
{
    name = strdup(a_name);
}
SExpressionLabel::~SExpressionLabel()
{
    free(name);
}
SString SExpressionLabel::TextRepresentation() const
{
    return name;
}
//SString SExpressionLabel::GetName() const
//{
//  return name;
//}
#endif

// Some useful label terms

SReference *GetEmptyList()
{
    static SLabel STheEmptyListLabelRef("()");
    return &STheEmptyListLabelRef;
}

SReference *PTheEmptyList = GetEmptyList();

// SReference class

SReference::SReference(char c)
{
    (*this) = TheSingleCharBank[c];
}

SReference::SReference(signed char c)
{
    (*this) = TheSingleCharBank[c];
}

SReference::SReference(unsigned char c)
{
    (*this) = TheSingleCharBank[c];
}

SReference::SReference(signed short i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(unsigned short i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(signed int i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(unsigned int i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(signed long i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(unsigned long i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(signed long long i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(unsigned long long i)
        : GenericReference<SExpression>(new SExpressionInt(i))
{}

SReference::SReference(float f)
        : GenericReference<SExpression>(new SExpressionFloat(f))
{}

SReference::SReference(double f)
        : GenericReference<SExpression>(new SExpressionFloat(f))
{}

SReference::SReference(long double f)
        : GenericReference<SExpression>(new SExpressionFloat(f))
{}

SReference::SReference(const char *s)
{
    (*this) = new SExpressionString(s);
}

SReference::SReference(const unsigned char *s)
{
    (*this) = new SExpressionString(reinterpret_cast<const char*>(s));
}

SReference::SReference(const class SListConstructor &)
        : GenericReference<SExpression>(*PTheEmptyList)
{}

SReference::SReference(const SReference &rcar, const SReference &rcdr)
        : GenericReference<SExpression>(new SExpressionCons(rcar, rcdr))
{}


SReference& SReference::AddAnotherItemToList(const SReference &right)
{
    if(GetPtr() && (*this)->TermType() == SExpressionCons::TypeId) {
        (((SExpressionCons*)GetPtr())->Cdr()).AddAnotherItemToList(right);
    } else
    if(!GetPtr() || IsEmptyList()) {
        (*this) = new SExpressionCons(right, *PTheEmptyList);
    } else {
        throw IntelibX_not_a_list(*this);
    }
    return *this;
}

SReference& SReference::ChangeListEnd(const SReference &new_last)
{
    if(GetPtr() && (*this)->TermType() == SExpressionCons::TypeId) {
        (((SExpressionCons*)GetPtr())->Cdr()).ChangeListEnd(new_last);
    } else
    if(!GetPtr() || IsEmptyList()) {
        (*this) = new_last;
    } else {
        throw IntelibX_not_a_list(*this);
    }
    return *this;
}

SReference SReference::MakeCons(const SReference &a_cdr) const
{
    return SReference(new SExpressionCons(*this, a_cdr));
}

SReference& SReference::Car() const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());
    SExpressionCons *tp = DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(tp, IntelibX_not_a_cons(*this));
    return tp->Car();
}

SReference& SReference::Cdr() const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());
    SExpressionCons *tp = DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(tp, IntelibX_not_a_cons(*this));
    return tp->Cdr();
}

SReference& SReference::CCar() const
{
    if((*this)==*PTheEmptyList) {
        return *PTheEmptyList;
    } else {
        return Car();
    }
}

SReference& SReference::CCdr() const
{
    if((*this)==*PTheEmptyList) {
        return *PTheEmptyList;
    } else {
        return Cdr();
    }
}

intelib_float_t SReference::GetFloat() const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());
    SExpressionFloat *sf = DynamicCastGetPtr<SExpressionFloat>();
    if(sf) return sf->GetValue();
    SExpressionInt *si = DynamicCastGetPtr<SExpressionInt>();
    if(si) return si->GetValue();
    throw IntelibX_not_a_number(*this);
}

intelib_integer_t SReference::GetInt() const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());
    SExpressionInt *si = DynamicCastGetPtr<SExpressionInt>();
    if(si) return si->GetValue();
    SExpressionFloat *sf = DynamicCastGetPtr<SExpressionFloat>();
    if(sf) return (intelib_integer_t) (sf->GetValue());
    throw IntelibX_not_a_number(*this);
}

const char* SReference::GetString() const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());
    SExpressionString *ss = DynamicCastGetPtr<SExpressionString>();
    if(ss) return ss->GetValue();
    SExpressionChar *ch = DynamicCastGetPtr<SExpressionChar>();
    if(ch) return ch->GetString();
    throw IntelibX_not_a_string(*this);
}

char SReference::GetSingleChar() const
{
    SExpressionChar *ch = DynamicCastGetPtr<SExpressionChar>();
    if(ch) return ch->GetValue();
    // for compatibility, let's consider a string of one char as a char
    const char *p = GetString();
    if(p && *p && (*(p+1)==0)) return *p;
    throw IntelibX_not_a_char(*this);
}

SReference SReference::Clone() const
{
    if(!GetPtr()) return *this;
    if(!GetPtr()->IsMutable()) return *this;
    return SReference(GetPtr()->Clone());
}

SReference SReference::CopyList() const
{
    if(!GetPtr()) return *this;
    SExpressionCons *cons = DynamicCastGetPtr<SExpressionCons>();
    if(cons)
        return SReference(cons->Car(), cons->Cdr().CopyList());
    else
        return *this;
}

SReference SReference::CopyTree() const
{
    if(!GetPtr()) return *this;
    SExpressionCons *cons = DynamicCastGetPtr<SExpressionCons>();
    if(cons)
        return SReference(cons->Car().CopyTree(), cons->Cdr().CopyTree());
    else
        return *this;
}

bool SReference::IsEql(const SReference& other) const
{
    if(GetPtr() == other.GetPtr()) {
        return true;
    }
    if(GetPtr()->TermType() == other->TermType()) {
        return GetPtr()->SpecificEql(other.GetPtr());
    }
    return false;
}

bool SReference::IsEqual(const SReference& other) const
{
    if(IsEql(other)) return true;
    SExpressionCons *dp = DynamicCastGetPtr<SExpressionCons>();
    if(dp) {
        SExpressionCons *dp2 = other.DynamicCastGetPtr<SExpressionCons>();
        if(dp2) {
            return
                dp->Car().IsEqual(dp2->Car()) &&
                dp->Cdr().IsEqual(dp2->Cdr());
        }
    }
    return false;
}

bool SReference::IsEmptyList() const
{
    return GetPtr() == PTheEmptyList->GetPtr();
}

