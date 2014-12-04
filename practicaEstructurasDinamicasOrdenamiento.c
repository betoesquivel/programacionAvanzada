#include <stdio.h>
#include <stdlib.h>
#include "readingArbitrarilyLongString.h"

struct _persona{
  int m;
  char *n;
  char *a;
  struct _persona *next;
};

void ordenarListaPersonasPorMatricula(struct _persona **listaPersonas){
  struct _persona *lista = *listaPersonas;

  //insertion sort
  struct _persona *aux = lista, *auxOrd, *ordenada = NULL;

  struct _persona *nueva;

  nueva = (struct _persona*) malloc (sizeof(struct _persona));

  ordenada = nueva;
  aux = aux->next;
  int sLength = 0;
  while(aux){
    nueva->m = aux->m;

    sLength = strlen(aux->n);
    nueva->n = (char*) malloc (sizeof(char) * sLength);
    memcpy(nueva->n,aux->n,sizeof(char) * sLength);

    sLength = strlen(aux->a);
    nueva->a = (char*) malloc (sizeof(char) * sLength);
    memcpy(nueva->a,aux->a,sizeof(char) * sLength);

    printf("Ordenando al alumno: %d, %s\n", nueva->m, nueva->n);

    auxOrd = ordenada;
    struct _persona *anterior;
    while(auxOrd && nueva->m <= auxOrd->m){
       anterior = auxOrd;
       auxOrd = auxOrd->next;
    }
    anterior->next = nueva;
    nueva->next = auxOrd;

    aux = aux->next;
  }
  *listaPersonas = ordenada;

}

void llenarListaPersonas(struct _persona **listaPersonas){
  struct _persona *personas = *listaPersonas;
  struct _persona *nueva = (struct _persona*) malloc(sizeof(struct _persona));
  personas = nueva;

  struct _persona *anterior;

  int m;

  printf("Matricula: ");
  scanf("%d",&m); getch();
  while(m != 0){
    nueva->m = m;

    printf("Nombre: ");
    nueva->n = leerUnStringArbitrariamenteLargo();

    printf("Apellido: ");
    nueva->a = leerUnStringArbitrariamenteLargo();


    printf("Matricula: ");
    scanf("%d",&m); getch();

    anterior = nueva; 
    nueva = (struct _persona*)malloc(sizeof(struct _persona));
    anterior->next = nueva;
  }

  anterior->next = NULL;
  free(nueva);
  *listaPersonas = personas;
}

void imprimir(struct _persona *l){
  struct _persona *aux = l;
  while(aux){
    printf("Mat: %d\nNom: %s\nAp: %s\n", aux->m, aux->n, aux->a);
    aux = aux->next;
  }
}

int main(){
  struct _persona *lista = NULL;
  llenarListaPersonas(&lista);

  printf("Sin ordenar:\n");
  imprimir(lista);
  ordenarListaPersonasPorMatricula(&lista);
  printf("Ordenada por matricula:\n");
  imprimir(lista);

  return 0;
}
