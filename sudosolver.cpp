/* Sudo - De ultieme SUDOKU-hacker */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
using namespace std;

const bool debug_mode = false;

class sudoku_element
{public:
	int waarde;
	int aantal_opties;
	int opties[9];
};

class sudoku
{public:
	sudoku_element vakje[9][9];
};

class opgave
{public:
	char sudostring;
	char naam;
	int mode;
};

static int gokteller = 0;
static int sudo_mode = 1; /* 0 = regulier, 1 = NRC */

sudoku init_sudoku(sudoku s)
/* initialiseer sudoku */
{
	int x,y,z;

	if (sudo_mode == 0)	{cout << "Mode: Gewone sudoku\n";}
	else {cout << "Mode: NRC sudoku\n";}
	printf("Initialiseren...\n");

	for (x=0; x<9; x++)
  	{
    	for (y=0; y<9; y++)
    	{
     		s.vakje[x][y].waarde = 0;
     		for (z=1; z<10; z++)
     		{
     	  		s.vakje[x][y].opties[z-1] = z;
     		}
     		s.vakje[x][y].aantal_opties = 9;
    	}
  	}
	return s;
}

double opties_sudoku(sudoku s)
/* bereken het aantal mogelijkheden voor sudoku */
{
	int x,y;
	double result;
	result = 1;
	for (y=0; y<9; y++)
  	{
     	for (x=0; x<9; x++)
    	{
     		result = result * s.vakje[x][y].aantal_opties;
     	}
  	}
  	return result;
}

sudoku herstel_opties_sudoku(sudoku s)
/* herstel voor alle vakjes aantal_opties */
{
	int x,y,j;

	for (y=0; y<9; y++)
  	{
     	for (x=0; x<9; x++)
    	{
     		s.vakje[x][y].aantal_opties =0;
     		for (j=0; j<9; j++)
     		{
     			if (s.vakje[x][y].opties[j]>0)
     			{s.vakje[x][y].aantal_opties++;}
     		}
        }
  	}
  	return s;
}

int vakjes_gevuld(sudoku s)
/* tel het aantal gevulde vakjes voor sudoku */
{
	int x,y,result;
	result = 0;
	for (y=0; y<9; y++)
  	{
     	for (x=0; x<9; x++)
    	{
     		if (s.vakje[x][y].waarde>0) {result++;}
     	};
  	}
  	return result;
}

bool valid_rijen_en_kolommen(sudoku s)
{
	int r,i,j,test_waarde;
	bool result = true;

	for (r=0; r<9; r++) /* voor iedere rij r*/
	{
		for (i=0; i<9; i++) /* voor ieder vakje i (op rij r) */
		{
			test_waarde = s.vakje[i][r].waarde;
			if (test_waarde > 0)
			{
				for (j=0; j<9; j++)
				{
					if (j != i)
					{
						if (s.vakje[j][r].waarde == test_waarde) {result = false;}
					}
					if (j != r)
					{
						if (s.vakje[i][j].waarde == test_waarde)  {result = false;}
					}
				}
			}

		}
	}
	return result;
}

bool valid_drie_bij_drie(sudoku s, int x, int y)
{
	int r,i,j,k;
	bool result = true;

	for (r=0; r<=2; r++) /* voor iedere rij r*/
			{
				for (i=0; i<=2; i++) /* voor ieder vakje i (op rij r) */
				{
					for (j=0; j<=2; j++) /* voor iedere poetsrij */
					{
						for (k=0; k<=2; k++) /* voor ieder poetsvak k */
						{
							if (((r != j) or ( i != k)) and (s.vakje[x+r][y+i].waarde == s.vakje[x+j][y+k].waarde)
														and (s.vakje[x+r][y+i].waarde>0)) {result = false;}
						}
					}
				}
			}
	return result;
}

