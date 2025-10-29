from lexer import AutomatFinit

if __name__=="__main__":
    af = AutomatFinit()

    # ex1
    print("tastatura? (y/n)")
    ans = input()
    if ans == 'y':
        af.read_from_keyboard()
    else:
        af.read_from_file("literalicpp.txt")

    # ex2
    af.write_to_file("output.txt")

    # ex3
    while True:
        print()
        print("1. Verifica daca e acceptata secventa")
        print("2. Cel mai lung prefix valid")
        option = int(input(">>> "))
        if option == 1:
            print("Verificare secventa pt AF")
            sequence = str(input("Introdu secventa\n>>> "))
            print()
            if af.accepts(sequence):
                print(f"AF accepta {sequence}")
            else:
                print(f"AF nu accepta {sequence}")
        if option == 2:
            # ex4
            print("Cel mai lung prefix din secventa")
            sequence = str(input("Introdu secventa\n>>> "))
            print()
            print(f"{af.longest_accepted_prefix(sequence)}")

        print(f"Determinist?: {af.is_deterministic()}")
