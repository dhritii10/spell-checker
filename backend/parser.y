%{
#include <stdio.h>
#include <stdlib.h>
#include "spell.h"

void yyerror(const char *s);
int yylex();
%}

%union {
    char* str;
}

%token <str> WORD
%token PUNCT

%%

sentence:
    sentence element
    | element
    ;

element:
    WORD {
    }
    | PUNCT
    ;

%%
void yywrap(){
    return;
}
void yyerror(const char *s) {
    printf("Parse error: %s\n", s);
}