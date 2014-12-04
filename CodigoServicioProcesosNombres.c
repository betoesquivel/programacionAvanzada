#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/types.h>

long estado=0;	//Indicador para rastrear la actividad del servicio
				// (no muy importante en este ejemplo)
int fh_dia,fh_mes,fh_anio,fh_hora,fh_minuto,fh_segundo;
FILE *arch;

SERVICE_STATUS_HANDLE interfaz_al_SCM;
SERVICE_STATUS serviceStatus;
SERVICE_TABLE_ENTRY tabla;
char nombreServicio[]="ProgAva_Srv";

HANDLE elThread;
DWORD elThreadID;

VOID WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv);
VOID WINAPI mainHandle(DWORD fdwControl);

void main(void)
{
	tabla.lpServiceName=nombreServicio;	
	tabla.lpServiceProc=&ServiceMain;
	StartServiceCtrlDispatcher(&tabla);
}

//************************************************************************
/*
void fecha_hora(void) {
	struct tm *newtime;
	long ltime;
	time( &ltime );
	newtime = localtime( &ltime );

	fh_dia=(unsigned char)newtime->tm_mday;
	fh_mes=(unsigned char)newtime->tm_mon+1;
	fh_anio=(unsigned char)newtime->tm_year+1900;

	fh_hora=(unsigned char)newtime->tm_hour;
	fh_minuto=(unsigned char)newtime->tm_min;
	fh_segundo=(unsigned char)newtime->tm_sec;
}
*/
void setServiceStatusState(unsigned int state) {
	serviceStatus.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState=state;
	serviceStatus.dwControlsAccepted=SERVICE_CONTROL_INTERROGATE
										|SERVICE_ACCEPT_STOP
										|SERVICE_ACCEPT_PAUSE_CONTINUE
										|SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode=NO_ERROR;
	serviceStatus.dwServiceSpecificExitCode=0;
	serviceStatus.dwCheckPoint=estado++;
	serviceStatus.dwWaitHint=100;	
}

DWORD WINAPI corre(LPVOID lpParameter) {
	setServiceStatusState(SERVICE_RUNNING);	//Avisar que YA ESTAMOS CORRIENDO!!!
	SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

	while(TRUE) {
		if (arch=fopen("C:\\log.txt","a+")) {
			fecha_hora();
			fprintf(arch,"%02d/%02d/%04d %02d:%02d:%02d\n",fh_dia,fh_mes,fh_anio,fh_hora,fh_minuto,fh_segundo);
			fclose(arch);
		}
		Sleep(5000);	//Duérmete 5 seg.
	}
}

VOID WINAPI mainHandle(DWORD fdwControl) {
	switch(fdwControl) {
		case SERVICE_CONTROL_STOP:
					setServiceStatusState(SERVICE_STOP_PENDING);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					if (TerminateThread(elThread,0)) {
						setServiceStatusState(SERVICE_STOPPED);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					}
					break;
		case SERVICE_CONTROL_PAUSE:
					setServiceStatusState(SERVICE_PAUSE_PENDING);	//Avisar que YA ESTAMOS CORRIENDO!!!
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					SuspendThread(elThread);
					
					setServiceStatusState(SERVICE_PAUSED);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					break;
		case SERVICE_CONTROL_CONTINUE:
					setServiceStatusState(SERVICE_CONTINUE_PENDING);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					ResumeThread(elThread);

					setServiceStatusState(SERVICE_RUNNING);	//Avisar que YA ESTAMOS CORRIENDO!!!
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					break;
		default:
					setServiceStatusState(SERVICE_RUNNING);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					break;
	}
}


VOID WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv) {
	interfaz_al_SCM=RegisterServiceCtrlHandler(nombreServicio,mainHandle);

	setServiceStatusState(SERVICE_START_PENDING);
	SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Avisar que estamos "alive and kicking!"

	elThread=CreateThread(NULL,0,corre,NULL,0,&elThreadID);
}

