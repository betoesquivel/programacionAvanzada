#include <stdio.h>
#include <stdlib.h>
#include "conio.h"

char *leerUnStringArbitrariamenteLargo();

/*
void main()
{
    char *string;
    printf("Dame tu nombre: ");
    string = leerUnStringArbitrariamenteLargo();
    printf(" ... nombre: %s\n", string);
}
*/

char *leerUnStringArbitrariamenteLargo()
{

    char *s = NULL, c, *sAux;
    int contador = 0, contadorMax = 5, i;

    s = (char*)malloc(5*sizeof(char)); // son buenos modales usar un sizeof( )
    *s='\0';

    //getch() está en conio.h, pero no hace nada en pantalla.
    c = getch(); printf("%c", c);
    while(c!=10){//13 es el codigo ascii del Enter
        s[contador++] = c;
        if (contador >= contadorMax)
        {
            // pedir mas memoria porque el null no va a caber
            sAux = (char *) malloc((contadorMax + 5) * sizeof(char));
            for (i = 0; i<contadorMax; i++){
                sAux[i] = s[i];
            }
            free (s);
            s = sAux;
            contadorMax+=5;
        }
        s[contador] = '\0';
        c = getch(); printf("%c", c);
    }

    //c = getch(); printf("%c", 10); // para que se coma el salto de linea

    return s;
}
