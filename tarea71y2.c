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


int main (int argc, char *argv[])
{
    char *nomArch = argv[1];
    char *palabra = argv[2];
    int longitudPalabra = strlen(palabra);
    int longitudTexto;

    //dejo espacio para el \0 y para el espacio o salto de linea
    char *palabraAux;
    char *textoMod;

    char *texto;
    ptrArchivo = fopen(nomArch, "r");
    if (ptrArchivo != NULL) {
        printf("Archivo abierto\n");

        texto = getLine(ptrArchivo);
        printf ("%s %s\n", texto, palabra);
        textoMod = malloc ((strlen(texto) + 1) * sizeof(char));
        palabraAux = malloc ((strlen(texto) + 1) * sizeof(char));
        longitudTexto = strlen(texto);
        textoMod[0] = '\0';

        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");

    ptrArchivo = fopen(nomArch, "a");
    if (ptrArchivo != NULL) {
        printf("Archivo abierto\n");

        palabraAux = strtok (texto, " ,.-\n");
        while (palabraAux)
        {
            if (strcmp(palabra, palabraAux)){
                //no son iguales
                strcat(textoMod, palabraAux);
            }
            free (palabraAux);
            palabraAux = strtok (NULL, " ,.-");
        }
        printf ("Esto llevo concat: %s \n", textoMod);
        /*
        for (i = 0; i < longitudTexto; i++)
        {
            j = 0;
            while (texto[i] != ' ' && texto[i] != 10 && texto[i] != 13 && texto[i]){
                palabraAux[j] = texto[i];
                j++;
                i++;
            }
            palabraAux[j] = '\0';

            printf ("Esta es la palabra aux: %s \n", palabraAux);
            printf ("Un string vacio: %d\n", (int)'\0');
            //tengo una palabra en palabraAux asi que comparo
            if (j != longitudPalabra || strcmp(palabra, palabraAux) ){
                //no son iguales
                //concateno la palabra al textoMod
                // Falta agregar ' ' o '\n'
                palabraAux[j] = texto[i];
                palabraAux[++j] = '\0';
                strcat(textoMod, palabraAux);
            }
        }
        */
        //printf("Texto MOD:\n %s", textoMod);

        if (fclose(ptrArchivo) == 0)
            printf("Archivo cerrado\n");
        else
            printf("Error al cerrar el archivo");
    }
    else
        printf("Error al abrir el archivo\n");
    return 0;
}
