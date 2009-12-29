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




#include "../sexpress/slocatn.hpp"
#include "lispform.hpp"
#include "conteval.hpp"

static const int initial_result_stack_size = 1024;
static const int initial_task_stack_size = 1024;

////////////////////////////////////////////////////////

IntelibTypeId SExpressionSetfAgent::TypeId(&SExpression::TypeId, false);

////////////////////////////////////////////////////////

IntelibTypeId SExpressionSpecialToken::TypeId(&SExpressionLabel::TypeId, false);

////////////////////////////////////////////////////////

IntelibContinuation::IntelibContinuation()
{
    result_stack_size = initial_result_stack_size;
    result_stack_pointer = 0;
    result_stack = new SReference[result_stack_size];

    todo_stack_size = initial_task_stack_size;
    todo_stack_pointer = 0;
    todo_stack = new TodoItem[todo_stack_size];
}

IntelibContinuation::
IntelibContinuation(const IntelibContinuation &other, bool ignored)
{
    //result_stack_size = other.result_stack_size;
    result_stack_size = other.result_stack_pointer+1; // that's enough
    result_stack_pointer = other.result_stack_pointer;
    result_stack = new SReference[result_stack_size];

    //todo_stack_size = other.todo_stack_size;
    todo_stack_size = other.todo_stack_pointer+1; // that's enough
    todo_stack_pointer = other.todo_stack_pointer;
    todo_stack = new TodoItem[todo_stack_size];

    int i;
    for(i=0; i<other.result_stack_pointer; i++)
        result_stack[i] = other.result_stack[i];
    for(i=0; i<other.todo_stack_pointer; i++)
        todo_stack[i] = other.todo_stack[i];

    current_context = other.current_context;
}

IntelibContinuation::~IntelibContinuation()
{
    delete[] result_stack;
    delete[] todo_stack;
}

void IntelibContinuation::
ReplaceContinuation(const IntelibContinuation &other)
{
    delete[] result_stack;
    delete[] todo_stack;

    int i;

    result_stack_size = initial_result_stack_size;
    while(result_stack_size <= other.result_stack_pointer)
        result_stack_size *= 2;
    result_stack_pointer = other.result_stack_pointer;
    result_stack = new SReference[result_stack_size];
    for(i=0; i<other.result_stack_pointer; i++)
        result_stack[i] = other.result_stack[i];

    todo_stack_size = initial_task_stack_size;
    while(todo_stack_size <= other.todo_stack_pointer)
        todo_stack_size *= 2;
    todo_stack_pointer = other.todo_stack_pointer;
    todo_stack = new TodoItem[todo_stack_size];
    for(i=0; i<other.todo_stack_pointer; i++)
        todo_stack[i] = other.todo_stack[i];

    current_context = other.current_context;
}

SReference IntelibContinuation::Get()
{
    if(result_stack_pointer < 1) {
        SReference unbound;
        return unbound;
    }
    return result_stack[(result_stack_pointer--) - 1];
}

