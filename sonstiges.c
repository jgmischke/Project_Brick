/* Ein paar generelle Funktionen für alles 
 * 
 * 
 */
#include "sonstiges.h" 

void showSystem(int MySleep)
{
	// Liste alle Daten auf LCD aus
	int i,j;


	for ( i = 0; i < tinkerCounter; i++ ) {
		for ( j = 0; j < lcdCount; j++ ) {
			sprintf(zeile[j][0],"%.20s",MyDevice[i].TinkerName);
			sprintf(zeile[j][1],"%8s %8s",MyDevice[i].TinkerShortName, MyDevice[i].TinkerUID);
			sprintf(zeile[j][2],"Connect to %8s", MyDevice[i].TinkerConnectTo);
			sprintf(zeile[j][3],"Position : -> %c", MyDevice[i].TinkerPosition);
			lprintAll();
		}
		sleep(MySleep);
	}

	for ( j = 0; j < lcdCount; j++ ) {
		// Und Zeilentext auf leer
		for ( i = 0; i < 4; i++ ) {
			 sprintf(zeile[j][i],"%20s"," ");
		}
	}
}

