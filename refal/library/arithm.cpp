// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <math.h>


#include "../rclause.hpp"



enum binary_op {ADD, SUB, MUL, DIV, MOD, COMPARE, POW};
enum unary_op {REAL, TRUNC, SIN, COS, TAN, ASIN, ACOS, ATAN,
               SQRT, EXP, LOG, LOG10};
enum nullnary_op {PI};

template
<class numbers_type>
static numbers_type bin_op(numbers_type arg1, numbers_type arg2,
                           binary_op op)
{
    switch (op)
    {
        case ADD:

            return arg1 + arg2;

        case SUB:

            return arg1 - arg2;

        case MUL:

            return arg1 * arg2;

        case DIV:

            return arg1 / arg2;

        case POW:

            return pow(arg1, arg2);

        default:

            throw IntelibX_refal_failure(0);
    }
}

template
<class numbers_type>
static numbers_type pow_int(numbers_type arg1, intelib_integer_t arg2)
{
    if (arg1 == 0) {
        if (arg2 > 0)
            return 0;
        else
            throw IntelibX_refal_failure(0);
    }
    if (arg2 == 0 || arg1 == 1)
        return 1;
    if (arg1 == -1) {
        return ((arg2 & 1) ? -1 : 1);
    }
    if (arg2 < 0)
        return 1 / pow_int<intelib_float_t>(arg1, -arg2);
    if ((arg2 & 1) == 0) {
        return pow_int<numbers_type>(arg1 * arg1, arg2 >> 1);
    } else {
        return arg1 * pow_int<numbers_type>(arg1 * arg1, arg2 >> 1);
    }
    return 0;
}

static float un_op(double (*fun)(double), SExpressionInt *ti,
                   SExpressionFloat *tf)
{
    return fun(ti ? ti->GetValue() : tf->GetValue());
}

static void nullnary_operation(RfReference &ref, nullnary_op op)
{
    if (ref->GetFirst())
        throw IntelibX_refal_failure(ref);
    switch (op) {
        case PI:

            ref = new RfExpression(M_PI);
            break;

        default:

            throw IntelibX_refal_failure(0);
    }
}

static void unary_operation(RfReference &ref, unary_op op)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp || temp->next)
        throw IntelibX_refal_failure(ref);
    SExpressionInt *ti;
    SExpressionFloat *tf = 0;
    ti = temp->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (!ti)
    {
        tf = temp->symb_val.DynamicCastGetPtr<SExpressionFloat>();
        if (!tf)
            throw IntelibX_refal_failure(ref);
    }
    switch (op)
    {
        case REAL:

            if (!ti)
                throw IntelibX_refal_failure(ref);
            ref = new RfExpression((float)ti->GetValue());
            break;

        case TRUNC:

            if (!tf)
                throw IntelibX_refal_failure(ref);
            ref = new RfExpression((int)tf->GetValue());
            break;

        case SIN:

            ref = new RfExpression(un_op(sin, ti, tf));
            break;

        case COS:

            ref = new RfExpression(un_op(cos, ti, tf));
            break;

        case TAN:

            ref = new RfExpression(un_op(tan, ti, tf));
            break;

        case ASIN:

            ref = new RfExpression(un_op(asin, ti, tf));
            break;

        case ACOS:

            ref = new RfExpression(un_op(acos, ti, tf));
            break;

        case ATAN:

            ref = new RfExpression(un_op(atan, ti, tf));
            break;

        case SQRT:

            ref = new RfExpression(un_op(sqrt, ti, tf));
            break;

        case EXP:

            ref = new RfExpression(un_op(exp, ti, tf));
            break;

        case LOG:

            ref = new RfExpression(un_op(log, ti, tf));
            break;

        case LOG10:

            ref = new RfExpression(un_op(log10, ti, tf));
            break;

        default:

            throw IntelibX_refal_failure(ref);
    }
}

