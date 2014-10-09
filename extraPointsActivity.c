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

struct _persona *cambiarEspacioEstructura(struct _persona *alumnos, int contadorMax, int cambioEnEspacio){
    struct _persona *alumnosAux; int i;
    alumnosAux = (struct _persona *) malloc((contadorMax + cambioEnEspacio) * sizeof(struct _persona));
    for (i = 0; i<contadorMax; i++){
        alumnosAux[i] = alumnos[i];
    }
    free (alumnos);
    return alumnosAux;
}

void guardarAlumnosBinario(struct _persona *alumnos, int numAlumnos)
{
    // se guardan alumnos
    // se guarda el tam del struct en la primera linea.
    int cont = 0;
    FILE *fp;
    fp=fopen("alumnos.txt", "w");
    fwrite(&numAlumnos, sizeof(int), 1, fp);
    fwrite(alumnos, sizeof(struct _persona), numAlumnos, fp);
    fclose(fp);
}
void imprimirAlumnos(struct _persona *alumnos, int numAlumnos)
{
    int cont = 0;
    while (cont < numAlumnos)
    {
        printf("Matricula: %d\nNombre: %s %s\n", alumnos[cont].matricula,
                alumnos[cont].nombre, alumnos[cont].apellido);
        cont++;
    }
}
int leerAlumnosBinario(struct _persona *alumnos)
{
    // se guardan alumnos
    // se guarda el tam del struct en la primera linea.
    int cont = 0;
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

    while (cont < *numAlumnos)
    {
        printf("Matricula: %d\nNombre: %s %s\n", alumnos[cont].matricula,
                alumnos[cont].nombre, alumnos[cont].apellido);
        cont++;
    }
    //ciclar y guardar length string length string

    return *numAlumnos;
}

int main()
{
    int maxAlumnos = 5; // valor para cambiar la dimension
    int numAlumnos = 0; // valor con la cantidad de alumnos en la lista.
    struct _persona *alumnos = malloc(maxAlumnos * sizeof(struct _persona));

    int num;
    char *nom, *ap;
    do {
        printf("Dame matricula: ");
        scanf("%d",&num);
        if (num>0) {
            getch();// removes \n from last scan...
            printf("Dame nombre: ");
            nom = leerUnStringArbitrariamenteLargo();
            printf("Dame apellido: ");
            ap = leerUnStringArbitrariamenteLargo();

            //asigno en posicion numAlumnos y luego incremento el pointer
            alumnos[numAlumnos].matricula=num;
            alumnos[numAlumnos].nombre = nom;
            alumnos[numAlumnos].apellido = ap;
            numAlumnos++;

            if (numAlumnos >= maxAlumnos)
            {
                // pedir mas memoria porque el null no va a caber
                alumnos = cambiarEspacioEstructura(alumnos, maxAlumnos, 5);
                maxAlumnos+=5;
            }
        }
    } while(num!=0);

    printf("\nImprimo alumnos...\n");
    imprimirAlumnos(alumnos, numAlumnos);
    printf("Guardo en el archivo...\n");
    guardarAlumnosBinario(alumnos, numAlumnos);

    free(alumnos);
    printf("Leo del archivo...\n");
    numAlumnos = leerAlumnosBinario(alumnos);

//   printf("\nImprimo alumnos...\n");
//   imprimirAlumnos(alumnos, numAlumnos);

    return 0;
}
