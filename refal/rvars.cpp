//   InteLib                                    http://www.intelib.org
//   The file refal/rvars.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




// Modified by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <stdio.h>

#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"

#include "rvars.hpp"


/*
    There is as for now no possibility to make derived types of variables
    except for 3 standard ones. So there is only one TypeId
    (for RfExpressionVariable) here.
*/
IntelibTypeId
RfExpressionVariable::TypeId(&SExpressionLabel::TypeId);


