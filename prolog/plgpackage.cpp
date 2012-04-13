#include "plgpackage.hpp"
#include "prolog.hpp"

IntelibTypeId PlgExpressionPackage::TypeId(&SExpressionHashPackage::TypeId, true);

PlgExpressionPackage::PlgExpressionPackage()
    : SExpressionHashPackage(TypeId)
{}

PlgExpressionPackage::PlgExpressionPackage(const PlgPackageRef &parent)
    : SExpressionHashPackage(TypeId, parent)
{}

SReference PlgExpressionPackage::CreateNewSymbolObject(const char *name) const
{
    return SReference(new PlgExpressionAtom(name));
}
