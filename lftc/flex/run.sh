flex -o build/lex.yy.c analizor_lexical.l
gcc build/lex.yy.c -lfl -o build/lexer
./build/lexer test.txt
