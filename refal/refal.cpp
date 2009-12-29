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


#include "../sexpress/sstring.hpp"

#include "rclause.hpp"



INTELIB_IMPLEMENT_CUSTOM_MEMORY_MANAGEMENT(RfListItem)

SLabel RfListItem::left_par("(");
SLabel RfListItem::right_par(")");
SLabel RfListItem::left_call("<");
SLabel RfListItem::right_call(">");

// Text representation of a whole list of RfListItem's
SString RfListItem::TextRepresentation() const
{
    SString res = TextRepresentationThis();
    if (next)
    {
        //Temporarily eliminated:
        //res += " ";
        res += next->TextRepresentation();
    }
    return res;
}

// Text representation of one RfListItem
SString RfListItem::TextRepresentationThis() const
{
    SString res;
    if (IsChar())
    {
        /*
        char x[4] = "\".\"";
        x[1] = (char)charcode;
        res = x;
        */
 
        // New code:
        char x[2];
        x[0] = (char)charcode;
        x[1] = '\0';

        res = x;
    }
    else
    if (symb_val == left_par)
        res = "(";
    else
    if (symb_val == left_call) 
        res = "<";
    else
    if (symb_val == right_par) 
        res = ")";
    else
    if (symb_val == right_call) 
        res = ">";
    else  
    {
        res = symb_val->TextRepresentation();
    }
    return res;
}

void RfListItem::Pair(RfListItem* other)
{
    pair = other;
    other->pair = this;
}

RfListItem* RfListItem::NextTerm() const
{
    if (IsLeftMarkup() && pair)
    {
        return pair->next;
    }
    else
    {
        return next;
    }
}

RfListItem* RfListItem::PrevTerm() const
{
    RfListItem *res = prev;
    if (!res) return 0;
    if (res->pair && res->IsRightMarkup())
    {
        res = res->pair;
    }
    return res;
}

IntelibTypeId RfExpression::TypeId(&SExpression::TypeId, true);

RfExpression::~RfExpression()
{
    while (first)
    {
        RfListItem *tmp = first;
        first = tmp->next;
        delete tmp;
    }
}

SString RfExpression::TextRepresentation() const
{
    SString res;
 
    bool in_string = false;
    SString cur_string;
    bool needs_delimiter = false;
    RfListItem* last_seen = 0;

    for (RfListItem* i = first; i; i=i->next)
    {
        last_seen = i;
        if (in_string)
        {
            if (i->IsChar())
            {
                 cur_string += (char)(i->charcode);
                 continue; 
            }
            else
            {
                 if (needs_delimiter) res += " ";
                 res += cur_string->TextRepresentation();
                 in_string = false;
                 needs_delimiter = true;
            }
        }

        if (i->IsLeftMarkup())
        {
            if (needs_delimiter) res += " ";
            if (i->symb_val == RfListItem::left_par)
                res += "("; 
            else 
                res += "<";
            if (!i->pair)
            {
                res += "#";
            }
            needs_delimiter = false;
        }
        else 
            if (i->IsRightMarkup())
            {
                // no delimiter anyways
                if (!i->pair)
                {
                    res += "#";
                } 
                if (i->symb_val == RfListItem::right_par)
                    res += ")"; 
                else 
                    res += ">";
                needs_delimiter = true;
            }
            else 
                if (i->IsChar())
                {
                    in_string = true;
                    char c[2] = {0,0};
                    c[0] = i->charcode;
                    cur_string = c; 
                }
                else

                    // now it's clear that specval is 0
                    #if 0
                    if (!i->s_val.GetPtr())
                    {
                        if (needs_delimiter) res += " ";
                        res += "{NULL}";
                        needs_delimiter = true;
                    } else
                    #endif

                {
                    if (needs_delimiter) res += " ";
                    res += i->symb_val->TextRepresentation();
                    needs_delimiter = true;
                }
    }

    if (in_string)
    {
        if (needs_delimiter) res += " ";
        res += cur_string->TextRepresentation();
    }

    if (last_seen && last && last_seen != last) 
        res += " #%# ";

    return res;
}


