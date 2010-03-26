//   InteLib                                    http://www.intelib.org
//   The file genlisp/conteval.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file conteval.hpp
    \brief The base for continuation-based evaluator

    This file provides the class IntelibContinuation and some other
    stuff related to the continuation-based evaluation model.

*/

#ifndef INTELIB_CONTEVAL_HPP_SENTRY
#define INTELIB_CONTEVAL_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/gensref.hpp"

#ifndef INTELIB_CONTINUATION_KEEPS_STACK_INFO
    //! Whether the continuation object should keep debug information
#define INTELIB_CONTINUATION_KEEPS_STACK_INFO 1
#endif


//! Setf agent
/*! This class is used in case the thing we return should allow SETF 
    on it but it is not a reference to an existing SReference, so that
    SETF is something else than just an assignment. 
    \warning This is rarely used; if in doubt, you probably don't need it.
    \note One of the obvious usage cases is an assignment to a C++ global
    variable accessible via #&varname from Lisp/Scheme and via F&varname
    from Lisp-like code within C++.
 */
class SExpressionSetfAgent : public SExpression {
public:
    static IntelibTypeId TypeId;

    SExpressionSetfAgent() : SExpression(TypeId) {}
    virtual void Setf(const SReference &val) = 0;

protected:
    virtual ~SExpressionSetfAgent() {}
};

//! Special token a.k.a. a symbol
/*! This abstract class represents a non-list token which is to be
    evaluated in a special way, such as a symbol, or a hygienic
    symbol in Scheme, or a global (external) variable accessor.
 */
class SExpressionSpecialToken : public SExpressionLabel {
public:
    static IntelibTypeId TypeId;

    SExpressionSpecialToken(const IntelibTypeId &type, const char *name)
        : SExpressionLabel(type, name) {}
    ~SExpressionSpecialToken() {}

    virtual void Evaluate(class IntelibContinuation& lf) = 0;

        //! Get the applicable object if it exists
        /*! If there's a special form (spec. syntax) associated with
            the token, then return it through the form argument, otherwise
            reset the *form to zero pointer.
            If, in the course of search for the form, the value to
            be used as a function was found, but it is not a special form
            (e.g., for a symbol which is associated with a value/function
            which isn't special), then set the function argument to refer
            it.  Leave the second parameter unchanged in case the first
            parameter is set to non-zero.

            This function is intended to be used from within the form
            evaluation, when there's a need to determine what is associated
            with the first element of the form.
         */
    virtual void GetApplicable(class SExpressionForm **form,
                               SReference *function,
                               IntelibContinuation&) const
        { *form = 0; *function = SReference(); }
};

//! The Continuation
/*! The InteLib Continuation is a stack-based virtual machine intended
    to perform Lisp-like evaluations.  This class is used as a base for
    Lisp-specific and Scheme-specific classes.
    
    The machine holds two stacks, the result_stack and the todo_stack.
    The result_stack contains SReference's to which evaluations result;
    its fragments are used as argument vectors for functions, therefore
    avoiding allocations of such vectors.  The todo_stack holds records
    of two elements: an integer instruction code and a param which is an
    SReference.  There can also be debug-related stack information there
    (see the INTELIB_CONTINUATION_KEEPS_STACK_INFO conditional macro).

    Besides that, the continuation object is responsible for switching
    lexical contexts; the context is stored as a plain SReference, thus
    its exact type is determined by the specific subclasses of
    IntelibContinuation.  This is because Lisp and Scheme actually use
    differently organized lexical contexts.

    Evaluation is performed step-by-step.  On each step, an instruction is
    fetched from the top of the todo stack and interpreted.  The
    interpretation can lead to adding some more instructions to the stack,
    to put one (or even more) results to the result stack, and sometimes
    the instruction fetches uses up some information from the result
    stack (e.g., if the instruction is a N-ary function call, it uses the
    last N+1 elements of the result stack and removes them from there).

    To start an evaluation, the expression to be evaluated must be
    put into the todo stack with the just_evaluate instruction.
    The evaluation is considered finished when the todo stack is empty.
    Normally there should be exactly one item in the result stack;
    it should (but not required though) be considered error if there
    is no result at all and/or there's more than one item.

    Should you need to perform an evaluation on an existing machine
    keeping the existing state as it is, there's also a possibility
    to 'mark' the todo stack with the GetMark(), add the expression
    to the todo stack, and then use the mark to determine whether the
    stack returned to the marked position, which will mean the evaluation
    of your expression is complete.

    A single evaluation step is performed by the Step() method.  There's
    one important thing about it.  The function Step() is NEVER EVER
    reentered, neither directly nor consequentially.  This requirement is
    absolutely obligatory for continuations to work, as the continuation
    at each and every step must hold the list of all the operations
    necessary to finish the whole evaluation, and this would be impossible
    in case we reenter the Step() function because some information we
    need will reside in the call stack frames (local variables of
    functions) instead of the continuation object.

    A copy of a continuation can be made.  Such a copy is not intended to
    perform any evaluations itself, but any active continuation can replace
    its entire state with the content of such a copy.  That's how the
    CALL/CC works.
 */
