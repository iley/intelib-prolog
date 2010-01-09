//   InteLib                                    http://www.intelib.org
//   The file refal/rfmatch.cpp
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

#include "../sexpress/sstring.hpp"

#include "refal.hpp"
#include "rvars.hpp"
#include "rclause.hpp"
#include "rfmatch.hpp"



long total_matchprocess_calls = 0;

RfView::RfView(RfReference &ref)
{
    this->ref = ref;
    this->next = 0;
}

RfView::~RfView()
{
    if (next)
    {
        delete next;
        next = 0;
    }
}

RfRangesListElem::~RfRangesListElem()
{
    delete range;
    if (next)
    {
        delete next;
        next = 0;
    }
}

RfRange::~RfRange()
{
    if (left)
        delete left;
    if (right)
        delete right;
}

bool RfRange::IsPatternEmpty()
{
    return (pattern_start == 0);
}

bool RfRange::IsExpressionEmpty()
{
    return (expr_start == 0);
}

bool DoBracketsMatch(SReference &value1, SReference &value2,
                     bool left_to_right)
{
    if (left_to_right)
        return (value1 == RfListItem::left_par && 
                value2 == RfListItem::right_par);
    else
        return (value1 == RfListItem::right_par && 
                value2 == RfListItem::left_par);
}

void RfRange::NarrowPatternRangeChar(bool left_to_right)
{
    if (pattern_start == pattern_end)
    {
        pattern_start = pattern_end = 0;
    }
    else
    {
        if (left_to_right)
            pattern_start = pattern_start->next;
        else
            pattern_end = pattern_end->prev;
    }
}

void RfRange::NarrowExpressionRangeChar(bool left_to_right)
{
    if (expr_start == expr_end)
    {
        expr_start = expr_end = 0;
    }
    else
    {
        if (left_to_right)
            expr_start = expr_start->next;
        else
            expr_end = expr_end->prev;
    }
}

void RfRange::NarrowRangeChar(bool left_to_right)
{
    NarrowPatternRangeChar(left_to_right);
    NarrowExpressionRangeChar(left_to_right);
}

void RfRange::NarrowPatternRangeBrackets(bool left_to_right)
{
    if (left_to_right)
    {
        pattern_start = pattern_start->pair;
    }
    else
    {
        pattern_end = pattern_end->pair;
    }
    NarrowPatternRangeChar(left_to_right);
}

void RfRange::NarrowExpressionRangeBrackets(bool left_to_right)
{
    if (left_to_right)
    {
        expr_start = expr_start->pair;
    }
    else
    {
        expr_end = expr_end->pair;
    }
    NarrowExpressionRangeChar(left_to_right);
}

void RfRange::NarrowRangeBrackets(bool left_to_right)
{
    NarrowPatternRangeBrackets(left_to_right);
    NarrowExpressionRangeBrackets(left_to_right);
}

// Binding a single-match variable and
// exclude just bound expression from our range
bool RfRange::NarrowRangeSingleMatch(bool left_to_right,
                                     RfContext &varlist,
                                     RfExpressionVariable *var)
{
    RfListItem *expr_current = (left_to_right) ? expr_start : expr_end;
    if (var->IsT() && 
        expr_current && 
        expr_current->symb_val == 
        (left_to_right ? RfListItem::left_par : RfListItem::right_par))
    {
        if (!DoBracketsMatch(expr_current->symb_val,
                             expr_current->pair->symb_val,
                             left_to_right))
            throw IntelibX_invalid_refal_markup(0);
        NarrowPatternRangeChar(left_to_right);
        if (left_to_right)
        {
            expr_start = expr_start->pair;
            varlist.AddVariable(var, expr_current, expr_start, 0);
        }
        else
        {
            expr_end = expr_end->pair;
            varlist.AddVariable(var, expr_end, expr_current, 0);
        }
        NarrowExpressionRangeChar(left_to_right);
        return true;
    }
    if (expr_current && !expr_current->IsMarkup())
    {
        varlist.AddVariable(var, expr_current, expr_current, 0);
        NarrowRangeChar(left_to_right);
        return true;
    }
    return false;
}

// Excluding a variable from pattern range.
// Coresponding expression range will be created
// in EqualValues funcion as a side effect.
void RfRange::NarrowRangeMultiMatch()
{
    NarrowPatternRangeChar(true);
}

