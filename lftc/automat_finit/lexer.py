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

    def write_to_file(self, filename):
        """Write all automaton data to a file"""
        try:
            with open(filename, 'w') as f:
                # Write states
                if self.stari:
                    f.write("stari : " + " ".join(sorted(self.stari)) + "\n")

                # Write alphabet
                if self.alfabet:
                    f.write("alfabet : " + " ".join(sorted(self.alfabet)) + "\n")

                # Write transitions
                if self.tranzitii:
                    f.write("tranzitii\n")
                    for (from_state, symbol), to_states in sorted(self.tranzitii.items()):
                        for to_state in sorted(to_states):
                            f.write(f"{from_state}, {symbol} -> {to_state}\n")
                    f.write("sfarsit tranzitii\n")

                # Write initial state
                if self.stare_initiala:
                    f.write(f"stare_initiala : {self.stare_initiala}\n")

                # Write final states
                if self.stari_finale:
                    f.write("stari_finale : " + " ".join(sorted(self.stari_finale)) + "\n")

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
            stare.replace(',',"")
            if stariFinale:
                self.stari_finale.add(stare)
            else:
                self.stari.add(stare)

    def parser_alfabet(self, alfabet:list[str]):
        for element in alfabet:
            element.replace(',',"")
            if "-" in element:
                self.alfabet.add(self.expand_range(element))
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


            




    





    



