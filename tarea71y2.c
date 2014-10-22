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

/*
int main (int argc, char *argv[])
{
    char *nomArch = argv[1];
    char *palabra = argv[2];
    int longitudPalabra = strlen(palabra);
    int longitudTexto;
    int i, j;

    //dejo espacio para el \0 y para el espacio o salto de linea
    char *palabraAux;
    char *textoMod;

    char *texto;
    ptrArchivo = fopen(nomArch, "r");
    if (ptrArchivo != NULL) {
        printf("Archivo abierto\n");

        texto = getLine(ptrArchivo);
        printf ("%s %s\n", texto, palabra);
        longitudTexto = strlen(texto);

        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");

    ptrArchivo = fopen(nomArch, "w");
    if (ptrArchivo != NULL) {

        for (i = 0; i<longitudTexto; i++)
        {
            j = 0;
            while (j<longitudPalabra && texto[i+j] == palabra[j])
            {
                j++;
            }
            if(j == longitudPalabra && !(texto[j+i] <= 'z' && texto[j+i] >= 'a') && !(texto[j+i] <= 'Z' && texto[j+i] >= 'A'))
                i += j;
            fputc(texto[i], ptrArchivo);
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
*/

int contarPalabras(char *s)
{
    int length = strlen(s), i, palabras = 0, pasadoNoDelimitador = 1;
    for (i = 0; i<length; i++)
    {
        if(!( s[i]<= 'z' && s[i]>='a' ) && !( s[i]<='Z' && s[i] >= 'A') )
        {
            //entonces es un delimitador
            if(pasadoNoDelimitador)
            {
                palabras++;
                pasadoNoDelimitador = 0;
            }
        }else{
            pasadoNoDelimitador = 1;
        }
    }
    if(!pasadoNoDelimitador){
        palabras--;
    }
    return palabras+1;
}

int main (int argc, char *argv[])
{
    char *nomArchEnt = argv[1];
    char *nomArchSal = argv[2];
    char *oraciones[1000];

    int longitudTexto;

    //dejo espacio para el \0 y para el espacio o salto de linea
    char *texto;
    ptrArchivo = fopen(nomArchEnt, "r");
    if (ptrArchivo != NULL) {
        printf("Archivo abierto\n");

        texto = getLine(ptrArchivo);
        longitudTexto = strlen(texto);

        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");

    printf("Estas son las oraciones\n");
    int i = 0;
    oraciones[i] = strtok(texto, "\n");
    while (oraciones[i]){
        i++;
        oraciones[i] = strtok(NULL, "\n");
    }

    i = 0;
    while (oraciones[i]){
        printf("%s\nCon %d palabras.\n", oraciones[i], contarPalabras(oraciones[i]));
        i++;
    }

    ptrArchivo = fopen(nomArchSal, "w");
    if (ptrArchivo != NULL) {


        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");
    return 0;
}