class IntelibContinuation {
    friend void IntelibDumpContinuation(const class IntelibContinuation&,
                                        class SExpressionStream*);

    SReference *result_stack;
    int result_stack_size;
    int result_stack_pointer;

    struct TodoItem {
        int opcode;
        SReference param;
#if INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
        SReference stack_info;
#endif
    };
    TodoItem *todo_stack;
    int todo_stack_size;
    int todo_stack_pointer;

    SReference current_context;

#if 0 // INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
    SReference last_stack_info;
#endif

protected:
    SReference *PTheTrueValue;
    SReference *PTheFalseValue;

public:
        //! Creates an empty continuation, ready to work
    IntelibContinuation();
        //! Creates a copy to be used later with ReplaceContinuation
    IntelibContinuation(const IntelibContinuation &other, bool ignored);
        //! The destructor
    virtual ~IntelibContinuation();

        //! The method to evaluate a form
        /*! In fact, this is one of the two most visible differences
            between Lisp and Scheme evaluation models (while the other
            is implemented by SExpressionSpecialToken descenders).
            This method is pure virtual so it must be implemented by
            subclasses.  There's no idea on what to do here by default.
         */
    virtual void EvaluateForm(const SExpressionCons* form) = 0;


        //! Extend the instruction set
        /*! If the instruction code is negative and is not
            mentioned in the Instructions enum, this method
            is called; by default it throws an exception.
         */ 
    virtual void CustomCommand(int opcode, const SReference& param);

        //! Perform a single step of the evaluation
        /*! Returns false in case the todo stack is empty
            \note This function must never be reentered, otherwise
            CALL/CC won't work for obvious reasons.  Should you
            decide to work your own evaluation-based machine, make
            sure you never call evaluator from within itself.
         */
    bool Step();

        //! Mark the todo stack before starting an evaluation
    int GetMark() const { return todo_stack_pointer; }
        //! Is the evaluation complete?
        /*! \param mark is the optional mark returned by the
            GetMark() method.  This is to be used when you
            wish to evaluate an expression within the existing
            machine
         */
    bool Ready(int mark = 0) const { return todo_stack_pointer == mark; }
        //! Get the result of the evaluation
    SReference Get();

        //! Replace the entire state of the continuation
        /*! This is how the CALL/CC feature works.
            \param other should be an object made with the
            2-ary constructor.
         */
    void ReplaceContinuation(const IntelibContinuation &other);
private:
        //! no implicit copying for such a monster!
    IntelibContinuation(const IntelibContinuation &other) {}
        //! no assigning for such a monster!
    void operator=(const IntelibContinuation &other) {}

public:
        //! The instruction set of the machine
        /*! Zero and positive opcodes stand for "call a function with
            that many args", in which case the function object and the
            args are taken from the result stack.
            The numbers from -1 downto max_command are the special
            operation codes.  The numbers below max_command are for
            the operations added by subclasses (e.g., Scheme-specific
            and Lisp-specific instructions).
         */
    enum Instructions {
            //! Just evaluate the param
        just_evaluate =       -1,
            //! Evaluate a form in which all params are already computed
        evaluate_prepared =   -2,
            //! Evaluate a list of forms, one by one
        evaluate_progn =      -3,
            //! Just put the parameter into the result stack
        quote_parameter =     -4,
            //! Remove one item from the result stack thus dropping it
        drop_result =         -5,
            //! Return the value which stands for 'unspecified result'
        return_unspecified =  -6,
            //! No-op, used as a label for cond_clause and bail_on_false
        end_of_clauses =      -7,
            //! A conditional clause
            /*! Take the value on top of the result stack.  If it is true,
                then remove all the stuff from the todo stack until the
                end_of_clauses mark, and then add the param as progn to
                todo.  Otherwise (that is, the condition value was false),
                just do nothing.
             */
        cond_clause =         -8,
            //! Check and jump on false
            /*! Take the value on top of the result stack.  If it is false,
                then remove all the stuff from the todo stack until the
                end_of_clauses mark, and then return the false value
                back to stack.  Otherwise, just do nothing    
             */
        bail_on_false =       -9,
            //! Set the current context to the param
            /*! \note PushTodo() never adds another set_context if
                the top of the todo stack contains set_context
             */
        set_context =        -10,
            //! Assign to the given location
            /*! \warning param MUST be SExpressionLocation
             */
        assign_to =          -11, 
            //! Assign to a location taken from result stack
            /*! The top of the result stack should contain either
                a SExpressionLocation or a SExpressionSetfAgent
             */
        assign_location =    -12,
            //! Generic iteration
            /*! See SExpressionGenericIteration */
        generic_iteration =  -13,
            //! -max_command is the count of defined commands
        max_command = -13
    };

