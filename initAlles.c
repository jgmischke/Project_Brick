#include "initAlles.h"
// Initialisierungsteil für ALLE Bricks. Hinweise siege getTinker.c
// initBRICK als Funktionsname
// Alles wird ohne zusätzliche Logik erst mal angestellt und per default soweit geht vorbelegt bzw. aktiviert 
// Jeweils eine ganze Gruppe von Funktionen für jedes Brick 

// Ein paar temporäre Devices für Funktionen, die keinen Callback haben.
// Etwa Barometer - Temperatatur

static Device BARO_TMP;
static Device TEIR_TEMP;


// ---------- LCD ------------------------------
// Standardfunktionen 
//
// 		// KNOPF 1 - 4

/*
void LCD_Press( uint8_t button, void *data)
{
	LCD_AktionP( button ) ;
	return;
}

void LCD_Release( uint8_t button, void *data)
{
	LCD_AktionR( button ) ;
	return;
}

void LCD_AktionP( uint8_t button )
{
	switch (button) {
		case 0 :
			LCD_AktionP0();
			break;
		case 1 :
			LCD_AktionP1();
			break;
		case 2 :
			LCD_AktionP2();
			break;
		case 3 :
			LCD_AktionP3();
			break;
	}
}

void LCD_AktionR( uint8_t button )
{
	switch (button) {
		case 0 :
			LCD_AktionR0();
			break;
		case 1 :
			LCD_AktionR1();
			break;
		case 2 :
			LCD_AktionR2();
			break;
		case 3 :
			LCD_AktionR3();
			break;
	}
}

void LCD_AktionP0()
{
	static int LAP;
	return;
}

void LCD_AktionP1()
{
	static int LAP;
	return;
}

void LCD_AktionP2()
{
	static int LAP;
	return;
}

void LCD_AktionP3()
{
	static int LAP;
	return;
}


void LCD_AktionR0()
{
	static int LAR;
	return;
}

void LCD_AktionR1()
{
	static int LAR;	
	return;
}

void LCD_AktionR2()
{
	static int LAR;
	return;
}

void LCD_AktionR3()
{
	static int LAR;
	return;
}
*/

void initLCD(Device MD)
{
	lcd_20x4_backlight_on(&MD);
	lcd_20x4_clear_display(&MD);
	lcd_20x4_set_config(&MD,false,false);   // CURSOR AUS 
	LCD_Einstellen(MD);
}

void LCD_Einstellen(Device MD)
{
	int i,j;
	// Beispiel : Durcklaufender Balken von oben nach unten 
	uint8_t c[8][8] = {  
		{ 31,31,31,0,0,0,0,0 } ,  // Char 1 ( 0x8 
		{ 0,31,31,31,0,0,0,0 } ,  // Char 2 ( 0x8 
		{ 0,0,31,31,31,0,0,0 } ,  // Char 3 ( 0x8 
		{ 0,0,0,31,31,31,0,0 } ,  // Char 4 ( 0x8 
		{ 0,0,0,0,31,31,31,0 } ,  // Char 5 ( 0x8 
		{ 0,0,0,0,0,31,31,31 } ,  // Char 6 ( 0x8 
		{ 31,0,0,0,0,0,31,31 } ,  // Char 7 ( 0x8 
		{ 31,31,0,0,0,0,0,31 }   // Char 8 ( 0x8 
	} ; 		
	// Stelle jetzt alles ein, was geht. 
	//lcd_20x4_register_callback(&MD, LCD_20X4_CALLBACK_BUTTON_PRESSED, LCD_Press, NULL);
	// lcd_20x4_register_callback(&MD, LCD_20X4_CALLBACK_BUTTON_RELEASED, LCD_Press, NULL);
	lcd_20x4_set_default_text(&MD, 0 , "LCD Timeout     --->");
	lcd_20x4_set_default_text(&MD, 1 , "Nichts mehr los --->");
	lcd_20x4_set_default_text(&MD, 2 , "Kuck jetzt      --->");
	lcd_20x4_set_default_text(&MD, 3 , "dumm rum        ---->");
	// Nachricht nach 1 Minute absetzen. Counter läuft auf der LCD selber.
	lcd_20x4_set_default_text_counter(&MD, 1000*60); 

	// Auf Wunsch sonderzeichen
	for ( i = 0; i < 8; i++ ) {
		lcd_20x4_set_custom_character(&MD, i, c[i]);
	}
	
}

