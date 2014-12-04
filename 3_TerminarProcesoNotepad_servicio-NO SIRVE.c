#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <windows.h>

HINSTANCE hCodigoDll;
typedef BOOL _stdcall TipoRutina_EnumProcesses(DWORD *, DWORD , DWORD *);
TipoRutina_EnumProcesses *pEnumProcesses;

typedef BOOL _stdcall TipoRutina_EnumProcessModules(HANDLE,HMODULE *,DWORD,LPDWORD);
TipoRutina_EnumProcessModules *pEnumProcessModules;

typedef DWORD _stdcall TipoRutina_GetModuleBaseName(HANDLE,HMODULE,LPTSTR,DWORD);
TipoRutina_GetModuleBaseName *pGetModuleBaseName;

HANDLE hProcess;
HMODULE hMod;
DWORD cbNeeded;
char szProcessName[100];

DWORD array[1024];
DWORD sizeOfArray=sizeof(array);
DWORD bytesNeeded=13;
int i;

long estado=0;	//Indicador para rastrear la actividad del servicio
				// (no muy importante en este ejemplo)

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

	hCodigoDll=LoadLibrary("psapi.dll");

	while(TRUE) {
		if (hCodigoDll) {
			pEnumProcesses=(TipoRutina_EnumProcesses *)GetProcAddress(hCodigoDll,"EnumProcesses");
			pEnumProcessModules=(TipoRutina_EnumProcessModules *)GetProcAddress(hCodigoDll,"EnumProcessModules");
			pGetModuleBaseName=(TipoRutina_GetModuleBaseName *)GetProcAddress(hCodigoDll,"GetModuleBaseNameA");
			if (pEnumProcesses && pEnumProcessModules && pGetModuleBaseName) {
				(*pEnumProcesses)(&array[0],sizeOfArray,&bytesNeeded);
				printf("Cantidad de procesos ---> %d\n",bytesNeeded/4);

				for (i = 0; i < bytesNeeded / 4; i++) {
					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, array[i]);

					// Get the process name.

					if (hProcess) {
						if ((*pEnumProcessModules)(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
							(*pGetModuleBaseName)(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
							printf("Proceso %s (%d)\n", szProcessName, array[i]);
							if (strcmp(szProcessName, "notepad.exe") == 0) {
								TerminateProcess(hProcess,0);
								printf("Proceso notepad terminado\n");
							}
						}
						CloseHandle(hProcess);
					}
				}
			}
		}
		Sleep(5000);	//Duérmete 5 seg.
	}

	FreeLibrary(hCodigoDll);
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

