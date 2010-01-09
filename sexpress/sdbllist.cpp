//   InteLib                                    http://www.intelib.org
//   The file sexpress/sdbllist.cpp
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

#include "sdbllist.hpp"



IntelibTypeId SExpressionDoubleList::TypeId(&SExpression::TypeId, true);

const SBacklinkRef& SExpressionDoubleList::AddToBegin(const SReference &ref)
{
    if(!begin.GetPtr()) {
        SetTheOnlyItem(ref);
        return begin;
    }
    SExpressionBacklink *tmp = new SExpressionBacklink(ref, *PTheEmptyList);
    SBacklinkRef beg = begin;
    begin = tmp; // this version takes care
    beg->ConnectBack(begin);
    return begin;
}

const SBacklinkRef& SExpressionDoubleList::AddToEnd(const SReference &ref)
{
    if(!begin.GetPtr()) {
        SetTheOnlyItem(ref);
        return begin;
    }
    end->InsertAfter(ref);
    end = end->Next();
    return end;
}

void SExpressionDoubleList::Exclude(const SBacklinkRef &from, 
                                    const SBacklinkRef &thru)
{
    SBacklinkRef left = from->Prev();
    SBacklinkRef right = thru->Next();

    if(!left.IsEmptyList()) 
        from->DisconnectBack();
    else 
        begin = right;
    if(!right.IsEmptyList()) 
        right->DisconnectBack();
    else 
        end = left;

    if(!left.IsEmptyList() && ! right.IsEmptyList())
        right->ConnectBack(left);
}

void SExpressionDoubleList::InsertListAfter(const SBacklinkRef &from, 
                                            const SBacklinkRef &left, 
                                            const SBacklinkRef &right)
{
    SBacklinkRef thru;
    if(from.IsEmptyList()) thru = begin; else thru = from->Next();
    if(!thru.IsEmptyList()) {
        thru->DisconnectBack();
        thru->ConnectBack(right);
    } else {
        end = right;
    }
    if(!from.IsEmptyList()) {
        left->ConnectBack(from);
    } else {
        begin = left;
    }
}