        //! Add an instruction to the todo stack
        /*! \param opcode is the instruction code, see the
            Instructions enum for explanation
            \param param is the instruction parameter
         */
    void PushTodo(int opcode, const SReference& param);
        //! Add a parameterless instruction to the todo stack
        /*! \param opcode is the instruction code, see the
            Instructions enum for explanation
         */
    void PushTodo(int opcode); // for the 'no parameter' case
        //! Get (and remove) the top of the todo stack
        /*! \warning you shouldn't ever want to use this method
         */
    bool PopTodo(int &opcode, SReference& param);
        //! Put item into the result stack
    void PushResult(const SReference& param);
        //! Get (and remove) the item from top of the result stack
    bool PopResult(SReference& param);

    const SReference &True() const { return *PTheTrueValue; }
    const SReference &False() const { return *PTheFalseValue; }

        //! Return a value from a Lisp function
    void RegularReturn(const SReference &ref);
        //! Return an assignable reference from a Lisp function
    void ReferenceReturn(SReference &ref, const SReference &superstruct);
        //! Return a Setf Agent from a Lisp function
    void AgentReturn(const SReference &val, const SExpressionSetfAgent *ag);
        //! Return a tail (to be evaluated) from a Lisp function
    void TailReturn(const SReference &ref);
        //! Return the 'unspecified' value from a Lisp function
    void ReturnUnspecified();
        //! Return a boolean value
    void ReturnBoolean(bool b) { RegularReturn(b ? True() : False()); }

        //! Get current lexical context
    SReference GetContext() const { return current_context; }
        //! Set current lexical context
    void SetContext(const SReference &cont) { current_context = cont; }

private:
    static bool pending_interruption;
    static bool interruptions_suspended;
public:
        //! Interrupt the evaluation
    static void InterruptEvaluator() { pending_interruption = true; }
        //! Cancels the interruption of the evaluation
    static void RemoveInterruption() { pending_interruption = false; }
        //! Temporaryly disallow interruptions
    static void SuspendInterruptions() { interruptions_suspended = true; }
        //! Allow interruptions again
    static void ResumeInterruptions() { interruptions_suspended = false; }

    //! Evaluator is effectively interrupted throwing object of this class
    class Interruption {};

private:
    bool IsTrue(const SReference& expr)
        { return expr.GetPtr() != PTheFalseValue->GetPtr(); }
    void JustEvaluate(const SReference& expr);
    void EvaluatePrepared(const SReference& expr);
    void FunctionCall(int paramscount);
    void PlacePrognToStack(const SReference& expr);
    void EvaluateCondClause(const SReference& expr);
    void BailOnFalse();
    void AssignLocation();
    void GenericIteration(const SReference& expr);

    bool AcceptsLocation() const;

    void DoFunctionCall(const SReference &save_fun,
                        int paramscount,
                        const SReference *paramsvector);


protected:
        //! Place a prepared form to the stack
        /*! \param form is the list
            \param len is the count of already-pushed elements
                   e.g. if you have already passed the function
                   and form is only the list of parameters, use 1.
                   If the list contains the function as well, use 0
         */
    void PlaceFormToStack(const SExpressionCons *form, int len);
};

//! Interface class used for iterations (such as LOOP or MAPCAR)
/*! The instruction generic_iteration of the IntelibContinuation class
    use an object derived from this class to perform iterative evaluations.

    The operation is as follows: when the IntelibContinuation encounters
    the generic_iteration instruction (whose parameter must always be a
    subclass of SExpressionGenericIterator), it temporariry memorizes the
    object (so that it doesn't get deleted) and calls the DoIteration()
    method.  It is the duty of the method either to push another
    generic_interation command with the same object to the ToDo stack,
    or to push the final commands in case no more iterations are needed.

    \warning Make sure you don't reenter the IntelibContinuation::Step()
    (the evaluator) from within the methods of this class, or else
    you definitely break the CALL/CC!  Simply speaking, just NEVER TRY
    TO EVALUATE ANYTHING RIGHT FROM THE METHOD, rather schedule the needed
    evaluations as explained above.
 */
class SExpressionGenericIterator : public SExpression {
public:
    static IntelibTypeId TypeId;

    SExpressionGenericIterator() : SExpression(TypeId) {}

        //! The method to be called
        /*! IntelibContinuation calls this in response to the
            generic_iteration command
         */
    virtual void DoIteration(IntelibContinuation& lf) = 0;

protected:
    ~SExpressionGenericIterator() {}
};

    //! Unknown instruction code in the IntelibContinuation class
class IntelibX_continuation_unknown_operation : public IntelibX {
public:
    IntelibX_continuation_unknown_operation(SReference a_param);
};

    //! Not a function
class IntelibX_not_a_function : public IntelibX {
public:
    IntelibX_not_a_function(SReference a_param);
};


#endif
