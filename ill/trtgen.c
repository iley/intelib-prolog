#include <stdio.h>

unsigned char *trans[] = {
                             (unsigned char*)"!exclamation",
                             (unsigned char*)"?question",
                             (unsigned char*)"&and",
                             (unsigned char*)"*star",
                             (unsigned char*)"-dash",
                             (unsigned char*)"+plus",
                             (unsigned char*)"=equal",
                             (unsigned char*)"/div",
                             (unsigned char*)"\\backsl",
                             (unsigned char*)"%percent",
                             (unsigned char*)"^not",
                             (unsigned char*)"|pipe",
                             (unsigned char*)"~tilde",
                             (unsigned char*)"'quote",
                             (unsigned char*)"`bquote",
                             (unsigned char*)"\"quotes",
                             (unsigned char*)"@at",
                             (unsigned char*)"$bucks",
                             (unsigned char*)"#number",
                             (unsigned char*)"<lt",
                             (unsigned char*)">gt",
                             (unsigned char*)".dot",
                             (unsigned char*)",comma",
                             (unsigned char*)":colon",
                             (unsigned char*)";semicolon",
                             (unsigned char*)"(lbrack",
                             (unsigned char*)")rbrack",
                             (unsigned char*)"{lpar",
                             (unsigned char*)"}rpar",
                             (unsigned char*)"[lbar",
                             (unsigned char*)"]rbar",
                             NULL
                         };


int main() {
    int i;
    unsigned char **p;
    for (i = 'a'; i<='z'; i++)        /* Alphabet characters */
        printf("(DECLARE-CHAR #\\%c \"%c\")\n", i, i + ('A' - 'a'));
    for (p = trans; *p; p++)          /* Translated symbols */
        printf("(DECLARE-CHAR #\\%c \"%s\")\n", (*p)[0], ((*p)+1));
}
