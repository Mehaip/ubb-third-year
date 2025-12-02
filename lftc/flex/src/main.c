#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "globals.h"

extern FILE *yyin; //fisierul din care va fi citit textul
extern int yylex(void);
void inorder(void *table, FILE *out);

int yywrap(){
    return 1;
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
    inorder(tabel_simboluri, fout_ts);
    
    
    
    fprintf(fout, "EOF\n");
    fclose(fout);
    fclose(fout_ts);
    printf("Output written to data/fip.csv and data/ts.txt\n");

    freeTree(tabel_simboluri);

    
    return 0;
}