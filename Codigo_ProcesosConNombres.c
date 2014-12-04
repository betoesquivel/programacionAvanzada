#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// variable que va a tener el handle a la biblioteca dinámica. 
HINSTANCE hCodigoDll;
// variables que contendrán el apuntador a las funciones de la
// biblioteca dinámica. 
typedef BOOL _stdcall TipoRutina_EnumProcesses(DWORD *, DWORD , DWORD *);
TipoRutina_EnumProcesses *pEnumProcesses;

typedef BOOL _stdcall TipoRutina_EnumProcessModules(HANDLE,HMODULE *,DWORD,LPDWORD);
TipoRutina_EnumProcessModules *pEnumProcessModules;

typedef DWORD _stdcall TipoRutina_GetModuleBaseName(HANDLE,HMODULE,LPTSTR,DWORD);
TipoRutina_GetModuleBaseName *pGetModuleBaseName;

// Se crean las variables que recibiran el handle de 
// las librerías. 
HANDLE hProcess;
HMODULE hMod;
DWORD cbNeeded;
// Se crea un string de 100 caracteres que aparentemente tendrá el nombre
// del proceso.
char szProcessName[100];

void main() {
    //Se crea un array de dwords...
	DWORD array[1024];
	DWORD sizeOfArray=sizeof(array);
	DWORD bytesNeeded=13;
	int i;

    // se carga la librería. 
	hCodigoDll=LoadLibrary("psapi.dll");
	if (hCodigoDll) {
        // se obtienen los apuntadores a las funciones de la librería
		pEnumProcesses=(TipoRutina_EnumProcesses *)GetProcAddress(hCodigoDll,"EnumProcesses");
		pEnumProcessModules=(TipoRutina_EnumProcessModules *)GetProcAddress(hCodigoDll,"EnumProcessModules");
		pGetModuleBaseName=(TipoRutina_GetModuleBaseName *)GetProcAddress(hCodigoDll,"GetModuleBaseNameA");

        // y si se consiguieron se ejecutan 
		if (pEnumProcesses && pEnumProcessModules && pGetModuleBaseName) {
            // se cuentan los procesos existentes  
			(*pEnumProcesses)(&array[0],sizeOfArray,&bytesNeeded);
			printf("Cantidad de procesos ---> %d\n",bytesNeeded/4);

            // se recorren estos procesos
			for (i = 0; i < bytesNeeded / 4; i++) {
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, array[i]);

                // para cada proceso obtengo su nombre
				// Get the process name.

				if (hProcess) {
					if ((*pEnumProcessModules)(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
						(*pGetModuleBaseName)(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
						printf("Proceso %s (%d)\n", szProcessName, array[i]);
					}
                    // se cierra el handle al proceso 
					CloseHandle(hProcess);
				}
			}
		}
        // se libera la librería
		FreeLibrary(hCodigoDll);
	}
    // se despliega en pantall el final de la ejecución del programa
	printf("Fin!\n");
}
