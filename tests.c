#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "mymalloc.h"

char *globStr = NULL;

/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {
    //On cherche à allouer un bloc mémoire qui sera utilisé dans nos tests
    globStr = malloc(sizeof(char)*20);
    if (globStr == NULL)
        return -1;

    return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
    free(globStr); //on libère la mémoire allouée lors de l'initialisation de la suite
    return 0;
}


// @tri:test01 => [Allocation 1ere bloc dont la taille devrait être 8]
void test01(void) {
    struct bloc_header *a = mymalloc(sizeof(struct bloc_header));
printf("j'aloue un premier bloc sa taille est 8 = %d\n", (a-1)->size);
    CU_ASSERT_EQUAL((a-1)->size, 8); 
}
// @tri:test02 => [Allocation 2eme bloc dont la taille devrait être 12]
void test02(void) {
    struct bloc_header *c = mymalloc(2*sizeof(struct bloc_header));
printf("j'alloue un 2eme bloc, sa taille est 12 = %d\n", (c-1)->size); // ca marche
    CU_ASSERT_EQUAL((c-1)->size, 12); 
}
// @tri:test03 => [Allocation 3eme bloc dont la taille devrait être 28]
void test03(void) {
    int *f = mymalloc(21);
    struct bloc_header *g = (struct bloc_header*)f-1;
    CU_ASSERT_EQUAL(g->size, 28); 
}


int main() {
    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour le projet MyMalloc", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test01", test01) ||
       NULL == CU_add_test(pSuite, "test02", test02) ||
       NULL == CU_add_test(pSuite, "test03", test03)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
