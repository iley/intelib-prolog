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




#include "binds.hpp"



IntelibBindingsSet::Binding*
IntelibBindingsSet::Find(const SExpressionLabel *var) const
{
    // first three positions are searched manually
    // and if the binding is found there, no swaps is done
    if(!first) return 0;
    if(first->var == var) return first;
    Binding *second = first->next;
    if(!second) return 0;
    if(second->var == var) return second;
    Binding *third = first->next;
    if(!third) return 0;
    if(third->var == var) return third;
    // now, perform a cycle throug the rest of the list
    // and if we find the binding there, move it into the
    // first position
    for(Binding **t = &(third->next); *t; t = &((*t)->next)) {
        Binding *tmp = *t;
        if(tmp->var == var) {
            *t = tmp->next;
            tmp->next = first;
            first = tmp;
            return tmp;
        }
    }
    return 0;
}

IntelibBindingsSet::IntelibBindingsSet()
    : first(0)
{}

IntelibBindingsSet::~IntelibBindingsSet()
{
    while(first) {
        Binding *tmp = first->next;
        delete first;
        first = tmp;
    }
}

SReference* IntelibBindingsSet::AddBinding(const SExpressionLabel* var)
{
    Binding *tmp = new Binding;
    tmp->var = var;
    tmp->next = first;
    first = tmp;
    return &(tmp->val);
}

SReference* IntelibBindingsSet::GetBinding(const SExpressionLabel* var) const
{
    Binding *tmp = Find(var);
    return tmp ? &(tmp->val) : 0;
}