// -------------- LCD ENDE -----------------------------------------


// -------------- DISTANCE IR ---- ZEILE 0 ----------------------------------

void DISR_distanz( uint16_t dis, void *userdata) 
{
	int16_t temp;
	// fprintf(stderr,"-----> IR Distanz = %.8d\n",dis);
	// HIER ANPASSEN - Erst mal nur eine lokale Variable
	barometer_get_chip_temperature(&BARO_TMP, &temp );
	sprintf(MText,"%d %90s %.6d",(int) time(NULL),"Position von IR",dis);
}

void initDISR(Device MD) 
{
	DISR_Einstellen (MD);
}

void DISR_Einstellen( Device MD)
{
	distance_ir_register_callback(&MD, DISTANCE_IR_CALLBACK_DISTANCE, DISR_distanz, NULL);
	distance_ir_set_distance_callback_period(&MD, 1000);   // ERST MAL 0
}

// ------------- DISTANCE IR   ENDE ----------------------------------

// -------------  BAROMETER -----ZEILE 1-------------------------------------
// CALLBACKS
int baroWert;
void BARO_Luftdruck(int32_t luftdruck, void *data)
{
	int16_t ChipTemp;
	// Zusätzlich noch die Chiptemperatur 
	barometer_get_chip_temperature(&BARO_TMP ,&ChipTemp);
	sprintf(MText,"%d %90s %.6d",(int) time(NULL),"Luftdruck BARO : ",luftdruck);
	sprintf(zeile[0][1],"Luftdruck: %8.2f",(float) luftdruck/100.0 );
	sprintf(zeile[0][2],"ChipTmp  : %8.2f",(float) ChipTemp/100.0);
}

void BARO_Hoehe(int32_t hoehe, void *data)
{
	sprintf(MText,"%d %90s %.6d",(int) time(NULL),"Luftdruck BARO : ",hoehe);
	sprintf(zeile[0][3],"Hoehe(cm): %8.d",hoehe);
}

void initBARO( Device MD ) 
{
	BARO_TMP = MD;
	BARO_Einstellen(MD);
}

void BARO_Einstellen(Device MD)
{
  	int t = tinkerCounter;
	baroWert = t;
	// Einer für alles
	
	barometer_register_callback(&MD ,BAROMETER_CALLBACK_AIR_PRESSURE, BARO_Luftdruck, NULL );
	barometer_set_air_pressure_callback_period(&MD , 1000 );
	barometer_register_callback(&MD , BAROMETER_CALLBACK_ALTITUDE, BARO_Hoehe, NULL );
	barometer_set_altitude_callback_period(&MD , 1000 );
	
	sprintf(MyDevice[t].TiWP[0].SuchName,"BAR");		
	sprintf(MyDevice[t].TiWP[0].Name,"Status ");
	sprintf(MyDevice[t].TiWP[1].SuchName,"GPSST");		
	sprintf(MyDevice[t].TiWP[1].Name,"Status ");
	sprintf(MyDevice[t].TiWP[2].SuchName,"GPSST");		
	sprintf(MyDevice[t].TiWP[2].Name,"Status ");
	MyDevice[t].TiWPCounter = 3;
}

// -------------- BAROMETER ENDE -------------------------------------

// Farbtemp, Helligkeit und Farbtemp.
// --------------- COLOR -----ZEILE 2 ----------------------------------------
// CALLBACKS


int colorWert;
void COLB_Farben(uint16_t r, uint16_t g, uint16_t b, uint16_t c, void *user_data)
{
	uint32_t illu;
	uint16_t farbt;

	color_get_color_temperature(&MyDevice[colorWert].TinkerDevice, &farbt);
 	color_get_illuminance(&MyDevice[colorWert].TinkerDevice, &illu);
	sprintf(MyDevice[colorWert].TiWP[0].Wert,"%d",r);
	sprintf(MyDevice[colorWert].TiWP[1].Wert,"%d",illu);
	sprintf(MyDevice[colorWert].TiWP[2].Wert,"%d",farbt);
}