bool valid_sudoku(sudoku s)
{
	int x,y;
	bool result = true;

	/* test rijen en kolommen */
	result = valid_rijen_en_kolommen(s);

	/* test reguliere 3 bij 3-en */
	for (x=0; x<=6; x=x+3)
	{
		for (y=0; y<=6; y=y+3)
		{
			if (not (valid_drie_bij_drie(s,x,y))) {result=false;}
		}
	}

	if (sudo_mode == 1)
	{
		/* Test nu de 4 NRC 3 bij 3-en */
		if (not valid_drie_bij_drie(s,1,1)) {result=false;}
		if (not valid_drie_bij_drie(s,5,1)) {result=false;}
		if (not valid_drie_bij_drie(s,1,5)) {result=false;}
		if (not valid_drie_bij_drie(s,5,5)) {result=false;}
	}

   	return result;
}

void print_sudoku(sudoku s)
/* print sudoku */
{
	int x,y;

	cout << "\n";
	for (y=0; y<9; y++)
  	{
     	for (x=0; x<9; x++)
    	{
     		cout << s.vakje[x][y].waarde;
     		cout << " ";
      	};
  		cout << "\n";
  	}
  	cout << "\n";
  	cout << "Gevuld: " << vakjes_gevuld(s)<< " - Opties: " << opties_sudoku(s) ;
  	if (valid_sudoku(s)) {cout << " - valid -";} else {cout << " - *** NOT VALID ***";}
  	cout << "\n\n";
	return;
}

void print_sudoku_lijst(sudoku s)
/* print sudokulijst */
{
	int x,y,z;
	if (debug_mode)
	{
		printf("\n");
		for (y=0; y<9; y++)
		{
			for (x=0; x<9; x++)
			{
				cout << "(" << (x+1) << "," << (y+1) << "): " << s.vakje[x][y].waarde << "    - opties: ";
				cout << s.vakje[x][y].aantal_opties << "  -  ";
				for (z=1; z<10; z++)
				{
					cout << s.vakje[x][y].opties[z-1];
				}
				cout << "\n";
			}
			printf("\n");
		}
	}
	if (valid_sudoku(s)) {cout << " - valid -\n";} else {cout << " - *** NOT VALID ***\n";}
	return;
}

sudoku lees_sudoku(sudoku s)
/* lees sudoku van het keyboard */
{
	int x,y,z, karakter;
	printf("LEZEN...\n");
	printf("\n");
	for (y=0; y<1; y++)
  	{
  		cout << "regel " << y << "\n";
     	for (x=0; x<9; x++)
    	{
    		cout << "(" << x << "," << y << "):" << s.vakje[x][y].waarde << " -> ";
     		cin >> karakter;
     		s.vakje[x][y].waarde = karakter;
     		cout << " dus (" << x << "," << y << "): " << s.vakje[x][y].waarde << "  Er zijn ";
     		cout << s.vakje[x][y].aantal_opties << " opties: ";
     		for (z=1; z<10; z++)
     		{
     	  		cout << s.vakje[x][y].opties[z-1];
     		}
     		cout << "\n";
     	};
   	}
  	printf("\n");
	return s;
}

sudoku init_sudoku_element(sudoku s,int x, int y, int getal)
/* initialiseer het vakje met getal of met een nul maar doe dat alleen als het een valid_sudoku oplevert */
{
	int i;
	sudoku oorspronkelijk = s;

	if (not valid_sudoku(s)) {cout << "BUG: PLAATSEN IN ONGELDIGE SUDOKU!!!\n"; print_sudoku(s); print_sudoku_lijst(s);}

	if (getal > 0)
	{
		s.vakje[x][y].waarde = getal;
		s.vakje[x][y].aantal_opties=1;
		for (i=0; i<9; i++)
			{
				s.vakje[x][y].opties[i]=0;
			};
		s.vakje[x][y].opties[getal-1]=getal;
	}
	else
	{
		s.vakje[x][y].waarde = 0;
		s.vakje[x][y].aantal_opties=9;
		for (i=0; i<9; i++)
			{
				s.vakje[x][y].opties[i]=i+1;
			}
	}
	if (debug_mode) cout << "(" << x+1 << "," << y+1 << "): " << getal;
	if (valid_sudoku(s))
	{
		s = herstel_opties_sudoku(s);
		if (debug_mode) {cout << " geplaatst\n";}
	}
	else
	{
		s = herstel_opties_sudoku(oorspronkelijk); /* herstel_opties is ws overbodig */
	 	if (debug_mode) {cout << " gaat niet door\n";}
	}
	herstel_opties_sudoku(s);
	return s;
}

