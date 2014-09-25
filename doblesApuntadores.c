#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _persona
{
    int matricula;
    char nombre[20];
    char apellido[20];
    struct _persona *siguiente;
};

struct _persona *lista = NULL;
clock_t start, end;

void ordenarListaIncrementalmentePorMatricula(struct _persona *lista, int cont)
{

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
