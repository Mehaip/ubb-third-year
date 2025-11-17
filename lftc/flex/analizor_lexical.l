/*DECLARATIONS*/

%{
    int num_lines = 1;
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    
    FILE *fout;
    FILE *fout_ts;
    char *elemente_ts[100];  
    int elemente_ts_count = 0;
%}

letter [a-zA-Z]
digit [0-9]
operator [+\-*/=]
boolean_operator ("=="|"!="|">"|">="|"<"|"<=")

/*identifiers/numbers*/
type ("int"|"bool"|"double")
number {digit}+
double_number {number}\.{number}
identifier {letter}({letter}|{digit}|_)*

term ({identifier}|{number}|{double_number})


/*RULES*/
%%

\n  {++num_lines;}
"using namespace std;" {fprintf(fout, "USING_STATEMENT,,%d\n", num_lines);}
"#include" {fprintf(fout, "INCLUDE_STATEMENT,,%d\n", num_lines);}

{type} {fprintf(fout, "TYPE,%s,%d\n", yytext, num_lines);}

{double_number} {
    int pos = element_in_ts_list(yytext);
    if(pos == -1) {
        elemente_ts[elemente_ts_count] = strdup(yytext);
        pos = elemente_ts_count;
        elemente_ts_count++;
    }
    fprintf(fout, "DOUBLE,%s,%d,%d\n", yytext, num_lines, pos);
}

{number} {
    int pos = element_in_ts_list(yytext);
    if(pos == -1) {
        elemente_ts[elemente_ts_count] = strdup(yytext);
        pos = elemente_ts_count;
        elemente_ts_count++;
    }
    fprintf(fout, "INTEGER,%s,%d,%d\n", yytext, num_lines, pos);
}

{boolean_operator} {fprintf(fout, "BOOL OPERATOR,%s,%d\n", yytext, num_lines);}
{operator} {fprintf(fout, "OPERATOR,%s,%d\n", yytext, num_lines);}

{identifier} {
    int pos = element_in_ts_list(yytext);
    if(pos == -1) {
        elemente_ts[elemente_ts_count] = strdup(yytext);
        pos = elemente_ts_count;
        elemente_ts_count++;
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
. {fprintf(fout, "UNIDENTIFIED,%s,%d\n", yytext, num_lines);}

%%

int element_in_ts_list(char* text){
    for(int i = 0; i < elemente_ts_count; i++)
        if(strcmp(elemente_ts[i], text) == 0) 
            return i;
    return -1;
}

int main(int argc, char **argv){
    fout = fopen("data/fip.csv", "w");
    fout_ts = fopen("data/ts.csv", "w");
    if (!fout) {
        fprintf(stderr, "Error: Cannot open output file data/fip.csv\n");
        return 1;
    }
    if (!fout_ts) {
        fprintf(stderr, "Error: Cannot open output file data/ts.txt\n");
        fclose(fout);
        return 1;
    }

    fprintf(fout, "Atom lexical,Value,Line,Pozitie in TS\n");

    if (argc > 1){
        FILE *fp = fopen(argv[1], "r");
        if(fp)
            yyin = fp;
        else {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            fclose(fout);
            fclose(fout_ts);
            return 1;
        }
    }
    
    yylex();
    
    fprintf(fout_ts, "Position,Value\n");
    for(int i = 0; i < elemente_ts_count; i++){
        fprintf(fout_ts, "%d,%s\n", i, elemente_ts[i]);
        free(elemente_ts[i]);  // free allocated memory
    }
    
    fprintf(fout, "EOF\n");
    fclose(fout);
    fclose(fout_ts);
    printf("Output written to data/fip.csv and data/ts.txt\n");
    printf("Symbol table entries: %d\n", elemente_ts_count);
    
    return 0;
}