sudoku opgave_sudoku_van_string(char waardes[])
/* return een sudoku gegenereerd uit de string waardes */
{
	sudoku s;
	int i,j,t;

	s = init_sudoku(s);
	for (i=0; i<9; i++)
	{
		for (j=0; j<9; j++)
		{
			t = (j + (i*9));
			if (isdigit(waardes[t]))
			{
				s=init_sudoku_element(s,j,i,(waardes[t]-48));
			}
			else
			{
				s=init_sudoku_element(s,j,i,0);
			}
		}
	}
	herstel_opties_sudoku(s);
    return s;
}

sudoku opgave_sudoku_nr(int nr)
{
	sudoku s;
	opgave opg[10];
	char *sudostring;

	sudostring = "..1..4.......6.3.5...9.....8.....7.3.......285...7.6..3...8...6..92......4...1...";
	sudostring = ".........7.6....................26..3.9.5.1...1..4...3..........7...6.5.8.5.9.7..";
	sudostring = "2.......7......5...9.3.....9.8...4.......4........79.......2....3.47.....6.......";
	sudostring = ".....1...6.....47....3......9.....51..........4...2.....7..96...2...........6.5..";
	s = opgave_sudoku_van_string(sudostring);

    return s;
}

sudoku veeg_rijen_en_kolommen(sudoku s)
/* pas opties en aantal_opties aan van sudoku s */
{
	int i,j,k,r,test_optie, getal,t, enige;
	bool solitair;

	for (r=0; r<9; r++) /* voor iedere rij r*/
	{
		for (i=0; i<9; i++) /* voor ieder vakje i (op rij r) */
		{
			/* haal de opties weg die niet kunnen vanwege rij of kolom */
			for (j=0; j<9; j++)
			{
				if (j != i)
				{	/* veeg de rij muv vakje zelf */
					getal = s.vakje[j][r].waarde;
					if ((s.vakje[i][r].aantal_opties >1)
					and (s.vakje[i][r].waarde == 0)
					and (s.vakje[i][r].opties[getal-1] == getal))
					{
						s.vakje[i][r].opties[getal-1] = 0;
						s.vakje[i][r].aantal_opties--;
						if (s.vakje[i][r].aantal_opties == 1)
						{
							for (t=0; t<9; t++)
							{
								if (s.vakje[i][r].opties[t] > 0) {enige=s.vakje[i][r].opties[t];}
							}
							if (debug_mode) {cout << "veeg rij  " << (r+1) << "    -> ";}
							s=init_sudoku_element(s,i,r,enige);
						}
					}
				}
				if (j != r)
				{	/* veeg de kolom muv vakje zelf*/
					getal = s.vakje[i][j].waarde;
					if ((s.vakje[i][r].aantal_opties >1)
					and (s.vakje[i][r].waarde == 0)
					and (s.vakje[i][r].opties[getal-1] == getal))
					{
						s.vakje[i][r].opties[getal-1] = 0;
						s.vakje[i][r].aantal_opties--;
						if (s.vakje[i][r].aantal_opties == 1)
						{
							for (t=0; t<9; t++)
							{
								if (s.vakje[i][r].opties[t] > 0) {enige=s.vakje[i][r].opties[t];}
							}
							if (debug_mode) {cout << "veeg kolom " << (i+1) << "   -> ";}
							s=init_sudoku_element(s,i,r,enige);
						}

					}
				}
			}

			/* Scoor nu eventuele solitaire opties in rij of kolom */
			if (s.vakje[i][r].waarde == 0)
			{
				for (k=0; k<9; k++) /* voor alle opties */
				{
					test_optie = s.vakje[i][r].opties[k];
					if (test_optie != 0)
					{
						solitair = true;
						for (j=0; j<9; j++)
						{
							if ((j !=i) and (s.vakje[j][r].opties[k] > 0)) {solitair = false;};
							if ((j !=r) and (s.vakje[i][j].opties[k] > 0)) {solitair = false;};
						}
						if (solitair == true)
						{
							if (debug_mode) {cout << "      solitair -> ";}
							s = init_sudoku_element(s,i,r,test_optie);
						}
					}
				}
			}
		}
	}
	return s;
}

