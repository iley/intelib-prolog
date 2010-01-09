;   InteLib                                    http://www.intelib.org
;   The file ill/nill_run.lsp
; 
;   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
; 
; 
;   This is free software, licensed under GNU GPL v.2
;   See the file COPYING for further details.
; 
;   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
;   Please see the file WARRANTY for the detailed explanation.




(load "ill.lsp")

(setq lispstdout *standard-output*)

(handlecommandlinedirectives 
    (plus 
        "(MODULE-NAME " #\" "ill_nill" #\" ")"
        "(INIT-FUNCTION-NAME " #\" "TheLispProgram" #\" ")"
    )
)

(lispentry '("illdef._ls" "ill.lsp"))
