/* Prototype de myMalloc
* Gregory Creupelandt
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mymalloc.h"


void *base_heap;
size_t memsize = 100;


void *mymalloc(size_t t)
{
	
if (t<=0 || t>536870911) return NULL; 

    size_t paddingSize = t%4;
    if (paddingSize != 0) paddingSize=4-paddingSize;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    
    	if (realSize>memsize) return NULL;
    	
if(base_heap==NULL)  // premier 
{
	base_heap=sbrk(0);
	sbrk(memsize);
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
	 if((temp+realSize)>(base_heap+memsize))
	{
	printf("pas assez de place dans le heap");
	return NULL;
	}

	 
	temp2 = (struct bloc_header*) temp; //cast
	//taille = temp2->size;
	
	taille = ((struct bloc_header *) temp)->size;
	
	if(temp2->size==0) {

	struct bloc_header *entete = temp;
	 entete->size = realSize;
	 entete->zero=0;
	 entete->alloc=1;
	return (void*) (temp+4);
	}


  if (taille==realSize && temp2->alloc==0) // ils ont la meme taille 
   {	
	   temp2->alloc=1;
	    return temp2+4;
	}
	
  if (taille>realSize && temp2->alloc==0) // on peut mettre dedans
	{
		temp2->alloc=1; //on modifie le header existant
		temp2->size=realSize;
		
		
		struct bloc_header *temp3 = (temp2+(realSize)/4); //on va a l'adresse ou on mettra le prochain header
		temp3->alloc=1; //on cree le header
		temp3->zero=0;
		temp3->size=taille-realSize;
		return temp2;
	}
	


	temp=temp+taille;

	
	
	
	
 }

printf("pas assez de place dans le heap prealloue \n");

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

printf("taille du premier block alloué ça devrait etre 8 = %d\n", (a-1)->size); // ca marche

  struct bloc_header *c = mymalloc(sizeof(struct bloc_header)*2);

 
printf(" devrait etre 12 = %d\n", (c-1)->size); // ca marche


 
   int *f = mymalloc(21);
 struct bloc_header *g = f-1;
 printf("ça devrait etre 28 = %d\n", g->size); // ca marche
 
    int *l = mymalloc(48);
 struct bloc_header *m = l-1;
 printf("devrait etre 52 = %d\n", m->size); // ca marche. Si je met 49 et pas 48, la taille passe à 56 et pas 52 et du coup on depasse le heap => ca met une erreur NIQUEL
 
 printf("ça devrait etre 28 encore = %d\n", g->size); // ca marche
 
  // limite de 100 atteinte (pile poile)
  
     int *n = mymalloc(4); // il renvoit null et envoit une segment fault car on depasse la limite


 
	return 0;

}
