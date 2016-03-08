#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mymallocfree.h"

// Fonction experimentale qui agrandit le heap a chaque appel
void *mymalloc(size_t t)
{
    if (t<=0 || t>536870911) return NULL;

    size_t paddingSize = t%4;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    int *headerPointer;
    int header = createHeader(realSize);
    void *ptr;
    headerPointer = sbrk(0);
    if (sbrk(t+paddingSize)==NULL) return NULL; //il essaye d'agrandir la place dispo, si ça renvoit null alors on arrete ici. Autrement, on continue.
    ptr = headerPointer+4;
    *headerPointer = header;
    return ptr;
}


//Cree une entete dont les 29 premiers bits sont la realSize et les 3 derniers sont 001 car on appelle cette fonction quand on alloue ave mymalloc
int createHeader(size_t s)
{

    int entete =(s<<3)+1;

    return entete;

}









// Main permettant de tester et compiler
int main(int argc, const char *argv[])
{
    int a;
    a = createHeader(550);
    printf("%d", a);
    return 0;
}

/*

void *mycalloc(size_t t)
{
    if (t<=0 || t>536870911) return NULL;

    size_t paddingSize = t%4;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    int *headerPointer;
    int header = createHeader(realSize);
    void *ptr;
    headerPointer = sbrk(0);
    if (sbrk(t+paddingSize)==NULL) return NULL; //il essaye d'agrandir la place dispo, si ça renvoit null alors on arrete ici. Autrement, on continue.
    ptr = headerPointer+4;
    *headerPointer = header;
    *ptr=NULL;
    return ptr;
}


*/
