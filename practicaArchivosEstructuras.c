#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include <string.h>

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

struct _persona {
  int matricula;
  char *nombre;
  char *apellido;
};

struct _persona *cambiarEspacioDeVector(struct _persona *p, int tamanio, int cambioEnEspacio){
    struct _persona *pAux;
    pAux = (struct _persona *) malloc((tamanio + cambioEnEspacio) * sizeof(struct _persona));
    memcpy(pAux, p, tamanio * sizeof(struct _persona));
    free (p);
    return pAux;
}

int llenarPersonas(struct _persona **listaPersonas){
  int max = 5;
  struct _persona *personas = (struct _persona*) malloc( max * sizeof(struct _persona));
  personas = *listaPersonas;
  int matricula;

  int i = 0;
  printf("Matricula: ");
  scanf("%d", &matricula);
  while (matricula != 0){
    getch();
    personas[i].matricula = matricula;
    printf("Nombre: ");
    personas[i].nombre = leerUnStringArbitrariamenteLargo();
    printf("Apellido: ");
    personas[i].apellido = leerUnStringArbitrariamenteLargo();

    i++;
    if (i >= max) {
      personas = cambiarEspacioDeVector(personas, max, 5);
      max += 5;
    }

    printf("Matricula: ");
    scanf("%d", &matricula);
  }
  *listaPersonas = personas;
  return i;
}

int main(){
  struct _persona *lista;
  int tam = 0;
  tam = llenarPersonas(&lista);

  int i;
  for (i = 0; i<tam; i++){
    printf("Mat: %d\nNom: %s\nAp: %s\n", lista[i].matricula, lista[i].nombre, lista[i].apellido);
  }

  FILE *fp;
  fp = fopen("personas.dat", "w");
  int sLength;
  fwrite(&tam, sizeof(int), 1, fp);
  for (i = 0; i<tam; i++){
    fwrite(&lista[i].matricula, sizeof(int), 1, fp);

    sLength = strlen(lista[i].nombre);
    fwrite(&sLength, sizeof(int), 1, fp);
    fwrite(lista[i].nombre, sizeof(char), sLength + 1, fp);

    sLength = strlen(lista[i].apellido);
    fwrite(&sLength, sizeof(int), 1, fp);
    fwrite(lista[i].apellido, sizeof(char), sLength + 1, fp);
  }
  fclose(fp);

  printf("Termine de guardar el archivo.\n");

  fp = fopen("personas.dat", "r");
  fread(&tam, sizeof(int), 1, fp);
  struct _persona *dbP = (struct _persona *) malloc (sizeof(struct _persona) * tam);
  for (i = 0; i<tam; i++){
    fread(&dbP[i].matricula, sizeof(int), 1, fp);

    fread(&sLength, sizeof(int), 1, fp);
    dbP[i].nombre = (char *)malloc(sizeof(char)*sLength + 1);
    fread(dbP[i].nombre, sizeof(char), sLength + 1, fp);

    fread(&sLength, sizeof(int), 1, fp);
    dbP[i].apellido = (char *)malloc(sizeof(char)*sLength + 1);
    fread(dbP[i].apellido, sizeof(char), sLength + 1, fp);
  }
  fclose(fp);

  printf("Termine de leer el archivo.\n");

  printf("En el archivo.\n");
  for (i = 0; i<tam; i++){
    printf("Mat: %d\nNom: %s\nAp: %s\n", dbP[i].matricula, dbP[i].nombre, dbP[i].apellido);
  }


  return 0;
}
