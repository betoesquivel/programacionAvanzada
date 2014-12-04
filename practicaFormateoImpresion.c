#include <stdlib.h>
#include <stdio.h>

int main(){
  double flotante = 3.14151612;
  printf("Flotante sin formato: %f\n", flotante);

  printf("Flotante con formato: %-8.4fx\n", flotante);

  return 0;
}
