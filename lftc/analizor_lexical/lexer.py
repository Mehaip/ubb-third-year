from typing import List, Optional
from token_types import Token, TokenType

class Lexer:
    def __init__(self, source_code: str):
        self.source = source_code
        self.position = 0
        self.line = 1
        self.column = 1
        self.tokens: List[Token] = []
        self.err: List[str] = []
        
        # Cuvinte cheie și tipuri
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
        
    
    def current_char(self) -> Optional[str]:
        if self.position >= len(self.source):
            return None
        return self.source[self.position]
    
    def peek_char(self, offset: int = 1) -> Optional[str]:
        pos = self.position + offset
        if pos >= len(self.source):
            return None
        return self.source[pos]
    
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
    
    def read_number(self) -> Token:
        start_line = self.line
        start_col = self.column
        num_str = ""
        is_double = False
        
        while self.current_char() and self.current_char().isdigit():
            num_str += self.current_char()
            self.advance()
        
        # Verifică dacă este double
        if self.current_char() == '.' and self.peek_char() and self.peek_char().isdigit():
            is_double = True
            num_str += self.current_char()
            self.advance()
            
            while self.current_char() and self.current_char().isdigit():
                num_str += self.current_char()
                self.advance()

        if self.current_char().isalpha() or self.current_char() == '.':
            token_type = TokenType.ERROR
            while self.current_char() and (self.current_char().isalnum() or self.current_char()=='.'):
                num_str += self.current_char()
                self.advance()
            error_msg = f"ERR: LINE {start_line} COL {start_col}: invalid number: \"{num_str}\""
            self.err.append(error_msg)
            return Token(token_type, num_str, start_line, start_col)

        
        token_type = TokenType.DOUBLE_LITERAL if is_double else TokenType.INTEGER_LITERAL
        return Token(token_type, num_str, start_line, start_col)
    
    def read_identifier(self) -> Token:
        start_line = self.line
        start_col = self.column
        identifier = ""
        
        while self.current_char() and (self.current_char().isalnum() or self.current_char() == '_'):
            identifier += self.current_char()
            self.advance()
        
        if self.current_char() and self.current_char() not in ['+', '-', '*', '/', '=', ';',',', ' ', '<', '>', '(',')']:
            while self.current_char() and self.current_char() not in ['+', '-', '*', '/', '=', ';',',', ' ', '>', '<','(',')']:
                identifier += self.current_char()
                self.advance()
            error_msg = f"ERR: LINE {start_line} COL {start_col}: invalid identifier name: \"{identifier}\""
            self.err.append(error_msg)
            token_type = TokenType.ERROR
            return Token(token_type, identifier, start_line, start_col)

        

        # Verifică dacă este cuvânt cheie sau tip
        token_type = self.keywords.get(identifier, TokenType.IDENTIFIER)
        
        return Token(token_type, identifier, start_line, start_col)
    
    def tokenize(self) -> List[Token]:
        while self.current_char():
            # Sari peste whitespace
            if self.current_char().isspace():
                self.skip_whitespace()
                continue
            
            start_line = self.line
            start_col = self.column
            char = self.current_char()
            
            # Numere
            if char.isdigit():
                self.tokens.append(self.read_number())
                continue
            
            # Identificatori și cuvinte cheie
            if char.isalpha() or char == '_':
                self.tokens.append(self.read_identifier())
                continue
            
            # Operatori cu două caractere
            if char == '=' and self.peek_char() == '=':
                self.advance()
                if self.peek_char() == '=':
                    self.tokens.append(Token(TokenType.ERROR, "===", start_line, start_col))
                    error_msg = f"ERR: LINE {start_line} COL {start_col}: \"===\" is an invalid operator "
                    self.err.append(error_msg)
                    self.advance()
                else:
                    self.tokens.append(Token(TokenType.EQ, "==", start_line, start_col))
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
            
            # Operatori și delimitatori cu un singur caracter
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
            
            # Caracter necunoscut
            self.tokens.append(Token(TokenType.UNKNOWN, char, start_line, start_col))
            self.advance()
        
        # Adaugă EOF
        self.tokens.append(Token(TokenType.EOF, "", self.line, self.column))
        return self.tokens
    
    def print_tokens(self):
        print("=== ANALIZA LEXICALA ===\n")
        
        for token in self.tokens:
            if token.type != TokenType.EOF:
                print(token)

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