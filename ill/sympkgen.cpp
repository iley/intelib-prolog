// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <stdio.h>

#include "../sexpress/sexpress.hpp"
#include "../tools/sreader.hpp"

void Preamble(SReference headerlist) 
{
    printf("// GENERATED FILE -- DO NOT EDIT\n\n");

    for(SReference i=headerlist; !i.IsEmptyList(); i=i.Cdr()) {
        printf("#include \"%s\"\n", i.Car().GetString());
    }

    printf("\n\n\n");
}


void Funclist(SReference funlist) 
{
    int j = 1;
    for(SReference i=funlist; !i.IsEmptyList(); i=i.Cdr()) {
        printf("static void DoImport_%d(LExpressionPackage *p) {\n", j++);
#if 0
        printf("    p->Import(LFunctionalSymbol<%s>(\"%s\"));\n", 
                    i.Car().Car().GetString(),
                    i.Car().Cdr().GetString());
#else
        printf("    LFunctionalSymbol<%s> symb(\"%s\");\n", 
                    i.Car().Car().GetString(),
                    i.Car().Cdr().GetString());
        printf("    p->Import(symb);\n");
#endif
        printf("}\n");
    }
}

void Listbegin()
{
    printf("LExpressionPackage* MakeLispPackage() {\n");
    printf("    LExpressionPackage *p = new LExpressionPackageIntelib;\n");
}

void List(SReference funlist) 
{
    int j = 1;
    for(SReference i=funlist; !i.IsEmptyList(); i=i.Cdr()) {
        printf("    DoImport_%d(p);\n", j++);
    }
}

void Epilogue()
{
    printf("    return p;\n");
    printf("}\n\n\n");
}

SListConstructor L;
SReference declfun(new SExpressionClassicAtom("DECLARE-FUNCTION"));
SReference declhdr(new SExpressionClassicAtom("ADD-LIB-HEADERS"));
SReference directives(new SExpressionClassicAtom("%%%"));
SReference headerlist;
SReference funclist;

void ProcessDirective(const SReference &dir)
{
    SExpressionCons *c = dir.SimpleCastGetPtr<SExpressionCons>();
    if(!c) return;
    
    if(c->Car().IsEql(declfun)) {
        if(c->Cdr().Cdr().Car().GetString()[0] != '#') 
            funclist,SReference(c->Cdr().Car(), c->Cdr().Cdr().Car());
    }
    if(c->Car().IsEql(declhdr)) {
        for(SReference i=c->Cdr(); !i.IsEmptyList(); i=i.Cdr()) {
            headerlist,i.Car();        
        }
    }
}


int main()
{
    SStreamStdin s_in;
    IntelibReader reader;

    headerlist = (L|
        "sexpress/sexpress.hpp",
        "sexpress/iexcept.hpp",
        "genlisp/lispform.hpp",
        "lisp/lisp.hpp",
        "lisp/lsymbol.hpp",
        "lisp/lpackage.hpp");

    funclist = *PTheEmptyList;

    do {
        SReference r;
        try {
            r = reader.Read(s_in);
        }
        catch(const IntelibX_reader_error &err) {
            fprintf(stderr, "reader: %s %s line %d\n", 
                             err.Description(),
                             err.Parameter()->TextRepresentation().c_str(),
                             err.Line()
            );
            return 1;
        }
#if 0
        if(!reader.Success()) {
            fprintf(stderr, "reader: %s\n", reader.LastErrorDescription());
        }
#endif
        if(r==IntelibReader::EofMarker) {
            break;
        }

        SExpressionCons *c = r.SimpleCastGetPtr<SExpressionCons>();
        if(!c) continue;

        

        if(c->Car().IsEql(directives)) {
            for(SReference i=r.Cdr(); !i.IsEmptyList(); i=i.Cdr()) {
                ProcessDirective(i.Car());        
            }
        } else {
            ProcessDirective(r);        
        }
    } while(true);

    Preamble(headerlist);
    Funclist(funclist);
    Listbegin();
    List(funclist);
    Epilogue();

    return 0;
}
