#include <stdio.h>
#include <stdlib.h>

#include "readingArbitrarilyLongString.h"

//I need the file conio.h which allows me to use getch on linux
//I need readingArbitrarilyLongString.h to do precisely that

struct _persona {
    int matricula;
    char *nombre;
    char *apellido;
};
void imprimirAlumnos(struct _persona *alumnos, int numAlumnos)
{
}
int leerAlumnosBinario(struct _persona *alumnos)
{
    // se guardan alumnos
    // se guarda el tam del struct en la primera linea.
    FILE *fp;
    int *numAlumnos = malloc(sizeof(int));
    fp=fopen("alumnos.txt", "r");
    fread(numAlumnos, sizeof(int), 1, fp);
    printf("Se leyeron %d alumnos.\n", *numAlumnos);
    if (*numAlumnos){
        alumnos = malloc(sizeof(struct _persona) * *numAlumnos);
        fread(alumnos, sizeof(struct _persona), *numAlumnos, fp);
    }
    fclose(fp);

    printf("\nImprimo alumnos...\n");
    int cont = 0;
    while (cont < *numAlumnos)
    {
        printf("Matricula: %d\nNombre: %s %s\n", alumnos[cont].matricula,
                alumnos[cont].nombre, alumnos[cont].apellido);
        cont++;
    }

    return *numAlumnos;
}

int main()
{
    int numAlumnos = 0;
    struct _persona *alumnos;

    printf("Leo del archivo...\n");
    numAlumnos = leerAlumnosBinario(alumnos);

    return 0;
}