// deleting parenthesis from both pattern and expression
void RfRange::CountRange(RfListItem *pat_elem, RfListItem *expr_elem,
                         bool left_to_right)
{
    if (left_to_right)
    {
        pattern_start = pat_elem;
        pattern_end = pat_elem->pair;
        expr_start = expr_elem;
        expr_end = expr_elem->pair;
    }
    else
    {
        pattern_end = pat_elem;
        pattern_start = pat_elem->pair;
        expr_end = expr_elem;
        expr_start = expr_elem->pair;
    }
    NarrowRangeChar(true);
    NarrowRangeChar(false);
}

// maybe, we'll find hint for this case
void RfRange::MaybeHintable(RfBinding *bind)
{
    int brackets_number = 0;
    RfListItem *temp = pattern_start->next;
    while (temp && temp->symb_val == RfListItem::left_par)
    {
        brackets_number++;
        temp = temp->next;
    }
    if (!temp || temp->IsMarkup())
    {
        bind->hint_brackets_num = brackets_number;
        return;
    }
    if (temp->IsChar())
    {
        bind->hint = temp;
        bind->hint_brackets_num = brackets_number;
        return;
    }
    RfExpressionVariable *var =
        temp->symb_val.DynamicCastGetPtr<RfExpressionVariable>();
    if (var)
    {
      bind->hint_brackets_num = brackets_number;
      return;
    }
    bind->hint = temp;
    bind->hint_brackets_num = brackets_number;
}

RfBinding::~RfBinding()
{
    if (next)
    {
        delete next;
        next = 0;
    }
}

// maybe, we can avoid this case (thanks to our hint) as
// impossible
bool RfBinding::IsRight()
{
    if (var_cur == var_end)
        return false;
    RfListItem *temp = var_cur->next;
    int br = hint_brackets_num;
    while (temp != var_end && br > 0 && 
           temp->symb_val == RfListItem::left_par)
    {
        br--;
        temp = temp->next;
    }
    if (br > 0)
        return false;
    if (!hint)
        return true;
    //XXX Check this code one more time:
    return temp->IsEql(*hint);
}

RfContext::RfContext()
    : first(0), expressions(0), frame_number(1), ranges(0), next_range(0)
    {}

RfContext::~RfContext()
{
    if (first)
    {
        delete first;
        first = 0;
    }
    if (expressions)
    {
        delete expressions;
        expressions = 0;
    }
    if (ranges)
    {
        delete ranges;
        ranges = 0;
    }
}

void RfContext::Print()
{
    printf("%s", TextRepresentation().c_str());
}

SString RfContext::TextRepresentation() const
{
    SString res;
    RfBinding *temp = first;
    while (temp)
    {
        res += temp->TextRepresentation().c_str();
        res += "\n";
        temp = temp->next;
    }
    return res;
}

// adding an expression to the list of stored expressions
void RfContext::AddExpression(RfReference &ref)
{
    if (!expressions)
    {
        expressions = new RfView(ref);
    }
    else
    {
        RfView *temp = new RfView(ref);
        temp->next = expressions;
        expressions = temp;
    }
}

void RfContext::AddRangesTree(RfRange *rng)
{
    if (!ranges) {
        ranges = new RfRangesListElem(rng);
    }
    else {
        RfRangesListElem *temp = new RfRangesListElem(rng);
        temp->next = ranges;
        ranges = temp;
    }
}

// deleting an expression from the list of stored expressions
void RfContext::DeleteExpression()
{
    if (expressions)
    {
        RfView *temp = expressions;
        expressions = expressions->next;
        temp->next = 0;
        delete temp;
    }
}

void RfContext::DeleteRangesTree()
{
    if (ranges) {
        RfRangesListElem *temp = ranges;
        ranges = ranges->next;
        temp->next = 0;
        delete temp;
    }
}

bool RfContext::AlreadyHere(RfRange *rng)
{
    return (ranges && ranges->range == rng);
}

// getting the first expressions from the list of expressions
RfReference &RfContext::GetExpression()
{
    if (expressions)
    {
        return expressions->ref;
    }
    throw IntelibX_refal_failure(0);
}

void RfBinding::Print()
{
    printf("%s", TextRepresentation().c_str());
}

SString RfBinding::TextRepresentation() const
{
    SString res;
    RfListItem *temp;
    res += var->GetName();
    res += " = ";
    if (var->IsMultiMatch())
    {
        if (!var_cur)
            return res;
        temp = var_start;
        while (temp && temp != var_cur->next)
        {
            res += temp->TextRepresentationThis().c_str();
            res += " ";
            temp = temp->next;
        }
    }
    else
    {
        temp = var_start;
        while (temp && temp != var_end->next)
        {
            res += temp->TextRepresentationThis().c_str();
            res += " ";
            temp = temp->next;
        }
    }
    return res;
}

