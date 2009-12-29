// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include "../../sexpress/sstring.hpp"

#include "../rvars.hpp"
#include "../rclause.hpp"

#include "piggybox.hpp"

#define START_HASH_VALUE 5381



RfPiggyBox box;

bool are_equal(RfReference &ref1, RfReference &ref2)
{
    RfListItem *item1 = ref1->GetFirst();
    RfListItem *item2 = ref2->GetFirst();
    while (item1 && item2) {
        if (!item1->IsEql(*item2))
            return false;
        item1 = item1->next;
        item2 = item2->next;
    }
    return (!item1 && !item2);
}

void update_hash(unsigned long int &hash, int c)
{
    hash = ((hash << 5) + hash) + c;
}

int get_c(RfListItem *typed)
{
    if (typed->IsChar())
        return typed->charcode;
    if (typed->IsMarkup())
        return (int)((long)(typed->symb_val.GetPtr()) & 0xFF);
    SExpressionInt *ti;
    ti = typed->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (ti)
        return ti->GetValue() & 0xFF;
    SExpressionFloat *tf;
    tf = typed->symb_val.DynamicCastGetPtr<SExpressionFloat>();
    if (tf)
        return (int)tf->GetValue() & 0xFF;
    return (int)((long)(typed->symb_val.GetPtr()) & 0xFF);
}

int hash_function(RfReference &key)
{
    unsigned long hash = START_HASH_VALUE;

    for (RfListItem *i = key->GetFirst(); i; i = i->next) {
       update_hash(hash, get_c(i));
    }

    return (int)(hash % PIGGYBOXSIZE);
}

RfPiggyBoxValue::RfPiggyBoxValue(RfReference &value)
{
    this->value = value;
    next = 0;
}

RfPiggyBoxValue::~RfPiggyBoxValue()
{
    if (next) {
        delete next;
        next = 0;
    }
}

void RfPiggyBoxValue::Dgall(RfReference &ref, RfReference &key, bool flag)
{
    RfFormConstructor R;
    RfContext context;

    RfExpressionVariable_E *e_Key = new RfExpressionVariable_E("e.Key");
    RfExpressionVariable_E *e_Value =
        new RfExpressionVariable_E("e.Value");
    RfExpressionVariable_E *e_Ref = new RfExpressionVariable_E("e.Ref");

    RfBinding *bind = context.AddVariable(e_Key, key->GetFirst(),
                                          key->GetLast(), key->GetLast());
    context.AddVariable(e_Value, value->GetFirst(),
                        value->GetLast(), value->GetLast());
    context.AddVariable(e_Ref, ref->GetFirst(),
                        ref->GetLast(), ref->GetLast());

    if (next)
        bind->was_used = true;
    else
        bind->was_used = flag;

    ref =
	RefalSubstitute((R|e_Ref, (R|e_Key, "=", e_Value)), context, true);

    if (next)
        next->Dgall(ref, key, flag);
}

RfPiggyBoxKey::RfPiggyBoxKey(RfReference &key, RfReference &value)
{
    values = new RfPiggyBoxValue(value);
    this->key = key;
    next = 0;
}

RfPiggyBoxKey::~RfPiggyBoxKey()
{
    if (values) {
        delete values;
        values = 0;
    }
    if (next) {
        delete next;
        next = 0;
    }
}

RfPiggyBox::RfPiggyBox()
{
    for (int i = 0; i < PIGGYBOXSIZE; i++)
        elems[i] = 0;
    //Why this wasn't before?
    files = 0;
    argc = 0;
    argv = 0;
}

RfPiggyBox::~RfPiggyBox() {}

void RfPiggyBoxKey::BuryOrReplaceExpression(RfReference &value,
                                            bool bury_flag)
{
    if (!values) {
        values = new RfPiggyBoxValue(value);
        return;
    }
    if (bury_flag) {
        RfPiggyBoxValue *temp = new RfPiggyBoxValue(value);
        temp->next = values;
        values = temp;
    }
    else {
        values->value = value;
    }
}

void RfPiggyBoxKey::Dgall(RfReference &ref)
{
    if (values)
        values->Dgall(ref, key, false);
    if (next)
        next->Dgall(ref);
}

void RfPiggyBoxKey::DgValues(RfReference &ref)
{
    if (values) {
        values->Dgall(ref, key, true);
        delete values;
        values = 0;
    }
}

