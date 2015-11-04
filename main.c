/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2015 ichwersonst <jgm@ichwersonst>
 * 
 * Project_Brick is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project_Brick is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ownheader.h"

int main(int argc, char **argv)
{
	createSharedMemory();
	ipcon_create(&MyIP);
    // Connect to brickd
    if(ipcon_connect(&MyIP, HOST, PORT) < 0) {
        fprintf(stderr, "Could not connect to brickd\n");
        exit(1);
    }


	getTinker();
	sleep(2);   // Es dauert etwas, bis die Enumeration der Devices durch ist, also warten !!!

	if ( tinkerCounter <= 0 ) {
		fprintf(stderr,"Kein Gerät vorhanden ");
		exit(2);
	}

	curses_init();
	showSystem(1);
	// Threads für die Anzeige erstellen
	tf1_init();
	getchar();

	curses_end();
	deleteSharedMemory();
	return (0);
}






