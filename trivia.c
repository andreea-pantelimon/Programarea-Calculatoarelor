#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define BUFFMAX 1005

//functie pentru afisarea timpului curent
void current_time(int i)
{
    time_t tsec;
    struct tm* x;
    tsec=time(NULL);
    x=localtime(&tsec);
    mvprintw(1, 40, "Current time: %s", asctime(x));
} 

void new_game(char question[], char* choices[], char* right[]);

//functie pentru printarea meniului
int menu()
{
	char** answers, question[BUFFMAX]; 
	char** right;
	int score, i, nr, corecte, gresite;
	answers = (char**)malloc(4 * sizeof(char*)); //intentionez sa memorez doar cate 4 raspunsuri pe rand
	for(int i = 0; i < 4; i++)
		answers[i] = malloc(BUFFMAX* sizeof(char)); //aloc memorie pentru fiecare element / linie care se va salva
	right = (char**)malloc(1 * sizeof(char*)); //intetionez sa memorez doar cate un raspuns pe rand
	for(int j = 0; j < 1; j++)
		right[j] = malloc(BUFFMAX * sizeof(char)); //aloc memorie pentruelement / linia care se va salva

	//facem marimea ferestrei
	int ymax, xmax;
	getmaxyx(stdscr, ymax, xmax);

	//cream o fereastra pentru inputul nostru
	WINDOW * menu = newwin(6, xmax - 12, ymax - 8, 5);
	box(menu, 0, 0);
	refresh(); //ce am realizat pe fereastra imaginara punem pe cea reala
	wrefresh(menu);

	//denumirea jocului se va afla deasupra casutei cu meniul
	char game_name[]="TRIVIA";
	mvprintw(7, 36, "%s", game_name);
	refresh();

	//pentru folosirea sagetilor
	keypad(menu, TRUE);

	//variante ale meniului
	char *choices[] = {"New game", "Resume game", "Quit"};
	int choice;
	int highlight = 0;

	//folosirea sagetilor pentru parcurgere
	while(1)
	{
		for(int i = 0; i < 3; i++)
		{
			if(i == highlight)
				wattron(menu, A_REVERSE); //imi arata zona cu highlight / luminata
			mvwprintw(menu, i + 1, 1, choices[i]);
			wattroff(menu, A_REVERSE);
		}
		choice = wgetch(menu);

		switch(choice)
		{
			case KEY_UP:
				highlight--;
				if(highlight == -1) //ma asigur ca sageata nu merge mai sus decat trebuie
					highlight = 0;
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 3) // ma asigur ca sageata nu merge mai jos decta trebuie
					highlight = 2;
				break;
			default:
			break;
		}
		if(choice == 10) //daca se va face cumva o alta selectie inafara de sageti sa nu fie acceptate
			break;
	}

	//am sa iau fiecare caz in parte
	if(choices[highlight] == "New game")
	{
		clear();
		refresh();
		mvprintw(10, 20, "Jocul va incepe in curand, pregateste-te");
		mvprintw(11, 20, "Apasa F pentru optiunea de 50/50"); //un mic mesaj inainte de inceperea efectiva a jocului
		refresh();
		sleep(2);
		clear();
		refresh();

		new_game(question, answers, right); //se va incepe noul joc

	}
	else if(choices[highlight] == "Resume game")
		refresh();
	else if(choices[highlight] == "Quit")
		{
			endwin();
			return 0;
		}
	//ne asiguram ca programul asteapta introducerea utilizatorului + oprim ncurses
	getch();
	endwin();

	//eliberez memoria alocata
	free(answers);
	free(right);
}	
int main()
{
	//Ncurses start
	initscr();
	noecho();
	cbreak();

	//cursorul va disparea de pe ecran
	curs_set(0);

	menu();

	return 0;
}

