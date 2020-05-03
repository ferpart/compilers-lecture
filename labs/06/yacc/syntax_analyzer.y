%{
#include <stdio.h>
#include <stdlib.h>

#define YYDEBUG 1
%}

%start list 
%token A B C Cfin
%%

list:
  | list s '\n'
  | list error '\n' { yyerrok; }
;

s : A b Cfin   { printf("Parse completed without errors\n"); }
  | A B Cfin  { printf("Parse completed without errors\n"); }
;

b : B C 
;
%%

int main(int argc, char **argv){
  yydebug = 0;
  extern FILE *yyin;
  if( argc == 2){
      yyin = fopen(argv[1], "r");
      if( yyin != NULL){
        yyparse();
      }
      else{
          printf("ERROR\n");
          return(1);
      }
  }
  else{
      printf("Usage: ./lexical_scan <input file>\n");
      return(1);
  }
  fclose(yyin);

  return (0);
}

yyerror(s)
char *s;
{
  fprintf(stderr, "parsing usuccessful: %s\n",s);
}
yywrap()
{
  return(1);
}