// Adding a new variable to our context
RfBinding *RfContext::AddVariable(const RfExpressionVariable *var,
                                  RfListItem *var_start,
                                  RfListItem *var_end,
                                  RfListItem *var_cur)
{
    if (first)
    {
        RfBinding *temp =
            new RfBinding(var, var_start, var_end, var_cur, frame_number);
        temp->next = first;
        first = temp;
    }
    else
    {
        first =
            new RfBinding(var, var_start, var_end, var_cur, frame_number);
    }
    return first;
}

// Searching for a binding by a certain variable
RfBinding *RfContext::GetBinding(RfExpressionVariable *var)
{
    RfBinding *temp = first;
    while (temp)
    {
        if (temp->var == var)
            return temp;
        temp = temp->next;
    }
    return 0;
}

bool RfContext::IsEmpty()
{
    return (first == 0);
}

// Are there any variables which allow backtracking?
bool RfContext::IsBacktrackingPossible()
{
    RfBinding *temp;
    while (first && (first->frame_number == frame_number) && 
           !(first->var->IsMultiMatch() && 
             first->var_cur != first->var_end))
    {
        temp = first;
        first = first->next;
        temp->next = 0;
        delete temp;
    }
    if (!first || (first->frame_number != frame_number))
    {
        DeleteFrame();
        next_range = 0;
        return false;
    }
    while ((!first->var_cur) || first->var_cur != first->var_end)
    {   
        if (first->var_cur)
            first->var_cur = first->var_cur->next;
        else
            first->var_cur = first->var_start;
        //XXX Check this code one more time:
        if (first->var_cur && 
            first->var_cur->symb_val == RfListItem::left_par)
            first->var_cur = first->var_cur->pair;
        //XXX The end of the code to be checked.
        if (first->hint || first->hint_brackets_num)
        {
            if (!first->IsRight())
            {
                if (first->var_cur == first->var_end)
                    return IsBacktrackingPossible();
                else
                    continue;
            }
        }
        break;
    }
    next_range = first->range;
    return true;
}

void RfContext::NewFrame()
{
    frame_number++;
}

void RfContext::DeleteFrame()
{
    frame_number--;
}

void RfContext::Renew()
{
    frame_number = 1;
}

void RfContext::Refresh()
{
    frame_number = 1;
    RfBinding *temp = first;
    while (temp)
    {
        temp->frame_number = 0;
        temp = temp->next;
    }
}

// Is a value of our variable equal to the beginning of
// our expression?
bool EqualValues(RfBinding *bind, RfRange *current_range,
                 bool left_to_right)
{
    //One value
    if (!bind->var->IsMultiMatch())
    {
        if (left_to_right)
        {
            if (bind->var_start->symb_val == RfListItem::left_par)
            {
                for (RfListItem *temp = bind->var_start;
                     temp != bind->var_start->pair->next;
                     temp=temp->next)
                {
                    if (!current_range->expr_start)
                        return false;
                    if (!temp->IsEql(*current_range->expr_start))
                        return false;
                    current_range->
                        NarrowExpressionRangeChar(left_to_right);
                }
                current_range->NarrowPatternRangeChar(left_to_right);
                return true;
            }
            if (!current_range->expr_start)
                return false;
            if (bind->var_start->IsEql(*current_range->expr_start))
            {
                current_range->NarrowRangeChar(left_to_right);
                return true;
            }
            else
                return false;
        }
        else
        {
            if (bind->var_end->symb_val == RfListItem::right_par)
            {
                for (RfListItem *temp = bind->var_end;
                     temp != bind->var_end->pair->prev;
                     temp=temp->prev)
                {
                    if (!current_range->expr_end)
                        return false;
                    if (!temp->IsEql(*current_range->expr_end))
                        return false;
                    current_range->
                        NarrowExpressionRangeChar(left_to_right);
                }
                current_range->NarrowPatternRangeChar(left_to_right);
                return true;
            }
            if (!current_range->expr_end)
                return false;
            if (bind->var_end->IsEql(*current_range->expr_end))
            {
                current_range->NarrowRangeChar(left_to_right);
                return true;
            }
            else
                return false;
        }
    }

    //Empty value
    if (!bind->var_cur)
    {
        current_range->NarrowPatternRangeChar(left_to_right);
        return true;
    }

    //Multivalues
    if (left_to_right)
    {
        //Maybe, wrong:
        if (bind->var_start == current_range->expr_start)
        {
            current_range->expr_start = (bind->var_cur != 0) ?
                                        bind->var_cur->next :
                                        bind->var_start;
            if (current_range->expr_start == 0)
                current_range->expr_end = 0;
            current_range->NarrowPatternRangeChar(left_to_right);
            return true;
        }

        for (RfListItem *temp = bind->var_start;
             temp != bind->var_cur->next;
             temp=temp->next)
        {
            if (!current_range->expr_start)
                return false;
            if (!temp->IsEql(*current_range->expr_start))
                return false;
            current_range->NarrowExpressionRangeChar(left_to_right);
        }
        current_range->NarrowPatternRangeChar(left_to_right);
        return true;
    }
    else
    {
        //Maybe, wrong:
        if (bind->var_cur == current_range->expr_end)
        {
            current_range->expr_end = (bind->var_start != 0) ?
                                      bind->var_start->prev :
                                      0;
            if (current_range->expr_end == 0)
                current_range->expr_start = 0;
            current_range->NarrowPatternRangeChar(left_to_right);
            return true;
        }

        for (RfListItem *temp = bind->var_cur;
             temp != bind->var_start->prev;
             temp=temp->prev)
        {
            if (!current_range->expr_end)
                return false;
            if (!temp->IsEql(*current_range->expr_end))
                return false;
            current_range->NarrowExpressionRangeChar(left_to_right);
        }
        current_range->NarrowPatternRangeChar(left_to_right);
        return true;
    }
}

