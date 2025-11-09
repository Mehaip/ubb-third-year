# Proiect Farcas

exemplu:
StateMachine
 description: String /** A description of the state machine */
 startState :State
 states: Set<State> /** Can be List, Map, … */
 /** What about redundancy? */
State
 description: String /** The name of the state */
 isAcceptState: Boolean
 transitions: MultiMap<Symbol, State> /** Can be List, Map … ? */


Masina stari:
    descriere: String
    stareStart: String
    stariFinale: Set<String>

 Stare:
    descriere: String
    tranzitii: Map<pair<String, String>, Set<String>>