sudoku veeg_drie_bij_drie(sudoku s, int x, int y)
/* veeg het 3 bij 3 blokje x,y in sudoku s */
{
	int r,i,j,p,k,t,getal,test_optie;
	bool solitair;

	for (r=0; r<=2; r++) /* voor iedere rij r*/
	{
		for (i=0; i<=2; i++) /* voor ieder vakje i (op rij r) */
		{	/* opties vegen */

			for (j=0; j<=2; j++) /* voor iedere poetsrij */
			{
				for (k=0; k<=2; k++) /* voor ieder poetsvak k */
				{
					getal = s.vakje[x+j][y+k].waarde;
					if ((getal > 0)
					and (s.vakje[x+r][y+i].aantal_opties >1)
					and (s.vakje[x+r][y+i].waarde == 0)
					and (s.vakje[x+r][y+i].opties[getal-1] == getal))
					{
						s.vakje[x+r][y+i].opties[getal-1] = 0;
						s.vakje[x+r][y+i].aantal_opties--;
						if (s.vakje[x+r][y+i].aantal_opties == 1)
						{
							for (t=0; t<9; t++)
							{
								if (s.vakje[x+r][y+i].opties[t] > 0) {getal=s.vakje[x+r][y+i].opties[t];}
							}
							if (debug_mode) {cout << "veeg 3x3 (" << (x+1) << "," << (y+1) <<   ") -> ";}
							s=init_sudoku_element(s,(x+r),(y+i),getal);
						}
					}
				}
			}

			s = herstel_opties_sudoku(s); // DIT MOET EFFICIENTER of niet. INTEGREREN IN DE VOLGENDE!! //

			/* solitaire opties scoren */
			if (s.vakje[x+r][y+i].waarde == 0)
			{
				for (k=0; k<9; k++) /* voor alle opties */
				{
					test_optie = s.vakje[x+r][y+i].opties[k];
					if (test_optie != 0)
					{
						solitair = true;
						for (j=0; j<=2; j++)
						{
							for (p=0; p<=2; p++)
							{
								if (((r != j) or (i != p)) and (s.vakje[x+j][y+p].opties[k]==test_optie)) {solitair = false;};
							}
						}
						if (solitair==true)
						{
							if (debug_mode) {cout << "      solitair -> ";}
							s=init_sudoku_element(s,(x+r),(y+i),test_optie);
						}
					}
				}
			}
		}
	}
	return s;
}

sudoku veeg_alle_drie_bij_drie(sudoku s)
/* pas opties en aantal_opties aan van sudoku s */
{
	int x,y;

	/* Veeg de 9 reguliere 3 bij 3-en */
	for (x=0; x<=6; x=x+3)
	{
		for (y=0; y<=6; y=y+3)
		{
			s=veeg_drie_bij_drie(s,x,y);
		}
	}
	if (sudo_mode == 1)
	{
		/* Veeg nu de NRC-sudoku 3 bij 3-en */
		s=veeg_drie_bij_drie(s,1,1);
		s=veeg_drie_bij_drie(s,5,1);
		s=veeg_drie_bij_drie(s,1,5);
		s=veeg_drie_bij_drie(s,5,5);
	}
	return s;
}

