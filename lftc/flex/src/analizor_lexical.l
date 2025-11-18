/*DECLARATIONS*/

%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "bst.h"
    #include "globals.h"
%}

letter [a-zA-Z]
digit [0-9]
operator [+\-*/=]
boolean_operator ("=="|"!="|">"|">="|"<"|"<=")

/*identifiers/numbers*/
type ("int"|"bool"|"double")
number {digit}+
double_number {number}\.{number}
identifier (_|{letter})({letter}|{digit}|_)*

term ({identifier}|{number}|{double_number})


/*RULES*/
%%

\n  {++num_lines;}
"using namespace std;" {fprintf(fout, "USING_STATEMENT,,%d\n", num_lines);}
"#include" {fprintf(fout, "INCLUDE_STATEMENT,,%d\n", num_lines);}



{type} {fprintf(fout, "TYPE,%s,%d\n", yytext, num_lines);}

0[xX][0-9a-fA-F]+ {
    struct BinaryTreeNode *found = searchNode(tabel_simboluri, yytext);
    int pos;
    if(found==NULL){
        tabel_simboluri = insertNode(tabel_simboluri, yytext, &next_position);
        pos = next_position - 1;
    } else {
        pos = found->position;
    }
    
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


}

{boolean_operator} {fprintf(fout, "BOOL OPERATOR,%s,%d\n", yytext, num_lines);}
{operator} {fprintf(fout, "OPERATOR,%s,%d\n", yytext, num_lines);}

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


}

"(" {fprintf(fout, "LEFT PARAN,(,%d\n", num_lines);}
")" {fprintf(fout, "RIGHT PARAN,),%d\n", num_lines);}
"{" {fprintf(fout, "LEFT BRACE,{,%d\n", num_lines);}
"}" {fprintf(fout, "RIGHT BRACE,},%d\n", num_lines);}
"," {fprintf(fout, "COMMA,comma_symbol,%d\n", num_lines);}
";" {fprintf(fout, "SEMI COLON,;,%d\n",num_lines);}

[ \t]+ ;  
0[1-7]*[8-9][1,7]* {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
\<\<\<+ {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
\>\>\>+ {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
{number}{identifier} {fprintf(fout, "EROARE,%s,%d\n", yytext, num_lines);}
{number}[.] {fprintf(fout, "EROARE,%s,%d\n",yytext, num_lines);}
[.]{number} {fprintf(fout, "EROARE,%s,%d\n",yytext, num_lines);}
. {fprintf(fout, "UNIDENTIFIED,%s,%d\n", yytext, num_lines);}

%%