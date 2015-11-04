#include "ownheader.h"
#include "getTinker.h"

void getTinker_enumerate(const char *uid, const char *connected_uid,
                  char position, uint8_t hardware_version[3],
                  uint8_t firmware_version[3], uint16_t device_identifier,
                  uint8_t enumeration_type, void *user_data) 
{
    (void)user_data;
	
	if(enumeration_type != IPCON_ENUMERATION_TYPE_AVAILABLE) {
        // Nicht vom Programm angefordert, ignorieren ;
        return;
    }
	//mvwprintw(LCD,1,1,"TC = %d      ",tinkerCounter); wrefresh(LCD); // sleep( 1) ;
	// In die Struktur eintragen
	MyDevice[tinkerCounter].TinkerDevIDNr = device_identifier;
	strcpy(MyDevice[tinkerCounter].TinkerUID,uid);
	MyDevice[tinkerCounter].TinkerPosition = position;
	strcpy(MyDevice[tinkerCounter].TinkerConnectTo,connected_uid);
	setTinkerName(device_identifier);	  
	//mvwprintw(LCD,4,1,"ENDE:%d **%s**",tinkerCounter,MyDevice[tinkerCounter].TinkerShortName); 	wrefresh(LCD); // sleep(1);
	tinkerCounter++;
}

int getTinker()
{
	// printf("getTinker OK\n");
	// mvwprintw(LCD,1,7,"get"); 	wrefresh(LCD); sleep(3);
	MText = malloc(128);   // Einmal für alle Sharedmemeinträge aufbauen 
	tinkerCounter = 0;
	ipcon_register_callback(&MyIP,IPCON_CALLBACK_ENUMERATE, getTinker_enumerate, NULL);
	// Trigger enumerate
    ipcon_enumerate(&MyIP);
	// Callback wieder entfernen. Bei Aufruf von brickv wird sonst das Teil wieder aufgerufen
	// ipcon_register_callback(&MyIP,IPCON_CALLBACK_ENUMERATE,NULL, NULL);
	return 0;
}

int setTinkerName(uint8_t di)
{
	int i,j,n; 
	int l = sizeof(MyDevID);    // Länge aller IDs der TI Devices ---
	char SearchName[SN];			// Genausolang wie in den Strukturen 
	
	for ( i = 0; i < l ; i++ ) {
		if ( di == MyDevID[i].IDNummer ) {
			// Ja, ID gibt es . trage Alle Werte ein, die möglich sind.
			// Schaue zuerst nach, ob und wieviele gleiche Bricks es eventuell gibt.
			n=0;
			strcpy(SearchName, MyDevID[i].DevShortName);
			for ( j = 0; j < tinkerCounter; j++ ) {
					if ( strncmp( SearchName, MyDevice[j].TinkerShortName ,4 ) == 0 ) {
							// Ja, es gibt das Device schon mal
							n++;
							// fprintf(stderr,"BEEP -> DOPPELT\n");
					}
			}
			MyDevice[tinkerCounter].TinkerMemPos = 0;
			strcpy(MyDevice[tinkerCounter].TinkerName,MyDevID[i].DevName);
			if ( n > 0 ) sprintf(MyDevice[tinkerCounter].TinkerShortName,"%s%d",MyDevID[i].DevShortName,n);   // Mehrfach device 
			else strcpy(MyDevice[tinkerCounter].TinkerShortName,MyDevID[i].DevShortName);
			// fprintf(stderr,"Dev-ID = \t%d -->  \t%30s \t%8s\n",di,MyDevice[tinkerCounter].TinkerName, MyDevice[tinkerCounter].TinkerShortName);
			// Brick aktivieren 
			//mvwprintw(LCD,2,1,"JETZT: **%s**",MyDevice[tinkerCounter].TinkerShortName); 	wrefresh(LCD); sleep(1);
			activateTinker();   // Wird nur hier gemacht, alle Werte sind global verfügbar
			return 0; 
		}
	}
	return -1;
}

