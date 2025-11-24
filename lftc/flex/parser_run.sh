rm -f build/parser build/lex.yy.c build/parser.tab.c build/parser.tab.h

bison -d -o build/parser.tab.c src/parser.y -Wcounterexamples
flex -o build/lex.yy.c src/analizor_lexical.l
gcc build/lex.yy.c src/bst.c src/globals.c build/parser.tab.c -I src -lfl -o build/parser

./build/parser test.txt
