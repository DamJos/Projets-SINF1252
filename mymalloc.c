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
	
if (t<=0 || t>536870911) 
	{
	printf("argument invalide\n");
	return NULL; 
	}
	
    size_t paddingSize = t%4;
    if (paddingSize != 0) paddingSize=4-paddingSize;
    size_t headerSize = 4;
    size_t realSize = t + paddingSize + headerSize;
    
    	if (realSize>memsize) 
    	{
			printf("la memoire est trop petite pour contenir un tel bloc\n");
			return NULL;
		}
    	
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
	printf("pas assez de place dans le heap\n");
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
	       printf("je vais mettre dans lancien bloc car ils ont la meme taille\n");
	    return temp2+1;
	
	}
	
  if (taille>realSize && temp2->alloc==0) // on peut mettre dedans
	{
		temp2->alloc=1; //on modifie le header existant
		temp2->size=realSize;
		
		
		struct bloc_header *temp3 = (temp2+(realSize)/4); //on va a l'adresse ou on mettra le prochain header
		temp3->alloc=1; //on cree le header
		temp3->zero=0;
		temp3->size=taille-realSize;
		printf("j'ai un bloc trop grand donc je le rapeticie\n");
		return temp2+1;
	}
	


	temp=temp+taille; // on va aller regarder au bloc suivant

	
	
	
	
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

void myfree(void *ptr)
{
	struct bloc_header* modif= ((struct bloc_header*)ptr)-1;
	if(modif->alloc==0)
	{
	printf("vous voulez free une adresse non-allouée");	
	return;
		
	}
	
	modif->alloc=0;
	

	struct bloc_header *suivant = modif+((modif->size)/4);
	
	
	if (suivant->alloc==0)
	{
		printf("coucou");
	modif->size= (modif->size)+(suivant->size);
			return;
	}
	
	return;
	
	
	
}




int main(int argc, const char *argv[])
{

 struct bloc_header *a = mymalloc(sizeof(struct bloc_header));

printf("j'aloue un premier bloc sa taille est 8 = %d\n", (a-1)->size); // ca marche

  struct bloc_header *c = mymalloc(sizeof(struct bloc_header)*2);

 
printf("j'alloue un 2eme bloc, sa taille est 12 = %d\n", (c-1)->size); // ca marche
 
   int *f = mymalloc(21);
 struct bloc_header *g = (struct bloc_header*)f-1;
 printf(" j'aloue un 3eme bloc sa taille est 28 = %d\n", g->size); // ca marche
 
    int *l = mymalloc(48);
 struct bloc_header *m = (struct bloc_header*)l-1;
 printf("j'alloue un 4eme bloc sa taille est 52, le heap est full = %d\n", m->size); // ca marche. Si je met 49 et pas 48, la taille passe à 56 et pas 52 et du coup on depasse le heap => ca met une erreur NIQUEL
 
 printf("je reaffiche la taille du 3eme bloc sa taille est 28 = %d\n", g->size); // ca marche
 
  // limite de 100 atteinte (pile poile)
  
   //  int *n = mymalloc(4); // il renvoit null et envoit une segment fault car on depasse la limite

 printf("je verifie que le premier bloc est bien mis en alloue donc 1 = %d\n", ((struct bloc_header*)a-1)->alloc); 
  printf("je reverifie la taille de a qui est 8 = %d\n", ((struct bloc_header*)a-1)->size); 
 myfree(a); // je libere a qui faisait 8 bytes
 printf("j'ai libéré le premier bloc donc il n'est plus alloué donc 0 = %d\n",((struct bloc_header*)a-1)->alloc); 
  printf("le premier bloc a ete libere mais sa taille est toujours 8 = %d\n", ((struct bloc_header*)a-1)->size); 
  
  int *x=mymalloc(sizeof(int));  // on a free une taille de 8 donc on peut remettre dedans un nouveau pointeur de taille 8 (4+header)
  printf("je realloue pour un bloc de taille 8 or le premier bloc désaoullé faisait 8 donc il a le mettre dedans et afficher 1 car désormais alloué = %d\n", ((struct bloc_header*)a-1)->alloc); 
  
  printf("je verifie la taille de ce bloc qui a remplacé le bloc1 la taille est 8 = %d\n", ((struct bloc_header*)x-1)->size);
  
  myfree(c); //je libere c qui faisait 12 bytes
  printf("je vais malloc un bloc trop gros donc erreur=");
 int *y = mymalloc(16); // je veux mettre 20 bytes (16+4) alors qu'il n'y en a que 12 de libres donc erreur
 printf("je veux afficher ce pointeur retourne mais ca m'a retourné null car pas assez de place ca affichera donc nill=");
 printf("%p\n",y);
 int *z = mymalloc(4); //je veux mettre 8 (4+4) et j'ai 12 de libres donc je devrais mettre a la place ou il y avait c donc ca devrait marcher et il mettra une ettiquette qui vaut 4 derriere
 printf("l'etiquette affiche désormais 8 = %d\n", ((struct bloc_header*)z-1)->size);
 printf("une petite etiquette derriere affiche 4 = %d\n", ((struct bloc_header*)z+1)->size);
 
 myfree(l); // je libere l qui faisait 52
 myfree(f); // je libere f qui faisait 28 et l derriere est libre donc ca fusionne et jai un bloc de 80
 int *r = mycalloc(76); // jessaye de reremplir ce gros bloc créé par fusion via calloc donc rempli de 0
 printf("j'ai free les blocs de 28 et 52 qui ont du fusionner pour faire 80 bytes de libre\n");
 printf("jessaye de mettre un bloc de 80 (76+4) dans le bloc resultant de la fusion\n");
 printf("voyons si le bloc de 80 a été alloué ca devrait mettre 1 = %d\n", ((struct bloc_header*)r-1)->alloc);
  printf("voyons si le bloc de 80 a été rempli de 0 donc ca devrait afficher 0 = %d\n", *r);
printf("voyons si le bloc de 80 a bien taille de 80 en header= %d\n", ((struct bloc_header*)r-1)->size);
  
	return 0;

}