//functie pentru crearea variantei de new menu
void new_game(char question[], char* choices[], char* right[])
{
	//deschid fisierele necesare
	FILE * fq = fopen("intrebari.txt", "r");
	FILE * fa = fopen("raspunsuri.txt", "r");
	FILE * fr = fopen("corecte.txt", "r");
	FILE * fc = fopen("re.txt", "r");
	FILE * fd = fopen("rezerva.txt", "r");
	int i, count, highlight = 0, choice, j, kount, score = 0, corecte = 0, gresite = 0;

	//parcurg fisierul de intrebari
	while(fgets(question, BUFFMAX, fq) != NULL)
	{
		count = 4;
		i = 0;
		current_time(i); //afisez data curenta

		mvprintw(9, 28, "%s", question);

		//ma folosesc de variabila count ca sa iau strict 4 raspunsuri din fisier
		while(count)
		{
			fgets(choices[i], BUFFMAX, fa);
			count--;
			i++;

			if(count == 0)
			{
				//creez o noua fereastra pentru plasarea raspunsurilor in aceasta
				//se va proceda exact la fel ca la meniu
				int ymax, xmax;
				getmaxyx(stdscr, ymax, xmax);

				WINDOW * game = newwin(6, xmax - 12, ymax - 8, 5);
				box(game, 0, 0);
				refresh();
				wrefresh(game);

				keypad(game, TRUE);

				//doresc sa parcurg raspunsurile cu sagetile prin urmare procedez exact la fel ca la meniu
				while(1)
				{
					if(choice == 'q' || choice == 'Q')
					{
						endwin();
						clear();
						refresh();
						menu();
						endwin();
					}
					//optiunea de fifty fifty
					if(choice == 'f' || choice == 'F')
					{
						int ch = fgetc(fd); //iau raspunsul corect de tip int din acest fisier
						if(ch != 10) //doresc sa evit caracterul '\n' din fisier
						{
							if(ch == 65) //daca raspunsul este A se vor inlocui B si D cu -
							{
								choices[1] = strcpy(choices[1], "-");
								choices[3] = strcpy(choices[3], "-");
							}
							if(ch == 66) //daca raspunsul este B se vor inlocui A si C cu -
							{
								choices[0] = strcpy(choices[0], "-");
								choices[2] = strcpy(choices[2], "-");
							}
							if(ch == 67) //daca raspunsul este C se vor inlocui A si D cu -
							{
								choices[0] = strcpy(choices[0], "-");
								choices[3] = strcpy(choices[3], "-");
							}
							if(ch == 68) //daca raspunsul este D se vor inlocui C si B cu -
							{
								choices[2] = strcpy(choices[2], "-");
								choices[1] = strcpy(choices[1], "-");
							}
						}
						else
						{
							ch = fgetc(fd); //se va sari peste caracterul '\n'
							if(ch == 65) //daca raspunsul este A se vor inlocui B si D cu -
							{
								choices[1] = strcpy(choices[1], "-");
								choices[3] = strcpy(choices[3], "-");
							}
							if(ch == 66) //daca raspunsul este B se vor inlocui A si C cu -
							{
								choices[0] = strcpy(choices[0], "-");
								choices[2] = strcpy(choices[2], "-");
							}
							if(ch == 67) //daca raspunsul este C se vor inlocui A si D cu -
							{
								choices[0] = strcpy(choices[0], "-");
								choices[3] = strcpy(choices[3], "-");
							}
							if(ch == 68) //daca raspunsul este D se vor inlocui C si B cu -
							{
								choices[2] = strcpy(choices[2], "-");
								choices[1] = strcpy(choices[1], "-");
							}
						}
					}

					for(int i = 0; i < 4; i++)
					{
						if(i == highlight)
							wattron(game, A_REVERSE);
						mvwprintw(game, i + 1, 1, choices[i]);
						wattroff(game, A_REVERSE);
					}
					choice = wgetch(game);

					switch(choice)
					{
						case KEY_UP:
							highlight--;
							if(highlight == -1)
								highlight = 0;
							break;
						case KEY_DOWN:
							highlight++;
							if(highlight == 4)
								highlight = 3;
							break;
						default:
							break;
					}
					if(choice == 10)
							break;
				}
				//clear();
				refresh();
				mvprintw(2, 3,"Raspunsul tau: %s\n", choices[highlight]); //printez raspunsul ales de jucator
				//getch();
			}
		}

		//parcurg fisierul corecte.txt ca sa extrag raspunsul corect
		//ma folosesc de variabila kount ca sa iau cate o linie pe rand
		j = 0;
		kount = 1;
		while(kount)
		{
			fgets(right[j], BUFFMAX, fr);
			mvprintw(3, 3, "Raspuns corect: %s", right[j]);
			kount--;
			j++;
		}

		//deschid ultimul fisier si transform caracterele in int-uri
		//dupa aceea fac comparatia cu alegerea jucatorului pentru a adauga punctaj la scor
		int c = fgetc(fc);
		int line;
		if(c == 65) //pentru varianta A
		{
			line = 0;
		}
		if(c == 66) // pentru varianta B
		{
			line = 1;
		}
		if(c == 67) //pentru varianta C
		{
			line = 2;
		}
		if(c == 68) //pentru varianta D
		{
			line = 3;
		}

		c = fgetc(fc); //doresc sa trec peste caracterul '\n' care are valoarea 10 in coduri ASCII

		if(line == highlight) //fac comparatia asa cum am precizat mai sus
		{
			score = score + 10;
			corecte++;
		}
		else
		{
			score = score - 5;
			gresite++;
		}
		mvprintw(4, 3, "SCORE: %d", score);
		refresh(); //ecranul va astepta dupa care se va sterge continutul si se va afisa urmatoarea intrebare
		sleep(2);
		clear();
		refresh();
	}
	mvprintw(4, 27, "FINAL SCORE: %d", score);
	mvprintw(5, 27, "RASPUNSURI CORECTE: %d", corecte);
	mvprintw(6, 27, "RASPUNSURI INCORECTE: %d", gresite);
	mvprintw(7, 27, "APASA ENTER SA IESI");
	fclose(fq);
}

