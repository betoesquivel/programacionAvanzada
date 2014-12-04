#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/types.h>

char dirStr[]="C:\\Users\\ProgAva\\Temp";
char dirFileStr[]="C:\\Users\\ProgAva\\Temp\\Algo.txt";
char dirFileStr2[]="C:\\Users\\ProgAva\\Temp\\Algo2.txt";
HANDLE changeNotifHandle;
// Handle que va a manejar la lectura y observación de los archivos
HANDLE fileHandle;

// Declaración de variables para ayudar en la lectura e identificación de cambios en los archivos
char chr = 0;
DWORD waitResult;
int flag=0;
FILETIME lastWriteTime,t1,t2,t3;
// Guarda el último error en la ejecución del programa
DWORD lastError;

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


        // se busca el primer cambio si aún no se ha encontrado un cambio
        // si no, se busca el siguiente cambio.
		if (!flag)
        {
            // Se inicia la búsqueda por cambios en el directorio (sin contar subdirectorios 
            // con la condición de filtro de Notificar cambio.. y todo esto es regresado
            // en un handle, porque no queda de otra. 
            changeNotifHandle=FindFirstChangeNotification(dirStr,FALSE,FILE_NOTIFY_CHANGE_LAST_WRITE);
        }
		else
        {
            // pide por un handle que busca la siguiente vez que se hagan cambios
            FindNextChangeNotification(changeNotifHandle);
        }

        // se espera a ver si hubo algún cambio
		waitResult = WaitForSingleObject(changeNotifHandle,500);	//Esperar hasta 500 mseg.
		flag=1;
		switch(waitResult) {
			case WAIT_OBJECT_0:
                    // Hubo un cambio en los archivos
					printf("Cambio algun archivo\n");
					Sleep(50);	//Dar un tiempo para evitar el "LOCK" cuando se esta grabando el archivo
                    // Se abre el archivo para buscar los cambios, solamente con permisos de lectura, y de que el 
                    // archivo ya exista. 
					fileHandle=CreateFile(dirFileStr,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
					if (fileHandle!=INVALID_HANDLE_VALUE) {
                        // se obtiene la info del archivo
						if(GetFileTime(fileHandle,&t1,&t2,&t3)) {
							//printf("Time: %ld, %ld\n",t3.dwHighDateTime,t3.dwLowDateTime);
                            // Si el último tiempo de cambios es diferente a el 3er tiempo registrado de cambios
                            // o si es diferente al primer tiempo registrado de este tercer tiempo
							if ((t3.dwHighDateTime!=lastWriteTime.dwHighDateTime)||(t3.dwLowDateTime!=lastWriteTime.dwLowDateTime)) {
								printf("El archivo cambio\n");
                                // se copia el tiempo de la última escritura, en la variable t3
								memcpy(&lastWriteTime,&t3,sizeof(FILETIME));
								//unlink(dirFileStr2);
                                // copia un archivo existente a otro existente.
								CopyFile(dirFileStr,dirFileStr2,FALSE);
							} else {
                                // el archivo sigue siendo el mismo
								printf("El archivo NO cambio\n");
							}
						}
                        // Se cierra el handle para manipular los archivos
						CloseHandle(fileHandle);
					} else {
                        // se pie el último error 
						lastError=GetLastError();
						if (lastError == ERROR_SHARING_VIOLATION)	//¡Ups!, que mala suerte el archivo cambio y esta LOCKED
							printf("El archivo cambio!\n");
						else if (lastError == ERROR_FILE_NOT_FOUND) {
							printf("Borraron el achivo ... que hacemos?\n");
							return;		//Por lo pronto finalizar este programa
						} else
							printf("INVALID_HANDLE_VALUE! ... %ld\n",lastError);
					}
					break;
			case WAIT_TIMEOUT:
                    // en caso de que se siga en espera
					printf(".");
					break;
			default:
                    // así se maneja el error
					printf("ERROR!!!\n");
					break;
		}

		Sleep(1000);	//Duérmete 1 seg.
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
    //
	//Asegurar tiempos de facto para el archivo
	lastWriteTime.dwHighDateTime=0;	//Asignar los 32bits HIGH
	lastWriteTime.dwLowDateTime=0;	//Asignar los 32bits LOW
	//Abrir un "handle" con el archivo para obtener (inicialmente) el tiempo de ultima escritura
	fileHandle=CreateFile(dirFileStr,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	//Si el archivo no es inválido 
	if (fileHandle!=INVALID_HANDLE_VALUE) {
		if(GetFileTime(fileHandle,&t1,&t2,&t3))
        {
            // Se obtiene el tiempo de la última escritura en el archivo
            memcpy(&lastWriteTime,&t3,sizeof(FILETIME));
        }
        // Se cierra el handle.
		CloseHandle(fileHandle);
	} else {
        // Hubo un error al abrir o crear el archivo
		lastError=GetLastError();
        // Si el error fue que no se encontró el archivo...
		if (lastError==ERROR_FILE_NOT_FOUND)
			return;	//Archivo no localizado, quizas lo borraron ... terminar el programa
		//Otra posibilidad del error es que el archivo este abierto y LOCKED ...
		// ... habrá que decidir como proceder (terminar el programa también o ciclarse esperando)
		// ... pero esto no lo vamos a hacer aquí simplemente tomamos valores de tiempo en ceros.
	}

	elThread=CreateThread(NULL,0,corre,NULL,0,&elThreadID);

	FindCloseChangeNotification(changeNotifHandle);
	printf("Listo\n");
}

