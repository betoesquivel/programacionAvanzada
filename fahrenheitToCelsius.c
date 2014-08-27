#include <stdio.h>

float convertFahrenheitToCelsius(int f)
{
    return ( f - 32 ) * ( 5.0/9.0 );
}

int main()
{
    int fTemp;

    printf("Introduzca la temperatura en Fahrenheit: \n", &fTemp);
    scanf("%d", &fTemp);
    printf("La temperatura %d Fahrenheit es equivalente a %-7.2f Celsius.\n", fTemp, convertFahrenheitToCelsius(fTemp));

    return 0;
}
