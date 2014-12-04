#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include <string.h>

// returns -1 when it didnt echo because it was a backspace
void echo(char c){
    if (c == 127 || c == 8) {
        printf("\b \b");
    } else {
        printf("%c", c);
    }
}

char *cambiarEspacioDeString(char *s, int contadorMax, int cambioEnEspacio){
    char *sAux; int i;
    sAux = (char *) malloc((contadorMax + cambioEnEspacio) * sizeof(char));
    memcpy(sAux, s, contadorMax * sizeof(char));
    /*
    for (i = 0; i<contadorMax; i++){
        sAux[i] = s[i];
    }
    */
    free (s);
    return sAux;
}

char *leerUnStringArbitrariamenteLargo() {

    char *s = NULL, c, *sAux;
    int contador = 0, contadorMax = 5, i;

    s = (char*)malloc(5*sizeof(char)); // son buenos modales usar un sizeof( )
    *s='\0';

    //getch() está en conio.h, pero no hace nada en pantalla.
    c = getch();
    echo(c);
    while(c!=10){//13 es el codigo ascii del Enter
        if (c == 127 || c == 8) {// si el char era un backspace
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

int main(){
  printf("Introduce tu nombre: ");
  char *nom = leerUnStringArbitrariamenteLargo();
  printf("Este es tu nombre: %s\n", nom);
  return 0;
}
