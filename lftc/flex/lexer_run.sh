rm -f build/lexer build/lex.yy.c
rm -rf data/*
flex -o build/lex.yy.c src/analizor_lexical.l
gcc build/lex.yy.c src/bst.c src/globals.c src/main.c -I src -lfl -o build/lexer
./build/lexer test.txt