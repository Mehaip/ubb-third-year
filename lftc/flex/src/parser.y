%{
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
//#include "globals.h"
int yylex(void);
void yyerror(const char *s);
extern int yylineno;

%}
%token NUMBER OPERATOR BOOL_OPERATOR IDENTIFIER INCLUDE USING_NAMESPACE GREATER_THAN SMALLER_THAN
%token TYPE L_PAR R_PAR COMMA L_BRACE R_BRACE EQUAL
%token SEMI_COLON RETURN IF ELSE WHILE READ WRITE
%token BOOLEAN_LITERAL
%token SHIFT_LEFT SHIFT_RIGHT
%token ERROR

%%

///Structura programului
program:
    include_list using_statement function_list
    | error {YYABORT;}
    ;

include_list:
    |
    include_file include_list
    ;

using_statement:
    |
    USING_NAMESPACE
    ;

include_file:
    INCLUDE SMALLER_THAN IDENTIFIER GREATER_THAN
    ;

function_definition:
    TYPE IDENTIFIER L_PAR parameter_list R_PAR statement_block
    ;

function_list:
    function_definition 
    | function_definition function_list
    ;

parameter:
    TYPE IDENTIFIER
    ;

parameter_list:
    |
    non_empty_parameter_list
    ;

non_empty_parameter_list:
    parameter
    |
    parameter COMMA non_empty_parameter_list
    ;

///Statement

statement_block:
    L_BRACE statement_list R_BRACE
    ;

statement_list:
    |
    statement statement_list
    ;

statement:
    variable_declaration SEMI_COLON
    |expression_statement SEMI_COLON
    |return_statement SEMI_COLON
    |if_statement
    |while_statement
    |read_statement SEMI_COLON
    |print_statement SEMI_COLON
    ;

variable_declaration:
    TYPE
    IDENTIFIER
    variable_value_assignation
    variable_declaration_enumeration
    ;

variable_value_assignation: ///a SAU a=0
    |
    EQUAL
    arithmetic_expression
    ;

variable_declaration_enumeration: ///
    |
    COMMA
    IDENTIFIER
    variable_value_assignation
    variable_declaration_enumeration
    ;

expression_statement:
    |
    expression
    ;

return_statement:
    RETURN
    expression_statement
    ;

else_if_statement:
    |
    ELSE
    statement_block
    ;

if_statement:
    IF
    L_PAR
    boolean_expression
    R_PAR
    statement_block
    else_if_statement
    ;

while_statement:
    WHILE
    L_PAR
    boolean_expression
    R_PAR
    statement_block
    ;

read_statement:
    READ
    SHIFT_RIGHT
    
    ;

print_statement:
    WRITE
    SHIFT_LEFT
    expression //aici eroare!
    ;


///Expressions
expression:
    term
    | expression OPERATOR term
    | expression BOOL_OPERATOR term
    | expression EQUAL term ///ASTA ERA EROAREA
    ;

arithmetic_expression:
    term
    operator_usage
    ;

boolean_expression:
    term
    boolean_operator_usage
    ;

operator_usage:
    |
    OPERATOR
    term
    operator_usage
    ;

boolean_operator_usage:
    |
    BOOL_OPERATOR
    term
    boolean_operator_usage
    ;

term:
    IDENTIFIER |
    NUMBER |
    BOOLEAN_LITERAL
    ;



%%

void yyerror(const char *s) {
    fprintf(stderr, "ERROR LINE %d: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    extern FILE *fout;
    extern FILE *fout_ts;
    extern int ceremonie_count;
    extern int word_count;
    extern struct BinaryTreeNode *tabel_simboluri;
    
    // Initialize output files
    fout = fopen("data/fip.csv", "w");
    if (!fout) {
        fprintf(stderr, "Cannot open output file: data/fip.csv\n");
        return 1;
    }

    fout_ts = fopen("data/ts.csv", "w");
    if (!fout_ts) {
        fprintf(stderr, "Cannot open output file: data/ts.csv\n");
        fclose(fout);
        return 1;
    }

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            fclose(fout);
            fclose(fout_ts);
            return 1;
        }
        extern FILE *yyin;
        yyin = file;
    }
    fprintf(fout, "Atom lexical,Value,Line,Pozitie in TS\n");
    fflush(fout);
    if (yyparse() == 0) {
        printf("Syntax is correct!\n");
        printf("Ceremonie count: %d\n", ceremonie_count);
        printf("Word count: %d\n", word_count);
        fprintf(fout_ts, "Position,Value\n");
        inorder(tabel_simboluri, fout_ts);
        fclose(fout);
        fclose(fout_ts);
        return 0;
    } else {
        printf("Syntax errors found.\n");
        fclose(fout);
        fclose(fout_ts);
        return 1;
    }
}