void initCOLB( Device MD )
{
	COLB_Einstellen(MD);
}

void COLB_Einstellen( Device MD ) 
{
	int t = tinkerCounter;
	colorWert=t;
		
	color_register_callback(&MD, COLOR_CALLBACK_COLOR , COLB_Farben, NULL);
	color_set_color_callback_period(&MD ,5000);

	sprintf(MyDevice[t].TiWP[0].SuchName,"COLORA");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Alle 3 Farben");
	sprintf(MyDevice[t].TiWP[1].SuchName,"COLORHGPSST");		// Status : 
	sprintf(MyDevice[t].TiWP[1].Name,"Helligkeit in Lux");
	sprintf(MyDevice[t].TiWP[2].SuchName,"COLORFT");		// Status : 
	sprintf(MyDevice[t].TiWP[2].Name,"Farbtemperatur");
	MyDevice[t].TiWPCounter = 3;
}
// --------------- COLB ENDE -------------------------------

// ---------------- DIS US - DISU --------------------------
// Entfernung 
// CALLBACK 

int disuWert;
void DISU_distanz(uint16_t distance, void *data)
{
   		sprintf(MyDevice[disuWert].TiWP[0].Wert,"%d",distance);
}

void initDISU( Device MD )
{
	DISU_Einstellungen(MD);
}

void DISU_Einstellungen( Device MD ) 
{
	int t = tinkerCounter;
	disuWert = t;
	distance_us_register_callback(&MD, DISTANCE_US_CALLBACK_DISTANCE, DISU_distanz, NULL);
	distance_us_set_distance_callback_period(&MD , 10000);

	sprintf(MyDevice[t].TiWP[0].SuchName,"DISU");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Entfer. US");
	MyDevice[t].TiWPCounter = 1;
}

// ---------------- DISU ENDE ------------------------------

// ---------------- HUMI ----------------------------------
// Luftfeuchtigkeit 
// CALLBACK
int humiWert;
void HUMI_lf( uint16_t p, void *data )
{
//	(void) data;
//	fprintf(stderr,"A=%d",*(int*)data);
	// P -> 100% = 1000 
	sprintf(MyDevice[humiWert].TiWP[0].Wert,"%d",p);
	
}

void initHUMI (Device MD ) 
{
	HUMI_Einstellungen(MD);
}

void HUMI_Einstellungen( Device MD ) 
{   
	int t = tinkerCounter;
	humiWert=t;
	humidity_register_callback(&MD , HUMIDITY_CALLBACK_HUMIDITY, HUMI_lf, NULL);
	humidity_set_humidity_callback_period(&MD, 1100 );

	sprintf(MyDevice[t].TiWP[0].SuchName,"HUMI");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Luftfeuchte");

	MyDevice[t].TiWPCounter = 1;	
}


// ----------------- HUMI Ende ------------------------

// ------------------ TEIR --------------------------
// IR-Temp objekt und Umgebung

int irtempWert;
void TEIR_Daten(int16_t objTemp, void *data)
{	
	int16_t umTemp;
	// Hole auch gleich die 2. Temperatur ( Umgebung)
	temperature_ir_get_ambient_temperature(&TEIR_TEMP , &umTemp );
	sprintf(MyDevice[irtempWert].TiWP[0].Wert,"%d",umTemp);
	sprintf(MyDevice[irtempWert].TiWP[1].Wert,"%d",objTemp);
	
}

void initTEIR( Device MD )
{
	TEIR_TEMP = MD;
	TEIR_Einstellungen(MD); 
}

void TEIR_Einstellungen( Device MD ) 
{
	int t = tinkerCounter;
	irtempWert = t;
	
	temperature_ir_register_callback(&MD, TEMPERATURE_IR_CALLBACK_OBJECT_TEMPERATURE, TEIR_Daten, NULL);
	temperature_ir_set_object_temperature_callback_period(&MD, 0);

	sprintf(MyDevice[t].TiWP[0].SuchName,"IRATMP");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Umgebungstemp.");
	sprintf(MyDevice[t].TiWP[1].SuchName,"IROTMP");		// Status : 
	sprintf(MyDevice[t].TiWP[1].Name,"Objekttemp.");
	MyDevice[t].TiWPCounter = 2;	
}


