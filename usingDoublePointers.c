#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"

struct _persona
{
    int matricula;
    char nombre[20];
    char apellido[20];
    struct _persona *siguiente;
};

struct _persona *lista = NULL;
clock_t start, end;

void ordenarListaIncrementalmentePorMatricula()
{
    struct _persona *nuevaLista = NULL;
    struct _persona *nodoMayor;
    struct _persona *nodoAnteriorAlMayor;

    while(lista){

    }

}

struct _persona* buscaNodoMatriculaMayor(struct _persona *lista){
    struct _persona *nodoMayor = NULL;
    nodoMayor = lista;
    lista = lista->siguiente;
    while(lista)
    {
        if (nodoMayor->matricula < lista->matricula)
            nodoMayor = lista;
        lista = lista->siguiente;
    }
    return nodoMayor;
}

void imprimirLista()
{
    struct _persona *nodo = lista;
    while (nodo)
    {
        printf("%d\t%s\t%s\n", nodo->matricula, nodo->nombre, nodo->apellido);
        nodo = nodo->siguiente;
    }
}

void freeList()
{
    struct _persona *nodo = lista;
    while (lista)
    {
        nodo = lista;
        free(nodo->nombre);
        free(nodo->apellido);
        lista = lista->siguiente;
        free(nodo);
    }
}

int main(int argc, const char *argv[])
{
    struct _persona *nuevoNodo;
    struct _persona *nodo;
    int mat;
    do{
        printf("Dame tu matricula: ");
        scanf("%d", &mat);

        if(mat > 0){
            nuevoNodo = (struct _persona*)malloc(sizeof(struct _persona));
            nuevoNodo->matricula = mat;

            printf("Dame tu nombre: ");
            scanf("%s", &nuevoNodo->nombre[0]);
            printf("Dame tu apellido: ");
            scanf("%s", &nuevoNodo->apellido[0]);

            nuevoNodo->siguiente = lista;
            lista = nuevoNodo;
        }
    }while(!mat);

    return 0;
}
