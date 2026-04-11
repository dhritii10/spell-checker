%{
#include "parser.tab.h"
#include <string.h>
%}

WORD    [a-zA-Z]+
PUNCT   [.,!?]

%%

{WORD} {
    yylval.str = strdup(yytext);
    return WORD;
}

{PUNCT} {
    return PUNCT;
}

[ \t\n] ;

. ;

%%