// ---------------- TEIR ENDE -----------------


// --------------------- GPS -------------------------------


static int gpsWert;

void GPS_Daten( uint32_t gdate , uint32_t gtime, void *user_data)
{
	uint32_t wert1, wert2;
	uint16_t leer1,leer2,leer3, abweich;
	uint8_t fix, vsat, usat; 

	gps_get_status( &MyDevice[gpsWert].TinkerDevice ,&fix ,&vsat ,&usat ); 
	// Von hier werden alle anderen Daten geholt, wenn der Status ok ist
	// Fix?
	if ( fix < GPS_FIX_3D_FIX ) {
		sprintf(MyDevice[gpsWert].TiWP[0].Wert,"%s","KEIN FIX");
		return;
	}
	// OK, Daten liegen vor
	sprintf(MyDevice[gpsWert].TiWP[0].Wert,"%s","FIX");
	sprintf(MyDevice[gpsWert].TiWP[11].Wert,"%d",vsat);
	sprintf(MyDevice[gpsWert].TiWP[12].Wert,"%d",usat);
	// Und jetzt den Rest 
	gps_get_altitude(&MyDevice[gpsWert].TinkerDevice , &wert1 , &wert2);  // wert1 = hoehe
	sprintf(MyDevice[gpsWert].TiWP[6].Wert,"%d",wert1);

	gps_get_motion(&MyDevice[gpsWert].TinkerDevice , &wert1 , &wert2);  // wert1 = kurs   2=Geschw.
	sprintf(MyDevice[gpsWert].TiWP[7].Wert,"%d",wert1);
	sprintf(MyDevice[gpsWert].TiWP[8].Wert,"%d",wert2);
	
	sprintf(MyDevice[gpsWert].TiWP[9].Wert,"%d",gdate);
	sprintf(MyDevice[gpsWert].TiWP[10].Wert,"%d",gtime);

	gps_get_coordinates(&MyDevice[gpsWert].TinkerDevice, &wert1, (char *) MyDevice[gpsWert].TiWP[3].Wert , &wert2, (char *) MyDevice[gpsWert].TiWP[4].Wert , &leer1, &leer2, &leer3 , &abweich);
	// TEST 
}

void initGPS(Device MD )
{
	int t = tinkerCounter;
	
	gpsWert = tinkerCounter;
	GPS_Einstellungen( MD ) ;

	sprintf(MyDevice[t].TiWP[0].SuchName,"GPSST");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Status ");
	sprintf(MyDevice[t].TiWP[1].SuchName,"GPSRE");		// Längengrad Rektszension
	sprintf(MyDevice[t].TiWP[1].Name,"Laeng.Grd. ");
	sprintf(MyDevice[t].TiWP[2].SuchName,"GPSDE");		// Breitengrad Deklination
	sprintf(MyDevice[t].TiWP[2].Name,"Breit.Grd.");
	sprintf(MyDevice[t].TiWP[3].SuchName,"GPSH1");  	// Himmelsrichtung für Längen und Breitengrad 
	sprintf(MyDevice[t].TiWP[3].Name," ");
	sprintf(MyDevice[t].TiWP[4].SuchName,"GPSH2");
	sprintf(MyDevice[t].TiWP[4].Name," .");
	sprintf(MyDevice[t].TiWP[5].SuchName,"GPSERR");		
	sprintf(MyDevice[t].TiWP[5].Name,"Abweich.");		// Abweichung in cm.
	sprintf(MyDevice[t].TiWP[6].SuchName,"GPSAL");		// Aktuelle Hohe		
	sprintf(MyDevice[t].TiWP[6].Name,"Hoehe");		
	sprintf(MyDevice[t].TiWP[7].SuchName,"GPSMOT");		
	sprintf(MyDevice[t].TiWP[7].Name,"Wohin");			//	Bewegungsrichtung hunderstel Grad 
	sprintf(MyDevice[t].TiWP[8].SuchName,"GPSMOV");		
	sprintf(MyDevice[t].TiWP[8].Name,"Geschw.");		//  Geschwindigkeit 
	sprintf(MyDevice[t].TiWP[9].SuchName,"GPSDAT");		
	sprintf(MyDevice[t].TiWP[9].Name,"Datum");			// .
	sprintf(MyDevice[t].TiWP[10].SuchName,"GPSTIM");		
	sprintf(MyDevice[t].TiWP[10].Name,"Zeit");			// 
	sprintf(MyDevice[t].TiWP[11].SuchName,"GPSVSAT");		
	sprintf(MyDevice[t].TiWP[11].Name,"VSat");			//   Visble Sats. .
	sprintf(MyDevice[t].TiWP[12].SuchName,"GPSUSAT");		
	sprintf(MyDevice[t].TiWP[12].Name,"USat");			// 		Used sats.

	MyDevice[t].TiWPCounter = 13;	
}

