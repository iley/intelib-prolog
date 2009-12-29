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






#include "spkghsh.hpp"

/////////////////////
// SExpressionHashPackage

IntelibTypeId
SExpressionHashPackage::TypeId(&SExpressionHashTable::TypeId, false);
   // SExpressionHashPackage is declared unchangeable so that it is never 
   // Clone()'d but it is still possible to clone expressions which contain
   // references to a package

SExpressionHashPackage::SExpressionHashPackage()
    : SExpressionHashTable(TypeId)
{}

SExpressionHashPackage::SExpressionHashPackage(const SHashPackage& par)
    : SExpressionHashTable(TypeId), parent(par)
{}

SExpressionHashPackage::SExpressionHashPackage(const IntelibTypeId& tid)
    : SExpressionHashTable(tid)
{}

SExpressionHashPackage::SExpressionHashPackage(const IntelibTypeId& tid,
                                               const SHashPackage& par)
    : SExpressionHashTable(tid), parent(par)
{}

SExpressionHashPackage::~SExpressionHashPackage()
{}

bool SExpressionHashPackage::Import(const SReference &ref, 
                                    const char *symbname, 
                                    bool safe)
{
    SExpressionLabel *symb = ref.DynamicCastGetPtr<SExpressionLabel>();
    INTELIB_ASSERT(symb, IntelibX_not_a_label(ref));

    const char *name = symbname ? symbname : symb->GetName();

    if(safe) {
        return SafeAddItem(name, ref);
    } else {
        AddItem(name, ref);
        return true;
    }
}

SReference SExpressionHashPackage::Intern(const char *name)
{
    SReference ret = FindSymbol(name);
    if(ret.GetPtr() == EmptySlotMark.GetPtr()) {
        // Now we create a new symbol
        ret = CreateNewSymbolObject(name); 
        AddItem(name, ret);
    }
    return ret;
} 

SReference SExpressionHashPackage::FindSymbol(const char *name)
{
    SReference tmp = FindItem(name);
    if(tmp.GetPtr() == EmptySlotMark.GetPtr() && parent.GetPtr()) {
        return parent->FindSymbol(name);
    } else {
        return tmp;
    }
}

#if 0
SReference SExpressionHashPackage::GetAllSymbols() const
{
    SReference res(*PTheEmptyList);
    Iterator it(*this);
    SReference r = it.GetNext();
    while(r.GetPtr()) {
        res = SReference(r.Cdr(), res);
        r = it.GetNext();
    }
}
#endif

SReference SExpressionHashPackage::MakeSymbol(const char *name)
{
    return Intern(name);
}

SString SExpressionHashPackage::TextRepresentation() const
{
    return SString("#<PACKAGE>");
}


SReference SExpressionHashPackage::CreateNewSymbolObject(const char *nm) const
{
    return SReference(new SExpressionLabel(nm)); 
}

/////////////////////
// exceptions

IntelibX_not_a_package::
IntelibX_not_a_package(SReference a_param) 
    : IntelibX("Not a package", a_param) {}

IntelibX_package_conflict::
IntelibX_package_conflict(SReference a_param) 
    : IntelibX("Name conflict within a package", a_param) {}
