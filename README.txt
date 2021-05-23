Timpul de rezolvare:
	Rezolvarea acestei teme mi-a luat aproximativ 3 saptamani. 

	Nu este nevoie sa fie apelat vreun argument la compilarea codului.

Metoda de rezolvare:
	Am implementat acest joc folosindu-ma de biblioteca ncurses.
	Pentru meniu am folosit 3 optiuni asa cum se poate observa: New Game, Resume Game, Quit,Quit. Acest meniu poate fi parcurs cu ajutorul sagetilor, iar optiunea dorita va fi selectata cu tasta de enter. Pentru crearea meniului am creat o fereastra imaginara careia i-am dat o forma si am integrat-o in fereastra reala.
	Daca optiunea de New Game a fost aleasa, jocul va apela fisierele atasate de unde va extrage intrabarile, raspunsurile si raspunsurile corecte. Am 5 fisiere: intrebari.txt, raspunsuri.txt, corecte.txt, rezerva.txt si re.txt (unde rezerva.txt si re.txt sunt identice). De aici, primul fisier care va fi deschis este intrebari.txt urmate de celelalte impreuna cu instructiunile necesare. 
	Daca optinuea de Quit a fost aleasa atunci se va iesi din joc si se va reveni in terminal.

Mod de joc:
	Daca se apasa pe tasta F, a fost selectata optiunea de 50/50 si doua raspunsuri gresite vor disparea. Tasta va trebui apasata doar o singura data pe intrebare.
	Daca in timpul jocului se apasa tasta de Q, se revina la meniu.
	Raspunsurile se vor alege cu sagetile si cu tasta enter. Se va afisa in stanga sus raspunsul jucatorului precum si raspunsul gresit cu scorul. 
