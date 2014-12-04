#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>

long estado=0;	//Indicador para rastrear la actividad del servicio
				// (no muy importante en este ejemplo)

HINSTANCE hCodigoDll;
typedef BOOL _stdcall TipoRutina_EnumProcesses(DWORD *, DWORD , DWORD *);
TipoRutina_EnumProcesses *pEnumProcesses;

typedef BOOL _stdcall TipoRutina_EnumProcessModules(HANDLE,HMODULE *,DWORD,LPDWORD);
TipoRutina_EnumProcessModules *pEnumProcessModules;

typedef DWORD _stdcall TipoRutina_GetModuleBaseName(HANDLE,HMODULE,LPTSTR,DWORD);
TipoRutina_GetModuleBaseName *pGetModuleBaseName;

SERVICE_STATUS_HANDLE interfaz_al_SCM;
SERVICE_STATUS serviceStatus;
SERVICE_TABLE_ENTRY tabla;
char nombreServicio[]="ProgAva_Srv";

FILE *arch;

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
	HANDLE hProcess;
	HMODULE hMod;
	DWORD cbNeeded;
	char szProcessName[100];
	DWORD procArray[1024];
	DWORD bytesNeeded;
	DWORD sizeOfArray=sizeof(procArray);
	int cantidadProcesos;
	int i;

	setServiceStatusState(SERVICE_RUNNING);	//Avisar que YA ESTAMOS CORRIENDO!!!
	SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

	fprintf(arch,"CORRIENDO!!!\n");fflush(arch);

	while(TRUE) {
		fprintf(arch,"EnumProcesses...\n");fflush(arch);
		(*pEnumProcesses)(&procArray[0],4096,&bytesNeeded);
		cantidadProcesos = bytesNeeded/4;
		fprintf(arch,"Cantidad de procesos:%d\n",cantidadProcesos);fflush(arch);
		for(i=0;i<cantidadProcesos;i++) {
			hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,procArray[i]);

		   // Get the process name.
		   if (hProcess) {
			   if ((*pEnumProcessModules)(hProcess, &hMod, sizeof(hMod),&cbNeeded)) {
				   (*pGetModuleBaseName)(hProcess,hMod,szProcessName,sizeof(szProcessName)/sizeof(TCHAR));
				   if (strcmp(szProcessName,"notepad.exe")==0) {
					   TerminateProcess(hProcess,0);
					   fprintf(arch,"Notepad encontrado y finalizado!!!\n");fflush(arch);
				   }
			   }
			   CloseHandle(hProcess);
		   }
		}
		Sleep(5000);	//Duérmete 5 seg.
	}
}

VOID WINAPI mainHandle(DWORD fdwControl) {
	switch(fdwControl) {
		case SERVICE_CONTROL_STOP:
					setServiceStatusState(SERVICE_STOP_PENDING);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					fprintf(arch,"STOP_PENDING\n");fflush(arch);

					if (TerminateThread(elThread,0)!=0) {
						setServiceStatusState(SERVICE_STOPPED);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
						FreeLibrary(hCodigoDll);
						fprintf(arch,"STOPPED!!!\n");fflush(arch);
					} else {
						setServiceStatusState(SERVICE_RUNNING);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
						fprintf(arch,"STOP_PENDING ... ERROR ... RUNING AGAIN\n");fflush(arch);
					}
					break;
		case SERVICE_CONTROL_PAUSE:
					setServiceStatusState(SERVICE_PAUSE_PENDING);	//Avisar que YA ESTAMOS CORRIENDO!!!
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					if (SuspendThread(elThread)!=-1) {
						setServiceStatusState(SERVICE_PAUSED);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					} else {
						setServiceStatusState(SERVICE_RUNNING);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					}
					break;
		case SERVICE_CONTROL_CONTINUE:
					setServiceStatusState(SERVICE_CONTINUE_PENDING);
					SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios

					if (ResumeThread(elThread)!=-1) {
						setServiceStatusState(SERVICE_RUNNING);	//Avisar que YA ESTAMOS CORRIENDO!!!
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios
					} else {
						setServiceStatusState(SERVICE_STOPPED);
						SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Enviar mensaje al manejador de servicios						
					}
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

	arch=fopen("C:\\log.txt","a+");
	hCodigoDll=LoadLibrary("psapi.dll");
	if (hCodigoDll) {
		pEnumProcesses=(TipoRutina_EnumProcesses *)GetProcAddress(hCodigoDll,"EnumProcesses");
		pEnumProcessModules=(TipoRutina_EnumProcessModules *)GetProcAddress(hCodigoDll,"EnumProcessModules");
		pGetModuleBaseName=(TipoRutina_GetModuleBaseName *)GetProcAddress(hCodigoDll,"GetModuleBaseNameA");
		if (pEnumProcesses && pEnumProcessModules && pGetModuleBaseName) {
			elThread=CreateThread(NULL,0,corre,NULL,0,&elThreadID);
			fprintf(arch,"Thread creado\n");fflush(arch);
		} else {
			fprintf(arch,"Error al crear el thread\n");fflush(arch);
			setServiceStatusState(SERVICE_STOPPED);
			SetServiceStatus(interfaz_al_SCM,&serviceStatus);	//Avisar que estamos "alive and kicking!"
		}

	} else {
		setServiceStatusState(SERVICE_STOPPED);
		SetServiceStatus(interfaz_al_SCM,&serviceStatus);
	}
}

