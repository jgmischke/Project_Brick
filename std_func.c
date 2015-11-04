/**
 *
 **/
void strMid( char *string, char *erg, int counter, int startpos )
{
	int l;
	l = strlen(string)-1;
	while ( counter-- ){
		*erg++ = *(string + startpos++ );
	}
	*erg = '\0';
}

/*! \brief Liefert eine Zeile aus dem Speicher zurueck<BR>
	- Wird zum Start ein Pointer uebergeben, wird dieser abgespeichert und dann fuer jeden Aufruf<BR>
	hochgezaehlt. Dazu wird dann immer ein NULL-Pointer als Parameter uebergeben. <BR>
	Als Int wird die Groesse der Zeile zurueckgegeben.<BR>
	Ist *Line == "I" wird das ganze erst mal nur zur Initialisierung genutzt, damit<BR>
	*point gesetzt werden kann, aber es wird kein Wert zurueckgegeben.<BR>
	Da die Funktion auch rekursiv genutzt wird, kann bis zu einer Ebene[10] aufgerufen werden,<BR>
	wobei in jeder Ebene ein eigener Zaehler zuer Verfuegung steht<BR>
*/
int strReadLine( char *ReadMe, char *Line, int level )
{
        int i = 0;
        static char *point[10];

        if ( ReadMe != NULL )  point[level] = (char *) ReadMe;
        if ( *Line == 'I' ) return 0;

        while(1) {
		if ( *point[level] == '\0' ) break;
		if ( *point[level] == '\n' ) {
			*Line++ = *point[level]++;
                        *Line = '\0';
                        i++;
                        break;
                }
                *Line++ = *point[level]++;
                i++;
        }
        return i;
}

/*! \brief Liefert eine Zeile aus dem Speicher zurueck<BR>
	- Eigene Funktion für den Thread2, bis auf die Pointer identisch mit strReadLine
	Hier ist dann sichergestellt, das die gleiche Funktion nicht doch versehentlich 
	doppelt aufgerufen wird.
*/
int strReadLine2( char *ReadMe, char *Line)
{
        int i = 0;
        static char *point;

        if ( ReadMe != NULL )  point = (char *) ReadMe;
        if ( *Line == 'I' ) return 0;

        while(1) {
		if ( *point == '\0' ) break;
		if ( *point == '\n' ) {
			*Line++ = *point++;
                        *Line = '\0';
                        i++;
                        break;
                }
                *Line++ = *point++;
                i++;
        }
        return i;
}


/**
 *
 **/
void strLeft( char *string, char *erg, int counter )
{
	while ( counter-- ) *erg++ = *string++;
	*erg = '\0';
}


/**
 *
 **/
void strRight( char *string, char *erg, int counter )
{
	int l;
	l = strlen(string)-1;
	while (counter--)     *erg++ = *(string +l -counter);
	*erg = '\0';
}

/**
 *
 **/
int strSearch( char *suchen, char ch)
{
	int counter = 0;
	while ( *suchen != '\0' ){
		if ( *suchen++ == ch ) counter++;
	}
	return counter;
}


/**
 *
 **/
int strDelNL( char *suchen )
{
	int counter = 0;
	while ( *suchen != '\0' ){
		if ( *suchen == '\n'  ) *suchen = '\0';
		suchen++;
	}
	return counter;
}


/**
 *
 **/
int  strStrip( char  *strip_it )
{
	char *x_buf, *y_buf ;
	int i  = 0;

	x_buf = malloc (strlen (strip_it));
	y_buf = malloc (strlen (strip_it));
	memset( x_buf, 0, strlen (strip_it));
	strcpy( y_buf , strip_it);
	
	while ( *y_buf  != '\0' ){
		if ( *y_buf == ' ' ) {
			*y_buf++ ;
			continue;
		}
		x_buf[i++] = *y_buf++;
	}
	x_buf[i] = '\0';

	strcpy( strip_it,  x_buf);
	strDelNL( strip_it );

	return 1;
}

/*! \brief
	- liefert einen aktuellen Zeitstempel im Format (s.u) zurueckgegeben
	- Der Speicherplatz wird nur einmal beim Start aufgerufen und bleibt ansonsten immer gleich,
	  spart Platz, vor allem wenn die Funktion tausendemal aufgerufen wird
*/

char * strnow()
{
	time_t t_strnow;
	static char *s_strnow;
	struct tm *gm_time;
	static int lauf ;

	if ( lauf == 0 ) {
		s_strnow = (char *) malloc(100 ) ;
		lauf = 1;
	}
	time(&t_strnow);
	
	gm_time = gmtime(&t_strnow);
	strftime(s_strnow, 100, "%Y-%m-%d %H:%M:%S" , gm_time );

	return s_strnow;
}
	

/**
 *  
 **/
