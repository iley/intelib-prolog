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




#include "rdr_inc.h"

DECLARE_CFUNCTION(SFunctionSaferead,  1,  4,  "#<FUNCTION SAFEREAD>")
DECLARE_CFUNCTION(SFunctionReaderror, 0, 0, "#<FUNCTION READERROR>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

static SReference reader_error_param;
static int reader_error_line = -1;
static int reader_error_begline = -1;

void SFunctionSaferead::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
            // <stream> [<error-value> [<eof-value> [<package>]]]]
            //         *O*R*
            // <string> [<error-value> [<eof-value> [<package>]]]]
            //     <package> may be NIL, to force using dummy-package
{
    SListConstructor L;
    SExpressionStream *stream = 
        paramsv[0].DynamicCastGetPtr<SExpressionStream>();
    SReference keep_stream;
    if(!stream) {
        SExpressionString *the_string = 
            paramsv[0].DynamicCastGetPtr<SExpressionString>();
        stream = new SExpressionStreamCharbuf(the_string->GetValue());
        keep_stream = stream;
    }
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
    IntelibPackage *pkg = 0; 
    if(paramsc>=4) {
        if(paramsv[3].IsEmptyList()) {
            pkg = &TheDummyPackage;
        } else {
            pkg = paramsv[3].DynamicCastGetPtr<SExpressionHashPackage>();
            INTELIB_ASSERT(pkg, IntelibX_not_a_package(paramsv[3]));
        }
    }
    SReference ret;
    try {
        SReference res; 
        if(PTheIntelibReader) {
            IntelibPackage *p = 0;
            if(pkg)
                p = PTheIntelibReader->SetPackage(pkg);
            res = PTheIntelibReader->Read(stream);
            if(pkg)
                PTheIntelibReader->SetPackage(p);
        } else {
            IntelibReader reader;
            if(pkg) reader.SetPackage(pkg);
            res = reader.Read(stream);
        }

        if(res.GetPtr() == IntelibReader::EofMarker.GetPtr()) { 
            /* EOF */
            if(paramsc >= 3) ret = paramsv[2]; 
            else if(paramsc == 2) ret = paramsv[1];
            else ret = *PTheEmptyList;
        } else { 
            ret = res;
        }

        reader_error_param = SReference();
        reader_error_line = -1;
        reader_error_begline = -1;
    }
    catch(const IntelibX_reader_error &err) {
        ret = paramsc > 1 ? paramsv[1] : *PTheEmptyList;
        reader_error_param = err.Parameter();
        reader_error_line = err.Line();
        reader_error_begline = err.ExpBegLine();
    }
    lf.RegularReturn(ret);
}

void SFunctionReaderror::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SListConstructor L;
    lf.RegularReturn(
        reader_error_param.GetPtr() ? 
            (L|reader_error_param, reader_error_line, reader_error_begline)
            : lf.False() 
    );
}

#endif

