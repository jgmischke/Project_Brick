#include "mtimer.h"
// Thread geht durch alle LCDS durch 

void *t1f()
{   
	int i;
	static int balk=8;
	while ( 1 ) {
		sleep(1);
		balk++;
		if ( balk > 15 ) balk=8;
		for ( i = 0; i < lcdCount; i++ ) {
			zeile[i][3][19] = balk;
		}
		lprintAll();
	}
}

void tf1_init()
{
	int erg;
	erg = pthread_create(&T1, NULL, &t1f, NULL);
	return;
}

