/* Prototype de myMalloc
* Gregory Creupelandt
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mymalloc.h"


void *base_heap;
size_t memsize = 1000;


void *mymalloc(size_t t)
{
	
if (t<=0 || t>536870911) return NULL; 

    size_t paddingSize = t%4;
    if (paddingSize != 0) paddingSize=4-paddingSize;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    
if(base_heap==NULL)  // premier 
{
	printf("premier");
	base_heap=sbrk(0);
	sbrk(memsize);
	if (realSize>memsize) return NULL; //modifie
	struct bloc_header *entete = base_heap;
	 entete->size = realSize;
	 entete->zero=0;
	 entete->alloc=1;
	return (void*) (base_heap+4);
	
}
 //cas general
 
 
 void *temp = base_heap;
 struct bloc_header *temp2;
 unsigned int taille;

 
 
 while(temp<(base_heap+memsize)) // on parcourt les adresses de base_heap jusqu'à la limite
 { 
	 
	temp2 = (struct bloc_header*) temp; //cast
	//taille = temp2->size;
	taille = ((struct bloc_header *) temp)->size;
    printf("MERDE %d et CHIOTTE %d", taille, ((struct bloc_header *) temp)->size);
  if (taille==realSize ) // ils ont la meme taille && temp2->alloc==0
   {	
	   temp2->alloc=1;
	    return temp2+4;
	}
	
  if (taille>realSize ) // on peut mettre dedans && temp2->alloc==0
	{
		temp2->alloc=1; //on modifie le header existant
		temp2->size=realSize;
		
		
		struct bloc_header *temp3 = (temp2+(realSize)/4); //on va a l'adresse ou on mettra le prochain header
		temp3->alloc=1; //on cree le header
		temp3->zero=0;
		temp3->size=taille-realSize;
		return temp2;
	}
	
	printf("taille %d", taille);
	//printf("adresse avant %d\n",temp);
	temp=temp+taille;
	//printf("adresse apres %d\n",temp);
	//printf("taille %d",taille);
	
	
	
	
 }

return NULL;


}


void *mycalloc(size_t t)
{
	
void *pointeur = mymalloc(t);
	
	size_t paddingSize = t%4;
    if (paddingSize != 0) paddingSize=4-paddingSize;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    
     memset(pointeur,0, (realSize-4));
	
	return pointeur;
	
	
}






int main(int argc, const char *argv[])
{

 struct bloc_header *a = mymalloc(sizeof(struct bloc_header));
 struct bloc_header *b=a-1;
 printf("MAlloc 1 %d\n",(b->size)); //affiche 1

  struct bloc_header *c = mymalloc(sizeof(struct bloc_header));
 struct bloc_header *d=c-1;
 printf("Malloc 2 %d\n",(d->alloc)); //affiche 1
 
 
	return 0;

}
