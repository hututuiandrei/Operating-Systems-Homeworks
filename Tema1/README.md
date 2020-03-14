Nume : Hututui Andrei-Dan
Grupa : 336CC

# Tema 1 Multi-platform Development

Organizare

1. Explicatie : 
Solutia aleasa pentru aceasta tema consta in verificarea 
in prima faza a argumentelor programului, pentru a 
determina daca flag-urile primite sunt corespunzatoare (-D, -I, -o),
daca fisierul de input exista si poate fi deschis, si daca 
numarul de fisiere primite este maxim 2 (input si output).

Elementele de tip cheie valoare din flagul -D vor fi inserate
intr-o structura de date de tip hashmap, aceasta avand functionalitati
de creare, inserare, verificare, stergere, eliberare memorie.

Dupa ce am verificat argumentele vom citi linie cu line din
fisierul de input sau de la stdin. Tratam fiecare directiva
intalnita in parte in functie de numele acesteia : 
- #define : adaugam o noua inregistrare in hashmap
- #undef : stergem inregistrare din hashmap
- #if <COND> : citim urmatoarea linie doar daca, COND e adevarat
- #else : citim urmatoarea linie doar daca, conditia anterioare
a fost una falsa
- #include : apelam recursiv functia preprocess pentru a putea
aplica modificarile aduse de directive in fiecare fisier inclus
- etc.

Modificarile aduse sunt scrise in fisierul de output sau la stdout.

2. Utilitate : 
Consider ca tema este utila, deoarece poti astfel intelege cat de
dificila poate fi o parte mica din responsabilitatea unui preprocesor
	
3. Implementare :
Consider implementarea mea ca fiind una naiva, dar in acelasi timp
destul de simpla si eficienta (mai putin apelurile recursive ce ar 
putea duce rapid la incarcarea intregii stive in cazul unor teste
foarte complexe), deci este evident ca se putea mai bine, dar scopul
temei consider ca a fost atins.

Intregul enunt al temei a fost implementat.

4. Compilare si rulare :

Linux :

make -f GNUmakefile
./so-cpp [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] [ [-o] <OUTFILE>]

Windows :

make -f makefile
.\so-cpp.exe [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] 
[ [-o] <OUTFILE>]

5. Bibliografie :

http://www.kaushikbaruah.com/posts/data-structure-in-c-hashmap/
https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/


 