void RfPiggyBox::BuryOrReplaceExpression(RfReference &key,
                                         RfReference &value,
                                         int hash_fun,
                                         bool bury_flag)
{
    if (!elems[hash_fun]) {
        elems[hash_fun] = new RfPiggyBoxKey(key, value);
        return;
    }
    if (are_equal(elems[hash_fun]->key, key)) {
        elems[hash_fun]->BuryOrReplaceExpression(value, bury_flag);
        return;
    }
    RfPiggyBoxKey *temp = elems[hash_fun];
    while (temp->next) {
        if (are_equal(temp->next->key, key)) {
            temp->next->BuryOrReplaceExpression(value, bury_flag);
            return;
        }
        temp = temp->next;
    }
    temp->next = new RfPiggyBoxKey(key, value);
}

RfPiggyBoxKey *RfPiggyBox::GetKey(RfReference &key)
{
    int hash_fun = hash_function(key);
    RfPiggyBoxKey *temp = elems[hash_fun];
    while (temp) {
        if (are_equal(temp->key, key))
            return temp;
        temp = temp->next;
    }
    return 0;
}

RfReference RfPiggyBox::DigValue(RfPiggyBoxKey *key)
{
    if (!key || !key->values)
        return new RfExpression;
    RfPiggyBoxValue *temp = key->values;
    key->values = key->values->next;
    RfReference res = temp->value;
    temp->next = 0;
    delete temp;
    return res;
}

RfReference RfPiggyBox::CopyValue(RfPiggyBoxKey *key)
{
    if (!key || !key->values)
        return new RfExpression;
    return new RfExpression(key->values->value);
}

RfReference RfPiggyBox::DigExpression(RfReference &key)
{
    return DigValue(GetKey(key));
}

RfReference RfPiggyBox::CopyExpression(RfReference &key)
{
    return CopyValue(GetKey(key));
}

void RfPiggyBox::Clear()
{
    for (int i = 0; i < PIGGYBOXSIZE; i++) {
        if (elems[i]) {
            delete elems[i];
            elems[i] = 0;
        }
    }
    if (files) {
        delete files;
        files = 0;
    }
}

void RfPiggyBox::Dgall(RfReference &ref)
{
    ref = new RfExpression;
    for (int i = 0; i < PIGGYBOXSIZE; i++) {
        if (elems[i]) {
            elems[i]->Dgall(ref);
        }
    }
    Clear();
}

static void br_or_dg_function(RfReference &ref, bool bury_flag)
{
    RfFormConstructor R;
    RfContext context;

    RfExpressionVariable_E *e_Key = new RfExpressionVariable_E("e.Key");
    RfExpressionVariable_E *e_Value =
        new RfExpressionVariable_E("e.Value");

    unsigned long hash = START_HASH_VALUE;
    RfListItem *i;

    for (i = ref->GetFirst(); i; i = i->next) {
       if (i->IsChar() && i->charcode == '=')
           break;
       update_hash(hash, get_c(i));
    }

    if (!i)
        throw IntelibX_refal_failure(ref);

    context.AddVariable(e_Key, ref->GetFirst(), i->prev, i->prev);
    context.AddVariable(e_Value, i->next, ref->GetLast(), ref->GetLast());

    RfReference key =
	RefalSubstitute((R|e_Key), context, true);
    RfReference value =
	RefalSubstitute((R|e_Value), context, true);

    box.BuryOrReplaceExpression(key, value,
                                (int)(hash % PIGGYBOXSIZE), bury_flag);

    ref = new RfExpression;
    return;
}

static void br_function(RfReference &ref)
{
    br_or_dg_function(ref, true);
}

static void dg_function(RfReference &ref)
{
    ref = box.DigExpression(ref);
}

static void rp_function(RfReference &ref)
{
    br_or_dg_function(ref, false);
}

static void cp_function(RfReference &ref)
{
    ref = box.CopyExpression(ref);
}

static void dgall_function(RfReference &ref)
{   
    if (!ref->GetFirst()) {
        box.Dgall(ref);
        return;
    }

    RfFormConstructor R;
    RfContext context;

    RfVariable_E e_Key("e.Key");
    bool res_flag = RefalMatchProcess(ref, (R|(R|e_Key)), context);
    
    if (!res_flag)
        throw IntelibX_refal_failure(ref);

    RfReference key = RefalSubstitute((R|e_Key), context, true);

    ref = new RfExpression;
    RfPiggyBoxKey *temp = box.GetKey(key);
    if (!temp)
        return;
    temp->DgValues(ref);
}

static void clear_function(RfReference &ref)
{
    box.Clear();
    ref = new RfExpression;
}

RfLibSymbol Br("Br", br_function);
RfLibSymbol Dg("Dg", dg_function);
RfLibSymbol Rp("Rp", rp_function);
RfLibSymbol Cp("Cp", cp_function);
RfLibSymbol Dgall("Dgall", dgall_function);
RfLibSymbol Clear("Clear", clear_function);

