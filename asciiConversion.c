#include <stdio.h>

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
            return num;
        }
        num += val*pow;
        pow *= 16;
    }
    return num;
}
void main()
{
    char binary[32], hex[8], strDouble[50];
/*
    printf("Introduce un string binario:\n");
    scanf("%s", binary);
    printf("Este es el equivalente en decimal del binario %s: %d\n", binary, asciiBinaryToInt(binary));
*/
/*
    printf("Introduce un string hex:\n");
    scanf("%s", hex);
    printf("Este es el equivalente en decimal del hexadecimal %s: %d\n", hex, asciiHexToInt(hex));
*/
}
