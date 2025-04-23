Total: 96/100
Valgrind: 19/20

## structuri:

Lista tabsList va contine un pointer dnode care reprezinta head-ul listei(santinela) si un intreg care reprezinta
lungimea maxima a listei(pentru a stii cel mai mare id al unui tab, util cand adaugam un tab nou).

dnode este o structura care contine un pointer catre un tab si doua pointere catre dnode-urile de dinainte si de
dupa acesta(next si prev).

Stivele "stack" vor contine doar un pointer node catre top-ul stivei.

node este o structura care contine un pointer catre o pagina si un pointer catre urmatorul nod din stiva.

page, tab, si browser sunt structuri construite conform cerintei.
    
## init:

initStack() asigura ca top-ul stack-ului este NULL la inceput.
initList() initializeaza un prim tab cu id 0, ii atribuie pagina implicit, ii initializeaza stivele si apoi il adauga in
lista dublu inlantuita. initBrowser() apeleaza initList si atribuie browserului lista initializata si primul tab drept tab
curent.

## push/pop:

Functii simple pentru a aduga("push()") si scoate("pop()") pagini dintr-o stiva.

## functii de eliberare a memoriei:

freeList() parcurge toata lista tabsList si elibereaza fiecare tab cu freeTab(). freeTab() apeleaza freeStack() pentru
forwardStack si backwardStack si apoi elibereaza tab-ul. freeStack() parcurge stiva si elibereaza fiecare nod din ea.

## new_tab:

new_tab() initializeaza un tab nou, ii da id-ul, ii atribuie pagina implicit, ii initializeaza stivele, creeaza un nod nou
pe care il adauga in lista care va tine un pointer catre tab-ul nou creat.

## close:

close() scoate nodul tabului dat ca parametru din tabsList si apoi elibereaza memoria tabului si a nodului scosi. Daca
id-ul tabului este 0, se va afisa eroare.

## openTab:

openTab() parcurge tabsList pana cand id-ul dat ca parametru este gasit ca id al unui tab din tabsList. tab-ul curent
devine apoi acest tab gasit in tabsList. Daca nu se gaseste tab cu acest id(se ajunge in parcurgere la head) atunci se
va afisa eroare.

## next/prev:

next()/prev() parcurge tabsList pana ajunge la tab-ul curent. Tab-ul urmator/anterior(next/prev) va deveni tab-ul curent.

## openPage:

openPage() parcurge vectorul de pagini pana gaseste id-ul paginii dat ca parametru. Adauga pagina curenta in stiva
backwards si pagina curenta va deveni pagina gasita din parcurgere. Daca nu e gasita pagina se va afisa eroare.

## backward/forward:

backward() adauga pagina curenta in stiva forward, scoate ultima pagina din stiva backward, aceasta devinind pagina curenta.
forward() adauga pagina curenta in stiva backward, scoate ultima pagina din stiva forward, aceasta devinind pagina curenta.

## print:

print() parcurge tabsList pana gaseste tab-ul curent. ii printeaza id-ul, iar apoi id-ul urmatoarelor tab-uri pana reajunge
la cel curent. La final printeaza descrierea paginii curente.

## recursivePrint:

recursivePrint() printeaza in mod recursiv URL-urile paginilor dintr-o stiva. Va fi folosit de catre printHistory pe
forwardStack() si backwardStack().

## printHistory:

printHistory() parcurge tabsList pana gaseste tab-ul cu id dat ca parametru. Printeaza apoi URL-urile paginilor din
forwardStack cu recursivePrint, URL-ul paginii curente, iar apoi URL-urile paginilor din backwardStack in ordine normala.

## main:

main() initializeaza toate variabilele necesare, deschide paginile de input si output, initializeaza pagina implicita,
apeleaza initBrowser, iar apoi citeste numarul paginilor si paginile din tema1.in, folosit un sir temporar tempStr
pentru a stii exact cata memorie trebuie sa aloce descrierii fiecarei pagini. main() citeste apoi numarul de operatii
si toate operatiile si, in functie de sirul citit cheama functia corespunzatoare (NEXT -> next(), CLOSE -> close(), etc.).
La final se inchid fisierele si se apeleaza functiile de eliberare a memoriei.
    