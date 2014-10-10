#include <stdio.h>
#include <stdlib.h>
#include "conio.h"

char *cambiarEspacioDeString(char *s, int contadorMax, int cambioEnEspacio){
    char *sAux; int i;
    sAux = (char *) malloc((contadorMax + cambioEnEspacio) * sizeof(char));
    for (i = 0; i<contadorMax; i++){
        sAux[i] = s[i];
    }
    free (s);
    return sAux;
}

int protectedEcho(char c){
    if (c == 127 || c == 8) {
        printf("\b \b");
    } else {
        if (c >= 'A' && c <= 'Z' ||
            c >= 'a' && c <= 'z' ||
            c >= '0' && c <= '9') {
            printf("*");
        } else {
            return 0;
        }
    }
    return 1;
}

int floatEcho(char c, int hasDot, int signPossible){
    if (c == 127 || c == 8) {
        printf("\b \b");
    } else {
        if (!signPossible && ( c == '+' || c == '-' )){
                return 0;
        }
        if (hasDot && c == '.'){
                return 0;
        }
        if (c == '.' || c == '+' || c == '-' || ( c >= '0' && c <= '9' )) {
            printf("%c", c);
        }else{
            return 0;
        }
    }
    return 1;
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

char *readPassword(int l)
{

    char *s = NULL, c;
    int contador = 0, contadorMax = 5, i, valid;

    s = (char*)malloc(5*sizeof(char)); // son buenos modales usar un sizeof( )
    *s='\0';

    //getch() está en conio.h, pero no hace nada en pantalla.
    c = getch();
    valid = protectedEcho(c);
    while(contador < l && c != 13){//13 es el codigo ascii del Enter
        if(valid){
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
        }
        if (contador < l){
            c = getch();
            valid = protectedEcho(c);
        }
    }
    return s;
}

char *readFloatAsString()
{

    char *s = NULL, c;
    int contador = 0, contadorMax = 5, i, valid, hasDot = 0, signPossible = 1;

    s = (char*)malloc(5*sizeof(char)); // son buenos modales usar un sizeof( )
    *s='\0';

    //getch() está en conio.h, pero no hace nada en pantalla.
    c = getch();
    valid = floatEcho(c, hasDot, signPossible);
    while(c != 27 && c != 10){//27 es el codigo ascii del ESC
        if(valid){
            signPossible = 0;
            if (c == 127 || c == 8) {
                if (contador > 0) {
                    if (s[contador - 1] == '.') hasDot = 0;
                    s[--contador] = '\0';
                    if(contadorMax - contador > 5){
                        contadorMax -= 5;
                        s = cambiarEspacioDeString(s, contadorMax, 0);
                    }
                }else{
                    signPossible = 1;
                }
            }else {
                if (c == '.') hasDot = 1;
                s[contador++] = c;
                if (contador >= contadorMax)
                {
                    // pedir mas memoria porque el null no va a caber
                    s = cambiarEspacioDeString(s, contadorMax, 5);
                    contadorMax+=5;
                }
                s[contador] = '\0';
            }
        }
        c = getch();
        valid = floatEcho(c, hasDot, signPossible);
    }
    if (c == 27) return NULL;

    return s;
}

