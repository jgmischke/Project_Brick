#include "ownheader.h"


// Shared Mem 
char *myspace;
int myID;  // SHMID 
#define MAXSPACE 4096000    //  
#define MaxMessage 320
#define MaxMessageLen   128
#define SKey 100931928

// Shared Mem von 4 MB
// aufgeteilt in 320 Nachrichten a 128 Byte für 100 Devices
// Benutzt werden in Moment nur 50 Devices 
// Pro Device ein Zähler, wo aktuell der Zeiger für das Device steht
// int TinkerMemPos;

int  createSharedMemory()
{
	myID = shmget( SKey , MAXSPACE, IPC_CREAT  |  0666);	
//	myID = shmget( SKey , MAXSPACE, IPC_CREAT | IPC_EXCL |  0666);
	if ( myID < 0 ) {
		perror("SHMGET");
		exit (0); 
	}
	myspace = shmat(myID,NULL,SHM_RND);
	if ( myspace == (char *) -1) {
		perror("myspace");
		return 1;
	}
	memset(myspace,0,MAXSPACE);
	fprintf(stderr,"createSharedMemory mit %d Bytes = OK\n",MAXSPACE);
	return 0;
}

int insertSharedMemory(char *daten, int DeviceNummer)
{
	int MyPos;
	int i;
	long pPos; 
	MyPos = MyDevice[DeviceNummer].TinkerMemPos;

	pPos = (DeviceNummer*MaxMessageLen*MaxMessage + MyPos*128);
	for ( i = 0; i  < MaxMessageLen; i++ ) {
		*(myspace+pPos+i) = *(daten+i);
	}
	
	MyDevice[DeviceNummer].TinkerMemPos++;
	if ( MyDevice[DeviceNummer].TinkerMemPos >= MaxMessage  ) MyDevice[DeviceNummer].TinkerMemPos = 0;
	return 0;
}

char* readSharedMemory(int DeviceNummer, int Position)
{
	int i, MyPos ;
	char ds[128];
	long pPos;
	
	MyPos = MyDevice[DeviceNummer].TinkerMemPos;
	memset(ds,0,MaxMessageLen);

	for ( i = 0; i < MaxMessageLen; i++ ) {
			ds[i] = *(myspace+pPos+i);
	}
	return ds;
}

int deleteSharedMemory()
{
	shmdt(myspace);
	shmctl(myID,IPC_RMID, NULL);
	return 0;
}

