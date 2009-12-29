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
