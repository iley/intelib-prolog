#include "prolog.hpp"

IntelibX_not_a_prolog_object::IntelibX_not_a_prolog_object(SReference a_param) : IntelibX("Not a prolog object", a_param) {}

//IntelibTypeId PlgTerm::TypeId(&SExpression::TypeId, true);

class PlgResultImpl : public SExpression {
    public:
        static IntelibTypeId TypeId;
};

IntelibTypeId PlgResultImpl::TypeId;

void PlgResult::Next() {
    // TODO
}
