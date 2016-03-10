/* Prototype de myMalloc
* Gregory Creupelandt
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>


// Fonction experimentale qui agrandit le heap a chaque appel
void *mymalloc(size_t t)
{
    if (t<=0 || t>536870911) return NULL;

    size_t paddingSize = t%4;
    if (paddingSize != 0) paddingSize=4-paddingSize;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    int *headerPointer;
    int header = createHeader(realSize);
    void *ptr;
    headerPointer = sbrk(0);
    if (sbrk(realSize)==NULL) return NULL; //il essaye d'agrandir la place dispo, si ça renvoit null alors on arrete ici. Autrement, on continue.
    ptr = headerPointer+4;
    *headerPointer = header;
    return ptr;
}



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
    memset(ptr,0, (realSize-4));
    return ptr;
}

void myfree(void *ptr)
{
 *((&ptr)-4)=*((&ptr)-4)-1;
	
	
}


// Main permettant de tester et compiler
int main(int argc, const char *argv[])
{
 int *a = (int *) mycalloc(sizeof(int)*2);
 printf("%d", *a);
 int *b=(&a)-4;
 printf("%d",*b);
 
 myfree(a);
 
 printf("%d",*b);
}
