%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
\/\/(.+)        ;
f               { fprintf(yyout, "floatdcl"); }
i               { fprintf(yyout, "intdcl"); }
p               { fprintf(yyout, "PRINT"); }
=               { fprintf(yyout, "assign"); }
[a-eghj-oq-z]   { fprintf(yyout, "id"); }
[0-9]+\.[0-9]+  { fprintf(yyout, "fnum"); }
[0-9]+          { fprintf(yyout, "inum"); }
\+              { fprintf(yyout, "plus"); }
-               { fprintf(yyout, "minus"); }
\*              { fprintf(yyout, "multiply"); }
\/              { fprintf(yyout, "divide"); }
%%

int main(int argc, char **argv){
    extern FILE *yyin, *yyout;
    if( argc == 2){
        yyin = fopen(argv[1], "r");
        yyout = fopen("lex.out", "w");
        if( yyin != NULL && yyout != NULL){
            yylex();
        }
        else{
            printf("ERROR\n");
        }
    }
    else{
        printf("Usage: ./lexical_scan <input file>\n");
    }
    fclose(yyin);
    fclose(yyout);
}