void SetLabels(RfRange *tree_range)
{
    RfRange *temp1 = tree_range, *temp2;
    while (temp1->up) {
        temp2 = temp1->up;
        temp2->was_right = (temp2->right == temp1);
        temp1 = temp2;
    }
}

// Main matching function
bool MatchProcess(RfContext &varlist, RfRange *main_range)
{
    total_matchprocess_calls++;

    bool left_to_right;
    bool up_to_down = true;
    bool res_flag = true;

    RfRange *current_range = new RfRange(0);
    RfRange *tree_range    = main_range;

    RfListItem *current_pat_elem, *current_expr_elem;

    while (true)
    {
        if (!res_flag) {
            if (varlist.IsBacktrackingPossible()) {
                tree_range = varlist.next_range;
                SetLabels(tree_range);
                res_flag = true;
                up_to_down = true;
                continue;
            }
            else {
                while (tree_range->up)
                    tree_range = tree_range->up;
                if (!varlist.AlreadyHere(tree_range))
                    varlist.AddRangesTree(tree_range);
                delete current_range;
                return false;
            }
        }

        if (!up_to_down) {
            if (!tree_range->right || tree_range->was_right) {
                if (!tree_range->up) {
                    if (!varlist.AlreadyHere(tree_range))
                        varlist.AddRangesTree(tree_range);
                    delete current_range;
                    //Check:
                    varlist.next_range = 0;
                    return true;
                }
                else {
                    tree_range = tree_range->up;
                    continue;
                }
            }
            else {
                tree_range->was_right = true;
                up_to_down = true;
                tree_range = tree_range->right;
                continue;
            }
        }

        *current_range = tree_range;
        //Maybe, bullshit. REMOVE:
        tree_range->was_right = false;

        left_to_right = true;

        while (!current_range->IsPatternEmpty())
        {
            if (left_to_right)
            {
                current_pat_elem = current_range->pattern_start;
                current_expr_elem = current_range->expr_start;
            }
            else
            {
                current_pat_elem = current_range->pattern_end;
                current_expr_elem = current_range->expr_end;
            }

            //Char in pattern
            if (current_pat_elem->IsChar())
            {
                if (current_expr_elem           &&  
                    current_expr_elem->IsChar() && 
                    current_expr_elem->charcode == 
                    current_pat_elem->charcode)
                {
	            current_range->NarrowRangeChar(left_to_right);
                    continue;
                }
                else
                {
                    res_flag = false;
                    break;
                }
            }

            //Brackets in pattern
            if (current_pat_elem->symb_val == RfListItem::left_par || 
                current_pat_elem->symb_val == RfListItem::right_par)
            {
                if (!DoBracketsMatch(current_pat_elem->symb_val,
                                     current_pat_elem->pair->symb_val,
                                     left_to_right))
                    throw IntelibX_invalid_refal_markup(0);
                if (!(current_expr_elem && 
                    (current_expr_elem->symb_val ==
                     RfListItem::left_par || 
                     current_expr_elem->symb_val ==
                     RfListItem::right_par)))
                {
                    res_flag = false;
                    break;
                }
                if (!DoBracketsMatch(current_expr_elem->symb_val,
                                     current_expr_elem->pair->symb_val,
                                     left_to_right))
                    throw IntelibX_invalid_refal_markup(0);

                //Maybe, bullshit. REMOVE:
                if (tree_range->left) {
                    delete tree_range->left;
                    tree_range->left = 0;
                }
                if (tree_range->right) {
                    delete tree_range->right;
                    tree_range->right = 0;
                }

                if (!tree_range->left) {
                    tree_range->left = new RfRange(tree_range);
                }
                if (!tree_range->right) {
                    tree_range->right = new RfRange(tree_range);
                }
                if (left_to_right) {
                    tree_range->left->
                        CountRange(current_pat_elem,
                                       current_expr_elem, left_to_right);
                    *(tree_range->right) = current_range;
                    tree_range->right->
                        NarrowRangeBrackets(left_to_right);
                }
                else {
                    tree_range->right->
                        CountRange(current_pat_elem,
                                       current_expr_elem, left_to_right);
                    *(tree_range->left) = current_range;
                    tree_range->left->
                        NarrowRangeBrackets(left_to_right);
                }
                *current_range = tree_range->left;
                tree_range = tree_range->left;
                continue;
            }

            RfExpressionVariable *var =
                current_pat_elem->
                symb_val.DynamicCastGetPtr<RfExpressionVariable>();

            //Maybe still not a variable in pattern
            if (!current_pat_elem->IsChar() && 
                !current_pat_elem->IsMarkup() && !var)
            {
                if (current_expr_elem && 
                    current_pat_elem->IsEql(*current_expr_elem))
                {
                    current_range->NarrowRangeChar(left_to_right);
                    continue;
                }
                else
                {
                    res_flag = false;
                    break;
                }
            }

            //Refal call in pattern
            if (current_pat_elem->symb_val == RfListItem::left_call || 
                current_pat_elem->symb_val == RfListItem::right_call)
            {
                throw IntelibX_unexpected_refal_call(0);
            }

            //At last, a variable in pattern
            if (!var)
                throw IntelibX_invalid_refal_expression(0);

            RfBinding *bind = varlist.GetBinding(var);

            if (bind)
            {   
                // bound
                if (EqualValues(bind, current_range, left_to_right))
                {
                    //Maybe, this:
                    left_to_right = true;
                    continue;
                }
                res_flag = false;
                break;
            }
            else
            {   // unbound
                if (!var->IsMultiMatch())
                {
                    if (current_range->
                        NarrowRangeSingleMatch(left_to_right,
                                               varlist,
                                               var))
                    {
                        continue;
                    }
                    else
                    {
                        res_flag = false;
                        break;
                    }
                }
                if (left_to_right)
                {
                    left_to_right = false;
                    continue;
                }
                else
                {
                    left_to_right = true;
                    var = current_range->
                        pattern_start->
                        symb_val.
                        DynamicCastGetPtr<RfExpressionVariable>();
                    if (!var)
                        throw IntelibX_invalid_refal_expression(0);
                    bind = varlist.AddVariable(var, 0, 0, 0);
                    if (current_range->pattern_start == 
                        current_range->pattern_end)
                    {   //Closed variable
                        bind->var_start = current_range->expr_start;
                        bind->var_cur =
                            bind->var_end =
                            current_range->expr_end;
                        //Maybe, this:
                        current_range->pattern_start = 0;
                        current_range->pattern_end = 0;
                        current_range->expr_start = 0;
                        current_range->expr_end = 0;
                        break;
                    }
                    else
                    {   //Open variable
                        bind->var_start = current_range->expr_start;
                        bind->var_cur = 0;
                        bind->var_end = current_range->expr_end;
                        //Maybe, bullshit. REMOVE:
                        if (tree_range->left) {
                            delete tree_range->left;
                            tree_range->left = 0;
                        }

                        if (!tree_range->left) {
                            tree_range->left = new RfRange(tree_range);
                        }
                        tree_range = tree_range->left;
                        *tree_range = current_range;
                        bind->range = tree_range;
                        current_range->MaybeHintable(bind);
                        current_range->NarrowRangeMultiMatch();
                        continue;
                    }
                }
            }
        }

        up_to_down = false;

        if (current_range->IsPatternEmpty())
        {
            res_flag = current_range->IsExpressionEmpty();
            continue;
        }
    }

    return true;
}

