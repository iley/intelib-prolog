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




#include <stdlib.h> // for free()
#include "../sexpress/sexpress.hpp"
#include "../tools/sreader.hpp"
#include "../tools/sstream.hpp"

#ifndef INTELIB_USE_READLINE 
#define INTELIB_USE_READLINE 1
#endif

#if INTELIB_USE_READLINE == 1
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../sexpress/shashtbl.hpp"
#endif

static SReference plain_read(const SStreamRef &in,
                             const SStreamRef &out,
                             const SStreamRef &err,
                             IntelibGenericReader &reader,
                             const SString &prompt)
{
    char buf[512];
    out->Puts("> ");
    do {
	buf[sizeof(buf)-2] = 0;
	if(0 == in->Gets(buf, sizeof(buf))) {
	    reader.FeedEof();
	} else {
	    if(buf[sizeof(buf)-2]!=0 && buf[sizeof(buf)-2]!='\n') {
		err->Puts("#* input line too long\n");
		int c;
		do {
		    c = in->Getc();
		} while (c!=EOF && c!='\n');
		continue;
	    }
	    reader.FeedString(buf);
	}
    } while(!reader.IsReady());
    return reader.Get();
}

#if INTELIB_USE_READLINE == 1

static sigjmp_buf backtoloop;
static void (*savesig)(int);
 
static void inth(int a) {
    signal(SIGINT, savesig);
    rl_cleanup_after_signal();
    rl_free_line_state();
    siglongjmp(backtoloop, 1);
}

static bool emptyline(const char *line)
{
    while(*line) {
        if(!isspace(*line)) return false;
        line++;
    }
    return true;
}


static SExpressionHashTable *epack = 0;

static bool is_dirname(const char *fname)
{
    struct stat s;
    if(-1 == stat(fname, &s)) return false;
    return S_ISDIR(s.st_mode);
}

static char *macro_char_generator(const char *text, int seq)
{
    if(text[1] == '\0') {
        static int st;
        if(seq==0) st = 0;
        switch(st++) {
            case 0: return strdup("#\\");
            case 1: return strdup("#'");
            default: return 0;
        }
    }
    if(text[1] != '\\') return 0;

    static int state, len; 
    if(seq == 0) {
        state = 0;
        len = strlen(text);
    }

    static const char *names[] = { 
        "#\\Newline", "#\\Space", "#\\Tab", "#\\Backspace", "#\\Linefeed",
        "#\\Page", "#\\Return", "#\\Rubout", 0 
    };

    if(state>=0) {
        while(names[state]) {
            if(strncasecmp(names[state], text, len) == 0)
                return strdup(names[state++]);
            state++;
        }
        // no more names, try chars...
        state = -1;
    }

    if(text[2] != '\0' && text[3] != '\0') return 0;

    while(state > -256) {
        if(isgraph(-state) && (text[2]==(char)(-state) || text[2]=='\0')) {
            char *res = (char*) malloc(4);
            res[0] = '#'; res[1] = '\\'; res[2] = -state; res[3] = '\0';
            state--;
            return res;
        }
        state--;
    }
    return 0;
}

static char *generator(const char *text, int seq)
{
    if(text[0] == '"') {
        rl_filename_completion_desired = 1;
        char *fname = rl_filename_completion_function(text+1, seq);
        if(!fname) return 0;
        char *res = (char*) malloc(strlen(fname) + 3); 
            /* 3 is for leading dquote, trailing slash and term. zero */
        res[0] = '"';
        strcpy(res+1, fname);
        if(is_dirname(fname)) {
            strcat(res+1, "/");
        } else {
            strcat(res+1, "\"");
        }
        rl_completion_suppress_append = 1;
        free(fname);
        rl_filename_completion_desired = 1;
        return res;
    }

    if(text[0] == '#') {
        return macro_char_generator(text, seq);
    }

    static int state, len; 
    static SReference list;
    if(seq == 0) {
        state = 0;
        len = strlen(text);

        list = *PTheEmptyList;
        if(epack) {
            SExpressionHashTable::Iterator iter(*epack);
            SReference r = iter.GetNext();
            while(r.GetPtr()) {
                const char *s = r.Car().GetString();
                if(strncasecmp(s, text, len) == 0) 
                    list,s; 
                r = iter.GetNext();
            }
        }
    }

    if(list.IsEmptyList()) 
        return 0;
    else {
        char *res = strdup(list.Car().GetString());
        list = list.Cdr();
        strncpy(res, text, len);
        bool to_lower = false;
        for(int i=0; i<len; i++)
            if(isalpha(text[i])) to_lower = islower(text[i]);
        if(to_lower)
            for(int i=len; res[i]; i++) res[i] = tolower(res[i]);
        return res;
    }
}

static SReference do_readline(IntelibGenericReader &reader,
                              const SReference& pkg,
                              const SString &prompt)
{
    static bool init = false;
    if(!init) {
        init = true;
        rl_readline_name = "NILL";
        rl_completion_entry_function = generator;
        rl_basic_word_break_characters = " \n\t()';";
        rl_basic_quote_characters = "";
        //rl_special_prefixes = "\"";
    }
    epack = pkg.DynamicCastGetPtr<SExpressionHashTable>();

    char *line;

    for(;;) { /* to allow cancelling by pressing Ctrl-C and empty strings */
        if(0 == sigsetjmp(backtoloop, 1)) {
            savesig = signal(SIGINT, inth);
            line = readline(prompt.c_str());
            signal(SIGINT, savesig);
            SString hist_line("");
            if(line && emptyline(line)) {
                free(line);
                continue;
            }
            for(;;) {
                if(!line) {
                    reader.FeedEof();
                } else {
                    if(hist_line!="") hist_line += " ";
                    hist_line += line;
                    reader.FeedString(line);
                    reader.FeedChar('\n');
                    free(line);
                }
                if(reader.IsReady()) {
                    add_history(hist_line.c_str());
                    return reader.Get();
                }
                savesig = signal(SIGINT, inth);
                line = readline("> ");
                signal(SIGINT, savesig);
            }
        } else {
            reader.Drop();
            printf("\n");
        }
    }
} 
#endif


SReference intelib_read_line(const SStreamRef &in,
                             const SStreamRef &out,
                             const SStreamRef &err,
                             IntelibGenericReader &reader,
                             const SReference &package,
                             const SString &prompt)
{
#if INTELIB_USE_READLINE == 1
    if(in->Fileno() == 0 && out->Fileno() == 1) {
        return do_readline(reader, package, prompt);
    } else {
        return plain_read(in, out, err, reader, prompt);
    }
#else
    return plain_read(in, out, err, reader, prompt);
#endif
}