void SExpressionDoubleList::SetTheOnlyItem(const SReference &ref)
{
    begin = new SExpressionBacklink(ref, *PTheEmptyList);
    end = begin;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionDoubleList::TextRepresentation() const
{ 
    return begin.GetPtr() ? 
        begin->TextRepresentation() :
        SString("()");
}
#endif


#if 0
SDoubleListRef::SDoubleListRef(const SReference &ref)
{
    if(!ref.GetPtr() || 
       !ref->TermType().IsSubtypeOf(SExpressionDoubleList::TypeId) 
      ) 
    { throw IntelibX_not_a_doublelist(ref); }
}
#endif

#if 0
SDoubleListRef& SDoubleListRef::operator=(SExpressionDoubleList *ex)
{ this->SReference::operator=(ex); return *this; }
SDoubleListRef& SDoubleListRef::operator=(SExpressionDoubleList &ex)
{ this->SReference::operator=(ex); return *this; }
SDoubleListRef& SDoubleListRef::operator=(const SDoubleListRef &other)
{ this->SReference::operator=(other); return *this; }
SDoubleListRef& SDoubleListRef::operator=(const SReference &ref)
{ this->operator=(SDoubleListRef(ref)); return *this; }
#endif

void SExpressionDoubleList::Iterator::InsertBefore(const SReference& r)
{
    if(Exhausted()) { 
        master->AddToBegin(r);
        SetBegin();
    } else if(IsBegin()) {
        master->AddToBegin(r);
    } else {
        (*this)->Prev()->InsertAfter(r);
    }
}

void SExpressionDoubleList::Iterator::InsertAfter(const SReference& r)
{
    if(Exhausted()) {
        master->AddToEnd(r);
        SetEnd();
    } else if(IsEnd()) {
        master->AddToEnd(r);
    } else {
        (*this)->InsertAfter(r);
    }
}

bool SExpressionDoubleList::Iterator::Remove()
{
    if(!GetPtr() || Exhausted()) return false;
    bool b = IsBegin();
    bool e = IsEnd();
    SBacklinkRef::Remove();
    if(IsEmptyList()) // no empty lists here please... just unbound
        SReference::operator=(SReference());
    if(b) { master->begin = *this; }
    if(e) { master->end = *this; }
    return true;
}

///////////////////////////////////////////////////
// Ranges

SDoubleListRef SDoubleListRange::Copy() const
{
    SDoubleListRef res(new SExpressionDoubleList);
    for(SBacklinkRef tmp = first; 
        !tmp.IsEmptyList() && tmp.GetPtr() != follow.GetPtr();
        tmp++)
    {
        res->AddToEnd(tmp->Car());
    }
    return res;
}

SDoubleListRange::LeftEndMover::operator SBacklinkRef() const
{
    if(master->IsEmpty()) return *PTheEmptyList; 
    return master->first;
}

SBacklinkRef SDoubleListRange::LeftEndMover::operator++() const
{
    if(master->IsEmpty()) (master->follow)++;
    return ++(master->first);
}

SBacklinkRef SDoubleListRange::LeftEndMover::operator++(int) const
{
    if(master->IsEmpty()) (master->follow)++;
    return (master->first)++;
}

SBacklinkRef SDoubleListRange::LeftEndMover::operator--() const
{
    if(master->first.IsEmptyList()) 
        return (master->first = master->master->GetEnd());
    return --(master->first);
}

SBacklinkRef SDoubleListRange::LeftEndMover::operator--(int) const
{
    if(master->first.IsEmptyList()) {
        master->first = master->master->GetEnd();
        return *PTheEmptyList;
    }
    return (master->first)--;
}

void SDoubleListRange::LeftEndMover::JumpToFirst() const
{
    master->first = master->master->GetBegin();
}

void SDoubleListRange::LeftEndMover::JumpToLast() const
{
    master->first = *PTheEmptyList;
    master->follow = *PTheEmptyList;
}

void SDoubleListRange::LeftEndMover::JumpToRightEnd() const
{
    master->first = master->follow;
}

SDoubleListRange::RightEndMover::operator SBacklinkRef() const
{
    if(master->IsEmpty()) return *PTheEmptyList; 
    return master->follow;
}

SBacklinkRef SDoubleListRange::RightEndMover::operator++() const
{
    return ++(master->follow);
}

SBacklinkRef SDoubleListRange::RightEndMover::operator++(int) const
{
    return (master->follow)++;
}

SBacklinkRef SDoubleListRange::RightEndMover::operator--() const
{
    if(master->IsEmpty()) master->LeftEnd()--;
    if(master->follow.IsEmptyList()) {
        return (master->follow = master->master->GetEnd());
    }
    return --(master->follow);
}

SBacklinkRef SDoubleListRange::RightEndMover::operator--(int) const
{
    if(master->IsEmpty()) master->LeftEnd()--;
    if(master->follow.IsEmptyList()) {
        master->follow = master->master->GetEnd();
        return *PTheEmptyList;
    }
    return (master->follow)--;
}

void SDoubleListRange::RightEndMover::JumpToFirst() const
{
    master->first = master->master->GetBegin();
    master->follow = master->first;
}

void SDoubleListRange::RightEndMover::JumpToLast() const
{
    master->follow = *PTheEmptyList;
}

void SDoubleListRange::RightEndMover::JumpToLeftEnd() const
{
    master->follow = master->first;
}


SBacklinkRef SDoubleListRange::GetFirst() const
{ 
    if(IsEmpty()) return *PTheEmptyList;
    return first; 
}

const SBacklinkRef& SDoubleListRange::GetFollow() const
{
    return follow;
}

SBacklinkRef SDoubleListRange::GetLast() const
{
    if(IsEmpty()) return *PTheEmptyList;
    if(follow.IsEmptyList()) return master->GetEnd();
    return follow->Prev();
}



IntelibX_not_a_doublelist::
IntelibX_not_a_doublelist(SReference a_param) 
    : IntelibX("Not a double list", a_param) {}
