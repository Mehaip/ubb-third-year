from numpy import random

x = random.randint(9)
y = random.randint(9)
f = open("input_files/numar1/2.txt", "w")
g = open("input_files/numar2/2.txt", "w")

f.write("10000 ")
g.write("10000 ")
for _ in range(1,10001):
    f.write(str(x))
    g.write(str(y))
    x = random.randint(9)
    y = random.randint(9)

f.close()
g.close()

f = open("input_files/numar1/3.txt", "w")
g = open("input_files/numar2/3.txt", "w")

f.write("100000 ")
g.write("100 ")

for _ in range(1,100001):
    f.write(str(x))
    x = random.randint(9)
    if _ <= 100:
        g.write(str(y))
        y = random.randint(9)