bool IntelibContinuation::Step()
{
    if(pending_interruption && !interruptions_suspended) {
        pending_interruption = false;
        throw Interruption();
    }
    int opcode;
    SReference param;
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
    SReference save_stack_info =
        todo_stack_pointer > 0 ?
        todo_stack[todo_stack_pointer-1].stack_info : SReference();
#endif
    if(!PopTodo(opcode, param)) return false;
    try {
        if(opcode >= 0) { // this means function call
            FunctionCall(opcode);
        } else
        switch(opcode) {
            case just_evaluate:
                JustEvaluate(param);
                break;
            case evaluate_prepared:
                EvaluatePrepared(param);
                break;
            case evaluate_progn:
                PlacePrognToStack(param);
                break;
            case quote_parameter:
                PushResult(param);
                break;
            case drop_result: {
                SReference r;
                PopResult(r);
                break;
            }
            case return_unspecified:
                ReturnUnspecified();
                break;
            case end_of_clauses:
                // this is effectively just a no-op
                break;
            case cond_clause:
                EvaluateCondClause(param);
                break;
            case bail_on_false:
                BailOnFalse(); // param is ignored
                break;
            case set_context:
                current_context = param;
                break;
            case assign_to: {
                SReference r;
                PopResult(r);
                static_cast<SExpressionLocation*>(param.GetPtr())->Assign(r);
                break;
            }
            case assign_location:
                AssignLocation(); // param is ignored
                break;
            case generic_iteration:
                GenericIteration(param);
                break;
            case iteration_callback:
                IterationCallback(param);
                break;
            default:
                CustomCommand(opcode, param);
        }
    }
    catch(IntelibX &ex) {
        SReference reslist = *PTheEmptyList;
        for(int k = 0; k < result_stack_pointer; k++)
            reslist, result_stack[k];
        SListConstructor L;
        ex.AddStack((L|reslist, opcode,
                               param.GetPtr() ? param : *PTheEmptyList));
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
        if(save_stack_info.GetPtr())
            ex.AddStack(save_stack_info);
        for(int i = todo_stack_pointer - 1; i>=0; i--)
            if(todo_stack[i].stack_info.GetPtr())
                ex.AddStack(todo_stack[i].stack_info);
#endif
        throw ex;
    }
    return true;
}

void IntelibContinuation::CustomCommand(int opcode, const SReference& param)
{
    throw IntelibX_continuation_unknown_operation(opcode);
}

void IntelibContinuation::PushTodo(int opcode, const SReference& param)
{
    // this hack works around the problem with wasting stack
    //     (specially in tail contexts) by the restore-context
    //     commands; two consequent set_context commands are
    //     senseless anyway, so if there's already one there,
    //     this won't let the second one go to the stack
    if(opcode == set_context) {
        if(todo_stack_pointer > 0 &&
           todo_stack[todo_stack_pointer - 1].opcode == set_context)
        {
            return;
        }
    }

    if(todo_stack_pointer >= todo_stack_size) {
        // resize
        TodoItem *np = new TodoItem[todo_stack_size*2];
        for(int i = 0; i<todo_stack_pointer; i++)
            np[i] = todo_stack[i];
        delete[] todo_stack;
        todo_stack = np;
        todo_stack_size *= 2;
    }
    TodoItem *p = todo_stack + todo_stack_pointer;
    p->opcode = opcode;
    p->param = param;
    todo_stack_pointer++;
}

void IntelibContinuation::PushTodo(int opcode)
{
    SReference unbound;
    PushTodo(opcode, unbound);
}

bool IntelibContinuation::PopTodo(int &opcode, SReference& param)
{
    if(todo_stack_pointer <= 0) return false;
    todo_stack_pointer--;
    TodoItem *p = todo_stack + todo_stack_pointer;
    opcode = p->opcode;
    param = p->param;
    p->param = SReference();
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
    p->stack_info = SReference();
#endif
    return true;
}

void IntelibContinuation::PushResult(const SReference& r)
{
    INTELIB_ASSERT(r.GetPtr(), IntelibX_unexpected_unbound_value());
    if(result_stack_pointer >= result_stack_size) {
        // resize
        SReference *np = new SReference[result_stack_size*2];
        for(int i = 0; i<result_stack_pointer; i++)
            np[i] = result_stack[i];
        delete[] result_stack;
        result_stack = np;
        result_stack_size *= 2;
    }
    result_stack[result_stack_pointer] = r;
    result_stack_pointer++;
}

bool IntelibContinuation::PopResult(SReference& r)
{
    if(result_stack_pointer <= 0) return false;
    result_stack_pointer--;
    r = result_stack[result_stack_pointer];
    result_stack[result_stack_pointer] = SReference();
    return true;
}

void IntelibContinuation::RegularReturn(const SReference &ref)
{
    PushResult(ref);
}

