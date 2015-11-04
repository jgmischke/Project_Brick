#include "ownheader.h"
#include <time.h>
// ----- LCD ------
void LCD_Press( uint8_t button, void *data);
void LCD_Release( uint8_t button, void *data);
void LCD_AktionP( uint8_t button );
void LCD_AktionR( uint8_t button );
void LCD_AktionP0();
void LCD_AktionP1();
void LCD_AktionP2();
void LCD_AktionP3();
void LCD_AktionR0();
void LCD_AktionR1();
void LCD_AktionR2();
void LCD_AktionR3();
void initLCD(Device MD);
void LCD_Einstellen(Device MD);

// ------- DISR ---------
void DISR_distanz( uint16_t dis, void *userdata);
void DISR_Einstellen( Device MD);

// ------- BARO -------------- 

void BARO_Luftdruck(int32_t luftdruck, void *data);
void BARO_Hoehe(int32_t hoehe, void *data);
void BARO_Einstellen( Device MD ); 

// --------- COLB  ---------------

void COLB_Farben(uint16_t r, uint16_t g, uint16_t b, uint16_t c, void *user_data);
void COLB_Helligkeit(uint32_t helligkeit, void *user_data);
void COLB_Farbtemp(uint16_t farbtemp, void *user_data);
void COLB_Einstellen( Device MD ); 

// --------- DISU ----------------
void DISU_distanz(uint16_t distance, void *user_data);
void DISU_Einstellungen( Device MD ); 

// ---------- HUMI ----------------
void HUMI_lf( uint16_t p, void *data );
void HUMI_Einstellungen( Device MD ); 

// ---------- TEIR ---------------
void TEIR_Daten(int16_t objTemp, void *data);
void TEIR_Einstellungen( Device MD ); 

// ----------- GPS ----------------
void GPS_Daten(  uint32_t date, uint32_t time, void *user_data);
void GPS_Einstellungen( Device  ) ;

// ---------- POTI ---------------
void POTI_Daten();
void POTI_Einstellungen( Device MD );

// ---------- DUAL ---------------
void DUAL_Daten(uint8_t button_l, uint8_t button_r, uint8_t led_l, uint8_t led_r, void *daten);
void DUAL_Einstellungen ( Device MD );

// ----------- LASR ----------------
void LASR_Daten();
void LASR_Einstellungen ( Device MD ) ;

// ----------- MULT ----------------
void MULT_Daten(uint16_t state , void *data);
void MULT_Einstellungen( Device MD );

// ------------ weiter -----------------



