rm -f build/lexer build/lex.yy.c
rm -rf data/*
rm -f parser.tab.c parser.tab.h

bison -d -o build/parser.tab.c src/parser.y -Wcounterexamples
flex -o build/lex.yy.c src/analizor_lexical.l
gcc build/lex.yy.c src/bst.c src/globals.c build/parser.tab.c -I src -lfl -o build/lexer

./build/lexer test.txt
