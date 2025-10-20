Analyzer written in Python for C++ language 

Input: txt file of your code
OUTPUT: FIP (Forma interna a programului) and TS (tabela simboluri)

TS: information about identifiers (variable names) and constants (integer, double literals, boolean values)
FIP: the source code's tokens, associated with the TS's position in the ts.txt file (only for identifiers and constants, -1 otherwise)