bool RefalMatchProcess(RfReference expression, 
                       RfReference pattern,
                       RfContext &varlist)
{
    bool flag;
    //XXX Check this code one more time:
    if (!expression.GetPtr()) expression = new RfExpression;
    if (!pattern.GetPtr()) pattern = new RfExpression;
    //XXX The end of the code to be checked.
    //XXX Check this code one more time:
    if (!varlist.next_range) {
        RfRange *main_range = new RfRange(pattern->GetFirst(),
                                          pattern->GetLast(),
                                          expression->GetFirst(),
                                          expression->GetLast());
        flag = MatchProcess(varlist, main_range);
    }
    else {
        flag = MatchProcess(varlist, varlist.next_range);
    }
    //XXX Check this code one more time:
 
    if (flag)
    {
        varlist.AddExpression(expression);
        varlist.NewFrame();
    }

    return flag;
}

IntelibX_invalid_refal_expression::
IntelibX_invalid_refal_expression(SReference a_param) 
    : IntelibX("Invalid Refal expression encountered", a_param)
    {}

IntelibX_unexpected_refal_call::
IntelibX_unexpected_refal_call(SReference a_param) 
    : IntelibX("Refal calls are not allowed within a pattern", a_param)
    {}

IntelibX_invalid_refal_markup::
IntelibX_invalid_refal_markup(SReference a_param) 
    : IntelibX("Invalid Refal markup encountered", a_param)
    {}

