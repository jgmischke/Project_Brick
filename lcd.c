/* Zum ausgeben von Text 
*/
#include "lcd.h"
#define MAX 20 

const char *Vollzeile = { "12345678901234567890" };
const char *Leerzeile = { "                    " };


void lprintClear()
{
	int i;
	if ( lcdExist == 1 ) {
		for (i = 0; i < lcdCount; i++ ) {
			lcd_20x4_clear_display(&lcd[i]);
		}
	}
	werase(LCD[0]);
}

void lprintAll( )
{   
	int i,j;

	if ( lcdExist == 1 ) {
		for (j = 0; j < lcdCount; j++ ) {
			for ( i = 0; i < 4 ; i++ ) {
				lcd_20x4_write_line(&lcd[j], i, 0 , Leerzeile);	
				lcd_20x4_write_line(&lcd[j], i, 0 , zeile[j][i]);	
				lcd_20x4_set_default_text_counter(&lcd[j], 1000*60); 
			}
		}
	}
	for (j = 0; j < lcdCount; j++ ) {	
		for ( i = 0; i < 4 ; i++ ) {
			mvwprintw(LCD[j],i+1,1,Leerzeile);
			mvwprintw(LCD[j],i+1,1,zeile[j][i]);
		}
		wrefresh(LCD[j]);	
	}
}



void strsub2(int lfdnr, char *Ziel, char *Quelle) 
{
	int l,i,j;

	l = strlen(Quelle); 	// Das muss ersetzt werden
			    	// Ersetzt wird solange : 
				// Bis Quelle zu Ende ist
	i = ( lfdnr );
	for ( j = 0; j < l; j++ )  {
		fprintf(stderr,"SUB2 LAUF = %d Z = %c  Q = %c\n",j,*(Ziel+i+j),*(Quelle+j));
		*(Ziel+j+i) = *(Quelle+j);
	}
} 

void lprintMaske()
{
	int a,b,i,j, k,l ,maxsuchbyte,x;
	char such[21];
	char ziel[21];
	char quelle[21];
	// IM Moment nur lcd1
	// gehe durch aktuelle Einträe in MyDevice
	
	// o = OLCD_aktiv[0];    // Maskennummer
	maxsuchbyte=MAX-1;
	
	for ( i = 0; i < tinkerCounter; i++ ) { 		// 1. Nimm alle verfügbaren initialisierten Namen/Werte 
		if ( MyDevice[i].TiWPCounter <= 0 ) continue;	// Für dieses Device gibt es keinen Eintrag
		
		for ( j = 0; j < MyDevice[i].TiWPCounter; j++ ) {		// 2. Gehe durch alle Einträge für dieses Device 
			sprintf(such,MyDevice[i].TiWP[j].SuchName );			// Wir suchen nach diesem Wort
			l=strlen(such);

			for ( k = 0; k < 4; k++ )  {		// Gehe jetzt alle 4 Zeilen des gewähjlten LCD Dispplay
				sprintf(ziel,OLCD[0].z[k]);

				for ( a = 0 ; a < maxsuchbyte; a++ ) {	// Ist das Suchergebnis in dieser Zeile ??
					if ( *(ziel+a) == *(such) ) {
						fprintf(stderr,"Z = %c  Q = %c\n",*(ziel+k),*(such));
						x = 0;

						for ( b = 0; b < l; b++ ) {
							fprintf(stderr,"LAUF = %d Z = %c  Q = %c\n",j,*(ziel+a+b),*(such+b));
							if ( *(ziel + a+b ) == *(such+b) ) continue;
								x=1;
							}
							if ( x == 1 ) {
								fprintf(stderr,"KEIN TREFFER Z = %s  Q = %s \n",ziel,such);
								return ;
							}
							strsub2( a,ziel,quelle);
							sprintf(OLCD[0].z[k], ziel);
							break;
						}
					}		
				}
			}
		}
}


