class Grammar:
    nonTerminals = set()
    terminals = set()
    productions = []

    def read_from_file(self, fileName):
        file = open(fileName, "r")
        line = file.readline()
        productions_string = []
        while line:
            productions_string.append(line)
            line = file.readline()
        
        return productions_string
    

    def create_terminals_and_non_terminals(self, production_list):
        for prod in production_list:
            prod = prod.replace(" ", "")
            prod = prod.replace("\n", "")
            prod = prod.split("->")
            self.nonTerminals.add(prod[0])

            right_handside = prod[1]
            nonTerm = [l for l in right_handside if l.isupper()]
            term = [l for l in right_handside if not l.isupper()]

            for lit in nonTerm:
                self.nonTerminals.add(lit)
            for lit in term:
                self.terminals.add(lit)
        print(f"NonTerm: {self.nonTerminals}")
        print(f"Term: {self.terminals}")

    def find_non_terminal(self, nonTerminal, production_list):
        if nonTerminal not in self.nonTerminals:
            print("ERROR: nonTerminal not existent")
        else:
            for productie in production_list:
                nonTerminali_in_productie = [l for l in productie if l in self.nonTerminals and l == nonTerminal]
                if len(nonTerminali_in_productie) != 0:
                    print(productie.replace("\n", ""))



if __name__ == "__main__":
    gramatica = Grammar()
    input_file = gramatica.read_from_file("test.txt")
    gramatica.create_terminals_and_non_terminals(input_file)
    nonTerminal = input(">>> ")
    gramatica.find_non_terminal(nonTerminal, input_file)