bool IntelibContinuation::AcceptsLocation() const
{
    return
        todo_stack_pointer > 0 &&
        (
            todo_stack[todo_stack_pointer-1].opcode == assign_location ||
            todo_stack[todo_stack_pointer-1].opcode == set_context &&
                todo_stack_pointer > 1 &&
                todo_stack[todo_stack_pointer-2].opcode == assign_location
        );
}

void IntelibContinuation::ReferenceReturn(SReference &ref,
                                          const SReference &superstruct)
{
    if(AcceptsLocation()) {
        PushResult(SReference(new SExpressionLocation(superstruct, &ref)));     
    } else {
        PushResult(ref);
    }
}

void IntelibContinuation::AgentReturn(const SReference &val,
                                      const SExpressionSetfAgent *agent)
{
    if(AcceptsLocation()) {
        PushResult(agent);     
    } else {
        PushResult(val);
    }
}

void IntelibContinuation::TailReturn(const SReference &ref)
{
    PushTodo(just_evaluate, ref);
}

void IntelibContinuation::ReturnUnspecified()
{
    static SLabel unspecified("#<unspecified>");
    RegularReturn(unspecified);
}


void IntelibContinuation::JustEvaluate(const SReference& expr)
{
    if(!expr.GetPtr()) throw IntelibX_unexpected_unbound_value();
    const IntelibTypeId *t;
    for(t=&(expr->TermType()); t; t=t->Prev()) {
        if(*t == SExpressionCons::TypeId) {
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
            int keepstack_position = todo_stack_pointer;
#endif
            EvaluateForm(static_cast<SExpressionCons*>(expr.GetPtr()));
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
            todo_stack[keepstack_position].stack_info = expr;
#endif
            return;
        } else
        if(*t == SExpressionSpecialToken::TypeId) {
            static_cast<SExpressionSpecialToken*>(expr.GetPtr())
                ->Evaluate(*this);
#if 0
            LExpressionSymbol *sym =
                static_cast<LExpressionSymbol*>(expr.GetPtr());
            if(sym->IsDynamic()) {
                if(sym->IsConstant()) {
                    RegularReturn(sym->GetDynamicValue());
                } else {
                    ReferenceReturn(sym->GetDynamicValueRef(), expr);
                }
            } else {
                SReference &res = GetSymbolValue(sym); 
                if(!res.GetPtr())
                    throw IntelibX_lisp_symbol_has_no_value(expr);
                ReferenceReturn(res, expr);
            }
#endif
            return;
        }
    }
    // no parent means that it's a regular constant...
    // it is always self-evaluated
    PushResult(expr);
}



void IntelibContinuation::EvaluatePrepared(const SReference& expr)
{
    int len = 0;
    const SReference *t = &expr;
    do {
        PushResult(t->Car());
        t = &(t->Cdr());
        len++;
    } while(!t->IsEmptyList());
    FunctionCall(len-1);
}

void IntelibContinuation::FunctionCall(int paramscount)
{
    // result_stack_pointer is the first FREE position, so
    // the function object should be at r_s_p - 1 - paramscount
    int fun_pos = result_stack_pointer - 1 - paramscount;
    if(fun_pos < 0) throw IntelibX_bug();
    SReference save_fun = result_stack[fun_pos];
    result_stack_pointer = fun_pos;

    DoFunctionCall(save_fun, paramscount, result_stack+fun_pos+1);
}


void IntelibContinuation::EvaluateCondClause(const SReference& expr)
{
    // take the value on top of the result stack
    // if it is true, then
    //     remove all the stuff from the ToDo stack
    //     until the end_of_clauses mark, and then add the expr as
    //     progn to todo
    // otherwise,
    //     just do nothing    
    SReference pres;
    PopResult(pres);
    if(IsTrue(pres)) {
        int op;
        SReference parm;
        while(PopTodo(op, parm) && op != end_of_clauses) {}
        if(expr.IsEmptyList()) {
            PushResult(pres); 
        } else {
            PlacePrognToStack(expr);
        }
    }
}