int activateTinker()
{
	// Gesucht wird jetzt nach dem Device Identifier Wert laut Liste in getTinker.h
	// ist für die switchAnweisung am einfachsten 
	// Erst mal nur die Bricks, die ich habe 
	// Jetzt je nach Typ das Modul aktivieren, es wird angenommen, das die create Anweisung immer was zurückgibt.
	// Zudem kann hier auch gleich eine Grundinitialiserung vorgenommen werden, je nach Typ halt 
	// Zu jedem Brick gibt es auch gleich alle wichtigen Grundfunktionen, die dann individuell je nach Programm
	// angepasst werden können. Für jedes Brick/let sind immer alle Funktionen im Initteil mit drin.
	// Generell für jedes Teil:
	// 1.) create Anweisung für das Device
	// 2.) Initteil mit Standardsachen die gemacht werden müssen. ( Backlight an o.a. )
	// 3.) Typische Einstellungen der Callbacks etc.
	// 4.) Namenskonvention: init(BRICK) : initLCD(&Device) 
	int i;

	MyDevice[tinkerCounter].TinkerActiv = 0;      // Falls was nicht klappt !
	i = MyDevice[tinkerCounter].TinkerDevIDNr;
	
	switch( i ) {
		case 11 :	   // DC
			dc_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
       		break;
	    case 13 :	   // Master
			master_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			break;
		case 14 :	   // Servo
			servo_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			break;
		case 15 :	   // Stepper
			stepper_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			break;			
		case 25 :	   // Distance IR
			distance_ir_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initDISR(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 27:
			humidity_create (&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initHUMI(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 212 :	   // LCD 20x4     AUSNAHEMBEHANDLUNG, hier gleich alles anwerfen 
			// Unterstützt bis zu 5 LCD Displays . lcdCount zählt durch
			lcd_20x4_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initLCD(MyDevice[tinkerCounter].TinkerDevice);
			lcd[lcdCount++] = MyDevice[tinkerCounter].TinkerDevice; 
			lcdExist = 1;
			// Noch keine Abprüfung für mehr als 5 LCDs...
			break;
		case 217 :	  // Temp. IR 
			temperature_ir_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initTEIR(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 221 :	  // Barometer 
			barometer_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initBARO(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 222 :	  // GPS
			gps_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initGPS(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 229 :	  // Distance US
			distance_us_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initDISU(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 243 :	  // Color 
			color_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initCOLB(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 213 :	  // POTI
			linear_poti_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initPOTI(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 230 :	  // dual bUTTON
			dual_button_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initDUAL(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 255 :	  // LASR
			laser_range_finder_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initLASR(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 234 :	  // Multitouch (12 )
			multi_touch_create(	&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initMULT(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 250 : 		// Acceleraor ACCL
			accelerometer_create(&MyDevice[tinkerCounter].TinkerDevice, MyDevice[tinkerCounter].TinkerUID, &MyIP);
			initACCEL(MyDevice[tinkerCounter].TinkerDevice);
			break;
		case 
		// To be continued ....
	}   
	
 	MyDevice[tinkerCounter].TinkerActiv = 1;
	return 0;
}

Device getMyDevice(char *DevSuchName, int Nummer ) 
{
	// Gibt das entsprechende Device zurück
	// SuchName ist immer 4 Byte
	// Wenn Nummer nicht 0, dann das andere Gerät
	// Also etwa 2*DISU als DISU1/DISU0 oder so
	int i;

	if ( Nummer > 0 ) {
		for ( i = 0; i < tinkerCounter; i++ ) {
			if ( strncmp(DevSuchName,MyDevice[i].TinkerShortName,5 ) == 0 ) {
				return MyDevice[i].TinkerDevice;
			}
		}
	}
	
	for ( i = 0; i < tinkerCounter; i++ ) {
		if ( strncmp(DevSuchName,MyDevice[i].TinkerShortName,4 ) == 0 ) {
			return MyDevice[i].TinkerDevice;
		}
	}
	// Sollte nie passieren 
	return MyDevice[i].TinkerDevice ;
}

int getMyDeviceNumber(char *DevSuchName, int Nummer ) 
{
	int i;

	for ( i = 0; i < tinkerCounter; i++ ) {
		if ( strncmp(DevSuchName,MyDevice[i].TinkerShortName,4 ) == 0 ) {
			return i;
		}
	}
	// Sollte nie passieren 
	return -1;
}









