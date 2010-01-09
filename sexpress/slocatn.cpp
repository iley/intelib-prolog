//   InteLib                                    http://www.intelib.org
//   The file sexpress/slocatn.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "slocatn.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "sstring.hpp"
#endif

IntelibTypeId SExpressionLocation::TypeId(&SExpression::TypeId, false);

SExpressionLocation::SExpressionLocation(const SReference &ss, SReference *loc)
    : SExpression(TypeId), superstructure(ss), location_pointer(loc)
{}

SExpressionLocation::~SExpressionLocation()
{}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionLocation::TextRepresentation() const
{
    return "#<MEMORY-LOCATION>";
}
#endif

IntelibX_not_a_location::
IntelibX_not_a_location(SReference a_param) 
    : IntelibX("not a location", a_param) {}

