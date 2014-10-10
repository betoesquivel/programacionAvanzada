#include <stdio.h>
#include <stdlib.h>

int main()
{
    int numAlumnos = 0; // valor con la cantidad de alumnos en la lista.

    FILE *fp;
    fp=fopen("alumnos.txt", "r");

    int num, lenN, lenAp;
    char *nom, *ap;
    fread(&numAlumnos, sizeof(int), 1, fp);
    while(numAlumnos)
    {
        // leer matricula
        fread(&num, sizeof(int), 1, fp);
        printf("Matricula: %d\n", num);
        // leer nombre
        fread(&lenN, sizeof(int), 1, fp);
        nom = malloc(sizeof(char) * lenN + 1);
        fread(nom, sizeof(char), lenN, fp);
        nom[lenN] = '\0';

        printf("Nombre: %s\n", nom);
        // leer apellido
        fread(&lenAp, sizeof(int), 1, fp);
        ap = malloc(sizeof(char) * lenAp + 1);
        fread(ap, sizeof(char), lenAp, fp);
        ap[lenAp] = '\0';
        printf("Apellido: %s\n", ap);

        numAlumnos--;
    }
    fclose(fp);

//   printf("\nImprimo alumnos...\n");
//   imprimirAlumnos(alumnos, numAlumnos);

    return 0;
}

