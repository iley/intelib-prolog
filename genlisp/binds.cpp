//   InteLib                                    http://www.intelib.org
//   The file genlisp/binds.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
