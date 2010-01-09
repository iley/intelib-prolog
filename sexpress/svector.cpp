//   InteLib                                    http://www.intelib.org
//   The file sexpress/svector.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "iexcept.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "sstring.hpp"
#endif

#include "svector.hpp"


IntelibTypeId SExpressionVector::TypeId(&SExpression::TypeId, true);


SExpressionVector::SExpressionVector(int n)
    : SExpression(TypeId)
{
    if(n<=0) {
        size = 0;
        real_size = -n;
        resizeable = true;
    } else {
        size = n;
        real_size = n;
        resizeable = false;
    }
    vector = real_size!=0 ? new SReference[real_size] : 0;
}

SExpressionVector::
SExpressionVector(const IntelibTypeId &tid, int n)
    : SExpression(tid)
{
    if(n<=0) {
        size = 0;
        real_size = -n;
        resizeable = true;
    } else {
        size = n;
        real_size = n;
        resizeable = false;
    }
    vector = real_size!=0 ? new SReference[real_size] : 0;
}

const SReference& SExpressionVector::operator[](unsigned int n) const
{
    INTELIB_ASSERT(n<size, IntelibX_vector_out_of_range(SReference(this)));
    return vector[n];
}

SReference& SExpressionVector::operator[](unsigned int n)
{
    if(resizeable) {
        if(n>=size) Resize(n);
    } else {
        INTELIB_ASSERT(n<size,IntelibX_vector_out_of_range(SReference(this)));
    }
    return vector[n];
}

SExpression* SExpressionVector::Clone() const 
{
    SExpressionVector *res = new SExpressionVector(resizeable ? 0 : size);
    if(resizeable) 
        res->Resize(size);
    for(int i=size-1; i>=0; i--) 
        res->vector[i] = vector[i].Clone();
    return res;
}

SString SExpressionVector::TextRepresentation() const 
{
    // note that resizeable arrays are missing in Common Lisp
    // therefore we use CL notation for non-resizeable arrays
    // while the notation for resizeables is slightly changed
    SString res(resizeable ? "#~(" : "#(");
    for(int i=0; i<Size(); i++) {
        if(i>0) res += " ";
        SExpression *p = (*this)[i].GetPtr();
        res += p ? p->TextRepresentation() : "#<UNBOUND>";
    }
    res += ")";
    return res; 
}

void SExpressionVector::SetSize(unsigned int n)
{
    if(n==size) return;
    if(n>size) { Resize(n-1); return; }
    /* shrink... */
    for(unsigned int i=n; i<size; i++) vector[i] = SReference();
    size = n; 
}

void SExpressionVector::Resize(unsigned int n)
{
    if(n<size) return;
    if(n<real_size) {
        size = n+1;
        return;
    }
    unsigned int ns = real_size>0 ? real_size : 4; 
    while(ns <= n) ns*=2;
    if(!vector) {
        vector = new SReference[ns];
        real_size = ns;
    } else {
        SReference *newvector = new SReference[ns];
        for(unsigned int i=0; i<size; i++) newvector[i] = vector[i];
        delete[] vector;
        vector = newvector;
        real_size = ns;
    }
    size = n+1;
}

///////////////////////////////////////////////////////////

#if 0
SVectorRef::SVectorRef(const SReference& other)
    : SReference(other)
{
    if(!GetPtr() || 
       !(GetPtr()->TermType().IsSubtypeOf(SExpressionVector::TypeId)))
    {
        throw IntelibX_not_a_vector(other);
    }
}
#endif


SVectorRef::SVectorRef(SReference& other, int /*fake*/)
    : SVectorRefBase(other)
{
    if(!GetPtr()) {
        (*this) = new SExpressionVector;
        other = *this;
        return;
    } 
    if(!(GetPtr()->TermType().IsSubtypeOf(SExpressionVector::TypeId)))
    {
        throw IntelibX_not_a_vector(other);
    }
}

///////////////////////////////////////////////////////////
// SVectorRange

SVectorRef SVectorRange::Copy(bool resizeable) const   
{
    SVectorRef res(resizeable ? 0 : len);
    for(int i=0; i<len; i++) res[i] = (*this)[i+idx];
    return res;
}

SVectorRef SVectorRange::Copy() const
{
    SVectorRef res((*this)->IsResizeable() ? 0 : len);
    for(int i=0; i<len; i++) res[i] = (*this)[i+idx];
    return res;
}   

void SVectorRange::Erase()
{
    for(int i=idx; i<(*this)->Size()-len; i++)
        (*this)[i] = (*this)[i+len];
    (*this)->SetSize((*this)->Size()-len);
    len = 0;
}

void SVectorRange::Replace(const SVectorRange& other)
{
    if(len>other.len) {
        int delta = len - other.len;
        for(int i=idx+len-delta; i<(*this)->Size()-delta; i++)
             (*this)[i] = (*this)[i+delta];
        (*this)->SetSize((*this)->Size()-delta);
    } else 
    if(len<other.len) {
        int delta = other.len - len;
        (*this)->SetSize((*this)->Size()+delta);
        for(int i=(*this)->Size()-1; i>=idx+len+delta; i--)
             (*this)[i] = (*this)[i-delta];
    }
    len = other.len;
    for(int i=0;  i<len; i++)
        (*this)[idx+i] = other[other.idx+i];
}


///////////////////////////////////////////////////////////

IntelibX_not_a_vector::
IntelibX_not_a_vector(SReference a_param) 
    : IntelibX("Not a vector", a_param) {}

IntelibX_vector_out_of_range::
IntelibX_vector_out_of_range(SReference a_param) 
    : IntelibX("Vector out of range", a_param) {}

