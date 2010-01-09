//   InteLib                                    http://www.intelib.org
//   The file genlisp/dumpcont.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../tools/sstream.hpp"
#include "conteval.hpp"
#include "dumpcont.hpp"


static void print_opcode_name(int opcode, SExpressionStream *stream)
{
    if(opcode >= 0) {
        stream->Puts("CALL/");
        stream->Puts(SReference(opcode)->TextRepresentation().c_str());
    } else {
        switch(opcode) {
            case IntelibContinuation::just_evaluate:
                stream->Puts("just_evaluate");
                break;
            case IntelibContinuation::evaluate_prepared:
                stream->Puts("evaluate_prepared");
                break;
            case IntelibContinuation::evaluate_progn:
                stream->Puts("evaluate_progn");
                break;
            case IntelibContinuation::quote_parameter:
                stream->Puts("quote_parameter");
                break;
            case IntelibContinuation::drop_result:
                stream->Puts("drop_result");
                break;
            case IntelibContinuation::return_unspecified:
                stream->Puts("return_unspecified");
                break;
            case IntelibContinuation::end_of_clauses:
                stream->Puts("end_of_clauses");
                break;
            case IntelibContinuation::cond_clause:
                stream->Puts("cond_clause");
                break;
            case IntelibContinuation::bail_on_false:
                stream->Puts("bail_on_false");
                break;
            case IntelibContinuation::set_context:
                stream->Puts("set_context");
                break;
            case IntelibContinuation::assign_to:
                stream->Puts("assign_to");
                break;
            case IntelibContinuation::assign_location:
                stream->Puts("assign_location");
                break;
            case IntelibContinuation::generic_iteration:
                stream->Puts("generic_iteration");
                break;
            case IntelibContinuation::iteration_callback:
                stream->Puts("iteration_callback");
                break;
            default:
                stream->Puts("extra/");
                stream->Puts(SReference(opcode)->
                        TextRepresentation().c_str());
        }
    }
}

void IntelibDumpContinuation(const IntelibContinuation &cont,
                             SExpressionStream *stream)
{
    int i, j;
    stream->Puts("ToDo:\n");
    for(i = cont.todo_stack_pointer-1, j=0; i>=0; i--, j++) {
        stream->Puts("   #");
        stream->Puts(SReference(j)->TextRepresentation().c_str());
        stream->Puts("\t");
        print_opcode_name(cont.todo_stack[i].opcode, stream);
        SReference param = cont.todo_stack[i].param;
        if(param.GetPtr()) {
            stream->Puts("\t");
            stream->Puts(param->TextRepresentation().c_str());
        }
        stream->Putc('\n');
    }
    stream->Puts("Results:\n");
    for(i = cont.result_stack_pointer-1, j=0; i>=0; i--, j++) {
        stream->Puts("   #");
        stream->Puts(SReference(j)->TextRepresentation().c_str());
        stream->Puts("\t");
        stream->Puts(cont.result_stack[i]->TextRepresentation().c_str());
        stream->Putc('\n');
    }
}
