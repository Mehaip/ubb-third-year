class AutomatFinit:
    def __init__(self):
        self.stari = set()
        self.alfabet = set()
        self.tranzitii = {}
        self.stare_initiala = None
        self.stari_finale = set()

    def get_stari(self):
        return self.stari
    
    def get_alfabet(self):
        return self.alfabet
    
    def get_tranzitii(self):
        return self.tranzitii
    
    def get_stare_initiala(self):
        return self.stare_initiala
    
    def get_stari_finale(self):
        return self.stari_finale

    def read_from_keyboard(self):
        """Read automaton from keyboard input"""
        print("\n=== Enter Finite Automaton ===")
        print("Adauga fiecare sectiune (scrie STOP cand termini)\n")
        
        # Read states
        print("Stari (comma-separated, e.g., q0,q1,q2):")
        states_input = input().strip()
        for state in states_input.split(','):
            self.stari.add(state.strip())
        
        # Read alphabet
        print("Alfabet (comma-separated, supports ranges like 0-9, a-f):")
        alphabet_input = input().strip()
        for item in alphabet_input.split(','):
            self.alfabet.update(self.expand_range(item.strip()))
        
        # Read transitions
        print("\nTranzitii:")
        print("Una pe linie: stare_curenta, simbol->starea_urmatoare")
        lines = []
        while True:
            line = input().strip()
            if line == "STOP":
                break
            lines.append(line)


        self.parse_transitions(lines)
        
        # Read initial state
        print("\nStare initiala:")
        self.stare_initiala = input().strip()

        print("\nStari finale: (STOP ca sa te opresti)")
        while True:
            stare_finala = input()
            if stare_finala == "STOP":
                break
            self.stari_finale.add(stare_finala)

    
    def write_to_file(self, filename):
        """Write all automaton data to a file using proper mathematical symbols"""
        try:
            with open(filename, 'w') as f:
                
                if self.stari:
                    f.write("Q : " + " ".join(sorted(self.stari)) + "\n")

                
                if self.alfabet:
                    f.write("Σ : " + " ".join(sorted(self.alfabet)) + "\n")

                
                if self.tranzitii:
                    f.write("δ\n")
                    for (from_state, symbol), to_states in sorted(self.tranzitii.items()):
                        for to_state in sorted(to_states):
                            f.write(f"{from_state}, {symbol} -> {to_state}\n")
                    f.write("sfarsit δ\n")

                if self.stare_initiala:
                    f.write(f"q₀ : {self.stare_initiala}\n")

               
                if self.stari_finale:
                    f.write("F : " + " ".join(sorted(self.stari_finale)) + "\n")

                print(f"Date scrise in {filename} cu succes!")
        except Exception as e:
            print(f"ERR writing to {filename}: {e}")

    def expand_range(self, item:str):
        item = item.strip()
        if len(item) == 3 and item[1]=='-':
            start_char = item[0]
            end_char = item[2]
            return set(chr(i) for i in range(ord(start_char), ord(end_char) + 1))
        else:
            return item


    def read_from_file(self, filename):
        try:
            with open(filename, 'r') as f:
                text = f.read()
                self.parse_text(text)
                print(f"Text extras din {filename} cu succes!")
        except FileNotFoundError:
            print(f"ERR: {filename} not found!")
        except Exception as e:
            print(f"ERR {filename}: {e}")



    def parse_text(self, text: str):
        lines = text.strip().split("\n")
        i = 0
        while i < len(lines):
            line = lines[i]
            if len(line) > 0:
                first_word = line.split(" ")[0]
                splitLine = line.split(" ")
                match first_word:
                    case "stari":
                        self.parser_stari(splitLine[2:],False)
                    case "alfabet":
                        self.parser_alfabet(splitLine[2:])
                    case "tranzitii":
                        i+=1
                        transition_lines = []
                        while lines[i]!= "sfarsit tranzitii":
                            if len(lines[i])>0:
                                transition_lines.append(lines[i])
                            i+=1
                        self.parse_transitions(transition_lines)
                    case "stare_initiala":
                        self.stare_initiala = splitLine[2]
                    
                    case "stari_finale":
                        self.parser_stari(splitLine[2:], True)

                    case _:
                        print(f"Cuvantul {first_word} nedefinit")
            i+=1



    
    def parser_stari(self, stari:list[str],stariFinale:bool):
        for stare in stari:
            stare = stare.replace(',',"")
            if stariFinale:
                self.stari_finale.add(stare)
            else:
                self.stari.add(stare)

    def parser_alfabet(self, alfabet:list[str]):
        for element in alfabet:
            element = element.replace(',',"")
            if "-" in element:
                self.alfabet.update(self.expand_range(element))
            else:
                self.alfabet.add(element)

    def parse_transitions(self, transition_lines:list[str]):
        # format: from_stare, simbol -> next_stare
        for transition in transition_lines:
            if '->' not in transition:
                print("ERR: transition with bad format")
                return

            left, right = transition.split('->', 1)
            next_stare = right.strip()

            parts = left.split(',',1)
            from_stare = parts[0].strip()
            symbol_or_range = parts[1].strip()
            simboluri = self.expand_range(symbol_or_range)
            
            for simbol in simboluri:
                key = (from_stare, simbol)
                if key not in self.tranzitii:
                    self.tranzitii[key] = set() #adaugam cheia in tranzitii
                self.tranzitii[key].add(next_stare)

    def accepts(self, sequence):
        stare_curenta = self.stare_initiala

        for simbol in sequence:
            key = (stare_curenta, simbol)
            
            if key not in self.tranzitii:
                return False # nu exista tranzitie => reject

            stari_urmatoare = self.tranzitii[key]
            stare_curenta = next(iter(stari_urmatoare))
            print(f"{stare_curenta}, {simbol} -> {stare_curenta}")
        
        return stare_curenta in self.stari_finale #verificam daca e stare finala
    
    def longest_accepted_prefix(self, sequence):
        stare_curenta = self.stare_initiala
        prefix = ""

        for i, simbol in enumerate(sequence):
            key = (stare_curenta,simbol)
            if key not in self.tranzitii:
                break
            stari_urmatoare = self.tranzitii[key] #se acceseaza urmatoarele stari pentru key-ul (stare_curenta, simbol)
            stare_curenta = next(iter(stari_urmatoare)) #primul element din set
            if stare_curenta in self.stari_finale:
                prefix = sequence[:i + 1]
            print(f"{stare_curenta}, {simbol} -> {stare_curenta}")

        return prefix
    
    def is_deterministic(self):
        for (state, symbol), next_states in self.tranzitii.items():
            if len(next_states) > 1:
                return False
        return True

        


            




    





    



