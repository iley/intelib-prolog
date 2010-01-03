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




#include "../tools/sstream.hpp"
#include "conteval.hpp"


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