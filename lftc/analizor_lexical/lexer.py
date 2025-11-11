# lexer_with_dfa.py
from typing import List, Optional
from token_types import Token, TokenType
from dfa import *

class Lexer:
    def __init__(self, source_code: str):
        #atribute
        self.source = source_code
        self.position = 0
        self.line = 1
        self.column = 1
        self.tokens: List[Token] = []
        self.err: List[str] = []
        
        # Initialize DFAs
        self.identifier_dfa = create_identifier_dfa()
        self.decimal_dfa = create_decimal_dfa()
        self.hex_dfa = create_hexadecimal_dfa()
        self.binary_dfa = create_binary_dfa()
        self.octal_dfa = create_octal_dfa()
        self.double_dfa = create_double_dfa()
        
        # Keywords dictionary
        self.keywords = {
            'using': TokenType.KEYWORD_USING,
            'namespace': TokenType.KEYWORD_NAMESPACE,
            'if': TokenType.KEYWORD_IF,
            'else': TokenType.KEYWORD_ELSE,
            'while': TokenType.KEYWORD_WHILE,
            'return': TokenType.KEYWORD_RETURN,
            'cin': TokenType.KEYWORD_CIN,
            'cout': TokenType.KEYWORD_COUT,
            'endl': TokenType.KEYWORD_ENDL,
            'include': TokenType.KEYWORD_INCLUDE,
            'std': TokenType.KEYWORD_STD,
            'int': TokenType.TYPE_INT,
            'double': TokenType.TYPE_DOUBLE,
            'bool': TokenType.TYPE_BOOL,
            'true': TokenType.BOOLEAN_TRUE,
            'false': TokenType.BOOLEAN_FALSE,
        }
    
    #caracter curent
    def current_char(self) -> Optional[str]:
        if self.position >= len(self.source):
            return None
        return self.source[self.position]
    
    #ne uitam la urmatorul
    def peek_char(self, offset: int = 1) -> Optional[str]:
        pos = self.position + offset
        if pos >= len(self.source):
            return None
        return self.source[pos]
    
    #pozitie in fata
    def advance(self):
        if self.position < len(self.source):
            if self.source[self.position] == '\n':
                self.line += 1
                self.column = 1
            else:
                self.column += 1
            self.position += 1
    
    def skip_whitespace(self):
        while self.current_char() and self.current_char().isspace():
            self.advance()
    
    def read_number_with_dfa(self) -> Optional[Token]:
        """Try to match a number using DFAs"""
        start_line = self.line
        start_col = self.column
        remaining = self.source[self.position:]
        
        #  hex  (0x...)
        prefix, length = self.hex_dfa.longest_accepted_prefix(remaining)
        if length > 0:
            for _ in range(length):
                self.advance()
            return Token(TokenType.INTEGER_LITERAL, prefix, start_line, start_col,True)
        
        #  binary (0b...)
        prefix, length = self.binary_dfa.longest_accepted_prefix(remaining)
        if length > 0:
            for _ in range(length):
                self.advance()
            return Token(TokenType.INTEGER_LITERAL, prefix, start_line, start_col,True)
        
        #  octal (0...)
        prefix, length = self.octal_dfa.longest_accepted_prefix(remaining)
        if length > 0:
            for _ in range(length):
                self.advance()
            return Token(TokenType.INTEGER_LITERAL, prefix, start_line, start_col,True)
        
        #  double (123.45)
        prefix, length = self.double_dfa.longest_accepted_prefix(remaining)
        if length > 0:
            for _ in range(length):
                self.advance()
            return Token(TokenType.DOUBLE_LITERAL, prefix, start_line, start_col,True)
        
        #  decimal (123)
        prefix, length = self.decimal_dfa.longest_accepted_prefix(remaining)
        if length > 0:
            for _ in range(length):
                self.advance()
            return Token(TokenType.INTEGER_LITERAL, prefix, start_line, start_col,True)
        
        # No valid number found - error
        error_str = ""
        while self.current_char() and not self.current_char().isspace() and self.current_char() not in "+-*/=;<>(){},%":
            error_str += self.current_char()
            self.advance()
        
        error_msg = f"ERR: LINE {start_line} COL {start_col}: invalid number: \"{error_str}\""
        self.err.append(error_msg)
        return Token(TokenType.ERROR, error_str, start_line, start_col,False)
    
    def read_identifier_with_dfa(self) -> Optional[Token]:
        """Try to match an identifier using DFA"""
        start_line = self.line
        start_col = self.column
        remaining = self.source[self.position:]
        
        prefix, length = self.identifier_dfa.longest_accepted_prefix(remaining)
        
        if length > 0:
            for _ in range(length):
                self.advance()
            
            is_identifier = False
            token_type = self.keywords.get(prefix, TokenType.IDENTIFIER)
            if token_type == TokenType.IDENTIFIER:
                is_identifier = True
            return Token(token_type, prefix, start_line, start_col,is_identifier)
        
        # Invalid identifier
        error_str = ""
        while self.current_char() and not self.current_char().isspace() and self.current_char() not in "+-*/=;<>(){},%":
            error_str += self.current_char()
            self.advance()
        
        error_msg = f"ERR: LINE {start_line} COL {start_col}: invalid identifier: \"{error_str}\""
        self.err.append(error_msg)
        return Token(TokenType.ERROR, error_str, start_line, start_col,False)
    
    def tokenize(self) -> List[Token]:
        while self.current_char():
            # Skip whitespace
            if self.current_char().isspace():
                self.skip_whitespace()
                continue
            
            start_line = self.line
            start_col = self.column
            char = self.current_char()
            
            # Numbers -  DFA
            if char.isdigit():
                token = self.read_number_with_dfa()
                if token:
                    self.tokens.append(token)
                continue
            
            # Identifiers and keywords -  DFA
            if char.isalpha() or char == '_':
                token = self.read_identifier_with_dfa()
                if token:
                    self.tokens.append(token)
                continue
            
            # Two-character operators
            if char == '=' and self.peek_char() == '=':
                self.tokens.append(Token(TokenType.EQ, "==", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            if char == '!' and self.peek_char() == '=':
                self.tokens.append(Token(TokenType.NEQ, "!=", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            if char == '<' and self.peek_char() == '=':
                self.tokens.append(Token(TokenType.LTE, "<=", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            if char == '>' and self.peek_char() == '=':
                self.tokens.append(Token(TokenType.GTE, ">=", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            if char == '<' and self.peek_char() == '<':
                self.tokens.append(Token(TokenType.OUTPUT_OP, "<<", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            if char == '>' and self.peek_char() == '>':
                self.tokens.append(Token(TokenType.INPUT_OP, ">>", start_line, start_col))
                self.advance()
                self.advance()
                continue
            
            # Single character tokens
            single_char_tokens = {
                '+': TokenType.PLUS,
                '-': TokenType.MINUS,
                '*': TokenType.MULTIPLY,
                '/': TokenType.DIVIDE,
                '%': TokenType.MODULO,
                '=': TokenType.ASSIGN,
                '<': TokenType.LT,
                '>': TokenType.GT,
                '(': TokenType.LPAREN,
                ')': TokenType.RPAREN,
                '{': TokenType.LBRACE,
                '}': TokenType.RBRACE,
                ';': TokenType.SEMICOLON,
                ',': TokenType.COMMA,
                '#': TokenType.HASH,
            }
            
            if char in single_char_tokens:
                self.tokens.append(Token(single_char_tokens[char], char, start_line, start_col))
                self.advance()
                continue
            
            # Unknown character
            self.tokens.append(Token(TokenType.UNKNOWN, char, start_line, start_col))
            self.advance()
        
        # Add EOF
        self.tokens.append(Token(TokenType.EOF, "", self.line, self.column))
        return self.tokens
    
    def print_tokens(self):
        print("=== ANALIZA LEXICALA (cu FA) ===\n")
        
        for token in self.tokens:
            if token.type != TokenType.EOF:
                print(token)
        
        if self.err:
            print("\n=== ERORI ===")
            for error in self.err:
                print(error)
    
    def get_identifiers_and_constants(self) -> List[Token]:
        ts_token = []
        for token in self.tokens:
            if self.is_identifier_or_constant(token.type.name):
                ts_token.append(token)
        return ts_token
    
    def is_identifier_or_constant(self, token_type) -> bool:
        return token_type in ["IDENTIFIER", "INTEGER_LITERAL", "DOUBLE_LITERAL", "BOOLEAN_TRUE", "BOOLEAN_FALSE"]