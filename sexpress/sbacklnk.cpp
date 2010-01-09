//   InteLib                                    http://www.intelib.org
//   The file sexpress/sbacklnk.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "sexpress.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "sstring.hpp"
#endif

#include "sbacklnk.hpp"

IntelibTypeId SExpressionBacklink::
TypeId(&SExpressionCons::TypeId, true);

SExpressionBacklink::~SExpressionBacklink()
{
    if(Cdr().GetPtr() && !Cdr().IsEmptyList()) {
        SExpressionBacklink *nextitem = 
            Cdr().DynamicCastGetPtr<SExpressionBacklink>();
        if(nextitem && nextitem->back == this) 
            nextitem->back = 0;
    }
}

SExpression* SExpressionBacklink::Clone() const
{
    throw IntelibX_not_implemented();
}

bool SExpressionBacklink::ConnectBack(const SBacklinkRef &where_to)
{
    if(back || !where_to.Cdr().IsEmptyList()) return false;
    back = where_to.GetPtr();
    back->Cdr() = this;
    return true; 
}

bool SExpressionBacklink::DisconnectBack()
{
    if(!back) return false;
    back->Cdr() = *PTheEmptyList;
    back = 0;
    return true;
}

void SExpressionBacklink::InsertAfter(const SReference &ref)
{
    SExpressionBacklink *tmp = 
        new SExpressionBacklink(ref, Cdr());
    tmp->back = this;
    SExpressionBacklink *nextitem = 
        Cdr().DynamicCastGetPtr<SExpressionBacklink>();
    if(nextitem) nextitem->back = tmp;
    Cdr() = SReference(tmp);  // this form takes care about *tmp
}

bool SExpressionBacklink::RemoveNext()
{
    if(Cdr().IsEmptyList()) return false;
    
    SExpressionBacklink *item_to_remove = 
        Cdr().DynamicCastGetPtr<SExpressionBacklink>();

    if(!item_to_remove) { 
        // seems to be a dotted list
        Cdr() = *PTheEmptyList;
    } else {
        SExpressionBacklink *next_item = 
            item_to_remove->Cdr().DynamicCastGetPtr<SExpressionBacklink>();
        if(next_item) next_item->back = this;
        
        SReference tmp = item_to_remove->Cdr(); 
        item_to_remove->Cdr() = *PTheEmptyList;
        item_to_remove->back = 0;
        Cdr() = tmp;
    }
    return true;
}

SBacklinkRef SExpressionBacklink::Prev() const
{
    if(back) 
        return SBacklinkRef(back);
    else
        return SBacklinkRef(*PTheEmptyList);
}

SBacklinkRef SExpressionBacklink::Next() const
{
    return Cdr();
}



SBacklinkRef::SBacklinkRef()
   : SReference(*PTheEmptyList)
{}

SBacklinkRef::SBacklinkRef(SExpressionBacklink *ex)
   : SReference(ex)
{}

SBacklinkRef::
SBacklinkRef(const SBacklinkRef &other)
   : SReference(other)
{}

SBacklinkRef::SBacklinkRef(const SReference &other)
   : SReference(other)
{
    if(other.IsEmptyList()) return;
    if(!other.GetPtr() || 
       !other->TermType().IsSubtypeOf(SExpressionBacklink::TypeId)
      ) 
    {
        throw IntelibX_not_a_backlink(other);
    }
}

SBacklinkRef::~SBacklinkRef()
{}

SBacklinkRef& SBacklinkRef::operator=(const SReference &ref)
{
    if(!ref.IsEmptyList()) {
        if(!ref.GetPtr() || 
           !ref->TermType().IsSubtypeOf(SExpressionBacklink::TypeId)
          ) 
        {
            throw IntelibX_not_a_backlink(ref);
        }
    }
    this->SReference::operator=(ref);
    return *this;
}

SExpressionBacklink* SBacklinkRef::GetPtr() const
{
    if(IsEmptyList()) return 0; 
    return static_cast<SExpressionBacklink*>(SReference::GetPtr());
}

void SBacklinkRef::InsertAfter(const SReference &r)
{
    if(IsEmptyList()) {
        (*this) = new SExpressionBacklink(r, *PTheEmptyList);
    } else {
        (*this)->InsertAfter(r);
    }
}

bool SBacklinkRef::Remove()
{
    if(IsEmptyList()) return false;
    if(!(*this)->Prev().IsEmptyList()) {
        (*this) = (*this)->Prev();
        (*this)->RemoveNext();
        return true;
    }
    // no previous elem...
    SExpressionBacklink *nextitem = 
        (*this)->Cdr().DynamicCastGetPtr<SExpressionBacklink>();
    if(!nextitem) {
        // no next item as well, or it is a dotted end. just become empty.
        (*this) = *PTheEmptyList;
        return true; 
    }
    // there's the next item... we'll advance forwards
    SBacklinkRef tmp = *this;
    (*this) = nextitem;
    nextitem->DisconnectBack();
    return true;
}

SBacklinkRef SBacklinkRef::operator,(const SReference &s)
{
    if(IsEmptyList()) {
        SExpressionBacklink *tmp = 
            new SExpressionBacklink(s, *PTheEmptyList);
        (*this) = tmp;
        return *this;
    }
 
    SExpressionBacklink *sbl = GetPtr();
    SExpressionBacklink *tmp;
    while((tmp = sbl->Cdr().DynamicCastGetPtr<SExpressionBacklink>()))
    {
        sbl = tmp;
    }
    sbl->InsertAfter(s);
    return SBacklinkRef(static_cast<SExpressionBacklink*>
                                          (sbl->Cdr().GetPtr()));
}

SBacklinkRef SBacklinkRef::Begin() const
{
    if(IsEmptyList()) return *this;
    SBacklinkRef res(*this);
    SBacklinkRef prev;
    while(!(prev = res->Prev()).IsEmptyList())
    {
        res = prev;
    }
    return res;
}

SBacklinkRef SBacklinkRef::End() const
{
    if(IsEmptyList()) return *this;
    SBacklinkRef res(*this);
    SBacklinkRef next;
    while(!(next = res->Next()).IsEmptyList())
    {
        res = next;
    }
    return res;
}

SBacklinkRef& SBacklinkRef::operator++()
{
    if(!IsEmptyList()) (*this) = GetPtr()->Cdr();
    return *this;
}

SBacklinkRef& SBacklinkRef::operator--()
{
    if(!IsEmptyList()) (*this) = GetPtr()->Prev();
    return *this;
}

SBacklinkRef SBacklinkRef::operator++(int)
{
    if(IsEmptyList()) return *this;
    SBacklinkRef res(*this);
    (*this) = GetPtr()->Cdr();
    return res;
}

SBacklinkRef SBacklinkRef::operator--(int)
{
    if(IsEmptyList()) return *this;
    SBacklinkRef res(*this);
    (*this) = GetPtr()->Prev();
    return res;
}



IntelibX_not_a_backlink::
IntelibX_not_a_backlink(SReference a_param) 
    : IntelibX("Not a backlinked cons", a_param) {}