void GPS_Einstellungen( Device MD ) 
{
	// Nur ein Callback, welches alle anderen Daten aufruft.
	gps_register_callback(&MD, GPS_CALLBACK_DATE_TIME, GPS_Daten, NULL);
	gps_set_date_time_callback_period(&MD, 10000);  
}
// --------------------- GPS ENDE --------------------------

// --------------------- SERV ANFANG -------------------------

int servoWert;
void initSERV(Device MD)
{
	int t = tinkerCounter;
	servoWert = t;
	SERV_Einstellungen( MD );
	MyDevice[t].TiWPCounter = 0;	
}

void SERV_Einstellungen( Device MD ) 
{
	int i;
	// Standard: Alle Servos auf an und einmal mit kleiner Geschwindigkeit von Anschlag zu Anschlag
	// 
	for ( i = 0; i < 7; i++ ) {
		servo_enable(&MD,i);
		servo_set_velocity(&MD,i,1000);
		servo_set_acceleration(&MD,i,1000);
	}
	servo_set_output_voltage(&MD,6000);
	// Und einmal laufen lassen
	for ( i = 0; i < 7; i++ ) {
		servo_set_position(&MD,i,-9000);
	}
	sleep(2);
	for ( i = 0; i < 7; i++ ) {
		servo_set_position(&MD,i,9000);
	}
	sleep(2);
	for ( i = 0; i < 7; i++ ) {
		servo_set_position(&MD,i,0);
	}
}
// --------------------- SERV ENDE --------------------------

// --------------------- STEP Anfang ------------------------

// --------------------- STEP ENDE -------------------------


// ----------------- POTI ----------------------

int potiWert;
void POTIDaten(uint16_t position, void *data)
{
	sprintf(MyDevice[potiWert].TiWP[0].Wert,"%d",position);
	//			
}

void initPOTI( Device MD ) 
{
	POTI_Einstellungen(MD);
}

void POTI_Einstellungen ( Device MD )
{
	int t = tinkerCounter;
	potiWert=t;
	linear_poti_register_callback(&MD, LINEAR_POTI_CALLBACK_POSITION, POTIDaten, NULL);
	linear_poti_set_position_callback_period(&MD, 10000);
	sprintf(MyDevice[t].TiWP[0].SuchName,"POTI");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Linearpoti");
	MyDevice[t].TiWPCounter = 1;	
}

// ----------------- POTI ENDE ----------------

// ----------------- DUAL ---------------------

void DUAL_Daten(uint8_t button_l, uint8_t button_r, uint8_t led_l, uint8_t led_r, void *daten)
{
	//    0 = gedrückt    1 = losgelassen 
	//   = 0
	// DUAL_BUTTON_BUTTON_STATE_RELEASED = 1
	// DUAL_BUTTON_LED_STATE_AUTO_TOGGLE_ON = 0
	// DUAL_BUTTON_LED_STATE_AUTO_TOGGLE_OFF = 1
	// DUAL_BUTTON_LED_STATE_ON = 2
	// DUAL_BUTTON_LED_STATE_OFF = 3
	if ( button_l == DUAL_BUTTON_BUTTON_STATE_PRESSED ) barometer_set_reference_air_pressure(&BARO_TMP,0);	
	
} 

void initDUAL( Device MD ) 
{
	DUAL_Einstellungen(MD);
}

