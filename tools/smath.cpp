#include "smath.hpp"


const IntelibTypeId&
LispUnifyNumbericTypes(SReference &l1, SReference &l2);

#define BINARY_OP(funcname,op)\
SReference Intelib##funcname(SReference a, SReference b)\
{\
   const IntelibTypeId* tid= &LispUnifyNumbericTypes(a,b);\
   if (tid == &SExpressionInt::TypeId)\
       return SReference(a.GetInt() op b.GetInt());\
   else if (tid == &SExpressionFloat::TypeId)\
       return SReference(a.GetFloat() op b.GetFloat());\
   return SReference();\
}


BINARY_OP(Plus,+)
BINARY_OP(Minus,-)
BINARY_OP(Mult,*)


SReference IntelibDivide(SReference a, SReference b)
{
   const IntelibTypeId* tid=&LispUnifyNumbericTypes(a,b);
   if (tid == &SExpressionInt::TypeId){
       int a1 = a.GetInt();
       int b1 = b.GetInt();
       if (a1 % b1 == 0)
           return SReference(a1/b1);
       else
           return SReference(((intelib_float_t)a1)/b1);
   }
   else if (tid == &SExpressionFloat::TypeId)
       return SReference(a.GetFloat()/b.GetFloat());
   return SReference();
}

SReference IntelibMinus(SReference a)
{
   const IntelibTypeId* tid=&(a->TermType());
   if (tid == &SExpressionInt::TypeId){
       return SReference(-a.GetInt());
   }
   else if (tid == &SExpressionFloat::TypeId)
       return SReference(-a.GetFloat());
   return SReference();
}

SReference IntelibDiv(SReference a, SReference b)
{
   return SReference(a.GetInt()/b.GetInt());
}

SReference IntelibMod(SReference a, SReference b)
{
   return SReference(a.GetInt()%b.GetInt());
}


