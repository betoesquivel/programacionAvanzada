#include <stdio.h>

#include "lecturaArbitraria.h"

int stringLength(const char *s)
{
    int len = 0;
    for (; *s; s++){
        len++;
    }
    return len;
}
int asciiBinaryToInt(char *s)
{
    int num = 0, pow, i;
    int length = stringLength(s);
    pow = 1;

    for (i = length - 1; i>=0; i--){
        if (s[i] == '1'){
            num += pow;
        }else if (s[i] != '0'){
            return num;
        }
        pow *= 2;
    }
    return num;
}
int asciiHexToInt(char *s)
{
    int num = 0, pow, i, val;
    int length = stringLength(s);
    pow = 1;
    for (i = length - 1; i>=0; i--){
        if( s[i] >= 'A' && s[i] <= 'F'){
            val = s[i] - 'A' + 10;
        }else if ( s[i] >= 'a' && s[i] <= 'f'){
            val = s[i] - 'a' + 10;
        }else if ( s[i] >= '0' && s[i] <= '9'){
            val = s[i] - '0';
        }else{
            printf ("Caracter inválido.\n");
            return -1;
        }
        num += val*pow;
        pow *= 16;
    }
    return num;
}
double asciiToDouble(char *s)
{
    double num = 0;
    int i = 0, pointFound = 0, value = 0;
    double decimalPosicion = 10;
    int signo = 1;
    if (*s == '-')
    {
        signo = -1;
        s++;
    }else if (*s == '+'){
        signo = +1;
        s++;
    }
    int length = stringLength(s);

    for(i = 0; i < length; i++){
        if (!pointFound){
            if(s[i] == '.') {
                pointFound = 1;
            }else {
                value = s[i] - '0';
                //printf("num = %f, s[i] = %c, value = %d\n", num, s[i], value);
                num = 10 * num + value;
            }
        }else{
            if(s[i] == '.') {
                return num;
            }
            value = s[i] - '0';
            num += value*1.0 / decimalPosicion;
            decimalPosicion *= 10;
        }
    }

    return num * signo;
}
void main()
{
    char *hexString;
    int x;

    printf("Cuando quiera terminar, introduzca el $.\n");
    printf("Introduzca el primer numero a convertir:\n");
    while (*(hexString = leerUnStringArbitrariamenteLargo()) != '$'){
        x = asciiHexToInt(hexString);
        if (x != -1)
            printf("Este es el equivalente en decimal del hexadecimal %s: %d\n", hexString, x);
        printf("Introduzca otro numero a convertir:\n");
    }
}
