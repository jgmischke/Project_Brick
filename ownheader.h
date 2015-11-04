/* Beinhaltet alles was das Programm an Sachen braucht.
 * Hier sind auch alle anderen Headerdatein der Bricks drin 
 * und etliche Variablendeklarationen und so weiter.
 * 
 * */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

// Bricks 

#include "brick_master.h"
#include "brick_servo.h"
#include "bricklet_color.h"
#include "bricklet_barometer.h"
#include "bricklet_distance_us.h"
#include "bricklet_gps.h"
#include "bricklet_temperature_ir.h"
#include "bricklet_lcd_20x4.h"
#include "brick_stepper.h"
#include "ip_connection.h"
#include "bricklet_distance_ir.h"
#include "bricklet_lcd_20x4.h"
#include "brick_dc.h"
#include "bricklet_humidity.h"
#include "bricklet_dual_button.h"
#include "bricklet_laser_range_finder.h"
#include "bricklet_linear_poti.h"
#include "bricklet_multi_touch.h"

// ------------------------------------ Ein paar Defines für Grenzen etc.

#define SN  8	   // Shortname Länge = 8 Byte
#define LN  50		// Normale Namenslänge 
#define CL  20		// Länge der UID 

int createSharedMemory();
int insertSharedMemory(char *, int );
char * readSharedMemory(int , int );
int deleteSharedMemory();


// IPConnec und CO

IPConnection MyIP;

#define HOST "localhost"
#define PORT 4223
int tinkerCounter;   // Anzahl gefundene Geräte 

// Definitionen für die Bricks etc.

struct Anzeige {
	char Name[21];
	char SuchName[10];
	char Wert[21];
	// Für die LCD Anzeige
};

struct MyD {
	int  TinkerDevIDNr;				// Das ist die Nummer des Device Identifiers, KEINE laufende NR.!!!
	char TinkerName[LN];
	char TinkerShortName[SN];
	char TinkerUID[CL];			// Das ist die UID 
	char TinkerPosition;
	char TinkerConnectTo[CL];
	Device TinkerDevice;
	char TinkerActiv;           // Aktivierung automatisch. Erfolg = 1   Kein Erfolg = 0 
	int TinkerMemPos;			// Aktuelle Position in shared Mem (0-320)
	struct Anzeige TiWP[20];	// Maximale 20 Werte zur Anzeige sollten erst mal reichen 	
	int TiWPCounter;
};

struct MyD MyDevice[50];


// Funktionen 

int getTinker();
int setTinkerName(uint8_t );
int activateTinker();
Device getMyDevice(char *, int) ;
void lprint(char *, int );
void lprintClear();
void showSystem(int );
void lprintAll( ); 
// Automatisch benannte Devices
Device lcd[5];   // Unterstützt bis zu 5 LCD Anzeigen
WINDOW *LCD[5];  // Fake LCD 
int lcdExist;
int lcdCount; 
char zeile[5][4][21];     // Maximal 5 LCDS 
char *MText;

// INITBRICKS			
void initCOLB(Device);
void initBARO (Device);
void initDISR (Device);
void initDISU (Device);
void initHUMI (Device);
void initLCD (Device);
void initTEIR (Device);
void initGPS(Device);

FILE *fp;


// Als thread Ausgabezaehler 
long MythreadCounter;

// Für die 20x4 Anzeige gibt es jetzt ein Modell, in dem die Variablen durch reale Werte ersetzt werden.
struct OutLCD {
	char z[4][21];
} OLCD[3];   // Erst mal 3 Ausgabemodule	

int OLCD_aktiv[3];     // Welche Maske wird genutzt  aktiv=1 
int OLCD_wohin[3];		// auf welchen LCD Schirm. Maximal im Moment 3 


