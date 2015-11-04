#include "myDisplay.h"

void curses_init()
{
	int i;
	initscr();
	cbreak();
	noecho();
	nonl();
	keypad(stdscr,TRUE);
	start_color();

	init_pair(1,COLOR_WHITE,COLOR_BLUE);
	
	// baue ein fake-LCD Display auf, gehe erst mal in die Breite

	for ( i = 0; i < lcdCount; i++ ) {
		LCD[i] = newwin(6,22, 2,2+(i*25));
		wattron(LCD[i],COLOR_PAIR(1));
		box(LCD[i],0,0);
		wrefresh(LCD[i]);
	}
}

void curses_end()
{
	endwin();
}

void OLCD_init()
{
	// Einfach mal eine Ausgabemaske erstellen 
	sprintf(OLCD[0].z[0],"*TESTAUSGABE  OLCD1*");
	sprintf(OLCD[0].z[1],"laufnummer:  LAUF   ");
	sprintf(OLCD[0].z[2],"                    ");
	sprintf(OLCD[0].z[3],"--------------------");
	OLCD_aktiv[0] = 1;
	OLCD_wohin[0] = 0;

	sprintf(OLCD[1].z[0],"*TESTAUSGABE  OLCD2*");
	sprintf(OLCD[1].z[1],"laufnummer:  LAUF   ");
	sprintf(OLCD[1].z[2],"                    ");
	sprintf(OLCD[1].z[3],"--------------------");
	OLCD_aktiv[1] = 1;
	OLCD_wohin[1] = 1;
	
}
