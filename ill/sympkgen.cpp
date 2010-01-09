//   InteLib                                    http://www.intelib.org
//   The file ill/sympkgen.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
