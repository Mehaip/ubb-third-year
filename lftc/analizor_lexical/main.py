from lexer import Lexer
from bst import BSTNode

if __name__ == "__main__":
    input_file = input("Introdu numele fisierului txt: ")

    input_file = "source_code/" + input_file + ".txt"
    
    try:
        # Citește codul din fișier
        with open(input_file, 'r', encoding='utf-8') as f:
            source_code = f.read()
        
        print(f"\n Citire fișier: {input_file}")
        print(f"{'='*60}\n")
        print("Citit cu succes!\n")
        # Creează lexer-ul
        lexer = Lexer(source_code)
        
        # Tokenizează
        tokens = lexer.tokenize()
        ts_tokens = lexer.get_identifiers_and_constants()
    
        bst = BSTNode()
        for token in ts_tokens:
            bst.insert(token.value)

        bst_sorted = bst.inorder([])

        with open("data/ts.txt", "w") as f:
            for value in bst_sorted:
                f.write(value)
                f.write("\n")
        print("Fisierul ts.txt completat!")
        # Afișează tokenurile

        with open("data/fip.csv", "w") as f:
            f.write("Atom lexical,Value,Pozitie in TS\n")
            for token in tokens:
                if token.type.value != ",":
                    f.write(token.type.name + "," + token.value + ",")
                else:
                    f.write(token.type.name + "," + "commaValue,")
                if lexer.is_identifier_or_constant(token.type.name):
                    f.write(str(bst_sorted.index(token.value) + 1))
                else:
                    f.write("-1")
                f.write("\n")

        print("Fisierul fip.csv completat!")




        
    except FileNotFoundError:
        print(f" ERR: Fișierul '{input_file}' nu a fost găsit!")
    except Exception as e:
        print(f" ERR: {e}")