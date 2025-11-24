/*DECLARATIONS*/

%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "bst.h"
    #include "globals.h"
    #include "parser.tab.h"
%}

letter [a-zA-Z]
digit [0-9]
operator [+\-*/%]
boolean_operator ("=="|"!="|">"|">="|"<"|"<=")

/*identifiers/numbers*/
type ("int"|"bool"|"double")
number {digit}+
double_number {number}\.{number}
identifier (_|{letter})({letter}|{digit}|_)*



/*RULES*/
%%

\n  {++num_lines;}
"using namespace std;" {fprintf(fout, "USING_STATEMENT,%s,%d\n", yytext,num_lines);
                        return USING_NAMESPACE;}

"#include" {fprintf(fout, "INCLUDE_STATEMENT,%s,%d\n", yytext,num_lines);
            return INCLUDE;}

"cin" {fprintf(fout, "READ, %s, %d\n", yytext, num_lines);
        return READ;
}

"cout" {fprintf(fout, "WRITE, %s, %d\n", yytext, num_lines);
        return WRITE;
}

"return" {fprintf(fout, "RETURN, %s, %d\n", yytext, num_lines);
        return RETURN;
}

"if" {fprintf(fout, "IF, %s, %d\n", yytext, num_lines);
        return IF;
}

"else" {fprintf(fout, "ELSE, %s, %d\n", yytext, num_lines);
        return ELSE;
}

"while" {fprintf(fout, "WHILE, %s, %d\n", yytext, num_lines);
        return WHILE;
}
"=" {fprintf(fout, "EQUAL, %s, %d\n", yytext, num_lines);
        return EQUAL;
}

"(" {fprintf(fout, "LEFT PARAN,(,%d\n", num_lines);
    return L_PAR;}
")" {fprintf(fout, "RIGHT PARAN,),%d\n", num_lines);
    return R_PAR;}
"{" {fprintf(fout, "LEFT BRACE,{,%d\n", num_lines);
    return L_BRACE;}
"}" {fprintf(fout, "RIGHT BRACE,},%d\n", num_lines);
        return R_BRACE;}
"," {fprintf(fout, "COMMA,comma_symbol,%d\n", num_lines);
        return COMMA;}
";" {fprintf(fout, "SEMI COLON,;,%d\n",num_lines);
        return SEMI_COLON;}

"<<" {fprintf(fout, "SHIFT LEFT,<<,%d\n", num_lines);
        return SHIFT_LEFT;
    }
">>" {fprintf(fout, "SHIFT RIGHT,>>,%d\n", num_lines);
        return SHIFT_RIGHT;
    }

"<" {fprintf(fout, "LESS_THAN,<,%d\n", num_lines);
     return SMALLER_THAN;}
     
">" {fprintf(fout, "GREATER_THAN,>,%d\n", num_lines);
     return GREATER_THAN;}

"true"|"false" {
    return BOOLEAN_LITERAL;
}

{type} {fprintf(fout, "TYPE,%s,%d\n", yytext, num_lines);
        return TYPE;}

0[xX][0-9a-fA-F]+ {
    struct BinaryTreeNode *found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(found==NULL){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    return NUMBER;
    
}

0[bB][01]+ {
    struct BinaryTreeNode* found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(!found){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    fprintf(fout, "HEXA,%s,%d,%d\n", yytext, num_lines, pos);
    return NUMBER;

}

0[1-7][0-7]+ {
    struct BinaryTreeNode* found = searchNode(tabel_simboluri, yytext);
        int pos;
    if(!found){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    fprintf(fout, "OCTAL,%s,%d,%d\n", yytext, num_lines, pos);
    return NUMBER;

}


{double_number} {
    struct BinaryTreeNode* found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(!found){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    fprintf(fout, "DOUBLE,%s,%d,%d\n", yytext, num_lines, pos);
    return NUMBER;

}


{number}[ ] {
    struct BinaryTreeNode* found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(!found){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    fprintf(fout, "INTEGER,%s,%d,%d\n", yytext, num_lines, pos);
    return NUMBER;


}

{boolean_operator} {fprintf(fout, "BOOL OPERATOR,%s,%d\n", yytext, num_lines);
                    return BOOL_OPERATOR;
                    }
{operator} {fprintf(fout, "OPERATOR,%s,%d\n", yytext, num_lines);
            return OPERATOR;}

{identifier} {
    struct BinaryTreeNode* found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(!found){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    fprintf(fout, "IDENTIFIER,%s,%d,%d\n", yytext, num_lines, pos);
    return IDENTIFIER;

}

[ \t]+ ;  
0[1-7]*[8-9][1,7]* {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
\<\<\<+ {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
\>\>\>+ {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
{number}{identifier} {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
{number}[.] {fprintf(fout, "EROARE,%s,%d\n",yytext, num_lines);}
[.]{number} {fprintf(fout, "EROARE,%s,%d\n",yytext, num_lines);}
. {fprintf(fout, "UNIDENTIFIED,%s,%d\n", yytext, num_lines);}

%%