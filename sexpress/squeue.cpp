//   InteLib                                    http://www.intelib.org
//   The file sexpress/squeue.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "squeue.hpp"
//#if INTELIB_TEXT_REPRESENTATIONS == 1
//#  include "sstring.hpp"
//#endif



SQueue::SQueue()
    : SReference(*PTheEmptyList), last(0)
{}

SQueue::SQueue(const SReference &ref)
{
    if(!ref.GetPtr() || 
       !ref->TermType().IsSubtypeOf(SExpressionCons::TypeId)) 
    {
        throw IntelibX_not_a_list(ref);
    }
    SReference::operator=(ref.CopyList());
    for(last=GetPtr(); 
        last && !last->Cdr().IsEmptyList(); 
        last=last->Cdr().DynamicCastGetPtr<SExpressionCons>())
    {}
    if(!last)
        throw IntelibX_not_a_list(ref);
}

void SQueue::InsertFirst(const SReference& item)
{
    SExpressionCons *tmp = new SExpressionCons(item, *this);
    bool was_empty = IsEmpty();
    SReference::operator=(tmp);
    if(was_empty)
        last = tmp;
}

void SQueue::Append(const SReference& item)
{
    SExpressionCons *tmp = new SExpressionCons(item, *PTheEmptyList);
    if(IsEmpty()) {
        SReference::operator=(tmp);
        last = tmp;
    } else {
        last->Cdr() = tmp;
        last = tmp;
    }
}

bool SQueue::RemoveFirst() 
{
    if(IsEmpty()) return false;
    SReference::operator=(GetPtr()->Cdr());
    if(IsEmpty()) last = 0;
    return true;
}
