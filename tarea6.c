//A01139626 Jose Alberto E
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conio.h"// en windows usa <conio.h>, pero yo bajé mi propia versión para linux

char *cambiarEspacioDeString(char *s, int contadorMax, int cambioEnEspacio){
    char *sAux; int i;
    sAux = (char *) malloc((contadorMax + cambioEnEspacio) * sizeof(char));
    for (i = 0; i<contadorMax; i++){
        sAux[i] = s[i];
    }
    free (s);
    return sAux;
}

void echo(char c){
    if (c == 127 || c == 8) {
        printf("\b \b");
    }
    else {
        printf("%c", c);
    }
}


char *leerUnStringArbitrariamenteLargo()
{

    char *s = NULL, c;
    int contador = 0, contadorMax = 5, i;

    s = (char*)malloc(5*sizeof(char)); // son buenos modales usar un sizeof( )
    *s='\0';

    //getch() está en conio.h, pero no hace nada en pantalla.
    c = getch();
    echo(c);
    while(c!=10){//13 es el codigo ascii del Enter
        if (c == 127 || c == 8) {
            if (contador > 0) {
                s[--contador] = '\0';
                if(contadorMax - contador > 5){
                    contadorMax -= 5;
                    s = cambiarEspacioDeString(s, contadorMax, 0);
                }
            }
        }else {
            s[contador++] = c;
            if (contador >= contadorMax)
            {
                // pedir mas memoria porque el null no va a caber
                s = cambiarEspacioDeString(s, contadorMax, 5);
                contadorMax+=5;
            }
            s[contador] = '\0';
        }
        c = getch();
        echo(c);
    }
    return s;
}

//I need the file conio.h which allows me to use getch on linux
//I need readingArbitrarilyLongString.h to do precisely that

struct _persona {
    int matricula;
    char nombre[20];
    char apellido[20];
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
char *toUpper(const char *s)
{
    int length = strlen(s);
    char *upper = malloc(length * sizeof(char) + 1);
    int i;
    for(i=0;s[i];i++){
        if (s[i] >= 'a' && s[i] <= 'z') {
            upper[i] = s[i] - ('a' - 'A');
        }
        else {
            upper[i] = s[i];
        }
    }
    upper[i] = '\0';
    return upper;
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
void ordenarListaIncrementalmentePorMatricula(struct _persona **alumnos, int cont)
{
    int elMasPeque, elOtro;
    struct _persona aux;
    struct _persona *lista = *alumnos;
    int contadorInterno;

    elMasPeque = 0;
    elOtro = 1;
    while(cont -1){
        contadorInterno = cont;
        while(contadorInterno-1)
        {
            if (lista[elMasPeque].matricula>lista[elOtro].matricula)
            {
                memcpy(&aux,&lista[elMasPeque], sizeof(struct _persona));
                memcpy(&lista[elMasPeque],&lista[elOtro], sizeof(struct _persona));
                memcpy(&lista[elOtro],&aux, sizeof(struct _persona));
            }
            contadorInterno--;
            elOtro++;
        }
        elMasPeque++;
        elOtro=elMasPeque+1;
        cont--;
    }
}
void ordenarListaIncrementalmentePorApellido(struct _persona **alumnos, int cont)
{
    int elMasPeque, elOtro;
    struct _persona aux;
    struct _persona *lista = *alumnos;
    int contadorInterno;

    elMasPeque = 0;
    elOtro = 1;
    while(cont -1){
        contadorInterno = cont;
        while(contadorInterno-1)
        {
            if (strcmp(toUpper(lista[elMasPeque].apellido), toUpper(lista[elOtro].apellido)) > 0)
            {
                memcpy(&aux,&lista[elMasPeque], sizeof(struct _persona));
                memcpy(&lista[elMasPeque],&lista[elOtro], sizeof(struct _persona));
                memcpy(&lista[elOtro],&aux, sizeof(struct _persona));
            }
            contadorInterno--;
            elOtro++;
        }
        elMasPeque++;
        elOtro=elMasPeque+1;
        cont--;
    }
}
int main()
{
    int maxAlumnos = 5; // valor para cambiar la dimension
    int numAlumnos = 0; // valor con la cantidad de alumnos en la lista.
    struct _persona *alumnos = malloc(maxAlumnos * sizeof(struct _persona));

    int num;
    do {
        printf("Dame matricula: \n");
        scanf("%d",&num);
        if (num>0) {
            alumnos[numAlumnos].matricula = num;

            printf("Dame nombre: \n");
            scanf("%s", alumnos[numAlumnos].nombre);
            printf("Dame apellido: \n");
            scanf("%s", alumnos[numAlumnos].apellido);

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

    ordenarListaIncrementalmentePorMatricula(&alumnos, numAlumnos);
    printf("\nImprimo alumnos ordenados por mat...\n");
    imprimirAlumnos(alumnos, numAlumnos);

    ordenarListaIncrementalmentePorApellido(&alumnos, numAlumnos);
    printf("\nImprimo alumnos ordenados por apellido...\n");
    imprimirAlumnos(alumnos, numAlumnos);
    return 0;
}