int str_token( char *strin, char **array, const char *delimiter)
{
        int i,j;
        char *p = strin;
        char c;
	
	// Zum Anfang einmal die ganzen Daten anzeigen
	#ifdef WD
	DEBUG( REST, $DB  1 , "POINTER -> %s STRING : %.10s  ADRESSEN:  %x %x  ", delimiter, strin, strin, *array );
	#endif
	
        i = j = 0;
        while ( *p != '\0' ) {
                c = *p;
                if ( c == *delimiter ) {
                        *(*(array+i++)+j) = 0;
                        j = 0;
                        p++;
                        continue;
                }
                *(*(array+i)+j++) = c;
                p++;
        }
        *(*(array+i)+j) = 0;

	/*
	for ( j = 0 ; j <= i; j++ ) {
		DEBUG( REST, $DB  2 , "array[j] = **%s**\n", array[j]);
	}
	*/
        
	return i;
}


int strfill( char *strin, int count, char c )
{
	int i;
	
	for ( i = 0; i < count; i++ ) *strin++ = c;
	*strin = '\0';		

	return count;
}
/*! \brief
	- durchsucht alle Prozesse, die zu diesem Todo gehoeren und loescht diese.
	Der Programmname kann in status nur gefunden werden, wenn er nicht > 16 Zeichen ist !!
	Gesucht wurde bisher nach status.name: xxxxxxxxxx 
	Deshalb wird jetzt cmdline genutzt, welche etwa so aussieht : /bin/bash/0/XXXXXXXX_XXXXX.sh
	wegen den in dieser einen Zeile enthaltenen NULL-Zeichen s.u.
*/
static char *procpfad = "/proc"; 
int killall( char *programmname ) 
{
	struct dirent *datei;
	DIR *inhalt;
	int fp;
	char fpname[KV];
	char fpgets[KV];
	int pid,erg;

	if (( inhalt = opendir(procpfad)) == NULL ) {
		perror("Opendir brachte NULL");
		return -1;
	}

	while (1) {
		datei = readdir( inhalt );
		if ( datei == NULL ) {
			break;
		}
		if ( datei->d_type == DT_DIR &&  datei->d_name[0] != '.'  && ( datei->d_name[0] >= '0' && datei->d_name[0] <= '9') ) {
			sprintf(fpname,"%s/%s/cmdline",procpfad,datei->d_name);
			if ((fp = open(fpname,O_RDONLY)) != -1 ) {
				pid = 0;
				if ((erg=read(fp, fpgets, KV )) > 0 ) {
					int i;
					for ( i = 0; i < erg-1; i++ ) {
						if ( fpgets[i] == '\0' ) fpgets[i] = ' ';
					}
					if ( strstr(fpgets,programmname) != NULL ) {
						pid = (int) atol(datei->d_name);
					}	
					if ( pid != 0 ) { 
						#ifdef WD
						DEBUG( REST, $DB  1, "Kill_pid mit %s und %d",datei->d_name, pid);
						#endif
						kill_pid(datei->d_name, pid);
					}
				}
				close(fp);
			}
		}
	}
	closedir(inhalt);
	return 1;
}
static int deep;
int kill_pid( char *name, int pidx) 
{
	// Loesche alle Prozesse, die unter PPid diese ID haben
	struct dirent *datei;
	DIR *inhalt;
	FILE *fp;
	char fpname[KV];
	char fpgets[KV];
	int pid;

	deep++;
	if (( inhalt = opendir(procpfad)) == NULL ) {
		fprintf(stderr,"Opendir brachte NULL");
		return -1;
	}

	while (1) {
		datei = readdir( inhalt );
		if ( datei == NULL ) {
			break;
		}
		
		if ( datei->d_type == DT_DIR &&  datei->d_name[0] != '.'  && ( datei->d_name[0] >= '0' && datei->d_name[0] <= '9') ) {
			sprintf(fpname,"%s/%s/status",procpfad,datei->d_name);
			if ((fp = fopen(fpname,"r")) == NULL ) continue;
			pid = 0;
			while(1) {
				if ( (fgets( fpgets, KV, fp)) == NULL ) break;
				if ( (strstr(fpgets,name) != NULL)  && (strstr(fpgets,"PPid") != NULL) ) {
					pid = 1; 
				}	
			}
			fclose(fp);
			if ( pid != 0 ) {
				#ifdef WD
				DEBUG( REST, $DB  1, "PPID = %d in %s bei deep=%d\n",pid,datei->d_name,deep);
				#endif
				pid = (int) atol(datei->d_name);
				kill_pid(datei->d_name,pid);
				deep--;
			}
		}
	}
	closedir(inhalt);
	#ifdef WD
	DEBUG( REST, $DB  1, "Kill mit pidx = %d\n",pidx);
	#endif
	kill( pidx, SIGTERM);
	return 0;
}