void DUAL_Einstellungen ( Device MD )
{
	// Einmal die LED blinken lassen 
	dual_button_set_led_state( &MD, 0, 0);  // Autotoggle ON
	dual_button_register_callback(&MD, DUAL_BUTTON_CALLBACK_STATE_CHANGED, DUAL_Daten, NULL);	
	// Wie auf die Tasten reagiert wird muss noch rein 
}
// ----------------- DUAL ENDE ----------------

// ----------------- LASR ---------------------
static int laserWert;

void LASR_Daten_D(uint16_t distance, void *data)
{
	// Werte von 0 - 4000 cm
	sprintf(MyDevice[laserWert].TiWP[1].Wert,"%d",distance);		
}

void LASR_Daten_V(uint16_t velocity, void *data)
{
	// Werte von 0 -12700  1/100 m/s
	sprintf(MyDevice[laserWert].TiWP[2].Wert,"%d",velocity);		
}

void initLASR( Device MD )
{
	LASR_Einstellungen( MD );
}

void LASR_Einstellungen( Device MD ) 
{
	int t = tinkerCounter;
	laserWert = t;
	
	laser_range_finder_enable_laser( &MD );
	laser_range_finder_set_mode(&MD , LASER_RANGE_FINDER_MODE_DISTANCE );   // Default 
	LASR_To_Distance( MD );
	sprintf(MyDevice[t].TiWP[0].SuchName,"LMOD");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Laser Modus");
	sprintf(MyDevice[t].TiWP[0].Wert,"DIST");	
	sprintf(MyDevice[t].TiWP[1].SuchName,"LDIS");		// Status : 
	sprintf(MyDevice[t].TiWP[1].Name,"Laser Entfern.");
	sprintf(MyDevice[t].TiWP[2].SuchName,"LVEL");		// Status : 
	sprintf(MyDevice[t].TiWP[2].Name,"Laser Beschl");
	MyDevice[t].TiWPCounter = 3;	
	// TODO 
}

void LASR_To_Distance( Device MD )
{
	laser_range_finder_register_callback(&MD , LASER_RANGE_FINDER_CALLBACK_DISTANCE, LASR_Daten_D, NULL );
	laser_range_finder_set_distance_callback_period( &MD , 10000);
	laser_range_finder_set_velocity_callback_period( &MD , 0);
	sprintf(MyDevice[laserWert].TiWP[0].Wert,"DIST");	
}

void LASR_To_Velocity( Device MD ) 
{
	// 13MS = 45 km/h
	// 32MS = 115 km/h
	// 127MS = 457 hm/h 
	laser_range_finder_set_mode(&MD , LASER_RANGE_FINDER_MODE_VELOCITY_MAX_64MS);   // Max Geschw. = 228 km/h
	laser_range_finder_register_callback(&MD , LASER_RANGE_FINDER_CALLBACK_VELOCITY, LASR_Daten_V, NULL );
	laser_range_finder_set_distance_callback_period( &MD , 0);
	laser_range_finder_set_velocity_callback_period( &MD , 10000);
	sprintf(MyDevice[laserWert].TiWP[0].Wert,"VELO");	
}
// ----------------- LASR ENDE ----------------

// ----------------- MULT ---------------------

void MULT_Daten(uint16_t state , void *data)
{
	static char MU_STR[22];
	
	// Wert ist 12 bit   0-11 Elektroden  12 = Annäherung 

	if(state & (1 << 12)) sprintf(MU_STR,"NAH DRAN");
    if((state & 0xfff) == 0) return;

    int i;
    for(i = 0; i < 12; i++) { 
		if(state & (1 << i)) {
			sprintf(MU_STR,"AN = %3d",i);
			if ( i == 4 ) {  // nur mal als Test baro hoehe auf 0
				barometer_set_reference_air_pressure(&BARO_TMP,0);
			}
        }
    }
	sprintf(zeile[1][3],"%-20s",MU_STR);
}

void initMULT( Device MD ) 
{

	MULT_Einstellungen( MD ) ;
}

void MULT_Einstellungen( Device MD )
{
	multi_touch_register_callback(&MD, MULTI_TOUCH_CALLBACK_TOUCH_STATE, MULT_Daten, NULL);
	multi_touch_set_electrode_config(&MD , 8191 ) ;   // Alle Felder + Annäherung auf an 
}
// ----------------- MULT ENDE ----------------