static SReference DoConvertToLisp(const RfListItem* from)
{
    if (!from) return *PTheEmptyList;

    if (from->IsLeftMarkup())
    {
        RfListItem *pair = from->pair;
        return SReference(DoConvertToLisp(from->next),
                          DoConvertToLisp(pair->next));
    }
    else 
        if (from->IsRightMarkup())
        {
            return *PTheEmptyList;
        }

    if (from->IsChar())
    {
        SString str;
        str += (char)(from->charcode);
        from = from->next;
        while (from && from->IsChar())
        {
            str += (char)(from->charcode);
            from = from->next;
        }
        return SReference(str, DoConvertToLisp(from));
    }

    return SReference(from->symb_val, DoConvertToLisp(from->next));
}


SReference RfExpression::ConvertToLisp() const
{
    if (!first) return *PTheEmptyList;
    return DoConvertToLisp(first);
}

void RfExpression::Exclude(RfListItem* from, RfListItem* thru)
{
    RfListItem* left = from->prev;
    RfListItem* right = thru->next;

    from->prev = 0;
    thru->next = 0;

    if (left)
        left->next = right;
    else
        first = right;

    if (right)
        right->prev = left;
    else
        last = left;
}

void RfExpression::InsertListAfter(RfListItem* from, 
                                   RfListItem* left, 
                                   RfListItem* right)
{
    RfListItem* thru;
    if (!from)
        thru = first; 
    else 
        thru = from->next;

    if (thru)
    {
        thru->prev = right;
    }
    else
    {
        last = right;
    }
    right->next = thru;

    if (from)
    {
        from->next = left;
    }
    else
    {
        first = left;
    }
    left->prev = from;
}

void RfExpression::Add(const SReference &elem)
{
    if (elem->TermType() == SExpressionString::TypeId)
    {
        const char *str = 
            static_cast<SExpressionString*>(elem.GetPtr())->GetValue();
        while (*str)
        {
            unsigned char c = *str;
            AddToEnd(new RfListItem((int)c));
            str++;
        }
    }
    else
        if (elem->TermType() == RfExpression::TypeId)
        {
            RfExpression *expr =
                static_cast<RfExpression*>(elem.GetPtr());
            RfListItem* left = 0;
            if (expr->ShallBeEnclosed())
            {
                left = new RfListItem(0, RfListItem::left_par);
                AddToEnd(left);
            }
            for (RfListItem* i = expr->first; i; i = i->next)
            {
                //XXX possibly wrong
                QuickAdd(new RfListItem(*i));
            }
            if (expr->ShallBeEnclosed())
            {
                RfListItem* right =
                    new RfListItem(0, RfListItem::right_par);
                AddToEnd(right);
                left->Pair(right);
            }
        }
        else
        {
            QuickAdd(new RfListItem(0, elem));
        }
}

void RfExpression::BeginSubcall(const SReference &elem)
{
    QuickAdd(new RfListItem(0, RfListItem::left_call));
    if (elem.GetPtr()) Add(elem);
}

void RfExpression::BeginSubcall()
{
    QuickAdd(new RfListItem(0, RfListItem::left_call));
}

void RfExpression::EndSubcall(const SReference &elem)
{
    Add(elem);
    EndSubcall();
}

void RfExpression::EndSubcall()
{
    QuickAdd(new RfListItem(0, RfListItem::right_call));
}

void RfExpression::QuickAdd(RfListItem *elem)
{
    AddToEnd(elem);
    if (last->IsLeftMarkup())
    {
        last->pair = last_unpaired;
        last_unpaired = last;
    } else
    if (last_unpaired && last->IsRightMarkup())
    {   
        last->pair = last_unpaired;
        RfListItem *tmp = last_unpaired->pair;
        last_unpaired->pair = last;
        last_unpaired = tmp;
    }
}

