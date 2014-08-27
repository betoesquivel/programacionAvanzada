#include <stdio.h>

long long factorial(long long f)
{
    if ( f <= 0 ) return 1;
    return f * factorial( f - 1 );
}

int main()
{
    long long num;

    printf("Introduzca el número al que le quiere sacar factorial: \n", &num);
    scanf("%lld", &num);
    printf("El factorial de %lld es %lld.\n", num, factorial(num));

    return 0;
}
