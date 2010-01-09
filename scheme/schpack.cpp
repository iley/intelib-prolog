//   InteLib                                    http://www.intelib.org
//   The file scheme/schpack.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <string.h>
#include <ctype.h>

#include "schsymb.hpp"

#include "schpack.hpp"

/////////////////////
// SchExpressionPackage

IntelibTypeId
SchExpressionPackage::TypeId(&SExpressionHashPackage::TypeId);

SchExpressionPackage::SchExpressionPackage()
    : SExpressionHashPackage(TypeId)
{}

SchExpressionPackage::SchExpressionPackage(const SchPackageRef& par)
    : SExpressionHashPackage(TypeId, par)
{}

SchExpressionPackage::~SchExpressionPackage()
{}

SReference SchExpressionPackage::CreateNewSymbolObject(const char *nm) const
{
    return SReference(new SchExpressionSymbol(nm));
}

/////////////////////
// SchExpressionPackageIntelib

SchExpressionPackageIntelib::SchExpressionPackageIntelib()
    : SchExpressionPackage()
{
    Import(TheSchLibraryProvidedSymbols.Quote);
}
