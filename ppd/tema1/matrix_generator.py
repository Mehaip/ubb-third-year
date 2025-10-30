import numpy as np

#Generam 3 matrici pentru 3 fisiere in folderul data/

with open ("data/matrice10.txt", "w") as f:
    array = np.random.randint(10, size=(10,10))
    f.write("10 10 3\n")
    for line in array:
        f.write(" ".join(map(str,line)) + "\n")
    array2 = np.random.randint(10, size=(3,3))
    for line in array2:
        f.write(" ".join(map(str,line)) + "\n")

with open ("data/matrice1000.txt", "w") as f:
    array = np.random.randint(10, size=(1000,1000))
    f.write("1000 1000 3\n")
    for line in array:
        f.write(" ".join(map(str,line)) + "\n")
    array2 = np.random.randint(10, size=(3,3))
    for line in array2:
        f.write(" ".join(map(str,line)) + "\n")

with open ("data/matrice10000.txt", "w") as f:
    array = np.random.randint(10, size=(10000,10000))
    f.write("10000 10000 3\n")
    for line in array:
        f.write(" ".join(map(str,line)) + "\n")

    array2 = np.random.randint(10, size=(3,3))
    for line in array2:
        f.write(" ".join(map(str,line)) + "\n")


        
