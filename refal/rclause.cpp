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




// Modified by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <stdio.h>

#include "../sexpress/sstring.hpp"

#include "rfmatch.hpp"
#include "rvars.hpp"
#include "rclause.hpp"



RfClause& RfClause::operator|(const RfClause& a)
{
    if (additional)
        additional->operator|(a);
    else 
        additional = new RfClause(this, a);
    return *this;
}

RfClause& RfClause::operator||(const RfClause& a)
{
    if (next)
        next->operator||(a);
    else 
        next = new RfClause(a);
    return *this;
}

void RfClause::Apply(RfReference &view, RfContext &varlist) const
{
    const RfClause *cl = this;
    do
    {
        RfReference res;
        if (cl->EvaluateClause(view, res, varlist))
        {
            view = res;
            return;
        }
        // Making context ready for the next clause, i.e.
        // setting current frame number to 1
        varlist.Renew();
        cl = cl->next;
    } while (cl);
    throw IntelibX_refal_failure(view);
}

bool RfClause::EvaluateClause(RfReference& view, RfReference &res,
                              RfContext &varlist) const
{
    // current clause and view
    const RfClause *current_clause = this;
    RfReference current_view = view;
    // direction and flag which indicates where a match was successful
    bool direction_left_to_right = true, flag;

    while (true)
    {
        // if that is where-clause
        if (current_clause->where)
        {
            // getting function from pattern field
            SReference fun =
                current_clause->pattern.GetPtr()->GetFirst()->symb_val;
            const RfExpressionSymbol *rs = 
                static_cast<const RfExpressionSymbol*>(fun.GetPtr());
            // refreshing context, i.e. setting current frame number
            // to 1 and frame number of already bound variables to 0
            // to avoid their backtracking
            varlist.Refresh();
            if (!rs)
            {
                //Not 0 - to be corrected!!!
                throw IntelibX_refal_failure(0);
            }
            // getting into anonymous function;
            rs->GetFunction().Apply(current_view, varlist);
            res = current_view;
            return true;
        }
        // that is not where-clause
        flag = true;
        if (!direction_left_to_right)
        {
            // we are moving from right to left and there is no
            // backtracking possibility
            if (!varlist.IsBacktrackingPossible()) {
                flag = false;
            }
            else {
                //Maybe wrong:
                SetLabels(varlist.next_range);
            }
        }
        if (flag)
        {
            // Trying to match
            flag = RefalMatchProcess(current_view,
                                     current_clause->pattern, varlist);
        }
        if (flag)
        {   // Matching successful
            if (!current_clause->additional)
            {   // If there is no next pair, that is success
                res = RefalSubstitute(current_clause->result, varlist,
                                      true);
                return true;
            }
            else
            {   // otherwise, we go to the next pair
                direction_left_to_right = true;
                current_view = RefalSubstitute(current_clause->result,
                                               varlist, false);
                current_view.Evaluate();
                current_clause = current_clause->additional;
                continue;
            }
        }
        else
        {   // Either that matching was unsuccessful,
            // or there was unsuccessful matching elsewhere further
            // and we have no backtracking possibility. We must go back.

            //Check:
            varlist.DeleteRangesTree();
            if (!direction_left_to_right)
            {
                varlist.DeleteExpression();
            }
            else
            {
                direction_left_to_right = false;
            }
            // We can't go back because it is the first clause.
            // This clause is not for us. Fail.
            if (!current_clause->left)
            {
                return false;
            }
            current_clause = current_clause->left;
            current_view = varlist.GetExpression();
            continue;
        }
    }
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString RfClause::TextRepresentation() const
{
    SString res;

    // Text representation of a where-clause
    if (where)
    {
        res += "{\n";
        SReference fun = pattern.GetPtr()->GetFirst()->symb_val;
        const RfExpressionSymbol *rs = 
            static_cast<const RfExpressionSymbol*>(fun.GetPtr());
        if (!rs)
        {
            //Not 0 - to be corrected!!!
            throw IntelibX_refal_failure(0);
        }
        res += rs->GetFunction().TextRepresentation();
        res += "\n}";
        return res;
    }

    res = pattern->TextRepresentation();
    res += additional ? ", " : " = ";
    res += result->TextRepresentation();
    if (additional)
    {
        res += " : ";
        res += additional->TextRepresentation();
    }
    if (next)
    {
        res += ";\n";
        res += next->TextRepresentation();
    }
    return res;
}
#endif

// Adding an item to the tail of first list in RfWith
void RfWith::AddWithItem() {
    if (!first)
    {
        first = new RfWithItem();
    }
    else
    {
        RfWithItem *temp = new RfWithItem;
        temp->next = first;
        first = temp;
    }
}

// Moving an item from the first list to the second one
void RfWith::MoveToSecond() {
    if (!first)
    {
        //Not 0 - to be corrected!!!
        throw IntelibX_refal_failure(0);
    }
    RfWithItem *temp = first;
    first = first->next;
    temp->next = second;
    second = temp;
}

IntelibTypeId 
RfExpressionSymbol::TypeId(&SExpressionLabel::TypeId, false);

IntelibTypeId 
RfExpressionLibSymbol::TypeId(&SExpressionLabel::TypeId, false);

IntelibX_refal_failure::
IntelibX_refal_failure(SReference a_param) 
    : IntelibX("Refal matching failure", a_param) {}


