//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/permut/perm.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <intelib/sexpress/sstring.hpp>
#include <intelib/genlisp/conteval.hpp>
#include "perm.hpp"


class PermutationGenerator {
    int n;
    int *perm;
    int *tmp;
public:
    PermutationGenerator(int an)  { 
	n = an;
        perm = new int[n]; 
        tmp = new int[n]; 
	for(int i=0; i<n; i++) perm[i] = i;
    }
    ~PermutationGenerator() { 
        if(perm) delete [] perm; 
	delete [] tmp;
    }

    bool Next(int *a);
    int GetN() const { return n; }
    
private:
    bool PresentWithin(int elem, int max_index) const;
    void ArrangeRest(int min_index);
    void GetFreeElems(int max_index) const;
};

bool PermutationGenerator::PresentWithin(int elem, int max_index) const
{
    for(int i=0; i<=max_index; i++) 
        if(perm[i] == elem) return true;
    return false;
}

void PermutationGenerator::GetFreeElems(int max_index) const
{
    for(int i=0; i<n; i++) tmp[i] = 1;
    for(int i=0; i<=max_index; i++) tmp[perm[i]] = 0;
}

void PermutationGenerator::ArrangeRest(int min_index)
{
    GetFreeElems(min_index-1);
    int free_idx = 0;
    for(int i=min_index; i<n; i++) {
        while(tmp[free_idx]==0) free_idx++;
        perm[i] = free_idx;
	free_idx++;
    }
}

bool PermutationGenerator::Next(int *a) 
{
    if(!perm) return false; 
    int i;
    for(i=0; i<n; i++) a[i] = perm[i];

    for(i=n-1; i>=0; i--) {
        perm[i]++;
	while(perm[i]<n && PresentWithin(perm[i], i-1)) perm[i]++;
	if(perm[i]>=n) continue;
        ArrangeRest(i+1);
	break;
    }
    if(i<0) {
        delete [] perm;
	perm = 0;
    }
    return true;
}






///////////////////////////////
//  SExpressionPermGen


IntelibTypeId SExpressionPermGen::TypeId(&SExpression::TypeId);

SExpressionPermGen::SExpressionPermGen(int n)
    : SExpression(TypeId)
{
    pg = new PermutationGenerator(n);
}

SExpressionPermGen::~SExpressionPermGen()
{
    delete pg;
}

PermutationGenerator* SExpressionPermGen::Get() const
{
    return pg;
}

SString SExpressionPermGen::TextRepresentation() const
{
    return SString("#<PERMUTATION GENERATOR>");
}


///////////////////////////////////
// LFunctionCreatepg

void LFunctionCreatepg::DoApply(int paramsc, 
		const SReference paramsv[],
		IntelibContinuation &lf) const
{
    lf.RegularReturn(new SExpressionPermGen(paramsv[0].GetInt()));
}

SString LFunctionCreatepg::TextRepresentation() const
{
    return SString("#<FUNCTION CREATE-PERMUTATION-GENERATOR>");
}


///////////////////////////////////
// LFunctionNextpermutation

void LFunctionNextpermutation::DoApply(int paramsc, 
		const SReference paramsv[],
		IntelibContinuation &lf) const
{
    SExpressionPermGen *pg = 
	paramsv[0].DynamicCastGetPtr<SExpressionPermGen>();
    if(!pg) {
        throw IntelibX("not a perm gen");
    }
    int n = pg->Get()->GetN();
    int *p = new int[n];
    if(!pg->Get()->Next(p)) {
        lf.RegularReturn(*PTheEmptyList);
	return;
    }
    SReference t = *PTheEmptyList;
    for(int i=0; i<n; i++) t,(p[i]+1);
    delete [] p;
    lf.RegularReturn(t);
}

SString LFunctionNextpermutation::TextRepresentation() const
{
    return SString("#<FUNCTION NEXT-PERMUTATION>");
}
