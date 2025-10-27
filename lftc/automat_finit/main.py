from lexer import AutomatFinit

if __name__=="__main__":
    af = AutomatFinit()

    # ex1
    af.read_from_file("literalicpp.txt")

    # ex2
    af.write_to_file("output.txt")

    # ex3
    print("Verificare secventa pt AF")
    sequence = str(input("Introdu secventa\n>>> "))
    if af.accepts(sequence):
        print(f"AF accepta {sequence}")
    else:
        print(f"AF nu accepta {sequence}")
    
    # ex4
    print("Cel mai lung prefix din secventa")
    sequence = str(input("Introdu secventa\n>>> "))
    print(f"{af.longest_accepted_prefix(sequence)}")
