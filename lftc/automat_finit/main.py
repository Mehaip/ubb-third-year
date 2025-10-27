from lexer import AutomatFinit

if __name__=="__main__":
    af = AutomatFinit()

    # ex1
    af.read_from_file("literalicpp.txt")

    # ex2
    af.write_to_file("output.txt")

    
