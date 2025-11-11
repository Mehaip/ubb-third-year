from typing import Set, Dict, Tuple, Optional, List

class DFA:
    
    def __init__(self, 
                 states: Set[str],
                 alphabet: Set[str],
                 transitions: Dict[Tuple[str, str], str],
                 initial_state: str,
                 final_states: Set[str]):

        self.states = states
        self.alphabet = alphabet
        self.transitions = transitions
        self.initial_state = initial_state
        self.final_states = final_states
    
    def _get_char_category(self, char: str) -> Optional[str]:

        if char in self.alphabet:
            return char
        
        
        if char.isdigit():
            if 'digit' in self.alphabet:
                return 'digit'
            if char in '01' and 'binary_digit' in self.alphabet:
                return 'binary_digit'
            if char in '01234567' and 'octal_digit' in self.alphabet:
                return 'octal_digit'
        
        if char.isalpha():
            if 'letter' in self.alphabet:
                return 'letter'
            if char.lower() in 'abcdef' and 'hex_letter' in self.alphabet:
                return 'hex_letter'
        
        if char == '_' and 'underscore' in self.alphabet:
            return 'underscore'
        
        return None
    
    def accepts(self, sequence: str) -> bool:
        if not sequence:
            return self.initial_state in self.final_states
        
        current_state = self.initial_state
        
        for char in sequence:
            char_category = self._get_char_category(char)
            
            if char_category is None:
                return False
            
            transition_key = (current_state, char_category)
            #tranzitie invalida
            if transition_key not in self.transitions:
                return False
            
            current_state = self.transitions[transition_key]
        #returnam daca starea curenta este in final states
        return current_state in self.final_states
    
    def longest_accepted_prefix(self, sequence: str) -> Tuple[str, int]:

        if not sequence:
            if self.initial_state in self.final_states:
                return ("", 0)
            return ("", 0)
        
        current_state = self.initial_state
        last_accepting_position = -1
        
        
        if current_state in self.final_states:
            last_accepting_position = 0
        
        for i, char in enumerate(sequence):
            char_category = self._get_char_category(char)
            
            if char_category is None:
                break
            
            transition_key = (current_state, char_category)
            
            if transition_key not in self.transitions:
                break
            
            current_state = self.transitions[transition_key]
            
            # verificam daca suntem in accepting state
            if current_state in self.final_states:
                last_accepting_position = i + 1
        
        if last_accepting_position == -1:
            return ("", 0)
        
        return (sequence[:last_accepting_position], last_accepting_position)
    
    def __str__(self):
        """String representation of the DFA"""
        return f"DFA(states={len(self.states)}, initial={self.initial_state}, final={self.final_states})"


# DFA PENTRU IDNETIFICATORI 

def create_identifier_dfa() -> DFA:
    """
    letter (letter | digit | '_')*
    """
    states = {'q0', 'q1'}
    alphabet = {'letter', 'digit', 'underscore'}
    transitions = {
        ('q0', 'letter'): 'q1',
        ('q0', 'underscore'): 'q1',
        ('q1', 'letter'): 'q1',
        ('q1', 'digit'): 'q1',
        ('q1', 'underscore'): 'q1',
    }
    initial_state = 'q0'
    final_states = {'q1'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)


def create_decimal_dfa() -> DFA:
    """
    0 | [1-9][0-9]*
    """
    states = {'q0', 'q1'}
    alphabet = {'digit'}
    transitions = {
        ('q0', 'digit'): 'q1',  
        ('q1', 'digit'): 'q1',  
    }
    initial_state = 'q0'
    final_states = {'q1'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)


def create_hexadecimal_dfa() -> DFA:
    """
    0[xX][0-9a-fA-F]+
    """
    states = {'q0', 'q1', 'q2', 'q3'}
    alphabet = {'0', 'x', 'X', 'digit', 'hex_letter'}
    transitions = {
        ('q0', '0'): 'q1',
        ('q1', 'x'): 'q2',
        ('q1', 'X'): 'q2',
        ('q2', 'digit'): 'q3',
        ('q2', 'hex_letter'): 'q3',
        ('q3', 'digit'): 'q3',
        ('q3', 'hex_letter'): 'q3',
    }
    initial_state = 'q0'
    final_states = {'q3'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)


def create_binary_dfa() -> DFA:

    states = {'q0', 'q1', 'q2', 'q3'}
    alphabet = {'0', 'b', 'B', '1', 'binary_digit'} #ATENTIE!!
    transitions = {
        ('q0', '0'): 'q1',
        ('q1', 'b'): 'q2',
        ('q1', 'B'): 'q2',
        ('q2', '0'): 'q3',
        ('q2', '1'): 'q3',
        ('q3', '0'): 'q3',
        ('q3', '1'): 'q3'
    }
    initial_state = 'q0'
    final_states = {'q3'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)


def create_octal_dfa() -> DFA:

    states = {'q0', 'q1', 'q2'}
    alphabet = {'0', 'octal_digit'}
    transitions = {
        ('q0', '0'): 'q1',
        ('q1', 'octal_digit'): 'q2',
        ('q2', 'octal_digit'): 'q2',
    }
    initial_state = 'q0'
    final_states = {'q2'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)


def create_double_dfa() -> DFA:
    """
    DFA for double literals: [0-9]+.[0-9]+
    """
    states = {'q0', 'q1', 'q2', 'q3'}
    alphabet = {'digit', '.'}
    transitions = {
        ('q0', 'digit'): 'q1',
        ('q1', 'digit'): 'q1',
        ('q1', '.'): 'q2',
        ('q2', 'digit'): 'q3',
        ('q3', 'digit'): 'q3',
    }
    initial_state = 'q0'
    final_states = {'q3'}
    
    return DFA(states, alphabet, transitions, initial_state, final_states)