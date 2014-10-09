#include <stdio.h>
#include "conio.h"
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

    return 0;
}