sudoku veeg_sudoku(sudoku s)
{
	int stand;

	if (debug_mode) cout << "VEGEN..." << "\n";

	stand = vakjes_gevuld(s)-1;
	while (vakjes_gevuld(s) > stand)
	{
		stand = vakjes_gevuld(s);
		s = veeg_rijen_en_kolommen(s);
		s = veeg_alle_drie_bij_drie(s);
	}
	return s;
}

sudoku solve_sudoku(sudoku s)
/* Geef een opgeloste sudoku terug als er een oplossing is en anders een not valid_sudoku */
{
	int r,k,j,laagste_aantal_opties,gvx,gvy;
	bool klaar = false;
	sudoku_element gokvak;
	sudoku rs;

	s = veeg_sudoku(s); /* eerst vegen */

	if ((valid_sudoku(s)) and (vakjes_gevuld(s) < 81))
	{
		if (debug_mode) cout << "\nPROBEREN..." << "\n";
		/* c=getchar(); */

		laagste_aantal_opties = 10;
		for (r=0; r<9; r++) /* in iedere rij r*/
		{
			for (k=0; k<9; k++) /* in ieder vakje k (op rij r) */
			{
				if ((s.vakje[k][r].aantal_opties < laagste_aantal_opties) and (s.vakje[k][r].aantal_opties > 1))
				{
					laagste_aantal_opties = s.vakje[k][r].aantal_opties;
					gokvak = s.vakje[k][r];
					gvx=k;
					gvy=r;
				}
			}
		}

		for (j=0; j<9; j++) /* probeer alle opties die er zijn */
		{
			if ((not klaar) and (gokvak.opties[j]>0))
			{
				if (debug_mode) cout << "\nProberen (" << gvx+1 << "," << gvy+1 << "): " << gokvak.opties[j] << "\n";
				rs = init_sudoku_element(s,gvx,gvy,gokvak.opties[j]);
				gokteller++;
				if (vakjes_gevuld(rs) > vakjes_gevuld(s))
				{
					if (debug_mode) print_sudoku(rs);
									/*	print_sudoku_lijst(rs); */
					rs = solve_sudoku(rs);
					if (((valid_sudoku(rs)) and (vakjes_gevuld(rs) == 81)))
					{
						if (debug_mode) cout << "(" << gvx+1 << "," << gvy+1 << "): " << gokvak.opties[j]<< " was een goede gok \n";
						klaar = true;
						s = rs;
					}
					else
					{
						if (debug_mode) cout << "(" << gvx+1 << "," << gvy+1 << "): " << gokvak.opties[j]<< " is nu geen oplossing \n";
						/* maak rs zeker ongeldig */
						rs.vakje[0][0].aantal_opties = 1;
						rs.vakje[0][0].waarde = 0;
					}
				}
				else if (debug_mode) {cout << "niet geldig\n";} /* en probeer nu weer de volgende optie */
			}
		}
	}
	return s;
}

int main()
{
	int c, tijd1, tijd2;
	sudoku sudo;

	sudo = opgave_sudoku_nr(1);
	print_sudoku(sudo);
	/*sudo = lees_sudoku(sudo);*/

	tijd1 = time(NULL);
	cout << "Start oplossen...\n";
	sudo = solve_sudoku(sudo);
	print_sudoku(sudo);
	if ((valid_sudoku(sudo)) and ((vakjes_gevuld(sudo)==81)))
	{cout << "\nK.I.K.\n";}
	else {cout << "\nGeen geldige oplossing gevonden.\n";}

	/* Sluit nu af */
	tijd2 = time(NULL);
	cout << "Oplostijd: " << (tijd2-tijd1) << " sec\n";
	cout << "Aantal maal gegokt: " << gokteller << "\n";
	printf("Enter q om te stoppen: ");
	while ((c=getchar()) != 113) putchar(c);
	printf("BYE!!\n");
return 99;
}
