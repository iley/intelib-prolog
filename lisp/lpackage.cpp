//   InteLib                                    http://www.intelib.org
//   The file lisp/lpackage.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.





#include "lsymbol.hpp"

#include "lpackage.hpp"

/////////////////////
// LExpressionPackage

IntelibTypeId
LExpressionPackage::TypeId(&SExpressionHashPackage::TypeId);

LExpressionPackage::LExpressionPackage()
    : SExpressionHashPackage(TypeId)
{}

LExpressionPackage::LExpressionPackage(const LPackageRef& par)
    : SExpressionHashPackage(TypeId, par)
{}

LExpressionPackage::~LExpressionPackage()
{}

SReference LExpressionPackage::CreateNewSymbolObject(const char *nm) const
{
    return SReference(new LExpressionSymbol(nm));
}

/////////////////////
// LExpressionPackageIntelib

LExpressionPackageIntelib::LExpressionPackageIntelib()
    : LExpressionPackage()
{
    Import(TheLibraryProvidedSymbols.Quote);
    Import(TheLibraryProvidedSymbols.Function);
    Import(TheLibraryProvidedSymbols.Lambda);
    Import(TheLibraryProvidedSymbols.SymT);
    Import(TheLibraryProvidedSymbols.SymNil);
}
