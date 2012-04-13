#ifndef INTELIB_PLGPACKAGE_HPP_SENTRY
#define INTELIB_PLGPACKAGE_HPP_SENTRY

#include "../tools/spkghsh.hpp"

class PlgExpressionPackage;

typedef GenericSReference<PlgExpressionPackage, IntelibX_not_a_package> PlgPackageRef;

class PlgExpressionPackage : public SExpressionHashPackage 
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionPackage();
    PlgExpressionPackage(const PlgPackageRef &parent);

private:
    virtual SReference CreateNewSymbolObject(const char *name) const;
};

#endif
