#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void main() {
    // Se definen los nombres de los archivos que se van a observar
	char dirStr[]="C:\\Users\\ProgAva\\Temp";
	char dirFileStr[]="C:\\Users\\ProgAva\\Temp\\Algo.txt";
	char dirFileStr2[]="C:\\Users\\ProgAva\\Temp\\Algo2.txt";
    // Handle que va a manejar la notificación de cambio en los archivos
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
	while (chr!=27) {
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
		if (kbhit())
            // esperar un salto de línea o char
			chr=getch();
	}
    // se cierra y deja de escuchar por cambios
	FindCloseChangeNotification(changeNotifHandle);
	printf("Listo\n");
}
