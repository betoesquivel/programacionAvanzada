#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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

int main (int argc, char *args[])
{
    char *nomArch = args[0];
    char *palabra = args[1];

    char *linea;
    ptrArchivo = fopen(nomArch, "r");
    if (ptrArchivo != NULL) {
        printf("Archivo abierto\n");

        while (!feof(ptrArchivo)) {
            linea = getLine(ptrArchivo);
            printf ("%s %s", linea, palabra);
        }

        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");
    return 0;
}