void CopyAllButOne(RfReference &res,
                   RfListItem *&to_start,
                   RfListItem *&to_end)
{
    RfListItem *aux = new RfListItem(*to_start);
    res->AddToEndSmart(aux, aux);
    RfListItem *temp = to_start->next;
    if (to_start) {
        to_start->next = to_end->next;
    }
    if (to_start && to_end->next) {
        to_end->next->prev = to_start;
    }
    res->AddToEndSmart(temp, to_end);
    if (to_start->IsLeftMarkup()) {
        to_start->pair->pair = aux;
        aux->pair = to_start->pair;
    }
    to_start = aux;
}

void CopyAll(RfReference &res,
             RfListItem *to_start,
             RfListItem *to_end)
{
    if (to_start->prev) {
        to_start->prev->next = to_end->next;
    }
    if (to_end->next) {
        to_end->next->prev = to_start->prev;
    }
    res->AddToEndSmart(to_start, to_end);
}

RfReference RefalSubstitute(const RfReference &pattern,
                            RfContext &varlist,
                            bool is_last)
{
    RfReference res(new RfExpression);

    RfListItem *iter, *temp;

    for (iter = pattern->GetFirst(); iter; iter = iter->next)
    {
        RfExpressionVariable *var =
            (!iter->IsChar() && !iter->IsMarkup()) ?
            iter->symb_val.DynamicCastGetPtr<RfExpressionVariable>() : 0;
        RfBinding *bind;
        if (var && ((bind = varlist.GetBinding(var))))
        {
            if (bind->var->IsMultiMatch())
            {
                if (!bind->var_cur)
                    continue;
                if (is_last && !bind->was_used &&
                    bind->var_cur != bind->var_start) {
                    bind->was_used = true;
                    CopyAllButOne(res, bind->var_start, bind->var_cur);
                }
                else {
                    temp = bind->var_start;
                    while (temp && temp != bind->var_cur->next)
                    {
                        res->QuickAdd(new RfListItem(*temp));
                        temp = temp->next;
                    }
                }
            }
            else
            {
                if (is_last && !bind->was_used &&
                    bind->var_end != bind->var_start) {
                    bind->was_used = true;
                    CopyAllButOne(res, bind->var_start, bind->var_end);
                }
                else {
                    temp = bind->var_start;
                    while (temp && temp != bind->var_end->next)
                    {
                        res->QuickAdd(new RfListItem(*temp));
                        temp = temp->next;
                    }
                }
            }
        } 
        else
        {
            if (!var)
            {
                res->QuickAdd(new RfListItem(*iter));
            }
            else
            {
                throw IntelibX_invalid_refal_expression(0);
            }
        }
    }

    return res;
}