RfListItem* RfExpression::AddToEnd(const SReference &ref)
{
    //XXX check if no strings are added this way ;-)
    RfListItem *tmp = new RfListItem(0, ref);
    AddToEnd(tmp);
    return tmp;
}

void RfExpression::AddToEnd(RfListItem *ref)
{
    if (!first)
    {
        last = first = ref;
        return;
    }
    last->next = ref;
    ref->prev = last;
    last = ref;
}

void RfExpression::AddToEndSmart(RfListItem *ref1, RfListItem *ref2)
{
    if (!first)
    {
        first = ref1;
        last = ref2;
        last->next = 0;
        return;
    }
    last->next = ref1;
    ref1->prev = last;
    last = ref2;
    last->next = 0;
}

void RfReference::Evaluate() 
{
    RfListItem* from = GetPtr()->GetFirst();
    while (EvaluationStep(&from));
}

bool RfReference::SearchForCall(RfListItem** from, 
                                RfListItem** left,
                                RfListItem** right)
{
    // in order to find the most left-inner call, we just search 
    // for the first closing angle bracket
    while (*from)
    {
        if ((*from)->symb_val == RfListItem::right_call)
        {
            *left = (*from)->pair;
            *right = *from;
            return true;
        }
        from = &((*from)->next);
    }
    return false;
}

bool RfReference::EvaluationStep(RfListItem **from)
{
    RfListItem *left, *right;
    if (!SearchForCall(from, &left, &right)) return false;
    *from = left->prev; 
    (*this)->Exclude(left, right);

    if (left == right)
        throw IntelibX_not_a_refal_function(*PTheEmptyList);

    left = left->next;
    delete left->prev;
    left->prev = 0;

    right = right->prev;
    delete right->next;
    right->next = 0;

    //XXX IsMarkup() may be unneeded
    if (left->IsChar() || left->IsMarkup())
        throw IntelibX_not_a_refal_function(*PTheEmptyList);
    SReference fun = left->symb_val;

    if (left->next)
    {   // argument's not empty
        left = left->next;
        delete left->prev;
        left->prev = 0;
    }
    else
    {
        delete left;
        left = right = 0;
    }

    
    {   // argument localized here 
        RfReference argument(new RfExpression(left, right));
        if (fun->TermType().IsSubtypeOf(RfExpressionLibSymbol::TypeId)) 
        {
            const RfExpressionLibSymbol *rs = 
                static_cast<const RfExpressionLibSymbol*>(fun.GetPtr());
            rs->Apply(argument);
        }
        else
            if (fun->TermType() == RfExpressionSymbol::TypeId)
            {
                const RfExpressionSymbol *rs = 
                    static_cast<const RfExpressionSymbol*>(fun.GetPtr());
                RfContext varlist;
                rs->GetFunction().Apply(argument, varlist);
            }
            else 
            {
                throw IntelibX_not_a_refal_function(fun);
            }

            left = argument->GetFirst();
            right = argument->GetLast();

        if (left)
            argument->Exclude(left, right);

    }   // argument object destroyed

    if (left)
    {
        (*this)->InsertListAfter(*from, left, right);
        *from = left;
    } 

    // it is possible that *from is now 0
    if (!*from) 
        *from = GetPtr()->GetFirst();

    return true;
}

IntelibX_not_a_refal_expression::
IntelibX_not_a_refal_expression(SReference a_param) 
    : IntelibX("Not a refal expression", a_param) {}

IntelibX_not_a_refal_function::
IntelibX_not_a_refal_function(SReference a_param) 
    : IntelibX("Not a refal function", a_param) {}

IntelibX_not_a_refal_call::
IntelibX_not_a_refal_call(SReference a_param) 
    : IntelibX("Not a refal call", a_param) {}