// ---------------- MASTER ------------------------
// Der Wert LAUF zeigt an, wie of mtimer.c schon aufgerufen wurde.
static int masterWert; 

void MAST_Daten(uint16_t verbrauch, void *data) 
{
	uint16_t *voltage; 
	master_get_stack_voltage(&MyDevice[masterWert].TinkerDevice, voltage);
	sprintf(MyDevice[masterWert].TiWP[0].Wert,"%d",voltage);
	sprintf(MyDevice[masterWert].TiWP[1].Wert,"%d",verbrauch);
	sprintf(MyDevice[masterWert].TiWP[2].Wert,"%d",MythreadCounter);
}

void initMAST( Device MD ) 
{	
	int t = tinkerCounter;

	MAST_Einstellungen( MD ) ;
	masterWert = tinkerCounter;
	sprintf(MyDevice[t].TiWP[0].SuchName,"VOLT");		 
	sprintf(MyDevice[t].TiWP[0].Name,"Volt in mv");	
	sprintf(MyDevice[t].TiWP[1].SuchName,"AMP");		 
	sprintf(MyDevice[t].TiWP[1].Name,"Ampere in mA");	
	sprintf(MyDevice[t].TiWP[1].SuchName,"LAUF");		 
	sprintf(MyDevice[t].TiWP[1].Name,"Outputthread lfdnr");	

	MyDevice[t].TiWPCounter = 3;	
}

void MAST_Einstellungen( Device MD ) 
{	
	master_register_callback(&MD, MASTER_CALLBACK_STACK_CURRENT, MAST_Daten , NULL ); 
	master_set_stack_current_callback_period(&MD, 30000); // ale 30 sekunden 
}

// -------------------- MASTER ENDE -----------------------------

// ACCEL -----------------------------------------------------

int accelWert;
void ACCELDaten(int16_t x, int16_t y, int16_t z, void *data)
{
	// Und noch die Temp. 
	accelerometer_get_temperature(&MyDevice[accelWert].TinkerDevice, int16_t temp);
	
	sprintf(MyDevice[accelWert].TiWP[0].Wert,"%d",x);
	sprintf(MyDevice[accelWert].TiWP[1].Wert,"%d",y);
	sprintf(MyDevice[accelWert].TiWP[2].Wert,"%d",z);
	sprintf(MyDevice[accelWert].TiWP[3].Wert,"%d",temp);
	//			
}

void initACCEL( Device MD ) 
{
	ACCEL_Einstellungen(MD);
}

void ACCEL_Einstellungen ( Device MD )
{
	int t = tinkerCounter;

	accelWert=t;
	accelerometer_register_callback(&MD, ACCELEROMETER_CALLBACK_ACCELERATION, ACCELDaten, NULL);
	accelerometer_set_acceleration_callback_period(&MD, 2000);
	sprintf(MyDevice[t].TiWP[0].SuchName,"ACC_X");		// Status : 
	sprintf(MyDevice[t].TiWP[0].Name,"Beschl_x");
	sprintf(MyDevice[t].TiWP[1].SuchName,"ACC_Y");		// Status : 
	sprintf(MyDevice[t].TiWP[1].Name,"Beschl_y");
	sprintf(MyDevice[t].TiWP[2].SuchName,"ACC_Z");		// Status : 
	sprintf(MyDevice[t].TiWP[2].Name,"Beschl_z");
	sprintf(MyDevice[t].TiWP[3].SuchName,"ACC_T");		// Status : 
	sprintf(MyDevice[t].TiWP[3].Name,"Chiptemp");	
	MyDevice[t].TiWPCounter = 4;	
	// Noch ein paar Einstellungen für Auflösung etc.
	accelerometer_led_on(&MD);   // LED AN
	accelerometer_set_configuration(&MD, ,ACCELEROMETER_DATA_RATE_400HZ, ACCELEROMETER_FULL_SCALE_2G ,  ACCELEROMETER_FILTER_BANDWIDTH_200HZ); 
}


/// ACCEL ENDE -----------------------------------------------
