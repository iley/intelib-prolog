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




#include "iexcept.hpp"

IntelibX::IntelibX(const char *a_descr)
    : descr(a_descr)
{}

IntelibX::IntelibX(const char *a_descr, SReference a_param)
    : descr(a_descr), param(a_param)
{}

IntelibX::IntelibX(const IntelibX& other)
    : descr(other.descr), param(other.param), stack(other.stack)
{}

IntelibX::~IntelibX()
{}

void IntelibX::AddStack(SReference ref) 
{
    if(stack.GetPtr()) {
        stack = SReference(ref, stack);
    } else {
        stack = SReference(ref, *PTheEmptyList);
    }
}



IntelibX_wrong_expression_type::
IntelibX_wrong_expression_type(const SReference& a_param) 
    : IntelibX("Wrong expression type", a_param) {}

IntelibX_not_a_string::
IntelibX_not_a_string(SReference a_param) 
    : IntelibX("Not a string", a_param) {}

IntelibX_not_a_number::
IntelibX_not_a_number(SReference a_param) 
    : IntelibX("Not a number", a_param) {}

IntelibX_not_a_char::
IntelibX_not_a_char(SReference a_param) 
    : IntelibX("Not a char", a_param) {}

IntelibX_not_a_cons::
IntelibX_not_a_cons(SReference a_param) 
    : IntelibX("Not a cons", a_param) {}

IntelibX_not_a_list::
IntelibX_not_a_list(SReference a_param) 
    : IntelibX("Not a list", a_param) {}

IntelibX_not_a_label::
IntelibX_not_a_label(SReference a_param) 
    : IntelibX("Not a label", a_param) {}

IntelibX_unexpected_unbound_value::
IntelibX_unexpected_unbound_value()
    : IntelibX("Unexpected unbound value") {}

IntelibX_not_implemented::
IntelibX_not_implemented() 
    : IntelibX("Called for unimplemented feature. Sorry.") {}

IntelibX_bug::
IntelibX_bug() 
    : IntelibX("There's a bug in the library, refer to the source code") {}

