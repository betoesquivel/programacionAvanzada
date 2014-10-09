#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _persona {
    int matricula;
    char nombre[20];
    char apellido[20];
    struct _persona *sigNodo;
};

struct _persona *lista = NULL;

void main() {
    struct _persona *nuevoNodo;
    struct _persona *nodo;
    int num;

    do {
        printf("Dame matricula: ");
        scanf("%d",&num);
        if (num>0) {
            nuevoNodo = (struct _persona *)malloc(sizeof(struct _persona));
            nuevoNodo->matricula=num;
            printf("Dame nombre: ");
            scanf("%s",&nuevoNodo->nombre[0]);
            printf("Dame apellido: ");
            scanf("%s",&nuevoNodo->apellido[0]);
            //---------------------------------------------
            nuevoNodo->sigNodo=lista;
            lista = nuevoNodo;
        }
    } while(num!=0);

    nodo = lista;
    while(nodo!=NULL) {
        printf("%d: %s %s\n",nodo->matricula,nodo->nombre,nodo->apellido);
        nodo=nodo->sigNodo;
    }
    nodo = lista;
    while(nodo!=NULL) {
        printf("%d: %s %s\n",nodo->matricula,nodo->nombre,nodo->apellido);
        nodo=nodo->sigNodo;
    }
    while(lista) {
        nodo=lista;
        lista=lista->sigNodo;
        free(nodo);
    }
}
