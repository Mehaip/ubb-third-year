Cum/cand ar putea aparea un deadlock?

Deadlock-ul poate aparea doar daca sunt 4 conditii care sunt adevarate deodata:
1. mutual exclusion: doar un proces poate folosi o resursa la un moment dat
2. un proces da "hold" la o resursa si asteapta sa obtina o alta resursa tinuta de alt proces
3. o resursa nu poate fi luata de la un proces decat daca acel proces ii da release
4. asteptare circulara: un set de procese asteapta in ordine circulara.

exemplu:

Procesul A tine resursa R1 si cere resursa R2
Procesul B tine resursa R2 si cere resursa R1

Rezolvare pt exemplul cu arbore:
Toate metodele din clasa TreeNode contin metode synchronized. 

O metoda synchronised poate fi executata doar de un thread. Acest thread pune un lock pe obiect inainte sa intre in metoda. Celelalte thread-uri care vor sa acceseze orice metoda synchronized de pe acel obiect trebuie sa astepte pana cand lock-ul e released.
Lock-ul e released cand metoda este parcursa pana la final.
informatii de pe https://docs.oracle.com/javase/tutorial/essential/concurrency/syncmeth.html


In cazul nostru:

Ne putem folosi de addChild() ca sa facem deadlock-ul sa se intample.
Fiind o metoda synchronised, odata accesata de un thread, obiectul accesat este locked. (regula 1)
Metoda addChild lucreaza cu 2 obiecte: this si child.
Deci daca avem un thread care face
A.addChild(B);
si altul care face
B.addChild(A);
A si B sunt ambele locked, intrucat thread-urile incep deodata.
In metoda addChild(), cand ambele metode incerc sa dea call la setParentOnly(), nu va fi posibil deoarece acel obiect este LOCKED de celalalt thread.

In metoda exemplu_deadlock() se prezinta exact cazul de deadlock pentru arbore.
Fiindca thread-urile se completau prea repede, thread 1 termina deja inainte ca thread 2 sa inceapa, asa ca am introdus CountDownLatch. Este un sync tool care face thread-urile sa astepte pana un numar de operatii este completat.
In exemplul nostru, am setat ca numarul de operatii sa fie 2.
Threadul1 da lock la A => operatii ramase 1 (prin comanda countDown())
Threadul2 da lock la b => operatii ramase 0

Apoi ambele thread-uri incearca metoda addChild() dar vor astepta unul pentru celalalt, intrucat ambele
obiecte sunt locked