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




#include "../sexpress/sexpress.hpp"

/*
 * The function parses given string assuming it represents a number, 
 * either integer or floating point, and returns lisp s-expression 
 * of appropriate type.
 * NOTE the assumption made that (ch - '0') will give the numberic value 
 * for any digit character ch.  
 */
SReference Charp2LispNumber(const char* s)
{
    intelib_integer_t i=0;
    intelib_float_t f=0;
    int is_begin = 0;
    int is_ok = 0;
    int is_neg = 0;
    int is_float = 0;
    intelib_float_t float_mul = 1;
    for(const char *p = s; *p != 0; p++) {
        switch(*p) {
            case '+':
                if(is_begin)
                    goto FINISH;
                else
                    is_begin = 1;
                break;
            case '-':
                if(is_begin)
                    goto FINISH;
                else {
                    is_begin = 1;
                    is_neg = 1;
                }
                break;
            case '.':
                if(is_float)
                    goto FINISH;
                else {
                    is_float = 1;
                    is_begin = 1;
                    is_ok = 1;
                    f = i;
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                is_begin = 1;
                is_ok = 1;
                if(is_float) {
                    float_mul *= 10;
                    f += (*p - '0') / float_mul;
                } else {
                    i *= 10; i += (*p - '0');
                }
                break;
            default:
                if(is_begin) goto FINISH; // couldn't use break here...
        }
    }
FINISH:
    if(!is_ok) return *PTheEmptyList;
if(is_neg) { f = -f; i = -i; }
    if(is_float) return SReference(f);
    else return SReference(i);
}
