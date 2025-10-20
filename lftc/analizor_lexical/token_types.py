from enum import Enum
from dataclasses import dataclass

class TokenType(Enum):
    # Cuvinte cheie
    KEYWORD_USING = "using"
    KEYWORD_NAMESPACE = "namespace"
    KEYWORD_IF = "if"
    KEYWORD_ELSE = "else"
    KEYWORD_WHILE = "while"
    KEYWORD_RETURN = "return"
    KEYWORD_CIN = "cin"
    KEYWORD_COUT = "cout"
    KEYWORD_ENDL = "endl"
    KEYWORD_INCLUDE = "include"
    KEYWORD_STD = "std"
    
    # Tipuri
    TYPE_INT = "int"
    TYPE_DOUBLE = "double"
    TYPE_BOOL = "bool"
    
    # Literali boolean
    BOOLEAN_TRUE = "true"
    BOOLEAN_FALSE = "false"
    
    # Literali
    INTEGER_LITERAL = "INTEGER_LITERAL"
    DOUBLE_LITERAL = "DOUBLE_LITERAL"
    
    # Identificatori
    IDENTIFIER = "IDENTIFIER"
    
    # Operatori aritmetici
    PLUS = "+"
    MINUS = "-"
    MULTIPLY = "*"
    DIVIDE = "/"
    MODULO = "%"
    
    # Operatori de atribuire
    ASSIGN = "="
    
    # Operatori relationali
    EQ = "=="
    NEQ = "!="
    LT = "<"
    GT = ">"
    LTE = "<="
    GTE = ">="
    
    # Delimitatori
    LPAREN = "("
    RPAREN = ")"
    LBRACE = "{"
    RBRACE = "}"
    SEMICOLON = ";"
    COMMA = ","
    
    # Simboluri speciale
    HASH = "#"
    OUTPUT_OP = "<<"
    INPUT_OP = ">>"
    
    # Altele
    EOF = "EOF"
    UNKNOWN = "UNKNOWN"

@dataclass
class Token:
    type: TokenType
    value: str
    line: int
    column: int
    
    def __str__(self):
        return f"Line {self.line}, Col {self.column}: TOKEN[{self.type.name}] = '{self.value}'"