static void binary_operation(RfReference &ref, binary_op op)
{
    RfListItem *temp1 = ref->GetFirst(), *temp2;
    if (!temp1)
        throw IntelibX_refal_failure(ref);
    if (temp1->symb_val == RfListItem::left_par)
    {
        temp2 = temp1->pair->next;
        temp1 = temp1->next;
        if (!temp1->next || 
            temp1->next->symb_val != RfListItem::right_par)
            throw IntelibX_refal_failure(ref);
    }
    else
    {
        temp2 = temp1->next;
    }
    if (!temp1 || !temp2 || temp2->next)
        throw IntelibX_refal_failure(ref);
    SExpressionInt *ti1, *ti2;
    SExpressionFloat *tf1 = 0, *tf2 = 0;
    ti1 = temp1->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (!ti1)
    {
        tf1 = temp1->symb_val.DynamicCastGetPtr<SExpressionFloat>();
        if (!tf1)
            throw IntelibX_refal_failure(ref);
    }
    ti2 = temp2->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (!ti2)
    {
        tf2 = temp2->symb_val.DynamicCastGetPtr<SExpressionFloat>();
        if (!tf2)
            throw IntelibX_refal_failure(ref);
    }
    
    float res;
    char str[2];

    switch (op)
    {
        case ADD:
        case SUB:
        case MUL:
        case DIV:

            if (tf1 || tf2)
               ref = new RfExpression
                   (bin_op<intelib_float_t>
                   (ti1 ? ti1->GetValue() : tf1->GetValue(),
                   ti2 ? ti2->GetValue() : tf2->GetValue(), op));
            else
                ref = new RfExpression
                    (bin_op<intelib_integer_t>
                    (ti1->GetValue(), ti2->GetValue(), op));
            break;

        case POW:

            if (ti2) {
                if (ti1 != 0 && ti2->GetValue() >= 0) {
                    ref = new RfExpression
                    (
                        pow_int<intelib_integer_t>(ti1->GetValue(),
                                                   ti2->GetValue())
                    );
                    return;
                }
                ref = new RfExpression
                (
                    pow_int<intelib_float_t>
                        (
                            ti1
                            ?
                            ti1->GetValue()
                            :
                            tf1->GetValue(),
                            ti2->GetValue()
                        )
                );
                return;
            }
            ref = new RfExpression
                (bin_op<intelib_float_t>
                (ti1 ? ti1->GetValue() : tf1->GetValue(),
                tf2->GetValue(), op));
            break;

        case MOD:

            if (!ti1 || !ti2)
                throw IntelibX_refal_failure(ref);
            ref = new RfExpression(ti1->GetValue() % ti2->GetValue());
            break;

        case COMPARE:

            res =
                (ti1 ? ti1->GetValue() : tf1->GetValue()) -
                (ti2 ? ti2->GetValue() : tf2->GetValue());
            str[0] = (res < 0 ? '-' : (res == 0.0 ? '0' : '+'));
            str[1] = '\0';
            ref = new RfExpression(str);
            break;

        default:

            throw IntelibX_refal_failure(ref);
    }
}

static void add_function(RfReference &ref)
{
    binary_operation(ref, ADD);
}

static void sub_function(RfReference &ref)
{
    binary_operation(ref, SUB);
}

static void mul_function(RfReference &ref)
{
    binary_operation(ref, MUL);
}

static void div_function(RfReference &ref)
{
    binary_operation(ref, DIV);
}

static void mod_function(RfReference &ref)
{
    binary_operation(ref, MOD);
}

static void real_function(RfReference &ref)
{
    unary_operation(ref, REAL);
}

static void trunc_function(RfReference &ref)
{
    unary_operation(ref, TRUNC);
}

static void compare_function(RfReference &ref)
{
    binary_operation(ref, COMPARE);
}

static void sin_function(RfReference &ref)
{
    unary_operation(ref, SIN);
}

static void cos_function(RfReference &ref)
{
    unary_operation(ref, COS);
}

static void tan_function(RfReference &ref)
{
    unary_operation(ref, TAN);
}

static void asin_function(RfReference &ref)
{
    unary_operation(ref, ASIN);
}

static void acos_function(RfReference &ref)
{
    unary_operation(ref, ACOS);
}

static void atan_function(RfReference &ref)
{
    unary_operation(ref, ATAN);
}

static void pi_function(RfReference &ref)
{
    nullnary_operation(ref, PI);
}

static void sqrt_function(RfReference &ref)
{
    unary_operation(ref, SQRT);
}

static void exp_function(RfReference &ref)
{
    unary_operation(ref, EXP);
}

static void log_function(RfReference &ref)
{
    unary_operation(ref, LOG);
}

static void log10_function(RfReference &ref)
{
    unary_operation(ref, LOG10);
}

static void pow_function(RfReference &ref)
{
    binary_operation(ref, POW);
}


RfLibSymbol Add("Add", add_function);
RfLibSymbol Sub("Sub", sub_function);
RfLibSymbol Mul("Mul", mul_function);
RfLibSymbol Div("Div", div_function);
RfLibSymbol Mod("Mod", mod_function);
RfLibSymbol Real("Real", real_function);
RfLibSymbol Trunc("Trunc", trunc_function);
RfLibSymbol Compare("Compare", compare_function);
RfLibSymbol Sin("Sin", sin_function);
RfLibSymbol Cos("Cos", cos_function);
RfLibSymbol Tan("Tan", tan_function);
RfLibSymbol Asin("Asin", asin_function);
RfLibSymbol Acos("Acos", acos_function);
RfLibSymbol Atan("Atan", atan_function);
RfLibSymbol Pi("Pi", pi_function);
RfLibSymbol Sqrt("Sqrt", sqrt_function);
RfLibSymbol Exp("Exp", exp_function);
RfLibSymbol Log("Log", log_function);
RfLibSymbol Log10("Log10", log10_function);
RfLibSymbol Pow("Pow", pow_function);