void IntelibContinuation::BailOnFalse()
{
    // take the value on top of the result stack
    // if it is false, then
    //     remove all the stuff from the ToDo stack
    //     until the end_of_clauses mark, and then 
    //     return the result value back to stack
    // otherwise,
    //     just do nothing    
    SReference pres;
    PopResult(pres);
    if(!IsTrue(pres)) {
        int op;
        SReference parm;
        while(PopTodo(op, parm) && op != end_of_clauses) {}
        PushResult(pres); 
    }
}

void IntelibContinuation::AssignLocation()
{
    // first, take a location from stack, check if it is really a location
    SReference loc;
    PopResult(loc);
    SReference val;
    PopResult(val);
    SExpressionLocation *lc = loc.SimpleCastGetPtr<SExpressionLocation>();
    if(!lc) {
        SExpressionSetfAgent *ag =
            loc.DynamicCastGetPtr<SExpressionSetfAgent>();
        if(ag) {
            ag->Setf(val);
            return;
        }
        throw IntelibX_not_a_location(loc);
    }
    lc->Assign(val);
}

void IntelibContinuation::GenericIteration(const SReference &param)
{
    SExpressionGenericIterator *iter =
        static_cast<SExpressionGenericIterator*>(param.GetPtr()); 
    if(iter->NeedAnotherIteration(*this)) {
            // return it to the stack
        PushTodo(generic_iteration, param);
            // schedule the callback
        PushTodo(iteration_callback, param);
            // let the iteration plan execution of its body
        iter->ScheduleIteration(*this);
    } else {
            // no more iterations, just compute the final value
        iter->ReturnFinalValue(*this);
    }
}

void IntelibContinuation::IterationCallback(const SReference &param)
{
    SExpressionGenericIterator *iter =
        static_cast<SExpressionGenericIterator*>(param.GetPtr()); 
    iter->CollectResultOfIteration(*this);
}

void IntelibContinuation::PlacePrognToStack(const SReference &rest)
{
    if(rest.IsEmptyList()) {
        // empty progn should return NIL
        // hmm... in Scheme it might be different...
        PushTodo(quote_parameter, *PTheEmptyList);
        return;
    }
    SReference tail = rest.Cdr();
    if(!tail.IsEmptyList()) {
        PlacePrognToStack(tail);
        PushTodo(drop_result);
    }
    PushTodo(just_evaluate, rest.Car());
}

void IntelibContinuation::PlaceFormToStack(const SExpressionCons *form,
                                           int len)
{
    INTELIB_ASSERT(form, IntelibX_unexpected_unbound_value());
    SExpressionCons *cdr =
        form->Cdr().DynamicCastGetPtr<SExpressionCons>();
    if(cdr) {
        PlaceFormToStack(cdr, len+1);
    } else {
        // we don't check the list for correctness, so
        // for dotted lists, the last element is just ignored.
        // may be this chould change once...
        PushTodo(len);
    }
    PushTodo(just_evaluate, form->Car());
}

void IntelibContinuation::DoFunctionCall(const SReference &fun_ref,
                                         int paramscount,
                                         const SReference *paramsvector)
{
    SExpressionFunction *fun =
        fun_ref.DynamicCastGetPtr<SExpressionFunction>();
    if(!fun) {
        throw IntelibX_not_a_function(fun_ref);
    }
    fun->Apply(paramscount, paramsvector, *this);
}


bool IntelibContinuation::pending_interruption = false;
bool IntelibContinuation::interruptions_suspended = false;



IntelibX_continuation_unknown_operation::
IntelibX_continuation_unknown_operation(SReference a_param) 
    : IntelibX("continuation: unknown operation code", a_param) {}

IntelibX_not_a_function::
IntelibX_not_a_function(SReference a_param) 
    : IntelibX("not a function", a_param) {}

