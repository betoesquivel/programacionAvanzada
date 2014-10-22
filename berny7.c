//
//  main.c
//  Tarea7_PrograAvanzada
//
//  Created by Bernardo Trevino on 10/21/14.
//  Copyright (c) 2014 Bernardo Daniel Treviño Caballero. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *ptrArchivo;

char *getLine(FILE *archivo) {
	char *texto = NULL;
	char *auxTexto = NULL;
	int contMAX = 10;
	int contAct = 0;
	char chr;

	texto = (char *)malloc(contMAX*sizeof(char));
	while (!feof(archivo)) {
		chr = getc(archivo);
		if ((chr != 13) && (!feof(archivo))) {
			texto[contAct++] = chr;
			if (contAct == contMAX) {
				auxTexto = (char *)malloc((contMAX + 10)*sizeof(char));
				memcpy(auxTexto, texto, contMAX);
				free(texto);
				texto = auxTexto;
			}
			texto[contAct] = '\0';
		}
	}
	return(texto);
}



// main problema 1
int main(int argc, const char * argv[])
{
    char *texto;
    int i, j, contador;

    ptrArchivo = fopen(argv[1], "r");
	if (ptrArchivo != NULL) {
		printf("Archivo abierto\n");

        texto = getLine(ptrArchivo);

        printf("%s\n", texto);

		if (fclose(ptrArchivo) == 0)
			printf("Archivo cerrado\n");
		else
			printf("Error al cerrar el archivo");
	}
	else
		printf("Error al abrir el archivo\n");

    ptrArchivo = fopen(argv[1], "w");
	if (ptrArchivo != NULL) {
		printf("Archivo abierto\n");

        for (i = 0; i < strlen(texto); i++) {
            contador = 0;
            if (texto[i] == argv[2][0]) {
                contador++;
                for (j = 1; j < strlen(argv[2]); j++) {
                    if (texto[i+j] == argv[2][j]) {
                        contador++;
                    }
                }
                if (contador == strlen(argv[2]) && (texto[j] > 'Z' || texto[j] > 'z' || texto[j] < 'a' || texto[j] < 'A')) {
                    i+= contador;
                }
            }
            fputc(texto[i], ptrArchivo);
        }

		if (fclose(ptrArchivo) == 0)
			printf("Archivo cerrado\n");
		else
			printf("Error al cerrar el archivo");
	}
	else
		printf("Error al abrir el archivo\n");
}


// main problema 2
/*
int main(int argc, const char * argv[])
{
    char *texto;
    char *oraciones[1000];
    int tam[1000];
    int i, j, contador, k, x;

    ptrArchivo = fopen(argv[1], "r");
	if (ptrArchivo != NULL) {
		printf("Archivo abierto\n");

        texto = getLine(ptrArchivo);

        printf("%s\n", texto);

		if (fclose(ptrArchivo) == 0)
			printf("Archivo cerrado\n");
		else
			printf("Error al cerrar el archivo");
	}
	else
		printf("Error al abrir el archivo\n");

    int palabra = 1;

    j = 0;
    oraciones[j] = strtok(texto, "\n");

    while (oraciones[j] != NULL) {
        j++;
        oraciones[j] = strtok(NULL, "\n");
    }

    for (i = 0; i < j; i++) {
        contador = 0;
        for (x = 0; x < strlen(oraciones[i]); x++) {
            if ((oraciones[i][x] <= 'Z' && oraciones[i][x] >= 'A') || (oraciones[i][x] <= 'z' && oraciones[i][x] >= 'a')) {
                palabra = 1;
            }
            else if (palabra == 1) {
                palabra = 0;
                contador++;
            }
            if (x+1 == strlen(oraciones[i]) && ((oraciones[i][x] <= 'Z' && oraciones[i][x] >= 'A') || (oraciones[i][x] <= 'z' && oraciones[i][x] >= 'a'))) {
                contador++;
            }
        }
        tam[i] = contador;
    }

    ptrArchivo = fopen(argv[2], "w");
	if (ptrArchivo != NULL) {
		printf("Archivo abierto\n");

        int lugarMenor = 0;
        int menor;
        for (k = 0; k < j; k++) {
            menor = 999999;
            lugarMenor = -1;
            for (x = 0; x < j; x++) {
                if (tam[x] < menor && tam[x] != -1) {
                    menor = tam[x];
                    lugarMenor = x;
                }
            }
            tam[lugarMenor] = -1;
            fprintf(ptrArchivo,"%s\n", oraciones[lugarMenor]);
        }

		if (fclose(ptrArchivo) == 0)
			printf("Archivo cerrado\n");
		else
			printf("Error al cerrar el archivo");
	}
	else
		printf("Error al abrir el archivo\n");
}
*/
