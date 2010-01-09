//   InteLib                                    http://www.intelib.org
//   The file refal/rfmatch.hpp
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

#ifndef INTELIB_RFMATCH_HPP_SENTRY
#define INTELIB_RFMATCH_HPP_SENTRY

#include "refal.hpp"



class RfContext;
class RfReference;
struct RfListItem;
class RfExpressionVariable;
class RfContext;
struct RfBinding;

bool RefalMatchProcess(RfReference expression, 
                       RfReference pattern,
                       RfContext &varlist);

void CopyAllButOne(RfReference &res,
                   RfListItem *&to_start,
                   RfListItem *&to_end);

void CopyAll(RfReference &res,
             RfListItem *to_start,
             RfListItem *to_end);

RfReference RefalSubstitute(const RfReference &pattern,
                                  RfContext &varlist,
                                  bool is_last);

// RfRange contains both pattern and expression ranges
struct RfRange
{
    RfListItem *pattern_start;
    RfListItem *pattern_end;
    RfListItem *expr_start;
    RfListItem *expr_end;
    RfRange *up;
    RfRange *left;
    RfRange *right;
    bool was_right;
    RfRange(RfRange *up_range)
        : up(up_range), left(0), right(0),
          was_right(false)
        {}        
    RfRange(RfListItem *p_start, RfListItem *p_end,
            RfListItem *e_start, RfListItem *e_end,
            RfRange *up_range = 0)
        : pattern_start(p_start), pattern_end(p_end),
          expr_start(e_start), expr_end(e_end),
          up(up_range), left(0), right(0),
          was_right(false)
        {}

    RfRange *Clone()
    {
        return new RfRange(pattern_start, pattern_end,
                           expr_start, expr_end);
    }

    void operator =(RfRange *right)
    {
        pattern_start = right->pattern_start;
        pattern_end = right->pattern_end;
        expr_start = right->expr_start;
        expr_end = right->expr_end;
    }

    ~RfRange();
  
    bool IsPatternEmpty();
    bool IsExpressionEmpty();

    void NarrowPatternRangeChar(bool left_to_right);
    void NarrowExpressionRangeChar(bool left_to_right);
    void NarrowRangeChar(bool left_to_right);
    void NarrowPatternRangeBrackets(bool left_to_right);
    void NarrowExpressionRangeBrackets(bool left_to_right);
    void NarrowRangeBrackets(bool left_to_right);
    bool NarrowRangeSingleMatch(bool left_to_right, RfContext &varlist,
                                RfExpressionVariable *var);
    void NarrowRangeMultiMatch();
    void CountRange(RfListItem *pat_elem, RfListItem *expr_elem,
                    bool left_to_right);
    void MaybeHintable(RfBinding *bind);
};

void SetLabels(RfRange *);

/*
    RfBinding is a binding, i.e. one item of context. It contains:

    1) var = pointer to corresponding variable
    2) var_start, var_end, var_cur:
        i) for single-match types (s and t as for now)
            var_start = pointer to an expression
            var_cur = var_end = NULL
        ii) for multi-match types (e as for now)
            var_start = pointer to a beginning of expression
            var_end = pointer to a possible end of expression
            var_cur = pointer to an actual end of expression
            (var_cur runs from var_start to var_end while backtracking)
    3) frame_number = number of matching in a clause, in which
       this variable was bound. Backtracking for a variable is possible
       only its var_cur != var_end and its frame_number == curent
       frame_number
    4) hint and hint_brackets_num are needed for a so called
       "hint optimisation". If we find some non-variable in a pattern
       after a multi-match variable and several (hint_brackets_num)
       brackets, we can avoid some deliberately impossible variants
       while backtracking.
*/
struct RfBinding
{
    const RfExpressionVariable *var;
    RfListItem *var_start;
    RfListItem *var_end;
    RfListItem *var_cur;
    RfBinding *next;
    int frame_number;

    RfListItem *hint;
    int hint_brackets_num;

    RfRange *range;

    bool was_used;

    RfBinding(const RfExpressionVariable *v,
              RfListItem *v_start, RfListItem *v_end,
              RfListItem *v_cur, int f_n)
        : var(v),
          var_start(v_start), var_end(v_end), var_cur(v_cur),
          next(0), frame_number(f_n) 
        {
            hint = 0;
            hint_brackets_num = 0;
            was_used = false;
        }

    ~RfBinding();

    void Print();
    bool IsRight();

    #if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const;
    #endif
};

/*
    We store Refal expressions here, because there is context
    with the pointers to these expressins
*/
struct RfView
{
    RfReference ref;
    RfView *next;
    RfView(RfReference &ref);
    ~RfView();
};

struct RfRangesListElem
{
    RfRange *range;
    RfRangesListElem *next;
    RfRangesListElem(RfRange *a_range)
        : range(a_range), next(0)
        {}
    ~RfRangesListElem();
};

/*
    Context contains a list of RfBinding's, RfView (see above) and
    current frame number.
*/
class RfContext
{
    RfBinding *first;
    RfView *expressions;
    int frame_number;
    
    RfRangesListElem *ranges;
  
    public:

    RfRange *next_range;

    RfContext();

    ~RfContext();

    RfBinding *AddVariable(const RfExpressionVariable *var,
                           RfListItem *var_start,
                           RfListItem *var_end,
                           RfListItem *var_cur);
    RfBinding *GetBinding(RfExpressionVariable *var);
    bool IsEmpty();
    bool IsBacktrackingPossible();
    void Print();
    void AddExpression(RfReference &ref);
    void DeleteExpression();
    void AddRangesTree(RfRange *rng);
    void DeleteRangesTree();
    bool AlreadyHere(RfRange *rng);
    RfReference &GetExpression();
    void NewFrame();
    void DeleteFrame();
    void Renew();
    void Refresh();

    #if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const;
    #endif
};

#include "../sexpress/iexcept.hpp"



class IntelibX_unexpected_refal_call : public IntelibX
{
    public:

    IntelibX_unexpected_refal_call(SReference a_param);
};

class IntelibX_invalid_refal_expression : public IntelibX
{
    public:

    IntelibX_invalid_refal_expression(SReference a_param);
};

class IntelibX_invalid_refal_markup : public IntelibX {
    public:

    IntelibX_invalid_refal_markup(SReference a_param);
};


#endif


