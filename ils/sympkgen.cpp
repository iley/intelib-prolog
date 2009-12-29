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

SListConstructor L;
SReference declfun(new SExpressionClassicAtom("DECLARE-FUNCTION"));
SReference declform(new SExpressionClassicAtom("DECLARE-FORM"));
SReference declhdr(new SExpressionClassicAtom("ADD-LIB-HEADERS"));
SReference directives(new SExpressionClassicAtom("%%%"));

void Funclist(SReference funlist) 
{
    int j = 1;
    for(SReference i=funlist; !i.IsEmptyList(); i=i.Cdr()) {
        printf("static void DoImport_%d(SchExpressionPackage *p) {\n", j++);
        printf("    SchSymbol symb(\"%s\", SReference(new %s()));\n", 
                    i.Car().Cdr().Cdr().Car().GetString(),
                    i.Car().Cdr().Car().GetString());
        printf("    p->Import(symb);\n");
        printf("}\n");
    }
}

void Listbegin()
{
    printf("SchExpressionPackage* MakeSchemePackage() {\n");
    printf("    SchExpressionPackage *p = new SchExpressionPackageIntelib;\n");
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

SReference headerlist;
SReference funclist;

void ProcessDirective(const SReference &dir)
{
    SExpressionCons *c = dir.SimpleCastGetPtr<SExpressionCons>();
    if(!c) return;
    
    if(c->Car().IsEql(declfun)) {
        if(c->Cdr().Cdr().Car().GetString()[0] != '#') 
            funclist, (L| declfun, c->Cdr().Car(), c->Cdr().Cdr().Car());
    } else
    if(c->Car().IsEql(declform)) {
        if(c->Cdr().Cdr().Car().GetString()[0] != '#') 
            funclist, (L| declform, c->Cdr().Car(), c->Cdr().Cdr().Car());
    } else
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
        "scheme/scheme.hpp",
        "scheme/schsymb.hpp",
        "scheme/schfun.hpp",
        "scheme/schpack.hpp");

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

    try {
        Preamble(headerlist);
        Funclist(funclist);
        Listbegin();
        List(funclist);
        Epilogue();
    }
    catch(const IntelibX& ex) {
        fprintf(stderr, "Exception [%s] (%s) caught "
            "during execution of the file\n",
            ex.Description(),
            ex.Parameter().GetPtr() ? 
                ex.Parameter()->TextRepresentation().c_str() : "");
        #if 0
        if(ex.Stack().GetPtr()) {
            fprintf(stderr, "%s",
                stack_representation(ex.Stack()).c_str());
        }
        #endif
        return 1;
    }

    return 0;
}
